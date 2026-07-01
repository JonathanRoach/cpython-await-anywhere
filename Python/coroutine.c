#include "pycore_coroutine.h"
#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "cor_platform.h"

#include "coroutine_names_def.h"

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

typedef struct Coroutines Coroutines;

static void Coroutine_RunNext(void);
static Coroutine_Err Coroutine_Continue_(Coroutines *cors, Coroutine *cor, void *value, bool early);
static uintptr_t StackTopNow(void);
static void TrimActiveIfPossible(void);
static void EnlargeActiveIfPossible(void);

#ifndef NDEBUG
    // In debug builds, use the built-in assert
    #define MyAssert assert
#else
 #if 1
    // In non-debug builds, normally use this - all the asserts are disabled
    #define MyAssert(cond)
 #else
    // In non-debug builds with stack problems, you can use this.
    // This activates all the asserts, and gives a line to put a
    // breakpoint in your debugger.
    static void _MyAssert(bool cond, char const *msg)
    {
        if (!cond){
            fputs("Assertion failed: ", stdout);
            fputs(msg, stdout);
            fputs("\n", stdout);
        }
    }
    #define MyAssert(cond) _MyAssert(cond, #cond)
 #endif
#endif

static inline ptrdiff_t
StackPointerDiff
(
    unsigned char *a,
    unsigned char *b
){
#if COROUTINE_STACK_GROWS_UP
    return a - b;
#else
    return b - a;
#endif
}

static inline unsigned char *
StackPointerAdd
(
    unsigned char *a,
    ptrdiff_t b
){
#if COROUTINE_STACK_GROWS_UP
    return a + b;
#else
    return a - b;
#endif
}

// Unused...
// static inline unsigned char *
// StackBaseEnd
// (
//     unsigned char *memlo,
//     unsigned char *memhi
// ){
// #if COROUTINE_STACK_GROWS_UP
//     (void)memhi;
//     return memlo;
// #else
//     (void)memlo;
//     return memhi-1;
// #endif
// }
// ...unused

static inline unsigned char *
StackLimitEnd
(
    unsigned char *memlo,
    unsigned char *memhi
){
#if COROUTINE_STACK_GROWS_UP
    (void)memlo;
    return memhi;
#else
    (void)memhi;
    return memlo-1;
#endif
}

#define CHECK_SYSTEM_RUNNING \
    if (!g_c){ \
        return Coroutine_Err_SystemNotRunning; \
    }
#define CHECK_SYSTEM_NOT_RUNNING \
    if (g_c){ \
        return Coroutine_Err_SystemRunning; \
    }
#define CHECK_COROUTINE_THREAD \
    if (cor->coroutines != g_c){ \
        return Coroutine_Err_CoroutineFromWrongThread; \
    }
#define CHECK_NO_COROUTINE_RUNNING \
    if (g_c->state != Coroutines_Started){ \
        return Coroutine_Err_ACoroutineIsAlreadyRunning; \
    }
#define CHECK_STACK_OVERRUN \
    { \
        Coroutine_Err err = Coroutine_StackHasOverrun(); \
        if (err){ \
            return err; \
        } \
    } while (0);


static inline void ready_jmp_buf(jmp_buf buf) {
#if defined(_M_X64) || defined(_M_ARM64)
    // Win64:
    //   Set Frame to 0 on Windows 64 bit to prevent C++ stack unwinding in longjmp().
    // Win32:
    //   Doesn't do this, so only needed on the 2 64 bit Windows versions
    ((_JUMP_BUFFER*)buf)->Frame = 0;
#else
    (void)buf;
#endif
}

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


static inline List_Link *List_Begin(
    const List_Head *list
){
    return list->fwd.link.next;
}


static inline bool Link_NextIsLink(
    const List_Link *link
){
    return link->next != NULL;
}


static inline List_Link *Link_Next(
    List_Link *link
){
    return link->next;
}


static inline bool Link_PrevIsLink(
    const List_Link *link
){
    return link->prev != NULL;
}


static inline List_Link *Link_Prev(
    List_Link *link
){
    return link->prev;
}

// Unused...
// static inline List_Link *List_GetTail(
//     const List_Head *list
// ){
//     return List_IsEmpty(list) ? NULL : list->back.link.prev;
// }
// ...unused


#define OFFSETOF(Container, Field) ((char *)&((Container *)4)->Field - (char *)(Container *)4)
#define List_Link_Container(Container, Link, link) ((Container *)((char *)(link) - OFFSETOF(Container, Link)))


static inline void
List_Init(
    List_Head *list
){
    list->fwd.link.next = &list->back.link;
    list->fwd.link.prev = NULL;
    list->back.link.prev = &list->fwd.link;
}


static inline void
Link_AddAfter(
    List_Link *link,
    List_Link *after
){
    link->next = after->next;
    link->prev = after;
    after->next->prev = link;
    after->next = link;
}


static inline void
List_AddHead(
    List_Head *list,
    List_Link *link
){
    Link_AddAfter(link, &list->fwd.link);
}


static inline void
Link_AddBefore(
    List_Link *link,
    List_Link *before
){
    link->prev = before->prev;
    link->next = before;
    before->prev->next = link;
    before->prev = link;
}


static inline void
List_AddTail(
    List_Head *list,
    List_Link *link
){
    Link_AddBefore(link, &list->back.link);
}


