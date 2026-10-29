/* Structures used by pycore_debug_offsets.h.
 *
 * See InternalDocs/frames.md for an explanation of the frame stack
 * including explanation of the PyFrameObject and _PyInterpreterFrame
 * structs.
 */

#ifndef Py_INTERNAL_INTERP_FRAME_STRUCTS_H
#define Py_INTERNAL_INTERP_FRAME_STRUCTS_H

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif

#include "pycore_structs.h"       // _PyStackRef
#include "pycore_typedefs.h"      // _PyInterpreterFrame
#if PY_ASYNC_BY_COROUTINE_C
#include "pycore_coroutine.h"     // Coroutine
#else
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum _frameowner {
    FRAME_OWNED_BY_THREAD = 0,
    FRAME_OWNED_BY_GENERATOR = 1,
    FRAME_OWNED_BY_FRAME_OBJECT = 2,
    FRAME_OWNED_BY_INTERPRETER = 3,
    FRAME_OWNED_BY_CSTACK = 4,
};

struct _PyInterpreterFrame;

typedef struct ReturnAction_State {
    int step;
} ReturnAction_State;

#define ReturnAction_State_Wrapped (1<<30)
#define ReturnAction_State_ReUsed (1<<31)
#define ReturnAction_State_Flags (ReturnAction_State_Wrapped | ReturnAction_State_ReUsed)

typedef struct ReturnAction_StateWrapper ReturnAction_StateWrapper;
typedef struct ReturnAction_Class ReturnAction_Class;

struct ReturnAction_StateWrapper {
    ReturnAction_StateWrapper *next;
    ReturnAction_Class *class;
    ReturnAction_State wrapped;
};

typedef void ReturnAction_WrappedInit(ReturnAction_State *wrapped, ReturnAction_State *state);
typedef void ReturnAction_WrappedDtor(ReturnAction_State *wrapped);
typedef PyObject *ReturnAction_Step(ReturnAction_State *wrapped, PyObject *result, struct _PyInterpreterFrame **inlined);

static inline void
ReturnAction_BaseWrappedInit(ReturnAction_State *wrapped, ReturnAction_State *state){
    wrapped->step = state->step | (ReturnAction_State_Wrapped | ReturnAction_State_ReUsed);
}

struct ReturnAction_Class {
    ReturnAction_WrappedInit *init;
    ReturnAction_WrappedDtor *dtor;
    ReturnAction_Step *step;
    size_t size;
};

#define RETURNACTION_SETSTEP(N) (state->base.step = (state->base.step & ~ReturnAction_State_Flags) | (N))

#define RETURNACTION_STDCLASSDECLARATION(name) \
typedef struct name##_State name##_State; \
static void name##_WrappedInit(name##_State *wrapped, name##_State *state); \
static void name##_WrappedDtor(name##_State *wrapped); \
static PyObject *name##_Step(name##_State *wrapped, PyObject *result, struct _PyInterpreterFrame **inlined);

#define RETURNACTION_CHECKINLINED(N) \
    if (inlined && *inlined != frame) \
    { \
        RETURNACTION_SETSTEP(N); \
        _PyFrame_AddReturnAction(*inlined, &state->base, &MyReturnAction_Class); \
        return result; \
    } \
    l##N:;

#define RETURNACTION_STEPPREAMBLE_INNER_1 \
    case 1: goto l1;

#define RETURNACTION_STEPPREAMBLE_INNER_2 \
    RETURNACTION_STEPPREAMBLE_INNER_1 \
    case 2: goto l2;

#define RETURNACTION_STEPPREAMBLE_INNER_3 \
    RETURNACTION_STEPPREAMBLE_INNER_2 \
    case 3: goto l3;

#define RETURNACTION_STEPPREAMBLE_INNER_4 \
    RETURNACTION_STEPPREAMBLE_INNER_3 \
    case 4: goto l4;

#define RETURNACTION_STEPPREAMBLE_INNER_5 \
    RETURNACTION_STEPPREAMBLE_INNER_4 \
    case 5: goto l5;

#define RETURNACTION_STEPPREAMBLE_INNER_6 \
    RETURNACTION_STEPPREAMBLE_INNER_5 \
    case 6: goto l6;

