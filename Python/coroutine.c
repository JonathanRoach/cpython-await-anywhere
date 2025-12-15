#include "pycore_coroutine.h"
#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include "pycore_cor_platform.h"

// see CPython again, this time from ctypes.h
#if (defined (__SVR4) && defined (__sun)) || defined(COROUTINE_HAVE_ALLOCA_H)
#   include <alloca.h>
#elif defined(MS_WIN32)
#   include <malloc.h>
#endif

/* If the system does not define alloca(), we have to hope for a compiler builtin. */
#ifndef alloca
#   if defined __GNUC__ || (__clang_major__ >= 4)
#      define alloca __builtin_alloca
#   else
#     error "Could not define alloca() on your platform."
#   endif
#endif

static void Coroutine_RunNext(void);
static void _Coroutine_Continue(Coroutine *cor, void *value, bool early);
static unsigned char *StackTopNow(void);

///////////////////////////////////////////////////////////////////////////////
// 2-way linked lists...
//
// Brought inline here to avoid namespace polution
///////////////////////////////////////////////////////////////////////////////

typedef struct List_Link List_Link;
struct List_Link {
    List_Link *next;
    List_Link *prev;
};

typedef struct List_Head List_Head;
struct List_Head {
    union {
        struct {
            List_Link link;
            List_Link *filler;
        } fwd;
        struct {
            List_Link *filler;
            List_Link link;
        } back;
    };
};


static inline bool List_IsEmpty(
    const List_Head *list
){
    return list->fwd.link.next == &list->back.link;
}


static inline List_Link *List_GetHead(
    const List_Head *list
){
    return List_IsEmpty(list) ? NULL : list->fwd.link.next;
}


// static inline List_Link *List_GetTail(
//     const List_Head *list
// ){
//     return List_IsEmpty(list) ? NULL : list->back.link.prev;
// }


#define OFFSETOF(Container, Field) ((char *)&((Container *)4)->Field - (char *)(Container *)4)
#define List_Link_Container(Container, Link, link) ((Container *)((char *)(link) - OFFSETOF(Container, Link)))


static inline void List_Init(
    List_Head *list
){
    list->fwd.link.next = &list->back.link;
    list->fwd.link.prev = NULL;
    list->back.link.prev = &list->fwd.link;
}


static inline void List_AddHead(
    List_Head *list,
    List_Link *link
){
    List_Link *first = list->fwd.link.next;
    link->next = first;
    link->prev = &list->fwd.link;
    first->prev = link;
    list->fwd.link.next = link;
}


static inline void List_AddTail(
    List_Head *list,
    List_Link *link
){
    List_Link *last = list->back.link.prev;
    link->prev = last;
    link->next = &list->back.link;
    last->next = link;
    list->back.link.prev = link;
}


static inline void List_Remove(
    List_Link *link
){
    link->prev->next = link->next;
    link->next->prev = link->prev;
}

///////////////////////////////////////////////////////////////////////////////
// ...2-way linked lists
///////////////////////////////////////////////////////////////////////////////

typedef struct Coroutines Coroutines;

enum {
    Coroutines_Idle,
    Coroutines_Starting,
    Coroutines_Started,
    Coroutines_Active,
    Coroutines_Stopping
};

enum {
    Chunk_Initial,
    Chunk_Create,
    Chunk_Enter    
};

typedef enum Coroutine_State {
    Coroutine_Free,
    Coroutine_Idle,
    Coroutine_Running,
    Coroutine_Waiting,
    Coroutine_Complete
} Coroutine_State;

enum {
    Coroutines_Init,
    Coroutines_AllocatedChunk,
    Coroutines_CoroutineComplete,
};

struct Coroutine {
    Coroutines *coroutines;     // so can work with it off-thread
    List_Link link;             // for whichever list it's on
    jmp_buf buf;                // how to get back to it
    unsigned char *guard;       // where the stack overrun guard is
    Coroutine_Start start;      // entry point
    void *entry_param;          // to pass to start
    void *value;                // yielded/returned
    unsigned char *stack_top;   // recorded at yield
    Coroutine_State state;
};

