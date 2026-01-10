#include "pycore_coroutine.h"
#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
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

static inline List_Link *List_GetTail(
    const List_Head *list
){
    return List_IsEmpty(list) ? NULL : list->back.link.prev;
}


#define OFFSETOF(Container, Field) ((char *)&((Container *)4)->Field - (char *)(Container *)4)
#define List_Link_Container(Container, Link, link) ((Container *)((char *)(link) - OFFSETOF(Container, Link)))


static inline void List_Init(
    List_Head *list
){
    list->fwd.link.next = &list->back.link;
    list->fwd.link.prev = NULL;
    list->back.link.prev = &list->fwd.link;
}


static inline void Link_AddAfter(
    List_Link *link,
    List_Link *after
){
    link->next = after->next;
    link->prev = after;
    after->next->prev = link;
    after->next = link;
}


static inline void List_AddHead(
    List_Head *list,
    List_Link *link
){
    Link_AddAfter(link, &list->fwd.link);
}


static inline void Link_AddBefore(
    List_Link *link,
    List_Link *before
){
    link->prev = before->prev;
    link->next = before;
    before->prev->next = link;
    before->prev = link;
}


static inline void List_AddTail(
    List_Head *list,
    List_Link *link
){
    Link_AddBefore(link, &list->back.link);
}


static inline void Link_Remove(
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
    Coroutines_Starting,
    Coroutines_Started,
    Coroutines_Active,
    Coroutines_Stopping
};

enum {
    Chunk_Initial,
    Chunk_Create,
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
    Coroutines *coroutines;     // so can work with it off-thread
    List_Link link;             // for whichever list it's on
    List_Link all_link;         // list of all Coroutines
    jmp_buf buf;                // how to get back to it
    unsigned char *prev_limit;  // the previous Coroutine's stack limit
    unsigned char *base;        // where the base (high address) of this Coroutine's stack is
    unsigned char *limit;       // where the limit (low address) of this Coroutine's stack is
    unsigned char *guard;       // where the stack overrun guard is
    size_t size;
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
    size_t gap_before;      // bytes between previous's stack_top and next's Coroutine
    size_t gap_after;       // bytes between Coroutine and stack_base

    // singletons
    Coroutine *tip;     // top of stack chunk
    Coroutine *active;  // currently running coroutine
    Coroutine *primary; // Coroutine_Run coroutine
    unsigned char *stack_limit;  // when not NULL, where the stack finishes

    // lists
    List_Head all;          // all Coroutines (in address order)
    List_Head free;         // free Coroutines
    List_Head inactive;     // idle or complete
    List_Head runable;      // running or waiting to run
    List_Head waiting;      // yielded / waiting to run
    _Cor_Mutex waiting_mutex;

    // Summary of the system
    Coroutine_Report report;

    // state
    char state;
};

_Cor_thread_local Coroutines *g_c;
_Cor_thread_local unsigned char *g_stack_limit;

static void ReserveStackSpace(Coroutines *cors, Coroutine *parent, size_t chunk_size, unsigned char *childs_limit);
static void stack_chunk_base(Coroutines *cors, Coroutine *parent, unsigned char *prev_limit, unsigned char *limit);


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


static bool Coroutine_StackHasOverrun(void){
    unsigned char *stack_top = StackTopNow();
    unsigned char *stack_limit = g_c ? g_c->stack_limit : NULL;
    if (stack_limit && stack_top < stack_limit){
        // printf("top %p < limit %p\n", stack_top, stack_limit);
        // current stack top is beyond limit - we are overrunning NOW
        return true;
    }
    Coroutine *me = g_c ? g_c->active : NULL;
    if (!me){
        return false;
    }
    if (me->guard){
        bool ret = !Check_Guard(me->guard);
        // if (ret){
        //     printf("Broken guard me=%p; me->guard=%p me->limit-%p stack top=%p\n", me, me->guard, me->limit, stack_top);
        // }
        return ret;
    }
    bool ret = stack_top < me->limit;
    // if (ret){
    //     printf("stack top %p < me->limit %p\n", stack_top, me->limit);
    // }
    return ret;
}