static inline void 
Link_Remove(
    List_Link *link
){
    link->prev->next = link->next;
    link->next->prev = link->prev;
}

///////////////////////////////////////////////////////////////////////////////
// ...2-way linked lists
///////////////////////////////////////////////////////////////////////////////

enum {
    Coroutines_Started,
    Coroutines_Stopping
};

enum {
    Chunk_Initial,
    Chunk_Split,
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
    size_t min_size;
    size_t min_headroom;
    Coroutines *coroutines;     // so can work with it off-thread
    List_Link link;             // for whichever list it's on
    List_Link all_link;         // list of all Coroutines
    jmp_buf buf;                // how to get back to it
    unsigned char *base;        // where the base of this Coroutine's stack is (= previous block's limit)
    unsigned char *limit;       // where the limit of this Coroutine's stack is (= next block's base)
    unsigned char *guard;       // where the stack overrun guard is
    Coroutine_Start start;      // entry point
    void *entry_param;          // to pass to start
    void *value;                // yielded/returned
    unsigned char *stack_top;   // recorded at yield
    Coroutine_State state;

    int sequence;
};

struct Coroutines {
    _Cor_Mutex mutex;
    jmp_buf controller;     // to return from Coroutine_Run
    jmp_buf chunk_allocated;// for chunk allocation
    size_t size_to_retain;  // for Chunk_Split

    // singletons
    Coroutine *tip;     // top of stack chunk
    Coroutine *active;  // currently running coroutine
    Coroutine *primary; // Coroutine_Run coroutine
    unsigned char *stack_limit;  // when not NULL, where the stack finishes

    Coroutine *spare;   // spare struct Coroutine (instead of having to malloc & fail)

    // lists
    List_Head all;          // all Coroutines (in address order)
    List_Head free;         // free Coroutines
    List_Head inactive;     // idle or complete
    List_Head runable;      // running or waiting to run
    List_Head waiting;      // yielded / waiting to run
    _Cor_Mutex waiting_mutex;

    Coroutine *root;        // The coroutine for the thread which started Coroutines

    // Summary of the system
    Coroutine_Report report;

    // state
    char state;

    int sequence;
};

_Cor_thread_local Coroutines *g_c;
_Cor_thread_local unsigned char *g_stack_limit;

static void ReserveStackSpace(Coroutines *cors, Coroutine *parent, size_t chunk_size, unsigned char *childs_limit);
static void stack_chunk_base(Coroutines *cors, Coroutine *parent, unsigned char *prev_limit, unsigned char *limit);


static size_t
Coroutine_Size
(
    Coroutine *cor
){
    if (cor->limit){
        return (size_t)StackPointerDiff(cor->limit, cor->base);
    } else {
        return SIZE_MAX;
    }
}


static size_t
TrimmableSize(
    Coroutine *cor
){
    size_t current_used = StackPointerDiff(cor->stack_top, cor->base);
    size_t min_size = current_used + cor->min_headroom;
    if (min_size < cor->min_size){
        min_size = cor->min_size;
    }

    if (cor->limit){
        if (Coroutine_Size(cor) < min_size + COROUTINE_MINIMUM_STACK_SIZE){
            return 0;
        }
    }
    return min_size;
}


#define GUARD_PATTERN_SIZE (4)
/// @brief Checks whether a guard pattern is OK
/// @param guard The address of the stack base end of the pattern
/// @return true pattern is ok; false pattern is broken
static inline bool
Guard_Pattern_OK(
    unsigned char *guard
){
    return !guard ||
        (*StackPointerAdd(guard, 0) == 0xde &&
         *StackPointerAdd(guard, 1) == 0xad &&
         *StackPointerAdd(guard, 2) == 0xbe &&
         *StackPointerAdd(guard, 3) == 0xef);
}


/// @brief Writes a guard pattern
/// @param guard Where to write the guard pattern (stack base end)
static inline void
Apply_Guard(unsigned char *guard){
    *StackPointerAdd(guard, 0) = 0xde;
    *StackPointerAdd(guard, 1) = 0xad;
    *StackPointerAdd(guard, 2) = 0xbe;
    *StackPointerAdd(guard, 3) = 0xef;
}


#ifndef NDEBUG
/// @brief Checks a list of Coroutines to see whether it's OK
/// @param head The list to check
/// @param state1 One of the states Coroutines in the list are allowed
/// @param state2 One of the states Coroutines in the list are allowed
/// @return Coroutine_OK the is OK; other the list is broken
static Coroutine_Err
CheckListIntegrity(
    List_Head *head,
    Coroutine_State state1,
    Coroutine_State state2
){
    for (List_Link *link = List_Begin(head); Link_NextIsLink(link); link = Link_Next(link)){
        Coroutine *candidate = List_Link_Container(Coroutine, link, link);
        if (candidate->coroutines != g_c){
            return Coroutine_Err_InternalInsistency;
        }
        if(candidate->state != state1 && candidate->state != state2){
            return Coroutine_Err_InternalInsistency;
        }
        bool found = false;
        for (List_Link *link = List_Begin(&g_c->all); Link_NextIsLink(link); link = Link_Next(link)){
            Coroutine *candidate2 = List_Link_Container(Coroutine, all_link, link);
            if (candidate == candidate2){
                found = true;
            }
        }
        if (!found){
            return Coroutine_Err_InternalInsistency;
        }
    }
    return Coroutine_OK;
}


