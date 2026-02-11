#ifndef Py_INTERNAL_COROUTINE_H
#define Py_INTERNAL_COROUTINE_H

#include "Python.h"
#include <stdbool.h>
#include <stdint.h>

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

// No coroutine will ask for less stack than this
#ifndef COROUTINE_MINIMUM_STACK_SIZE
    #define COROUTINE_MINIMUM_STACK_SIZE PYOS_STACK_MARGIN_BYTES
#endif

// When Coroutine is started, an amount of stack is set aside to give
// the caller of Coroutine_StartSystem a bit of room to work before calling
// Coroutine_Run(), that is this amount:
#ifndef COROUTINE_STARTUP_STACK_SIZE
    #define COROUTINE_STARTUP_STACK_SIZE (128 * sizeof(void *))
#endif

// Returned by Coroutine_StopSystem(), this summarises the coroutine session
typedef struct Coroutine_Report {
    unsigned coroutines_created;
    unsigned coroutines_pool_size;
    size_t lowest_headroom;
    size_t largest_stack;
} Coroutine_Report;

typedef struct Coroutine Coroutine;

typedef void (*Coroutine_YieldCallback)(void *me);
typedef void *(*Coroutine_Start)(void *);

extern void Coroutine_StartSystem(void);
extern void Coroutine_SetStackLimit(void *);
extern Coroutine_Report Coroutine_StopSystem(void);
extern void Coroutine_Run_Coroutine(Coroutine *cor, void *value);
extern bool Coroutine_Run(size_t size, Coroutine_Start start, void *value, void **result);
extern void *Coroutine_GetCStackTop(void);
extern bool Coroutine_IsStarted(void);

extern void Coroutine_ClearStackForHWM(void);
extern void *Coroutine_GetStackHWM(void);

// export for _ctype, _json and _pickle for the _PY_ENSURE_COSTACK_HEADROOM_FOR_FN macros
PyAPI_FUNC(bool) _Py_Coroutine_CanStartCoroutine(size_t size);
PyAPI_FUNC(intptr_t) _Py_Coroutine_GetStackHeadroom(void);
PyAPI_FUNC(bool) _Py_Coroutine_Chain(size_t size, Coroutine_Start start, void *value, void **result);
PyAPI_FUNC(Coroutine *) _Py_Coroutine_New(size_t size, Coroutine_Start start);
PyAPI_FUNC(void) _Py_Coroutine_Delete(Coroutine *cor);
PyAPI_FUNC(bool) _Py_Coroutine_IsRunning(Coroutine *cor);
PyAPI_FUNC(bool) _Py_Coroutine_IsComplete(Coroutine *cor);
PyAPI_FUNC(bool) _Py_Coroutine_Continue(Coroutine *cor, void *value, bool early);
PyAPI_FUNC(void *) _Py_Coroutine_Yield(void *value, Coroutine_YieldCallback on_yield, void *me);
PyAPI_FUNC(void *) _Py_Coroutine_GetValue(Coroutine *cor);
PyAPI_FUNC(Coroutine *) _Py_Coroutine_GetActive(void);

#endif