static void ReserveStackSpace(
    Coroutines *cors,
    Coroutine *parent,
    size_t chunk_size,
    unsigned char *childs_limit
){
    unsigned char *chunk_of_stack = alloca(chunk_size);
#if COROUTINE_RECORD_LOWEST_HEADROOM
    for (size_t i = 0; i <= chunk_size-GUARD_PATTERN_SIZE; i += GUARD_PATTERN_SIZE){
        Apply_Guard(&chunk_of_stack[i]);
    }
#else
    Apply_Guard(chunk_of_stack);
#endif
    if (parent){
        parent->guard = chunk_of_stack;
        parent->limit = chunk_of_stack;
        parent->base = chunk_of_stack + chunk_size;
    }
    stack_chunk_base(cors, parent, chunk_of_stack, childs_limit);
}


static void stack_chunk_base(
    Coroutines *cors,
    Coroutine *parent,
    unsigned char *prev_limit,
    unsigned char *limit
){
    Coroutine here;
    here.coroutines = cors;
    here.state = Coroutine_Free;
    here.prev_limit = prev_limit;
    here.size = 0;
    here.base = NULL;
    here.guard = limit;
    here.limit = limit;
    if (limit){
        here.base = (unsigned char *)&here - cors->gap_after;
        here.size = here.base - here.limit;
        Apply_Guard(limit);
    }

    // insert into all list
    if (parent){
        Link_AddAfter(&here.all_link, &parent->all_link);
    } else {
        List_AddHead(&cors->all, &here.all_link);
    }
    // add to free list
    List_AddTail(&cors->free, &here.link);

    cors->report.coroutines_pool_size += 1;

    if (!cors->tip || &here < cors->tip){
        cors->tip = &here;
    }

    for(;;){
        switch (setjmp(here.buf)) {
        case Chunk_Initial:
            if (here.state == Coroutine_Free){
                // return to the coroutine allocator
                longjmp(cors->chunk_allocated, 1);
            } else {
                assert(here.state == Coroutine_Complete);
                // we finish here to ensure the setjmp is redone
                if (cors->primary == &here) {
                    // if primary coroutine - return to Coroutine_Run
                    longjmp(cors->controller, Coroutines_CoroutineComplete);
                }
                _Cor_Mutex_Unlock(&cors->mutex);
                Coroutine_RunNext();
                assert(false);
            }
        case Chunk_Create:
            // Request to create a new chunk on the stack
            // We're here if the coroutine is:
            // Allocated, but not 'run' (Coroutine_Idle)
            // Run, but not not entered yet (Coroutine_Running)
            // Completed (Coroutine_Complete)
            // Free, and the coroutines system is starting - we're characterising the system
            assert(here.state == Coroutine_Idle ||
                here.state == Coroutine_Running ||
                here.state == Coroutine_Complete ||
                (here.state == Coroutine_Free && cors->state == Coroutines_Starting));
            ReserveStackSpace(here.coroutines, &here, here.size, NULL);
            assert(false);
        case Chunk_Split:
            // Request to split this free block into two
            // here.size will be set to our shorter size
            ReserveStackSpace(here.coroutines, &here, here.size, here.limit);
            assert(false);
        case Chunk_Enter:
            // request to start a coroutine (ie use the chunk for a coroutine)
            // arrive here with mutex locked
            assert(here.state == Coroutine_Running);
            here.coroutines->active = &here;
            _Cor_Mutex_Unlock(&cors->mutex);
            here.value = here.start(here.entry_param);

            // check the guard
            assert(Check_Guard(here.guard));

            _Cor_Mutex_Lock(&here.coroutines->mutex);
            here.coroutines->active = NULL;
            assert(here.state == Coroutine_Running);
            Link_Remove(&here.link);
            here.state = Coroutine_Complete;
            List_AddTail(&here.coroutines->inactive, &here.link);
            // Coroutine has completed
            // Loop round to redo the setjmp() - if this coroutine yielded, then the setjmp will
            // need reseting
        }
    }
}


