#include "pycore_coroutine.h"
#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include "pycore_cor_platform.h"


static void Coroutine_RunNext(void);
static void _Coroutine_Continue(Coroutine *cor, void *value, bool early);

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
    Coroutine_Constructing,
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
    Coroutines *coroutines; // so can work with it off-thread
    List_Link link;         // for whichever list it's on
    jmp_buf buf;            // how to get back to it
    unsigned char *guard;   // where the stack overrun guard is
    Coroutine_Start start;  // entry point
    void *entry_param;      // to pass to start
    void *value;            // yielded/returned
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

static void stack_chunk_chunk(Coroutine *parent);
static void stack_chunk_base(Coroutine *parent, unsigned char *guard);


// Check whether the guard is intact
static inline bool Check_Guard(
    unsigned char *guard
){
    return guard[0] == 0xde &&
        guard[1] == 0xad &&
        guard[2] == 0xbe &&
        guard[3] == 0xef;
}


static void Coroutine_PrimeStackChunks(void)
{
    unsigned char chunk_of_stack[COROUTINE_STARTUP_STACK_SIZE];
    for (uintptr_t i = 0; i < COROUTINE_STARTUP_STACK_SIZE-3; i += 4){
        chunk_of_stack[i+0] = 0xde;
        chunk_of_stack[i+1] = 0xad;
        chunk_of_stack[i+2] = 0xbe;
        chunk_of_stack[i+3] = 0xef;
    }
    assert(Check_Guard(chunk_of_stack));

    // Stacks grow down in memory (almost always), so if the caller of this function changes
    // the guard before entering the coroutine system, it has overrun the startup stack
    g_c.guard = chunk_of_stack;
    
    stack_chunk_base(NULL, NULL);
}


static void stack_chunk_chunk(
    Coroutine *parent
){
    unsigned char chunk_of_stack[COROUTINE_STACK_SIZE];
    for (uintptr_t i = 0; i < COROUTINE_STACK_SIZE-3; i += 4){
        chunk_of_stack[i+0] = 0xde;
        chunk_of_stack[i+1] = 0xad;
        chunk_of_stack[i+2] = 0xbe;
        chunk_of_stack[i+3] = 0xef;
    }
    stack_chunk_base(parent, chunk_of_stack);
}