/// @brief Check the integrity of this thread's coroutine system
/// @return Coroutine_OK everything's OK; other something was wrong
static Coroutine_Err
Coroutine_CheckIntegrity_(
    void
){
    Coroutine_Err err;
    err = CheckListIntegrity(&g_c->free, Coroutine_Free, Coroutine_Free);
    if (err){
        return err;
    }
    err = CheckListIntegrity(&g_c->inactive, Coroutine_Idle, Coroutine_Complete);
    if (err){
        return err;
    }
    err = CheckListIntegrity(&g_c->runable, Coroutine_Running, Coroutine_Running);
    if (err){
        return err;
    }
    err = CheckListIntegrity(&g_c->waiting, Coroutine_Waiting, Coroutine_Waiting);
    return err;
}
#endif


/// @brief Check whether the stack has overrun
/// @return Coroutine_OK the stack hasn't; other it has
static Coroutine_Err
Coroutine_StackHasOverrun(
    void
){
    unsigned char *stack_top = (unsigned char *)StackTopNow();
    unsigned char *stack_limit = g_c ? g_c->stack_limit : NULL;
    if (stack_limit && StackPointerDiff(stack_limit, stack_top) < 0){
        // current stack top is beyond limit - we are overrunning NOW
        return Coroutine_Err_StackOverrun;
    }
    Coroutine *me = g_c ? g_c->active : NULL;
    if (!me){
        return Coroutine_OK;
    }
#if COROUTINE_CHECK_INTEGRITY_ON_STACK_CHECK
    // Check all coroutines integrity
    Coroutine_Err err = Coroutine_CheckIntegrity_();
    if (err){
        return err;
    }
#endif
    if (me->guard){
        if (StackPointerDiff(me->guard, stack_top) < 0){
            // Stack top beyond active stack limit
            return Coroutine_Err_StackOverrun;
        }
        if (!Guard_Pattern_OK(me->guard)){
            // Guard pattern trampled
            return Coroutine_Err_StackOverrun;
        }
    }
    return Coroutine_OK;
}

#ifndef NDEBUG
/// @brief Check system integrity - does stack overrun check and internals check
/// @return Coroutine_OK all is OK; other something was wrong
Coroutine_Err
Coroutine_CheckIntegrity(
    void
){
    Coroutine_Err err = Coroutine_StackHasOverrun();
#if !COROUTINE_CHECK_INTEGRITY_ON_STACK_CHECK
    if (!err && g_c){
        err = Coroutine_CheckIntegrity_();
    }
#endif
    return err;
}
#endif


static void
ReserveStackSpace(
    Coroutines *cors,
    Coroutine *parent,
    size_t chunk_size,
    unsigned char *childs_limit
){
    unsigned char *chunk_of_stack = alloca(chunk_size);
    unsigned char *limit = StackLimitEnd(chunk_of_stack, chunk_of_stack+chunk_size);
    unsigned char *guard = StackPointerAdd(limit, -GUARD_PATTERN_SIZE);
#if COROUTINE_RECORD_LOWEST_HEADROOM
    for (size_t i = 0; i <= chunk_size-GUARD_PATTERN_SIZE; i += GUARD_PATTERN_SIZE){
        Apply_Guard(StackPointerAdd(guard, -i));
    }
#else
    Apply_Guard(guard);
#endif
    if (parent){
        parent->limit = limit;
        parent->guard = guard;
    }
    stack_chunk_base(cors, parent, limit, childs_limit);
}


static void
stack_chunk_base(
    Coroutines *cors,
    Coroutine *parent,
    unsigned char *prev_limit,
    unsigned char *limit
){
    MyAssert(cors->spare);
    Coroutine *here = cors->spare;
    cors->spare = NULL;
    here->coroutines = cors;
    here->sequence = cors->sequence++;
    here->state = Coroutine_Free;
    here->base = prev_limit;
    here->limit = limit;
    here->stack_top = (unsigned char *)StackTopNow();
    if (limit){
        here->guard = StackPointerAdd(limit, -GUARD_PATTERN_SIZE);
        Apply_Guard(here->guard);
    } else {
        here->guard = NULL;
    }

    // insert into all list
    if (parent){
        Link_AddAfter(&here->all_link, &parent->all_link);
    } else {
        List_AddHead(&cors->all, &here->all_link);
    }
    // add to free list
    List_AddTail(&cors->free, &here->link);

    cors->report.coroutines_pool_size += 1;

    if (!cors->tip || !Link_NextIsLink(Link_Next(&here->all_link))){
        cors->tip = here;
    }

    for(;;){
        switch (setjmp(here->buf)) {
        case Chunk_Initial:
            ready_jmp_buf(here->buf);
            if (here->state == Coroutine_Free){
                // return to the coroutine allocator
                longjmp(cors->chunk_allocated, 1);
            } else {
                MyAssert(here->state == Coroutine_Complete);
                // we finish here to ensure the setjmp is redone
                if (cors->primary == here) {
                    // if primary coroutine - return to Coroutine_Run
                    longjmp(cors->controller, Coroutines_CoroutineComplete);
                }
                _Cor_Mutex_Unlock(&cors->mutex);
                Coroutine_RunNext();
            }
            MyAssert(false);
            break;
        case Chunk_Split:
            // Request to split this idle block into two
            // g_c->size_to_retain will be set to our shorter size
            ReserveStackSpace(here->coroutines, here, g_c->size_to_retain, here->limit);
            MyAssert(false);
            break;
        case Chunk_Enter:
            // request to start a coroutine (ie use the chunk for a coroutine)
            // arrive here with mutex locked
            MyAssert(here->state == Coroutine_Running);
            here->coroutines->active = here;
            EnlargeActiveIfPossible();
            _Cor_Mutex_Unlock(&cors->mutex);
            here->value = here->start(here->entry_param);

            // check the guard
            MyAssert(Guard_Pattern_OK(here->guard));

            here->stack_top = (unsigned char *)StackTopNow();
            _Cor_Mutex_Lock(&here->coroutines->mutex);
            TrimActiveIfPossible();
            here->coroutines->active = NULL;
            MyAssert(here->state == Coroutine_Running);
            Link_Remove(&here->link);
            here->state = Coroutine_Complete;
            List_AddTail(&here->coroutines->inactive, &here->link);
            // Coroutine has completed
            // Loop round to redo the setjmp() - if this coroutine yielded, then the setjmp will
            // need reseting
            break;
        }
    }
}


