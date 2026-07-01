#ifndef PYCORE_COROUTINE_H
#define PYCORE_COROUTINE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "pycore_cor_platform_inc.h"
#include "pycore_coroutine_names_def.h"

///////////////////////////////////////////////////////////////////////////////
// Coroutine
//
// Coroutines for C, based on setjmp/longjmp.
// Thread safe - each thread has its own coroutine system
// Coroutines are cooperatively scheduled
// Coroutines have their own stack (currently 16K each)
// A coroutine can be continued, queried, or deleted on a different thread.
//
// Usage:
//   Coroutine_StartSystem();  // call once per thread before using coroutines
//   Coroutine *co = Coroutine_New(start_function);
//   void *result;
//   if (Coroutine_Run(co, initial_value, &result)) {
//       // Handle the failure
//   }
//   Coroutine_Delete(co);
//   Coroutine_StopSystem();   // call once per thread when done with coroutines
//
// Inside the coroutine function:
//   void *value = Coroutine_Yield(yield_value, on_yield, this);
//   ...
//   return return_value;
//
// To create a coroutine:
//   Coroutine *co = Coroutine_New(start_function);
// To start or continue a coroutine:
//   void *result = Coroutine_Continue(co, value, early);
//   // early=true puts the coroutine at the head of the run queue
//   // early=false puts the coroutine at the tail of the run queue
// To yield from inside a coroutine:
//   void *value = Coroutine_Yield(yield_value, on_yield, this);
//   // on_yield is called before the next coroutine is run
//   // 'this' is passed to on_yield as its parameter
//   // value is the value passed to Coroutine_Continue
// To delete a coroutine:
//   Coroutine_Delete(co);
// To get the value yielded from, or returned by a corotuine:
//   void *value = Coroutine_GetValue(co);
// To get the currently running coroutine (NULL if none):
//   Coroutine *co = Coroutine_GetActive();
// To check if a coroutine is currently running:
//   bool running = Coroutine_IsRunning(co);
//
// Notes:
// Coroutine is not expected to be used directly, but as a foundation for
// higher level constructs such as Generators, Async, etc.
//
///////////////////////////////////////////////////////////////////////////////


// The stack is used as follows:
//   +------------------+  <- stack top
//   | coroutine header |  <- more claimed as needed in Coroutine_New
//   +------------------+  <-
//   | coroutine stack  |  <-
//   +------------------+  <-
//   | coroutine header |
//   +------------------+
//   | coroutine stack  |
//   +------------------+
//   | coroutine header |
//   +------------------+
//   | coroutine stack  |
//   +------------------+
//   | coroutine header |
//   +------------------+
//   | coroutine stack  |
//   +------------------+
//   | coroutine header |
//   +------------------+
//   | startup space    |  <- set aside by Coroutine_StartSystem
//   +------------------+
//   | caller           |  <- This calls Coroutine_StartSystem etc
//   +------------------+
//   | used stack       |
//   +------------------+  <- stack bottom

// Each coroutine has this much stack:
// For Python, we set it to 17 * (enough for a PyEval_EvalDefault), so we get at least 7
// calls deep before we need a new chunk, ie maximum multi-chunk wastage is under 6% address space.
//
// There's a trade-off between smaller chunk sizes, which allow more async tasks to co-exist
// on a thread, and larger chunk sizes which waste less memory in part-used chunks.
//
// ... which means 10000 async tasks need a 2.6 GB stack, which fits comfortably in the address map.
// 
// Note, when developing the use of Coroutine in Python, the author found the following used
// excessive amounts of stack space:
// Tk_Init: on an Intel 64 bit Mac it used 72k.
// _decimal multplies of big decimal numbers: 256k+640 (2 x 128k buffers in squaretrans_pow2() + workings)
//
// On 64 bit macos, PYOS_STACK_MARGIN_BYTES is 2k * sizeof(void *), ie 16k, or 17 of those, 272k, should give enough slack to operate well.


#ifndef Coroutine_API_FUNC
    #define Coroutine_API_FUNC(T) extern T
#endif

// No coroutine will ask for less stack than this
#ifndef COROUTINE_MINIMUM_STACK_SIZE
    #define COROUTINE_MINIMUM_STACK_SIZE (4096 * sizeof(void *))
#endif

// When Coroutine is started, an amount of stack is set aside to give
// the caller of Coroutine_StartSystem a bit of room to work before calling
// Coroutine_Run(), that is this amount:
#ifndef COROUTINE_STARTUP_STACK_SIZE
    #ifndef _NDEBUG
        #define COROUTINE_STARTUP_STACK_SIZE (1024 * sizeof(void *))
    #else
        #define COROUTINE_STARTUP_STACK_SIZE (128 * sizeof(void *))
    #endif