struct Coroutines {
    _Cor_Mutex mutex;
    jmp_buf controller;     // to return from Coroutine_Run
    jmp_buf chunk_allocated;// for chunk allocation
    unsigned char *guard;   // the stack guard for the startup sequence

    // singletons
    Coroutine *tip;     // top of stack chunk
    Coroutine *active;  // currently running coroutine
    Coroutine *primary; // Coroutine_Run coroutine
    unsigned char *stack_limit;  // when not NULL, where the stack finishes

    // lists
    List_Head free;
    List_Head inactive;     // idle or complete
    List_Head runable;      // running or waiting to run
    List_Head waiting;      // yielded / waiting to run
    _Cor_Mutex waiting_mutex;

    // Summary of the system
    Coroutine_Report report;

    // state
    char state;
};

_Cor_thread_local Coroutines g_c;

static void stack_chunk_chunk(Coroutine *parent, size_t chunk_size);
static void stack_chunk_base(void);


#define GUARD_PATTERN_SIZE (4)
// Check whether the guard is intact
static inline bool Check_Guard(
    unsigned char *guard
){
    return !guard ||
        (guard[0] == 0xde &&
         guard[1] == 0xad &&
         guard[2] == 0xbe &&
         guard[3] == 0xef);
}


static inline void Apply_Guard(unsigned char *guard){
    guard[0] = 0xde;
    guard[1] = 0xad;
    guard[2] = 0xbe;
    guard[3] = 0xef;
}


static bool Coroutine_StackHasNotOverrun(void){
    unsigned char *stack_top = StackTopNow();
    unsigned char *stack_limit = g_c.stack_limit;
    if (stack_limit && stack_top < stack_limit){
        // current stack top is beyond limit - we are overrunning NOW
        return false;
    }
    Coroutine *me = g_c.active;
    if (!me){
        return true;
    }
    if (me->guard){
        return Check_Guard(me->guard);
    }
    unsigned char *coroutine_limit;
    if (!stack_limit || stack_limit <= (unsigned char *)me - 2*COROUTINE_STACK_SIZE){
        // no stack limit, or can start a coroutine, so limit ourselves to one unit of coroutine stack
        coroutine_limit = (unsigned char *)me - 1*COROUTINE_STACK_SIZE + GUARD_PATTERN_SIZE;
    } else {
        // can't start coroutine, and have a stack limit - use that
        coroutine_limit = stack_limit;
    }
    return stack_top >= coroutine_limit;
}


static void Coroutine_PrimeStackChunks(void)
{
    unsigned char chunk_of_stack[COROUTINE_STARTUP_STACK_SIZE + GUARD_PATTERN_SIZE];
    Apply_Guard(chunk_of_stack);
    assert(Check_Guard(chunk_of_stack));

    // Stacks grow down in memory (almost always), so if the caller of this function changes
    // the guard before entering the coroutine system, it has overrun the startup stack
    g_c.guard = chunk_of_stack;
    
    stack_chunk_base();
}


static void stack_chunk_chunk(
    Coroutine *parent,
    size_t chunk_size
){
    unsigned char *chunk_of_stack = alloca(chunk_size);
#if COROUTINE_RECORD_LOWEST_HEADROOM
    for (size_t i = 0; i <= chunk_size-GUARD_PATTERN_SIZE; i += GUARD_PATTERN_SIZE){
        Apply_Guard(&chunk_of_stack[i]);
    }
#else
    Apply_Guard(chunk_of_stack);
#endif
    parent->guard = chunk_of_stack;
    stack_chunk_base();
}