static void
Coroutine_RunNext(
    void
){
    // arrive here with mutex unlocked
    _Cor_Mutex_Lock(&g_c->waiting_mutex);
    _Cor_Mutex_Lock(&g_c->mutex);
    Coroutine *next = List_Link_Container(Coroutine, link, List_GetHead(&g_c->runable));
    MyAssert(next->state == Coroutine_Running);
    longjmp(next->buf, Chunk_Enter);
    MyAssert(false);
}


/// @brief Ensures there's a spare Coroutine for cors
/// @param cors The Coroutines to ensure a spare for
/// @return true there is a spare; false there isn't
static inline bool
EnsureSpare
(
    Coroutines *cors
){
    if (cors->spare){
        return true;
    }
    cors->spare = malloc(sizeof(*cors->spare));
    return cors->spare != NULL;
}


static Coroutine_Err
Coroutines_ctor(
    Coroutines *cors,
    size_t min_size,
    size_t min_headroom
){
    if (_Cor_Mutex_ctor(&cors->mutex)){
        goto error;
    }
    cors->primary = NULL;
    cors->stack_limit = g_stack_limit;

    List_Init(&cors->all);
    List_Init(&cors->free);
    List_Init(&cors->inactive);
    List_Init(&cors->runable);
    List_Init(&cors->waiting);
    if (_Cor_Mutex_ctor(&cors->waiting_mutex)){
        goto error1;
    }
    if (_Cor_Mutex_Lock(&cors->waiting_mutex)){
        goto error2;
    }

    cors->report.coroutines_created = 0;
    cors->report.coroutines_pool_size = 0;
    cors->report.largest_stack = 0;
    cors->spare = NULL;
    cors->sequence = 0;

    Coroutine *cor = malloc(sizeof(*cor));
    cor->min_size = min_size;
    cor->min_headroom = min_headroom;
    cor->coroutines = cors;
    cor->sequence = cors->sequence++;
    cor->state = Coroutine_Running;
    cor->base = (unsigned char *)&cor;
    if (cors->stack_limit){
        cor->limit = cors->stack_limit;
        cor->guard = StackPointerAdd(cor->limit, -GUARD_PATTERN_SIZE);
        Apply_Guard(cor->guard);
    } else {
        cor->limit = cor->guard = NULL;
    }
    cors->root = cor;
    List_AddHead(&cors->all, &cor->all_link);
    List_AddTail(&cors->runable, &cor->link);
    cors->tip = cors->active = cor;

    cors->report.coroutines_pool_size += 1;

    cors->state = Coroutines_Started;
    return Coroutine_OK;
error2:
    _Cor_Mutex_dtor(&cors->waiting_mutex);
error1:
    _Cor_Mutex_dtor(&cors->mutex);
error:
    return Coroutine_Err_CouldNotInitialiseSystem;
}

static void
Coroutines_dtor(
    Coroutines *cors
){
    _Cor_Mutex_Lock(&cors->mutex);
    cors->state = Coroutines_Stopping;

    MyAssert(List_IsEmpty(&cors->inactive));

    // free the spare
    if (cors->spare){
        free(cors->spare);
    }

    // free all non-spares
    List_Link *link;
    List_Link *next;
    for (link = List_Begin(&cors->all); Link_NextIsLink(link); link = next){
        next = Link_Next(link);
        Coroutine *cor = List_Link_Container(Coroutine, all_link, link) ;
        free(cor);       
    }

    _Cor_Mutex_Unlock(&cors->waiting_mutex);
    _Cor_Mutex_dtor(&cors->waiting_mutex);

    MyAssert(cors->state == Coroutines_Stopping);
    _Cor_Mutex_Unlock(&cors->mutex);
    _Cor_Mutex_dtor(&cors->mutex);
}


Coroutine_Err
Coroutine_RunSystem(
    size_t min_size,
    size_t min_headroom,
    Coroutine_SystemStart start,
    void *value
){
    CHECK_SYSTEM_NOT_RUNNING

    Coroutines cors;
    Coroutine_Err err = Coroutines_ctor(&cors, min_size, min_headroom);
    if (err){
        return err;
    }
    g_c = &cors;
    err = start(value, cors.root);
    g_c = NULL;
    Coroutines_dtor(&cors);
    return err;
}


