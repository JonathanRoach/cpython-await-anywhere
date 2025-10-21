#ifndef Py_INTERNAL_COROUTINE_H
#define Py_INTERNAL_COROUTINE_H

#include "Python.h"
#include <stdbool.h>

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
//   void *result = Coroutine_Run(co, initial_value);
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
// In Pythonland, we set it to 4 * (enough for a PyEval_EvalDefault), so we get at least 3
// calls deep before we need a new chunk, ie maximum multi-chunk wastage is 25%. There's a
// trade-off between smaller chunk sizes which increase the number of async tasks which can
// possibly co-exist on a thread, and larger chunk sizes which waste less memory in part-used
// chunks.
#ifndef COROUTINE_STACK_SIZE
    #define COROUTINE_STACK_SIZE (PYOS_STACK_MARGIN_BYTES * 4)
#endif

// When Coroutine is started, an amount of stack is set aside to give
// the caller of Coroutine_StartSystem a bit of room to work before calling
// Coroutine_Run(), that is this amount:
#ifndef COROUTINE_STARTUP_STACK_SIZE
    #define COROUTINE_STARTUP_STACK_SIZE (1024 * sizeof(void *))
#endif

// Returned by Coroutine_StopSystem(), this summarises the coroutine session
typedef struct Coroutine_Report {
    unsigned coroutines_created;
    unsigned coroutines_pool_size;
    unsigned lowest_headroom;
} Coroutine_Report;

typedef struct Coroutine Coroutine;

typedef void (*Coroutine_YieldCallback)(void *me);
typedef void *(*Coroutine_Start)(void *);

extern void Coroutine_StartSystem(void);
extern Coroutine_Report Coroutine_StopSystem(void);
extern Coroutine *Coroutine_New(Coroutine_Start start);
extern void Coroutine_Run_Coroutine(Coroutine *cor, void *value);
extern void *Coroutine_Run(Coroutine_Start start, void *value);
extern void Coroutine_Delete(Coroutine *cor);
extern void Coroutine_Continue(Coroutine *cor, void *value, bool early);
extern void *Coroutine_Yield(void *value, Coroutine_YieldCallback on_yield, void *me);
extern void *Coroutine_GetValue(Coroutine *cor);
extern Coroutine *Coroutine_GetActive(void);
extern int Coroutine_GetStackHeadroom(void);
extern bool Coroutine_HasCoroutinesInFreePool(void);
extern void *Coroutine_GetCStackTop(void);
extern void *Coroutine_Chain(Coroutine_Start start, void *value);
extern bool Coroutine_IsStarted(void);
extern bool Coroutine_IsRunning(Coroutine *cor);

#endif