#define RETURNACTION_STEPDEFINITION(name, N) \
static PyObject * \
name##_Step( \
    name##_State *state, \
    PyObject *result, \
    struct _PyInterpreterFrame **inlined \
) \
{ \
    static ReturnAction_Class MyReturnAction_Class = { \
        .size = sizeof(name##_State), \
        .init = (ReturnAction_WrappedInit *)&name##_WrappedInit, \
        .dtor = (ReturnAction_WrappedDtor *)&name##_WrappedDtor, \
        .step = (ReturnAction_Step *)&name##_Step, \
    }; \
 \
    struct _PyInterpreterFrame *frame = inlined ? *inlined : NULL; \
    switch(state->base.step & ~ReturnAction_State_Flags) { \
    case 0: goto l0; \
    RETURNACTION_STEPPREAMBLE_INNER_##N \
    default: assert(0); \
    } \
l0:;


struct _PyInterpreterFrame {
    _PyStackRef f_executable; /* Deferred or strong reference (code object or None) */
    struct _PyInterpreterFrame *previous;
    _PyStackRef f_funcobj; /* Deferred or strong reference. Only valid if not on C stack */
    PyObject *f_globals; /* Borrowed reference. Only valid if not on C stack */
    PyObject *f_builtins; /* Borrowed reference. Only valid if not on C stack */
    PyObject *f_locals; /* Strong reference, may be NULL. Only valid if not on C stack */
    PyFrameObject *frame_obj; /* Strong reference, may be NULL. Only valid if not on C stack */
    _Py_CODEUNIT *instr_ptr; /* Instruction currently executing (or about to begin) */
    _PyStackRef *stackpointer;
    ReturnAction_StateWrapper *returnaction;
#ifdef Py_GIL_DISABLED
    /* Index of thread-local bytecode containing instr_ptr. */
    int32_t tlbc_index;
#endif
    uint16_t return_offset;  /* Only relevant during a function call */
    char owner;
#ifdef Py_DEBUG
    uint8_t visited:1;
    uint8_t lltrace:7;
#else
    uint8_t visited;
#endif
    /* Locals and stack */
    _PyStackRef localsplus[1];
};


#if PY_ASYNC_BY_COROUTINE_C
#define _PyGenObject_HEAD_CoroutineBits(prefix) \
    Coroutine *prefix##_coroutine;                                          \
    Coroutine *prefix##_return_coroutine;                                   \
    PyObject *prefix##_result;                                              \
    PyObject *prefix##_yield_from;                                          \
    int prefix##_py_recursion_depth_at_entry;                               \

#else
#define _PyGenObject_HEAD_CoroutineBits(prefix) \
    void *prefix##_unused0;                                                 \
    void *prefix##_unused1;                                                 \
    void *prefix##_unused2;                                                 \
    void *prefix##_unused3;                                                 \
    int prefix##_resume_frame_count;                                        \

#endif

/* _PyGenObject_HEAD defines the initial segment of generator
   and coroutine objects. */
#define _PyGenObject_HEAD(prefix)                                           \
    PyObject_HEAD                                                           \
    /* List of weak reference. */                                           \
    PyObject *prefix##_weakreflist;                                         \
    /* Name of the generator. */                                            \
    PyObject *prefix##_name;                                                \
    /* Qualified name of the generator. */                                  \
    PyObject *prefix##_qualname;                                            \
    _PyErr_StackItem prefix##_exc_state;                                    \
    PyObject *prefix##_origin_or_finalizer;                                 \
    /* the datastack owned by this generator */                             \
    _PyDataStack prefix##_datastack;                                        \
    /* (when frame is running) the datastack previous to this generator */  \
    _PyDataStack *prefix##_previous_datastack;                              \
    /* the gen and its frame to resume at */                                \
    struct _PyInterpreterFrame *prefix##_resume_iframe;                     \
    struct _PyGenObject *prefix##_resume_gen;                               \
    _PyGenObject_HEAD_CoroutineBits(prefix)                                 \
    char prefix##_hooks_inited;                                             \
    char prefix##_closed;                                                   \
    char prefix##_running_async;                                            \
    /* The frame */                                                         \
    int8_t prefix##_frame_state;                                            \
    _PyInterpreterFrame prefix##_iframe;                                    \

struct _PyGenObject {
    /* The gi_ prefix is intended to remind of generator-iterator. */
    _PyGenObject_HEAD(gi)
};

struct _PyCoroObject {
    _PyGenObject_HEAD(cr)
};

struct _PyAsyncGenObject {
    _PyGenObject_HEAD(ag)
};

#undef _PyGenObject_HEAD


#ifdef __cplusplus
}
#endif
#endif  // !Py_INTERNAL_INTERP_FRAME_STRUCTS_H