void
Coroutine_SetStackLimit(
    void *limit
){
    MyAssert(!limit || !g_c || !g_stack_limit || StackPointerDiff((unsigned char *)limit, (unsigned char *)g_stack_limit) <= 0);
    g_stack_limit = limit;
    if (g_c){
        g_c->stack_limit = limit;
        if (g_c->tip){
            g_c->tip->limit = limit;
            g_c->tip->guard = StackPointerAdd(limit, -GUARD_PATTERN_SIZE);
            Apply_Guard(g_c->tip->guard);
        }
    }
}


#if COROUTINE_RECORD_LOWEST_HEADROOM
static size_t
Coroutine_UpdateMinimumHeadroom(
    List_Head *list,
    size_t headroom
){
    for (List_Link *link = List_Begin(list); Link_NextIsLink(link); link = Link_Next(link)){
        Coroutine *cor = List_Link_Container(Coroutine, link, link);
        if (cor->guard){
            size_t chunk_size = Coroutine_Size(cor);
            for (size_t i = 0; i <= chunk_size-GUARD_PATTERN_SIZE; i += GUARD_PATTERN_SIZE){
                if (!Guard_Pattern_OK(StackPointerAdd(cor->guard, -i))){
                    headroom = i < headroom ? i : headroom;
                    break;
                }
            }
        }
    }
    return headroom;
}
#endif


Coroutine_Report
Coroutine_GetReport(
    void
){
    if (g_c){
        size_t headroom;
#if COROUTINE_RECORD_LOWEST_HEADROOM
        _Cor_Mutex_Lock(&g_c->mutex);
        headroom = g_c->report.lowest_headroom;
        headroom = Coroutine_UpdateMinimumHeadroom(&g_c->inactive, headroom);
        headroom = Coroutine_UpdateMinimumHeadroom(&g_c->runable, headroom);
        headroom = Coroutine_UpdateMinimumHeadroom(&g_c->waiting, headroom);
        _Cor_Mutex_Unlock(&g_c->mutex);
#else
        headroom = 0;
#endif
        g_c->report.lowest_headroom = headroom;

        return g_c->report;
    } else {
        Coroutine_Report ret = {0, 0, 0, 0};
        return ret;
    }
}


struct Coroutine_Run_Params {
    Coroutine_Start start;
    void *value;
    void **result;
};

static Coroutine_Err
Coroutine_Run_Starter(
    void *_params,
    Coroutine *root
){
    (void)root;
    struct Coroutine_Run_Params *params = (struct Coroutine_Run_Params *)_params;

    void *res = params->start(params->value);
    if (params->result){
        *params->result = res;
    }

    return Coroutine_OK;
}


Coroutine_Err Coroutine_Run(
    size_t min_size,
    size_t min_headroom,
    Coroutine_Start start,
    void *value,
    void **result
){
    if (!g_c){
        struct Coroutine_Run_Params params = {start, value, result};
        return Coroutine_RunSystem(min_size, min_headroom, Coroutine_Run_Starter, &params);
    }

    // We are in an active coroutine, so call start() directly
    CHECK_STACK_OVERRUN
    void *res = start(value);
    if (result){
        *result = res;
    }

    // no failures, so...
    return Coroutine_OK;
}


static Coroutine *Coroutine_New_Lock_Assumed(
    size_t min_size,
    size_t min_headroom,
    Coroutine_Start start
){
    List_Link *link;

    if (!EnsureSpare(g_c)){
        return NULL;
    }

    Coroutine *cor = NULL;
    for (link = List_Begin(&g_c->free); Link_NextIsLink(link); link = Link_Next(link)){
        Coroutine *candidate = List_Link_Container(Coroutine, link, link);
        MyAssert(candidate->coroutines == g_c);
        MyAssert(candidate->state == Coroutine_Free);
        if (candidate->limit){
            size_t candidate_size = Coroutine_Size(candidate);
            if (candidate_size < min_size){
                continue;
            }
        }
        // candidate has no limit, or is big enough

        // check if it's 'better' (lower in the C stack) than cor
        if (!cor || StackPointerDiff(candidate->base, cor->base) < 0){
            // chunk big enough, and a better choice than cor
            cor = candidate;
        }
    }

    if (!cor){
        return NULL;
    }

    // use the whole block - the tail will be freed in (New) or (Yield)
    cor->min_size = min_size;
    cor->min_headroom = min_headroom;
    cor->state = Coroutine_Idle;
    cor->start = start;
    cor->value = NULL;
    Link_Remove(&cor->link);
    List_AddHead(&g_c->inactive, &cor->link);

    // trim down to an appropriate size
    size_t trimmable_size = TrimmableSize(cor);
    if (trimmable_size){
        // split block into two
        if (EnsureSpare(g_c)){
            g_c->size_to_retain = trimmable_size;
            if (!setjmp(g_c->chunk_allocated)){
                ready_jmp_buf(g_c->chunk_allocated);
                longjmp(cor->buf, Chunk_Split);
            }
        }
    }

    g_c->report.coroutines_created += 1;

    return cor;
}