static void Coroutine_RunNext(void)
{
    // arrive here with mutex unlocked
    _Cor_Mutex_Lock(&g_c->waiting_mutex);
    _Cor_Mutex_Lock(&g_c->mutex);
    Coroutine *next = List_Link_Container(Coroutine, link, List_GetHead(&g_c->runable));
    assert(next->state == Coroutine_Running);
    longjmp(next->buf, Chunk_Enter);
    assert(false);
}


void Coroutine_StartSystem(void)
{
    assert(!g_c);

    Coroutines *cors = _Cor_Malloc(sizeof(*g_c));
    assert(cors);

    cors->state = Coroutines_Starting;
    _Cor_Mutex_ctor(&cors->mutex);

    cors->tip = NULL;
    cors->active = NULL;
    cors->primary = NULL;
    cors->stack_limit = g_stack_limit;

    List_Init(&cors->all);
    List_Init(&cors->free);
    List_Init(&cors->inactive);
    List_Init(&cors->runable);
    List_Init(&cors->waiting);
    _Cor_Mutex_ctor(&cors->waiting_mutex);
    _Cor_Mutex_Lock(&cors->waiting_mutex);

    cors->report.coroutines_created = 0;
    cors->report.coroutines_pool_size = 0;
    cors->report.largest_stack = 0;

    // Charactersize the system...
    if (!setjmp(cors->chunk_allocated)){
        ReserveStackSpace(cors, NULL, COROUTINE_STARTUP_STACK_SIZE, NULL);
    }
    Coroutine *cor = List_Link_Container(Coroutine, link, List_GetHead(&cors->free));
    cor->size = COROUTINE_STARTUP_STACK_SIZE;
    if (!setjmp(cors->chunk_allocated)){
        longjmp(cor->buf, Chunk_Create);
    }
    cors->gap_before = cor->prev_limit - (unsigned char *)cor;
    cors->gap_after = (unsigned char *)cor - cor->base;
    // ...charactersize the system

    // discard what we've just created
    List_Init(&cors->all);
    List_Init(&cors->free);
    cors->tip = NULL;

    cors->state = Coroutines_Started;

    g_c = cors;
}


void Coroutine_SetStackLimit(void *limit){
    assert(!limit || !g_c || !(g_c->state == Coroutines_Started || g_c->state == Coroutines_Active) || (unsigned char *)limit < (unsigned char *)g_c->tip || !g_c->tip);
    g_stack_limit = limit;
    if (g_c){
        g_c->stack_limit = limit;
    }
}