static void stack_chunk_base(
    void
){
    Coroutine here;
    here.state = Coroutine_Free;
    here.guard = NULL;
    here.coroutines = &g_c;
    List_AddHead(&g_c.free, &here.link);
    g_c.report.coroutines_pool_size += 1;
    g_c.tip = &here;
    for(;;){
        switch (setjmp(here.buf)) {
        case Chunk_Initial:
            if (here.state == Coroutine_Free){
                // return to the coroutine allocator
                longjmp(g_c.chunk_allocated, 1);
            } else {
                assert(here.state == Coroutine_Complete);
                // we finish here to ensure the setjmp is redone
                if (g_c.primary == &here) {
                    // if primary coroutine - return to Coroutine_Run
                    longjmp(g_c.controller, Coroutines_CoroutineComplete);
                }
                _Cor_Mutex_Unlock(&g_c.mutex);
                Coroutine_RunNext();
                assert(false);
            }
        case Chunk_Create:
            // Request to create a new chunk on the stack
            // We're here if the coroutine is:
            // Allocated, but not 'run' (Coroutine_Idle)
            // Run, but not not entered yet (Coroutine_Running)
            // Completed (Coroutine_Complete)
            assert(here.state == Coroutine_Idle || here.state == Coroutine_Running || here.state == Coroutine_Complete);
            unsigned char *ideal_limit = (unsigned char *)&here - COROUTINE_STACK_SIZE;
            stack_chunk_chunk(&here, StackTopNow() - ideal_limit);
            assert(false);
        case Chunk_Enter:
            // request to start a coroutine (ie use the chunk for a coroutine)
            // arrive here with mutex locked
            assert(here.state == Coroutine_Running);
            g_c.active = &here;
            _Cor_Mutex_Unlock(&g_c.mutex);
            here.value = here.start(here.entry_param);

            // check the guard
            assert(Check_Guard(here.guard));

            _Cor_Mutex_Lock(&g_c.mutex);
            g_c.active = NULL;
            assert(here.state == Coroutine_Running);
            List_Remove(&here.link);
            here.state = Coroutine_Complete;
            List_AddTail(&g_c.inactive, &here.link);
            // Coroutine has completed
            // Loop round to redo the setjmp() - if this coroutine yielded, then the setjmp will
            // need reseting
        }
    }
}


static void Coroutine_RunNext(void)
{
    // arrive here with mutex unlocked
    _Cor_Mutex_Lock(&g_c.waiting_mutex);
    _Cor_Mutex_Lock(&g_c.mutex);
    Coroutine *next = List_Link_Container(Coroutine, link, List_GetHead(&g_c.runable));
    assert(next->state == Coroutine_Running);
    longjmp(next->buf, Chunk_Enter);
    assert(false);
}


void Coroutine_StartSystem(void)
{
    assert(g_c.state == Coroutines_Idle);
    g_c.state = Coroutines_Starting;

    _Cor_Mutex_ctor(&g_c.mutex);

    g_c.tip = NULL;
    g_c.active = NULL;

    List_Init(&g_c.free);
    List_Init(&g_c.inactive);
    List_Init(&g_c.runable);
    List_Init(&g_c.waiting);
    _Cor_Mutex_ctor(&g_c.waiting_mutex);
    _Cor_Mutex_Lock(&g_c.waiting_mutex);

    g_c.report.coroutines_created = 0;
    g_c.report.coroutines_pool_size = 0;

    // prime the chunk system
    if (!setjmp(g_c.chunk_allocated)){
        Coroutine_PrimeStackChunks();
        assert(false);
    }

    assert(g_c.state == Coroutines_Starting);
    g_c.state = Coroutines_Started;
}


void Coroutine_SetStackLimit(void *limit){
    assert(!limit || !(g_c.state == Coroutines_Started || g_c.state == Coroutines_Active) || (unsigned char *)limit < (unsigned char *)g_c.tip);
    g_c.stack_limit = limit;
}