static void
TrimActiveIfPossible
(
    void
){
    MyAssert(g_c);
    
    // If we're the active coroutine, free the tail
    Coroutine *active = g_c->active;
    MyAssert(active);

    active->stack_top = (unsigned char *)StackTopNow();
    ptrdiff_t timmablesize = TrimmableSize(active);
    if (!timmablesize){
        return;
    }

    if (!EnsureSpare(g_c)){
        return;
    }

    // enough space for a second coroutine so free the unused stack space from active
    if (!setjmp(g_c->chunk_allocated)){
        ready_jmp_buf(g_c->chunk_allocated);
        ReserveStackSpace(g_c, active, timmablesize - StackPointerDiff(active->stack_top, active->base), active->limit);
        MyAssert(false);
    }
}


static void
EnlargeActiveIfPossible(
    void
){
    MyAssert(g_c);
    Coroutine *active = g_c->active;
    MyAssert(active);

    List_Link *next = Link_Next(&active->all_link);
    if (!Link_NextIsLink(next)){
        return;
    }
    Coroutine *cor = List_Link_Container(Coroutine, all_link, next);
    if (cor->state != Coroutine_Free){
        return;
    }

    // Following block is free, merge with this
    active->limit = cor->limit;
    active->guard = cor->guard;
    Link_Remove(&cor->link);
    Link_Remove(&cor->all_link);
    if (g_c->tip == cor){
        g_c->tip = active;
    }
    if (g_c->spare){
        free(cor);
    } else {
        g_c->spare = cor;
    }
}


Coroutine *
Coroutine_New(
    size_t min_size,
    size_t min_headroom,
    Coroutine_Start start
){
    MyAssert(g_c && g_c->state == Coroutines_Started);
    MyAssert(!Coroutine_StackHasOverrun());

    // Make the paramaters make sense
    if (min_size < min_headroom){
        min_size = min_headroom;
    }

    _Cor_Mutex_Lock(&g_c->mutex);

    TrimActiveIfPossible();

    Coroutine *cor = Coroutine_New_Lock_Assumed(min_size, min_headroom, start);

    if (cor && Coroutine_Size(cor) > g_c->report.largest_stack){
        g_c->report.largest_stack = Coroutine_Size(cor);
    }

    EnlargeActiveIfPossible();

    _Cor_Mutex_Unlock(&g_c->mutex);

    return cor;
}


void
Coroutine_Delete(
    Coroutine *cor
){
    MyAssert(!Coroutine_StackHasOverrun());
    if (cor){
        Coroutines *cors = cor->coroutines;
        _Cor_Mutex_Lock(&cors->mutex);
        MyAssert(cor->state == Coroutine_Idle || cor->state == Coroutine_Complete);

#if COROUTINE_RECORD_LOWEST_HEADROOM
        if (cor->guard){
            unsigned char *guard = cor->guard;
            ptrdiff_t chunk_size = Coroutine_Size(cor);
            ptrdiff_t myheadroom;
            for (myheadroom = 0; myheadroom <= chunk_size-(ptrdiff_t)GUARD_PATTERN_SIZE; myheadroom += GUARD_PATTERN_SIZE){
                if (!Guard_Pattern_OK(StackPointerAdd(guard, -myheadroom))){
                    break;
                }
            }
            if (myheadroom < (ptrdiff_t)g_c->report.lowest_headroom || g_c->report.lowest_headroom == 0){
                g_c->report.lowest_headroom = myheadroom;
            }
        }
#endif

        cor->state = Coroutine_Free;
        Link_Remove(&cor->link);

        // insert into free list
        List_AddHead(&cors->free, &cor->link);

        // Check for merge with following Coroutine
        List_Link *link = Link_Next(&cor->all_link);
        if (Link_NextIsLink(link)){
            Coroutine *listcor = List_Link_Container(Coroutine, all_link, link);
            if (listcor->state == Coroutine_Free){
                // merge
                cor->limit = listcor->limit;
                cor->guard = listcor->guard;
                Link_Remove(&listcor->all_link);
                Link_Remove(&listcor->link);
                if (g_c->tip == listcor){
                    g_c->tip = cor;
                }

                // free up the now unused struct
                if (g_c->spare){
                    free(listcor);
                } else {
                    g_c->spare = listcor;
                }
            }
        }

        // check for merge with prev coroutine
        link = Link_Prev(&cor->all_link);
        if (Link_PrevIsLink(link)){
            Coroutine *listcor = List_Link_Container(Coroutine, all_link, link);
            if (listcor->state == Coroutine_Free){
                // merge
                listcor->limit = cor->limit;
                listcor->guard = cor->guard;
                Link_Remove(&cor->all_link);
                Link_Remove(&cor->link);
                if (g_c->tip == cor){
                    g_c->tip = listcor;
                }

                // free up the now unused struct
                if (g_c->spare){
                    free(cor);
                } else {
                    g_c->spare = cor;
                }
            }
        }

        EnlargeActiveIfPossible();
        
        _Cor_Mutex_Unlock(&cors->mutex);
    }
}