Coroutine_Report Coroutine_StopSystem(void)
{
    assert(g_c);
    assert(g_c->state == Coroutines_Started);
    _Cor_Mutex_Lock(&g_c->mutex);
    g_c->state = Coroutines_Stopping;

    uintptr_t stackminheadroom;
#if COROUTINE_RECORD_LOWEST_HEADROOM
    stackminheadroom = g_c->report.largest_stack;
    for (List_Link *link = g_c->free.fwd.link.next; Link_NextIsLink(link); link = Link_Next(link)){
        Coroutine *cor = List_Link_Container(Coroutine, link, link);
        if (cor->guard){
            for (uintptr_t i = 4; i < cor->size-3; i += 4){
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
    g_c->report.lowest_headroom = stackminheadroom;

    assert(List_IsEmpty(&g_c->inactive));
    _Cor_Mutex_Unlock(&g_c->waiting_mutex);
    _Cor_Mutex_dtor(&g_c->waiting_mutex);

    assert(g_c->state == Coroutines_Stopping);
    _Cor_Mutex_Unlock(&g_c->mutex);
    _Cor_Mutex_dtor(&g_c->mutex);

    Coroutine_Report ret = g_c->report;

    _Cor_Free(g_c);
    g_c = NULL;

    return ret;
}


void Coroutine_Run_Coroutine(
    Coroutine *cor,
    void *value
){
    Coroutines *cors = cor->coroutines;

    // Can't Coroutine_Run_Coroutine() off-thread
    assert(g_c == cors);

    _Cor_Mutex_Lock(&cors->mutex);
    assert(cors->state == Coroutines_Started);
    cors->state = Coroutines_Active;
    cors->primary = cor;

    _Coroutine_Continue(cor, value, true);

    if (!setjmp(cors->controller)){
        _Cor_Mutex_Unlock(&cors->mutex);

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
    size_t stack,
    Coroutine_Start start,
    void *value,
    void **result
){
    if (g_c && g_c->active){
        void *res = start(value);
        if (result){
            *result = res;
        }
        // no failures, so...
        return false;
    }
    assert(!g_c || g_c->state == Coroutines_Started);
    bool need_start = !g_c;
    if (need_start){
        Coroutine_StartSystem();
    }
    Coroutine *cor = _Py_Coroutine_New(stack, start);
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


static void Coroutine_FreeToIdle(
    Coroutine *cor,
    Coroutine_Start start
){
    assert(cor->state == Coroutine_Free);
    cor->state = Coroutine_Idle;
    cor->start = start;
    cor->value = NULL;
    Link_Remove(&cor->link);
    List_AddHead(&g_c->inactive, &cor->link);

    g_c->report.coroutines_created += 1;
}


static void Coroutine_FreeToIdleSize(
    Coroutine *cor,
    Coroutine_Start start,
    size_t size
){
    assert(!cor->guard);
    cor->size = size;
    cor->base = (unsigned char *)cor - g_c->gap_after;
    cor->limit = cor->base - cor->size;
    Coroutine_FreeToIdle(cor, start);
}


static Coroutine *Coroutine_New_Lock_Assumed(
    size_t size,
    Coroutine_Start start
){
    List_Link *link;

    if (!g_c->tip){
        // no tip - time to create one

        // we're the non-Coroutine which starts the Coroutine system.
        // Add a single free block
        if (!setjmp(g_c->chunk_allocated)){
            ReserveStackSpace(g_c, NULL, COROUTINE_STARTUP_STACK_SIZE, NULL);
        }
    }

    Coroutine *cor = NULL;
    for (link = List_Begin(&g_c->free); Link_NextIsLink(link); link = Link_Next(link)){
        Coroutine *candidate = List_Link_Container(Coroutine, link, link);
        if (!candidate->guard) {
            // this must be the tip
            assert(candidate == g_c->tip);

            // If this is the only Coroutine in the system, go ahead and use it regardless of size.
            // Note: there can only be one free block if there's no other sort of blocks as we merge on free
            if (List_IsEmpty(&g_c->inactive) &&
                List_IsEmpty(&g_c->runable) &&
                List_IsEmpty(&g_c->waiting) ){
                if (g_c->stack_limit){
                    size_t available = (unsigned char *)candidate - g_c->stack_limit - g_c->gap_after;
                    size = available < size ? available : size;
                }
                Coroutine_FreeToIdleSize(candidate, start, size);
                return candidate;
            }

            // Not the only coroutine in the system - check size
            if (g_c->stack_limit){
                // there's a limit - see what that space allows....
                size_t available = (unsigned char *)candidate - g_c->stack_limit - g_c->gap_after;

                if (available < size){
                    // not enough space for this coroutine
                    // printf("Not enough stack space (A) %ld\n", available);
                    return NULL;
                }
                
                if (available < size + g_c->gap_before + g_c->gap_after + COROUTINE_MINIMUM_STACK_SIZE) {
                    // not enough space for another coroutine - use all the space for this one
                    size = available;
                }
            }
            Coroutine_FreeToIdleSize(candidate, start, size);
            return candidate;
        }
        if (candidate->size >= size && candidate > cor){
            // chunk big enough, and a better choice than cor
            cor = candidate;
        }
    }

    if (cor){
        // - work out whether we're splitting or using the whole chunk
        if (cor->size >= size + g_c->gap_before + g_c->gap_after + COROUTINE_MINIMUM_STACK_SIZE){
            // enough space for a second coroutine so split this free block
            cor->size = size;
            if (!setjmp(g_c->chunk_allocated)){
                longjmp(cor->buf, Chunk_Split);
            }
        }
        // cor now ready to use
        Coroutine_FreeToIdle(cor, start);
        return cor;
    }

    // No big-enough free blocks - check if there's space beyond the tip block

    if (g_c->stack_limit) {
        ptrdiff_t available = (unsigned char *)g_c->tip->limit - g_c->gap_before - g_c->gap_after - g_c->stack_limit;
        if (available < (ptrdiff_t)size){
            // no space for a new stack block
            // printf("Not enough stack space (B) %p %zu %zu %p %ld\n", g_c->tip->limit, g_c->gap_before, g_c->gap_after, g_c->stack_limit, available);
            // printf("g_c->tip = %p; tip-limit = %ld; tip->size = %zu\n", g_c->tip, (unsigned char *)g_c->tip - g_c->tip->limit, g_c->tip->size);
            return NULL;
        }
    }
    Coroutine *tip = g_c->tip;
    Coroutine *me = g_c->active;
    if (tip == me) {
        if (!setjmp(g_c->chunk_allocated)){
            ReserveStackSpace(g_c, me, StackTopNow() - me->limit, NULL);
        }
    } else {
        if (!setjmp(g_c->chunk_allocated)){
            longjmp(tip->buf, Chunk_Create);
        }
    }

    cor = List_Link_Container(Coroutine, link, List_GetTail(&g_c->free));
    assert(cor->state == Coroutine_Free);
    cor->size = size;
    cor->limit = (unsigned char *)cor - g_c->gap_after - size;
    cor->state = Coroutine_Idle;
    cor->start = start;
    cor->value = NULL;
    Link_Remove(&cor->link);
    List_AddHead(&g_c->inactive, &cor->link);

    g_c->report.coroutines_created += 1;
    return cor;
}


Coroutine *_Py_Coroutine_New(
    size_t stack,
    Coroutine_Start start
){
    assert(g_c);
    assert((g_c->state == Coroutines_Started && List_IsEmpty(&g_c->inactive)) || g_c->state == Coroutines_Active);
    assert(!Coroutine_StackHasOverrun());

    _Cor_Mutex_Lock(&g_c->mutex);

    Coroutine *cor = Coroutine_New_Lock_Assumed(stack, start);

    if (cor && cor->size > g_c->report.largest_stack){
        g_c->report.largest_stack = cor->size;
    }

    _Cor_Mutex_Unlock(&g_c->mutex);

    return cor;
}


void _Py_Coroutine_Delete(
    Coroutine *cor
){
    assert(!Coroutine_StackHasOverrun());
    if (cor){
        Coroutines *cors = cor->coroutines;
        _Cor_Mutex_Lock(&cors->mutex);
        assert(cor->state == Coroutine_Idle || cor->state == Coroutine_Complete);
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
                cor->size += cor->limit - listcor->limit;
                cor->limit = listcor->limit;
                cor->guard = listcor->guard;
                Link_Remove(&listcor->all_link);
                Link_Remove(&listcor->link);
                if (g_c->tip == listcor){
                    g_c->tip = cor;
                }
            }
        }

        // check for merge with prev coroutine
        link = Link_Prev(&cor->all_link);
        if (Link_PrevIsLink(link)){
            Coroutine *listcor = List_Link_Container(Coroutine, all_link, link);
            if (listcor->state == Coroutine_Free){
                // merge
                listcor->size += listcor->limit - cor->limit;
                listcor->limit = cor->limit;
                listcor->guard = cor->guard;
                Link_Remove(&cor->all_link);
                Link_Remove(&cor->link);
                if (g_c->tip == cor){
                    g_c->tip = listcor;
                }
            }
        }
        
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
    Link_Remove(&cor->link);
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
    assert(!Coroutine_StackHasOverrun());
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
    assert(g_c);
    Coroutine *me = g_c->active;
    assert(me);
    assert(!Coroutine_StackHasOverrun());

    _Cor_Mutex_Lock(&g_c->mutex);
    Coroutines *cors = me->coroutines;
    assert(me && me->state == Coroutine_Running && cors == g_c);
    me->stack_top = StackTopNow();
    me->value = value;
    me->state = Coroutine_Waiting;

    Link_Remove(&me->link);
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
        assert(me == g_c->tip);
        ReserveStackSpace(me->coroutines, me, me->stack_top - me->limit, NULL);
        assert(false);
    case Chunk_Enter:
        // arrive here with mutex locked
        cors->active = me;
        assert(!Coroutine_StackHasOverrun());
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
    return g_c ? g_c->active : NULL;
}


intptr_t _Py_Coroutine_GetStackHeadroom(void){
    assert(g_c);
    assert(!Coroutine_StackHasOverrun());
    Coroutine *me = g_c->active;
    if (!me){
        // no active coroutine
        unsigned char *stack_limit = g_c->stack_limit;
        if (stack_limit){
            // no stack limit - assume we'll use COROUTINE_STACK_SIZE
            return StackTopNow() - stack_limit;
        } else {
            // no information where the stack ends - return something
            return COROUTINE_MINIMUM_STACK_SIZE;
        }
    }
    return StackTopNow() - me->limit;
}


// This is used to avoid compiler warnings about returning the address of a local
static inline void *StopAddressWarnings(void *p)
{
    return p;
}


void *Coroutine_GetStackHWM(void){
    assert(g_c);
    assert(g_c->state == Coroutines_Active);
    assert(!Coroutine_StackHasOverrun());
    // Find where the guards end
    unsigned char *guard;
    for (guard = g_c->active->guard; Check_Guard(guard); guard += 4){
        // do nothing
    }
    return guard;
}


void Coroutine_ClearStackForHWM(void){
    assert(g_c);
    assert(g_c->state == Coroutines_Active);
    assert(!Coroutine_StackHasOverrun());
    unsigned char *end = StackTopNow() - GUARD_PATTERN_SIZE;
    for (unsigned char *guard = g_c->active->guard+GUARD_PATTERN_SIZE; guard <= end; guard += GUARD_PATTERN_SIZE){
        Apply_Guard(guard);
    }
}


static bool Coroutine_CanStartCoroutine_Lock_Assumed(
    size_t size
){
    if (!g_c->stack_limit){
        return true;
    }

    if (!g_c->tip){
        return true;
    }

    if (g_c->tip->state == Coroutine_Free){
        // last block is free
        if ((unsigned char *)g_c->tip - g_c->stack_limit >= (ptrdiff_t)(g_c->gap_after + size)){
            // enough room in free block, which is the last block
            return true;
        }
    } else {
        // last block is allocated
        if (g_c->tip->limit - g_c->stack_limit >= (ptrdiff_t)(g_c->gap_before + g_c->gap_after + size)){
            // enough room after the last block, which is allocated
            return true;
        }
    }

    // not enough room between allocated blocks and stack limit, so check free list
    List_Link *link;
    for (link = List_Begin(&g_c->free); Link_NextIsLink(link); link = Link_Next(link)){
        Coroutine *cor = List_Link_Container(Coroutine, link, link);
        if (cor->size >= size){
            return true;
        }
    }

    return false;
}


bool _Py_Coroutine_CanStartCoroutine(
    size_t size
){
    assert(g_c);
    assert(g_c->state == Coroutines_Started || g_c->state == Coroutines_Active);
    assert(!Coroutine_StackHasOverrun());

    _Cor_Mutex_Lock(&g_c->mutex);

    bool result = Coroutine_CanStartCoroutine_Lock_Assumed(size);

    _Cor_Mutex_Unlock(&g_c->mutex);

    return result;
}

void *Coroutine_GetCStackTop(void){
    assert(!Coroutine_StackHasOverrun());
    if ((g_c->state == Coroutines_Started || g_c->state == Coroutines_Active) && g_c->tip != g_c->active) {
        return g_c->tip->stack_top;
    } else {
        return StackTopNow();
    }
}


static unsigned char *StackTopNow(void){
    unsigned char here[4];
    return StopAddressWarnings(here);
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
    size_t size,
    Coroutine_Start start,
    void *value,
    void **result
){
    assert(Check_Guard(_Py_Coroutine_GetActive()->guard));
    Coroutine *cor = _Py_Coroutine_New(size, Coroutine_ChainFn);
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
    return g_c && (g_c->state == Coroutines_Active || g_c->state == Coroutines_Started);
}

void _Coroutine_Dump(void){
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
        printf("%d) %p (%s) %ld%s\n", idx++, cor, state_to_text[cor->state], cor->size, cor == g_c->tip ? " (TIP)" : "");
    }
}