#endif

// This is *expensive* to turn on, especially if you have lots of stack pieces (eg when there's lots of Tasks)
#ifndef COROUTINE_CHECK_INTEGRITY_ON_STACK_CHECK
    #define COROUTINE_CHECK_INTEGRITY_ON_STACK_CHECK 0
#endif

#ifndef COROUTINE_RECORD_LOWEST_HEADROOM
    #define COROUTINE_RECORD_LOWEST_HEADROOM 1
#endif

// Returned by Coroutine_StopSystem(), this summarises the coroutine session
typedef struct Coroutine_Report {
    unsigned coroutines_created;
    unsigned coroutines_pool_size;
    size_t lowest_headroom;
    size_t largest_stack;
} Coroutine_Report;

typedef enum Coroutine_Err {
    Coroutine_OK = 0,
    Coroutine_Err_SystemNotRunning,
    Coroutine_Err_SystemRunning,
    Coroutine_Err_NoStack,
    Coroutine_Err_CoroutineFromWrongThread,
    Coroutine_Err_ACoroutineIsAlreadyRunning,
    Coroutine_Err_ExitWithRunningCoroutines,
    Coroutine_Err_StackOverrun,
    Coroutine_Err_InternalInsistency,
    Coroutine_Err_CouldNotInitialiseSystem,
    Coroutine_Err_WrongState,
    Coroutine_Err_Canceled
} Coroutine_Err;

typedef struct Coroutine Coroutine;

typedef void (*Coroutine_YieldCallback)(void *me);
typedef Coroutine_Err (*Coroutine_SystemStart)(void *, Coroutine *);
typedef void *(*Coroutine_Start)(void *);

Coroutine_API_FUNC(void) Coroutine_SetStackLimit(void *);
Coroutine_API_FUNC(Coroutine_Report) Coroutine_GetReport(void);
#ifndef NDEBUG
    Coroutine_API_FUNC(Coroutine_Err) Coroutine_CheckIntegrity(void);
#else
    static inline Coroutine_Err Coroutine_CheckIntegrity(void){return Coroutine_OK;}
#endif
Coroutine_API_FUNC(Coroutine *) Coroutine_New(size_t min_size, size_t min_headroom, Coroutine_Start start);
Coroutine_API_FUNC(Coroutine_Err) Coroutine_RunSystem(size_t min_size, size_t min_headroom, Coroutine_SystemStart start, void *value);
Coroutine_API_FUNC(Coroutine_Err) Coroutine_Run(size_t min_size, size_t min_headroom, Coroutine_Start start, void *value, void **result);
Coroutine_API_FUNC(void) Coroutine_Delete(Coroutine *cor);
Coroutine_API_FUNC(Coroutine_Err) Coroutine_Continue(Coroutine *cor, void *value, bool early);
Coroutine_API_FUNC(void *) Coroutine_Yield(void *value, Coroutine_YieldCallback on_yield, void *me);
Coroutine_API_FUNC(void *) Coroutine_GetValue(Coroutine *cor);
Coroutine_API_FUNC(Coroutine *) Coroutine_GetActive(void);
Coroutine_API_FUNC(ptrdiff_t) Coroutine_GetStackHeadroom(void);
Coroutine_API_FUNC(void *) Coroutine_GetStackHWM(void);
Coroutine_API_FUNC(void) Coroutine_ClearStackForHWM(void);
Coroutine_API_FUNC(bool) Coroutine_CanStartCoroutine(size_t size);
Coroutine_API_FUNC(size_t) Coroutine_GetUsefulFreeSpace(size_t min_size, size_t overhead);
Coroutine_API_FUNC(void *) Coroutine_GetCStackTop(void);
Coroutine_API_FUNC(Coroutine_Err) Coroutine_Chain(size_t min_size, size_t min_headroom, Coroutine_Start start, void *value, void **result);
Coroutine_API_FUNC(Coroutine_Err) Coroutine_CallWithMaxStack(Coroutine_Start start, void *value, void **result);
Coroutine_API_FUNC(bool) Coroutine_IsStarted(void);
Coroutine_API_FUNC(bool) Coroutine_IsRunning(Coroutine *cor);
Coroutine_API_FUNC(bool) Coroutine_IsComplete(Coroutine *cor);

Coroutine_API_FUNC(void) Coroutine_Dump_(void);

#include "pycore_coroutine_names_undef.h"
#endif