// Coroutine_Continue, assuming the mutex is claimed
// return false for success, true for something went wrong
static Coroutine_Err
Coroutine_Continue_(
    Coroutines *cors,
    Coroutine *cor,
    void *value,
    bool early
){
    if (cor->state == Coroutine_Running){
        // already running
        return Coroutine_OK;
    }
    if (cor->state != Coroutine_Idle && cor->state != Coroutine_Waiting){
        return Coroutine_Err_WrongState;
    }
    cor->entry_param = value;
    cor->state = Coroutine_Running;
    Link_Remove(&cor->link);
    if ( early ) {
        List_AddHead(&cors->runable, &cor->link);
    } else {
        List_AddTail(&cors->runable, &cor->link);
    }
    _Cor_Mutex_Unlock(&cors->waiting_mutex);
    return Coroutine_OK;
}


Coroutine_Err
Coroutine_Continue(
    Coroutine *cor,
    void *value,
    bool early
){
    MyAssert(!Coroutine_StackHasOverrun());
    Coroutines *cors = cor->coroutines;
    _Cor_Mutex_Lock(&cors->mutex);
    Coroutine_Err err = Coroutine_Continue_(cors, cor, value, early);
    _Cor_Mutex_Unlock(&cors->mutex);
    return err;
}


void *
Coroutine_Yield(
    void *value,
    Coroutine_YieldCallback on_yield,
    void *yield_me
){
    MyAssert(g_c);
    Coroutine *me = g_c->active;
    MyAssert(me);
    MyAssert(!Coroutine_StackHasOverrun());

    _Cor_Mutex_Lock(&g_c->mutex);
    Coroutines *cors = me->coroutines;
    MyAssert(me && me->state == Coroutine_Running && cors == g_c);
    me->stack_top = (unsigned char *)StackTopNow();
    me->value = value;
    me->state = Coroutine_Waiting;

    TrimActiveIfPossible();

    Link_Remove(&me->link);
    if (!List_IsEmpty(&cors->runable)){
        _Cor_Mutex_Unlock(&cors->waiting_mutex);
    }
    List_AddTail(&cors->waiting, &me->link);

    switch (setjmp(me->buf)){
    case Chunk_Initial:
        ready_jmp_buf(me->buf);
        _Cor_Mutex_Unlock(&cors->mutex);
        on_yield(yield_me);
        Coroutine_RunNext();
        MyAssert(false);
        break;
    case Chunk_Enter:
        // arrive here with mutex locked
        cors->active = me;
        MyAssert(!Coroutine_StackHasOverrun());
        // when we return here - we are running again
        MyAssert(me->state == Coroutine_Running);
        EnlargeActiveIfPossible();
        void *res = me->entry_param;
        _Cor_Mutex_Unlock(&cors->mutex);
        return res;
    }
    MyAssert(false);
    return NULL;
}


void *
Coroutine_GetValue(
    Coroutine *cor
){
    return cor->value;
}


Coroutine *
Coroutine_GetActive(
    void
){
    return g_c ? g_c->active : NULL;
}


ptrdiff_t
Coroutine_GetStackHeadroom(
    void
){
    Coroutine *me = g_c ? g_c->active : NULL;
    if (!me){
        // no active coroutine
        if (g_stack_limit){
            return StackPointerDiff(g_stack_limit, (unsigned char *)StackTopNow());
        } else {
            // no information where the stack ends - return something
            // The biggest ptrdiff_t possible
            return PTRDIFF_MAX;
        }
    } else {
        if (me->guard){
            return StackPointerDiff(me->guard, (unsigned char *)StackTopNow());
        } else {
            // The biggest ptrdiff_t possible
            return PTRDIFF_MAX;
        }
    }
}


void *
Coroutine_GetStackHWM(
    void
){
    MyAssert(g_c);
    MyAssert(g_c->state == Coroutines_Started);
    MyAssert(!Coroutine_StackHasOverrun());
    // Find where the guards end
    unsigned char *guard = g_c->active->guard;
    if (guard){
        ptrdiff_t headroom = Coroutine_GetStackHeadroom();
        for (ptrdiff_t i = 0; i <= headroom; i += GUARD_PATTERN_SIZE){
            if (!Guard_Pattern_OK(StackPointerAdd(guard, -i))){
                return StackPointerAdd(guard, i);
            }
        }
    }
    return guard;
}


void
Coroutine_ClearStackForHWM(
    void
){
    MyAssert(g_c);
    MyAssert(!Coroutine_StackHasOverrun());
    unsigned char *guard = g_c->active->guard;
    if (guard){
        ptrdiff_t headroom = Coroutine_GetStackHeadroom();
        for (ptrdiff_t i = 0; i <= headroom; i += GUARD_PATTERN_SIZE){
            Apply_Guard(StackPointerAdd(guard, -i));
        }
    }
}


static bool
Coroutine_CanStartCoroutine_Lock_Assumed(
    size_t min_size
){
    if (!g_c->stack_limit){
        return true;
    }

    // check free list
    List_Link *link;
    for (link = List_Begin(&g_c->free); Link_NextIsLink(link); link = Link_Next(link)){
        Coroutine *cor = List_Link_Container(Coroutine, link, link);
        size_t cor_size = Coroutine_Size(cor);
        if (cor_size >= min_size){
            return true;
        }
    }

    // Check if this coroutine has enough size
    Coroutine *me = g_c->active;
    me->stack_top = (unsigned char *)StackTopNow();
    ptrdiff_t reduced_size = TrimmableSize(g_c->active);
    if (reduced_size && StackPointerDiff(g_c->active->limit, g_c->active->base) - reduced_size > (ptrdiff_t)min_size){
        return true;
    }

    return false;
}