static void stack_chunk_base(
    Coroutine *parent,
    unsigned char *guard
){
    Coroutine here;
    here.state = Coroutine_Constructing;
    switch (setjmp(here.buf)) {
    case Chunk_Initial:
        // got here for the first time
        // parent now has a chunk_of_stack - add it to the free list
        if (parent) {
            assert(parent->state == Coroutine_Constructing);
            assert(Check_Guard(guard));
            parent->guard = guard;
            parent->state = Coroutine_Free;
            List_AddHead(&g_c.free, &parent->link);
            g_c.report.coroutines_pool_size += 1;
        }
        // note that here is the tip of the chunk-claim stack
        here.coroutines = &g_c;
        g_c.tip = &here;

        // return to the coroutine allocator
        longjmp(g_c.chunk_allocated, 1);
    case Chunk_Create:
        // request to create a new chunk on the stack
        assert(here.state == Coroutine_Constructing);
        stack_chunk_chunk(&here);
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
        // coroutine has completed
        if (g_c.primary == &here) {
            // if primary coroutine - return to Coroutine_Run
            longjmp(g_c.controller, Coroutines_CoroutineComplete);
        }
        _Cor_Mutex_Unlock(&g_c.mutex);
        Coroutine_RunNext();
        assert(false);
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
    g_c.report.lowest_headroom = COROUTINE_STACK_SIZE;

    // prime the chunk system
    if (!setjmp(g_c.chunk_allocated)){
        Coroutine_PrimeStackChunks();
        assert(false);
    }

    assert(g_c.state == Coroutines_Starting);
    g_c.state = Coroutines_Started;
}


Coroutine_Report Coroutine_StopSystem(void)
{
    _Cor_Mutex_Lock(&g_c.mutex);
    assert(g_c.state == Coroutines_Started);
    g_c.state = Coroutines_Stopping;

    uintptr_t stackminheadroom = COROUTINE_STACK_SIZE;
    for (List_Link *link = g_c.free.fwd.link.next; link->next; link = link->next){
        Coroutine *cor = List_Link_Container(Coroutine, link, link);
        for (uintptr_t i = 4; i < COROUTINE_STACK_SIZE-3; i += 4){
            if (!Check_Guard(&cor->guard[i])){
                stackminheadroom = i < stackminheadroom ? i : stackminheadroom;
                break;
            }
        }
    }
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


void *Coroutine_Run(
    Coroutine_Start start,
    void *value
){
    Coroutine *cor = Coroutine_New(start);
    Coroutine_Run_Coroutine(cor, value);
    void *res = Coroutine_GetValue(cor);
    Coroutine_Delete(cor);
    return res;
}


Coroutine *Coroutine_New(
    Coroutine_Start start
){
    assert((g_c.state == Coroutines_Started && List_IsEmpty(&g_c.inactive)) || g_c.state == Coroutines_Active);

    // if none free - add one
    if (List_IsEmpty(&g_c.free)){
        if (!setjmp(g_c.chunk_allocated)){
            longjmp(g_c.tip->buf, Chunk_Create);
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


void Coroutine_Delete(
    Coroutine *cor
){
    Coroutines *cors = cor->coroutines;
    _Cor_Mutex_Lock(&cors->mutex);
    assert(cor->state == Coroutine_Idle || cor->state == Coroutine_Complete);
    cor->state = Coroutine_Free;
    List_Remove(&cor->link);
    List_AddTail(&cors->free, &cor->link);
    _Cor_Mutex_Unlock(&cors->mutex);
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


void Coroutine_Continue(
    Coroutine *cor,
    void *value,
    bool early
){
    Coroutines *cors = cor->coroutines;
    _Cor_Mutex_Lock(&cors->mutex);
    _Coroutine_Continue(cor, value, early);
    _Cor_Mutex_Unlock(&cors->mutex);
}


void *Coroutine_Yield(
    void *value,
    Coroutine_YieldCallback on_yield,
    void *yield_me
){
    Coroutine *me = g_c.active;
    assert(Check_Guard(me->guard));

    _Cor_Mutex_Lock(&g_c.mutex);
    Coroutines *cors = me->coroutines;
    assert(me && me->state == Coroutine_Running && cors == &g_c);
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
    case Chunk_Create:
        assert(false);
    case Chunk_Enter:
        // arrive here with mutex locked
        cors->active = me;
        // when we return here - we are running again
        assert(me->state == Coroutine_Running);
        void *res = me->entry_param;
        _Cor_Mutex_Unlock(&cors->mutex);
        return res;
    }
    return NULL;
}


void *Coroutine_GetValue(
    Coroutine *cor
){
    return cor->value;
}


Coroutine *Coroutine_GetActive(void)
{
    return g_c.active;
}


int Coroutine_GetStackHeadroom(void){
    unsigned char tbuf[4];
    return tbuf - g_c.active->guard - 4;
}


bool Coroutine_HasCoroutinesInFreePool(void){
    return (g_c.state == Coroutines_Started || g_c.state == Coroutines_Active) && !List_IsEmpty(&g_c.free);
}


// Pass the address of a local variable through this to avoid warnings about taking its address
static inline void *mask_address(void *ptr){return ptr;}


void *Coroutine_GetCStackTop(void){
    char here;
    return (g_c.state == Coroutines_Started || g_c.state == Coroutines_Active) ? (void *)g_c.tip : mask_address(&here);
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
    Coroutine_Continue(params->ret, params->start(params->value), true);
    return NULL;
}


static void Coroutine_ChainYield(
    void *unused
){
    (void)unused;
}


void *Coroutine_Chain(
    Coroutine_Start start,
    void *value
){
    assert(Check_Guard(Coroutine_GetActive()->guard));
    Coroutine *cor = Coroutine_New(Coroutine_ChainFn);
    struct Coroutine_ChainParam params = {
        start,
        value,
        Coroutine_GetActive()
    };
    Coroutine_Continue(cor, &params, true);
    void *res = Coroutine_Yield(NULL, Coroutine_ChainYield, NULL);
    Coroutine_Delete(cor);
    return res;
}


bool Coroutine_IsRunning(
    Coroutine *cor
)
{
    int state = cor->state;
    return state == Coroutine_Running || state == Coroutine_Waiting;
}


bool Coroutine_IsStarted(void){
    return g_c.state == Coroutines_Active || g_c.state == Coroutines_Started;
}