Coroutine_Report Coroutine_StopSystem(void)
{
    _Cor_Mutex_Lock(&g_c.mutex);
    assert(g_c.state == Coroutines_Started);
    g_c.state = Coroutines_Stopping;

    uintptr_t stackminheadroom;;
#if COROUTINE_RECORD_LOWEST_HEADROOM
    stackminheadroom = COROUTINE_STACK_SIZE;
    for (List_Link *link = g_c.free.fwd.link.next; link->next; link = link->next){
        Coroutine *cor = List_Link_Container(Coroutine, link, link);
        if (cor->guard){
            for (uintptr_t i = 4; i < COROUTINE_STACK_SIZE-3; i += 4){
                if (!Check_Guard(&cor->guard[i])){
                    stackminheadroom = i < stackminheadroom ? i : stackminheadroom;
                    break;
                }
            }
        }
    }
#else
    stackminheadroom = 0;
#endif
    g_c.report.lowest_headroom = stackminheadroom;

    assert(List_IsEmpty(&g_c.inactive));
    _Cor_Mutex_Unlock(&g_c.waiting_mutex);
    _Cor_Mutex_dtor(&g_c.waiting_mutex);

    assert(g_c.state == Coroutines_Stopping);
    g_c.state = Coroutines_Idle;
    _Cor_Mutex_Unlock(&g_c.mutex);
    _Cor_Mutex_dtor(&g_c.mutex);

    return g_c.report;
}


void Coroutine_Run_Coroutine(
    Coroutine *cor,
    void *value
){
    Coroutines *cors = cor->coroutines;
    assert(&g_c == cors);
    _Cor_Mutex_Lock(&cors->mutex);
    assert(cors->state == Coroutines_Started);
    cors->state = Coroutines_Active;
    cors->primary = cor;

    _Coroutine_Continue(cor, value, true);

    if (!setjmp(cors->controller)){
        _Cor_Mutex_Unlock(&cors->mutex);

        // check the guard
        assert(Check_Guard(cors->guard));

        // start the first coroutine
        Coroutine_RunNext();
    }
    // arrive here with mutex locked
    assert(List_IsEmpty(&cors->runable));
    assert(List_IsEmpty(&cors->waiting));
    assert(cors->state == Coroutines_Active);
    cors->state = Coroutines_Started;
    _Cor_Mutex_Unlock(&cors->mutex);
}


bool Coroutine_Run(
    Coroutine_Start start,
    void *value,
    void **result
){
    if (g_c.active){
        void *res = start(value);
        if (result){
            *result = res;
        }
        // no failures, so...
        return false;
    }
    assert(g_c.state == Coroutines_Idle || g_c.state == Coroutines_Started);
    bool need_start = g_c.state == Coroutines_Idle;
    if (need_start){
        Coroutine_StartSystem();
    }
    Coroutine *cor = _Py_Coroutine_New(start);
    if (!cor){
        // that didn't work
        return true;
    }
    Coroutine_Run_Coroutine(cor, value);
    if (result){
        *result = _Py_Coroutine_GetValue(cor);
    }
    _Py_Coroutine_Delete(cor);
    if (need_start){
        Coroutine_StopSystem();
    }
    // no failures, so...
    return false;
}


Coroutine *_Py_Coroutine_New(
    Coroutine_Start start
){
    assert((g_c.state == Coroutines_Started && List_IsEmpty(&g_c.inactive)) || g_c.state == Coroutines_Active);
    assert(Coroutine_StackHasNotOverrun());

    // if none free - add one
    if (List_IsEmpty(&g_c.free)){
        // no free stack blocks
        if (g_c.stack_limit && g_c.stack_limit > (unsigned char *)g_c.tip - 2*COROUTINE_STACK_SIZE){
            // no space for a new stack block
            return NULL;
        }
        Coroutine *tip = g_c.tip;
        Coroutine *me = g_c.active;
        if (tip == me) {
            if (!setjmp(g_c.chunk_allocated)){
                unsigned char *ideal_limit = (unsigned char *)me - COROUTINE_STACK_SIZE;
                stack_chunk_chunk(me, StackTopNow() - ideal_limit);
            }
        } else {
            if (!setjmp(g_c.chunk_allocated)){
                longjmp(tip->buf, Chunk_Create);
            }
        }
    }

    Coroutine *cor = List_Link_Container(Coroutine, link, List_GetHead(&g_c.free));
    assert(cor->state == Coroutine_Free);
    cor->state = Coroutine_Idle;
    cor->start = start;
    cor->value = NULL;
    List_Remove(&cor->link);
    List_AddHead(&g_c.inactive, &cor->link);

    g_c.report.coroutines_created += 1;

    return cor;
}