bool
Coroutine_CanStartCoroutine(
    size_t size
){
    MyAssert(g_c && g_c->state == Coroutines_Started);
    MyAssert(!Coroutine_StackHasOverrun());

    _Cor_Mutex_Lock(&g_c->mutex);

    bool result = Coroutine_CanStartCoroutine_Lock_Assumed(size);

    _Cor_Mutex_Unlock(&g_c->mutex);

    return result;
}

static bool
Coroutine_GetUsefulFreeSpace_Lock_Assumed(
    size_t min_size,
    size_t overhead
){
    if (!g_c->stack_limit){
        return SIZE_MAX;
    }

    size_t total = 0;

    // check free list
    List_Link *link;
    for (link = List_Begin(&g_c->free); Link_NextIsLink(link); link = Link_Next(link)){
        Coroutine *cor = List_Link_Container(Coroutine, link, link);
        size_t cor_size = Coroutine_Size(cor);
        if (cor_size >= min_size){
            total += cor_size - overhead;
        }
    }

    return total;
}

size_t
Coroutine_GetUsefulFreeSpace(
    size_t min_size,
    size_t overhead
){
    MyAssert(g_c && g_c->state == Coroutines_Started);
    MyAssert(!Coroutine_StackHasOverrun());

    if (min_size < overhead){
        min_size = overhead;
    }

    _Cor_Mutex_Lock(&g_c->mutex);

    size_t result = Coroutine_GetUsefulFreeSpace_Lock_Assumed(min_size, overhead);

    _Cor_Mutex_Unlock(&g_c->mutex);

    return result;
}

void *
Coroutine_GetCStackTop(
    void
){
    if (!g_c || g_c->tip == g_c->active){
        return (void *)StackTopNow();
    }

    return g_c->tip->stack_top;
}


// Inspired by cpython...
#ifdef __has_builtin
#  define Coroutine__has_builtin(x) __has_builtin(x)
#else
#  define Coroutine__has_builtin(x) 0
#endif

#if !Coroutine__has_builtin(__builtin_frame_address) && !defined(__GNUC__) && !defined(_MSC_VER)
static uintptr_t return_pointer_as_int(char* p) {
    return (uintptr_t)p;
}
#endif

static inline uintptr_t
StackTopNow(void) {
#if Coroutine__has_builtin(__builtin_frame_address) || defined(__GNUC__)
    return (uintptr_t)__builtin_frame_address(0);
#elif defined(_MSC_VER)
    return (uintptr_t)_AddressOfReturnAddress();
#else
    char here;
    /* Avoid compiler warning about returning stack address */
    return return_pointer_as_int(&here);
#endif
}
// ...inspired by cpython


struct Coroutine_ChainParam {
    Coroutine_Start start;
    void *value;
    Coroutine *ret;
};


static void *
Coroutine_ChainFn(
    void *param
){
    struct Coroutine_ChainParam *params = (struct Coroutine_ChainParam *)param;
    void *res = params->start(params->value);
    return (void *)(uintptr_t)Coroutine_Continue(params->ret, res, true);
}


static void
Coroutine_ChainYield(
    void *unused
){
    (void)unused;
}


Coroutine_Err
Coroutine_Chain(
    size_t min_size,
    size_t min_headroom,
    Coroutine_Start start,
    void *value,
    void **result
){
    MyAssert(!Coroutine_StackHasOverrun());
    Coroutine *cor = Coroutine_New(min_size, min_headroom, Coroutine_ChainFn);
    if (!cor){
        // failed
        return Coroutine_Err_NoStack;
    }
    struct Coroutine_ChainParam params = {
        start,
        value,
        Coroutine_GetActive()
    };
    Coroutine_Err err = Coroutine_Continue(cor, &params, true);
    if (err){
        goto error;
    }
    void *res = Coroutine_Yield(NULL, Coroutine_ChainYield, NULL);
    err = (Coroutine_Err)(uintptr_t)Coroutine_GetValue(cor);
error:
    Coroutine_Delete(cor);
    if (!err && result){
        *result = res;
    }
    // success! ...probably
    return err;
}


bool
Coroutine_IsRunning(
    Coroutine *cor
){
    int state = cor->state;
    return state == Coroutine_Running || state == Coroutine_Waiting;
}


bool Coroutine_IsComplete(
    Coroutine *cor
){
    int state = cor->state;
    return state == Coroutine_Complete;
}


bool
Coroutine_IsStarted(
    void
){
    return g_c && g_c->state == Coroutines_Started;
}

void
Coroutine_Dump_(
    void
){
    char *state_to_text[] = {
        "Free",
        "Idle",
        "Running",
        "Waiting",
        "Complete"
    };
    unsigned idx = 0;
    List_Link *link;
    for (link = List_Begin(&g_c->all); Link_NextIsLink(link); link = Link_Next(link)){
        Coroutine *cor = List_Link_Container(Coroutine, all_link, link);
        printf("%d) %p %p %zu (%s) %s\n", idx++, cor, cor->base, StackPointerDiff(cor->limit, cor->base), state_to_text[cor->state], cor == g_c->tip ? " (TIP)" : "");
    }
}
#include "coroutine_names_undef.h"