void _Py_Coroutine_Delete(
    Coroutine *cor
){
    assert(Coroutine_StackHasNotOverrun());
    if (cor){
        Coroutines *cors = cor->coroutines;
        _Cor_Mutex_Lock(&cors->mutex);
        assert(cor->state == Coroutine_Idle || cor->state == Coroutine_Complete);
        cor->state = Coroutine_Free;
        List_Remove(&cor->link);
        List_AddTail(&cors->free, &cor->link);
        _Cor_Mutex_Unlock(&cors->mutex);
    }
}


// Coroutine_Continue, assuming the mutex is claimed
static void _Coroutine_Continue(
    Coroutine *cor,
    void *value,
    bool early
){
    Coroutines *cors = cor->coroutines;
    assert(cor->state == Coroutine_Idle || cor->state == Coroutine_Waiting);
    cor->entry_param = value;
    cor->state = Coroutine_Running;
    List_Remove(&cor->link);
    if ( early ) {
        List_AddHead(&cors->runable, &cor->link);
    } else {
        List_AddTail(&cors->runable, &cor->link);
    }
    _Cor_Mutex_Unlock(&cors->waiting_mutex);
}


void _Py_Coroutine_Continue(
    Coroutine *cor,
    void *value,
    bool early
){
    assert(Coroutine_StackHasNotOverrun());
    Coroutines *cors = cor->coroutines;
    _Cor_Mutex_Lock(&cors->mutex);
    _Coroutine_Continue(cor, value, early);
    _Cor_Mutex_Unlock(&cors->mutex);
}


void *_Py_Coroutine_Yield(
    void *value,
    Coroutine_YieldCallback on_yield,
    void *yield_me
){
    Coroutine *me = g_c.active;
    assert(me);
    assert(Coroutine_StackHasNotOverrun());

    _Cor_Mutex_Lock(&g_c.mutex);
    Coroutines *cors = me->coroutines;
    assert(me && me->state == Coroutine_Running && cors == &g_c);
    me->stack_top = StackTopNow();
    me->value = value;
    me->state = Coroutine_Waiting;

    List_Remove(&me->link);
    if (!List_IsEmpty(&cors->runable)){
        _Cor_Mutex_Unlock(&cors->waiting_mutex);
    }
    List_AddTail(&cors->waiting, &me->link);

    switch (setjmp(me->buf)){
    case Chunk_Initial:
        _Cor_Mutex_Unlock(&cors->mutex);
        on_yield(yield_me);
        Coroutine_RunNext();
        assert(false);
    case Chunk_Create:
        assert(me == g_c.tip);
        unsigned char *ideal_limit = (unsigned char *)me - COROUTINE_STACK_SIZE;
        stack_chunk_chunk(me, me->stack_top - ideal_limit);
        assert(false);
    case Chunk_Enter:
        // arrive here with mutex locked
        cors->active = me;
        assert(Coroutine_StackHasNotOverrun());
        // when we return here - we are running again
        assert(me->state == Coroutine_Running);
        void *res = me->entry_param;
        _Cor_Mutex_Unlock(&cors->mutex);
        return res;
    }
    return NULL;
}


void *_Py_Coroutine_GetValue(
    Coroutine *cor
){
    return cor->value;
}


Coroutine *_Py_Coroutine_GetActive(void)
{
    return g_c.active;
}


intptr_t _Py_Coroutine_GetStackHeadroom(void){
    assert(Coroutine_StackHasNotOverrun());
    Coroutine *me = g_c.active;
    if (!me){
        // no active coroutine
        unsigned char *stack_limit = g_c.stack_limit;
        if (stack_limit){
            // no stack limit - assume we'll use COROUTINE_STACK_SIZE
            return StackTopNow() - stack_limit;
        } else {
            // no information where the stack ends - return something
            return COROUTINE_STACK_SIZE;
        }
    }
    unsigned char *stack_top = StackTopNow();
    if (me->guard){
        // guard established - that's where we'll measure to
        return stack_top - me->guard;
    }
    intptr_t used = (unsigned char *)me - stack_top;
    unsigned char *stack_limit = g_c.stack_limit;
    if (!stack_limit){
        // no stack limit - assume we'll use COROUTINE_STACK_SIZE
        return COROUTINE_STACK_SIZE - used;
    }
    intptr_t available = (unsigned char *)me - stack_limit;
    if (available < (intptr_t)(2*COROUTINE_STACK_SIZE)){
        // can't start another coroutine, so whatever's left in the C stack is what we've got
        return available - used;
    }
    // can start another coroutine, so limit ourselves to a coroutine stack size's worth
    return COROUTINE_STACK_SIZE - used;
}


// This is used to avoid compiler warnings about returning the address of a local
static inline void *StopAddressComplaints(void *p)
{
    return p;
}


void *Coroutine_GetStackHWM(void){
    assert(g_c.state == Coroutines_Active);
    assert(Coroutine_StackHasNotOverrun());
    // Find where the guards end
    unsigned char *guard;
    for (guard = g_c.active->guard; Check_Guard(guard); guard += 4){
        // do nothing
    }
    return guard;
}


void Coroutine_ClearStackForHWM(void){
    assert(g_c.state == Coroutines_Active);
    assert(Coroutine_StackHasNotOverrun());
    unsigned char *end = StackTopNow() - GUARD_PATTERN_SIZE;
    for (unsigned char *guard = g_c.active->guard+GUARD_PATTERN_SIZE; guard <= end; guard += GUARD_PATTERN_SIZE){
        Apply_Guard(guard);
    }
}


bool _Py_Coroutine_CanStartCoroutine(void){
    assert(g_c.state == Coroutines_Started || g_c.state == Coroutines_Active);
    assert(Coroutine_StackHasNotOverrun());
    if (!List_IsEmpty(&g_c.free)){
        return true;
    }

    return !g_c.stack_limit || g_c.stack_limit <= (unsigned char *)g_c.tip - 2*COROUTINE_STACK_SIZE;
}


void *Coroutine_GetCStackTop(void){
    assert(Coroutine_StackHasNotOverrun());
    if ((g_c.state == Coroutines_Started || g_c.state == Coroutines_Active) && g_c.tip != g_c.active) {
        return g_c.tip->stack_top;
    } else {
        return StackTopNow();
    }
}


static unsigned char *StackTopNow(void){
    unsigned char here[4];
    return StopAddressComplaints(here);
}


struct Coroutine_ChainParam {
    Coroutine_Start start;
    void *value;
    Coroutine *ret;
};


static void *Coroutine_ChainFn(
    void *param
){
    struct Coroutine_ChainParam *params = (struct Coroutine_ChainParam *)param;
    _Py_Coroutine_Continue(params->ret, params->start(params->value), true);
    return NULL;
}


static void Coroutine_ChainYield(
    void *unused
){
    (void)unused;
}


bool _Py_Coroutine_Chain(
    Coroutine_Start start,
    void *value,
    void **result
){
    assert(Check_Guard(_Py_Coroutine_GetActive()->guard));
    Coroutine *cor = _Py_Coroutine_New(Coroutine_ChainFn);
    if (!cor){
        // failed
        return true;
    }
    struct Coroutine_ChainParam params = {
        start,
        value,
        _Py_Coroutine_GetActive()
    };
    _Py_Coroutine_Continue(cor, &params, true);
    void *res = _Py_Coroutine_Yield(NULL, Coroutine_ChainYield, NULL);
    _Py_Coroutine_Delete(cor);
    if (result){
        *result = res;
    }
    // success!
    return false;
}


bool _Py_Coroutine_IsRunning(
    Coroutine *cor
)
{
    int state = cor->state;
    return state == Coroutine_Running || state == Coroutine_Waiting;
}


bool _Py_Coroutine_IsComplete(
    Coroutine *cor
)
{
    int state = cor->state;
    return state == Coroutine_Complete;
}


bool Coroutine_IsStarted(void){
    return g_c.state == Coroutines_Active || g_c.state == Coroutines_Started;
}
