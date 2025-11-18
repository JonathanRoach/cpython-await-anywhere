#ifndef Py_INTERNAL_COR_TOOLS_H
#define Py_INTERNAL_COR_TOOLS_H
#include "pycore_coroutine.h"
#include "pycore_pystate.h"

// _PY_ENSURE_COSTACK_HEADROOM_FOR_FN<N>_[A/B]
// Macros to ensure at least PYOS_STACK_MARGIN_BYTES to call a function
// <N> - the number of parameters
// [A/B] - need to have 2 parts, A before the function, and B within the function
// Assumes PYOS_STACK_MARGIN_BYTES is much smaller than coroutine stack
//
// _PY_ENSURE_COSTACK_FOR_FN<N>_[A/B]
// Macros to ensure a function is running within a coroutine
//
// _PY_ENSURE_STACK_FOR_FN<N>_[A/B]
// Macros to ensure a given amount of stack is available for running a routine
// Assumes 'PyObject *' returned; always marshalls the parameters

#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN0_A(decl, r_t, fn) \
decl r_t fn(void); \
static void *Do_Call_##fn(void *param) { \
    (void)param; \
    return (void *)(uintptr_t)fn(); \
}
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN0_B(r_t, fn) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_STACK_MARGIN_BYTES && \
        _Py_Coroutine_CanStartCoroutine()) { \
        return (r_t)(uintptr_t)_Py_Coroutine_Chain(Do_Call_##fn, NULL); \
    }

#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN1_A(decl, r_t, fn, p0_t) \
decl r_t fn(p0_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
}; \
static void *Do_Call_##fn(void *param) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)param; \
    return (void *)(uintptr_t)fn(params->v0); \
}
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN1_B(r_t, fn, p0) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_STACK_MARGIN_BYTES && \
        _Py_Coroutine_CanStartCoroutine()) { \
        struct Do_Call_Params_##fn params = {p0}; \
        return (r_t)(uintptr_t)_Py_Coroutine_Chain(Do_Call_##fn, (void *)&params); \
    }

#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN2_A(decl, r_t, fn, p0_t, p1_t) \
decl r_t fn(p0_t, p1_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
}; \
static void *Do_Call_##fn(void *param) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)param; \
    return (void *)(uintptr_t)fn(params->v0, params->v1); \
}
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN2_B(r_t, fn, p0, p1) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_STACK_MARGIN_BYTES && \
        _Py_Coroutine_CanStartCoroutine()) { \
        struct Do_Call_Params_##fn params = {p0, p1}; \
        return (r_t)(uintptr_t)_Py_Coroutine_Chain(Do_Call_##fn, (void *)&params); \
    }

#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN3_A(decl, r_t, fn, p0_t, p1_t, p2_t) \
decl r_t fn(p0_t, p1_t, p2_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
}; \
static void *Do_Call_##fn(void *param) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)param; \
    return (void *)(uintptr_t)fn(params->v0, params->v1, params->v2); \
}
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN3_B(r_t, fn, p0, p1, p2) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_STACK_MARGIN_BYTES && \
        _Py_Coroutine_CanStartCoroutine()) { \
        struct Do_Call_Params_##fn params = {p0, p1, p2}; \
        return (r_t)(uintptr_t)_Py_Coroutine_Chain(Do_Call_##fn, (void *)&params); \
    }

#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN4_A(decl, r_t, fn, p0_t, p1_t, p2_t, p3_t) \
decl r_t fn(p0_t, p1_t, p2_t, p3_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
    p3_t v3; \
}; \
static void *Do_Call_##fn(void *param) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)param; \
    return (void *)(uintptr_t)fn(params->v0, params->v1, params->v2, params->v3); \
}
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN4_B(r_t, fn, p0, p1, p2, p3) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_STACK_MARGIN_BYTES && \
        _Py_Coroutine_CanStartCoroutine()) { \
        struct Do_Call_Params_##fn params = {p0, p1, p2, p3}; \
        return (r_t)(uintptr_t)_Py_Coroutine_Chain(Do_Call_##fn, (void *)&params); \
    }

#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN5_A(decl, r_t, fn, p0_t, p1_t, p2_t, p3_t, p4_t) \
decl r_t fn(p0_t, p1_t, p2_t, p3_t, p4_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
    p3_t v3; \
    p4_t v4; \
}; \
static void *Do_Call_##fn(void *param) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)param; \
    return (void *)(uintptr_t)fn(params->v0, params->v1, params->v2, params->v3, params->v4); \
}
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN5_B(r_t, fn, p0, p1, p2, p3, p4) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_STACK_MARGIN_BYTES && \
        _Py_Coroutine_CanStartCoroutine()) { \
        struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4}; \
        return (r_t)(uintptr_t)_Py_Coroutine_Chain(Do_Call_##fn, (void *)&params); \
    }

#define _PY_ENSURE_COSTACK_FOR_FN1_A(fn, p0_t, p0) \
struct Do_Call_Params_##fn { \
    p0_t p0; \
}; \
static void *Do_Call_##fn(void *param);
#define _PY_ENSURE_COSTACK_FOR_FN1_B(r_t, fn, p0) \
    struct Do_Call_Params_##fn params = {p0}; \
    return (r_t)(uintptr_t)Coroutine_Run(Do_Call_##fn, (void *)&params); \
} \
static void *Do_Call_##fn(void *_params){ \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params;

#define _PY_ENSURE_COSTACK_FOR_FN2_A(fn, p0_t, p0, p1_t, p1) \
struct Do_Call_Params_##fn { \
    p0_t p0; \
    p1_t p1; \
}; \
static void *Do_Call_##fn(void *param);
#define _PY_ENSURE_COSTACK_FOR_FN2_B(r_t, fn, p0, p1) \
    struct Do_Call_Params_##fn params = {p0, p1}; \
    return (r_t)(uintptr_t)Coroutine_Run(Do_Call_##fn, (void *)&params); \
} \
static void *Do_Call_##fn(void *_params){ \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params;

#define _PY_ENSURE_COSTACK_FOR_FN3_A(fn, p0_t, p0, p1_t, p1, p2_t, p2) \
struct Do_Call_Params_##fn { \
    p0_t p0; \
    p1_t p1; \
    p2_t p2; \
}; \
static void *Do_Call_##fn(void *param);
#define _PY_ENSURE_COSTACK_FOR_FN3_B(r_t, fn, p0, p1, p2) \
    struct Do_Call_Params_##fn params = {p0, p1, p2}; \
    return (r_t)(uintptr_t)Coroutine_Run(Do_Call_##fn, (void *)&params); \
} \
static void *Do_Call_##fn(void *_params){ \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params;

#define _PY_ENSURE_COSTACK_FOR_FN4_A(fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3) \
struct Do_Call_Params_##fn { \
    p0_t p0; \
    p1_t p1; \
    p2_t p2; \
    p3_t p3; \
}; \
static void *Do_Call_##fn(void *param);
#define _PY_ENSURE_COSTACK_FOR_FN4_B(r_t, fn, p0, p1, p2, p3) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3}; \
    return (r_t)(uintptr_t)Coroutine_Run(Do_Call_##fn, (void *)&params); \
} \
static void *Do_Call_##fn(void *_params){ \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params;

#define _PY_ENSURE_COSTACK_FOR_FN5_A(fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3, p4_t, p4) \
struct Do_Call_Params_##fn { \
    p0_t p0; \
    p1_t p1; \
    p2_t p2; \
    p3_t p3; \
    p4_t p4; \
}; \
static void *Do_Call_##fn(void *param);
#define _PY_ENSURE_COSTACK_FOR_FN5_B(r_t, fn, p0, p1, p2, p3, p4) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4}; \
    return (r_t)(uintptr_t)Coroutine_Run(Do_Call_##fn, (void *)&params); \
} \
static void *Do_Call_##fn(void *_params){ \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params;

#define _PY_ENSURE_COSTACK_FOR_FN6_A(fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3, p4_t, p4, p5_t, p5) \
struct Do_Call_Params_##fn { \
    p0_t p0; \
    p1_t p1; \
    p2_t p2; \
    p3_t p3; \
    p4_t p4; \
    p5_t p5; \
}; \
static void *Do_Call_##fn(void *param);
#define _PY_ENSURE_COSTACK_FOR_FN6_B(r_t, fn, p0, p1, p2, p3, p4, p5) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4, p5}; \
    return (r_t)(uintptr_t)Coroutine_Run(Do_Call_##fn, (void *)&params); \
} \
static void *Do_Call_##fn(void *_params){ \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params;

#define _PY_ENSURE_COSTACK_FOR_FN7_A(fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3, p4_t, p4, p5_t, p5, p6_t, p6) \
struct Do_Call_Params_##fn { \
    p0_t p0; \
    p1_t p1; \
    p2_t p2; \
    p3_t p3; \
    p4_t p4; \
    p5_t p5; \
    p6_t p6; \
}; \
static void *Do_Call_##fn(void *param);
#define _PY_ENSURE_COSTACK_FOR_FN7_B(r_t, fn, p0, p1, p2, p3, p4, p5, p6) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4, p5, p6}; \
    return (r_t)(uintptr_t)Coroutine_Run(Do_Call_##fn, (void *)&params); \
} \
static void *Do_Call_##fn(void *_params){ \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params;


#define _PY_ENSURE_STACK_FOR_FN1_A(fn, p0_t) \
static void *Do_##fn(void *); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN1_B(space, nomemret, r_t, fn, p0_t, p0) \
    struct Do_Call_Params_##fn params = {p0}; \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        if ( _Py_Coroutine_CanStartCoroutine()) { \
            return (r_t)(intptr_t)_Py_Coroutine_Chain(Do_##fn, &params); \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return (r_t)(intptr_t)Do_##fn(&params); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    p0_t p0 = params->v0;


#define _PY_ENSURE_STACK_FOR_FN2_A(fn, p0_t, p1_t) \
static void *Do_##fn(void *); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN2_B(space, nomemret, r_t, fn, p0_t, p0, p1_t, p1) \
    struct Do_Call_Params_##fn params = {p0, p1}; \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        if ( _Py_Coroutine_CanStartCoroutine()) { \
            return (r_t)(intptr_t)_Py_Coroutine_Chain(Do_##fn, &params); \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return (r_t)(intptr_t)Do_##fn(&params); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    p0_t p0 = params->v0; \
    p1_t p1 = params->v1;


#define _PY_ENSURE_STACK_FOR_FN3_A(fn, p0_t, p1_t, p2_t) \
static void *Do_##fn(void *); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN3_B(space, nomemret, r_t, fn, p0_t, p0, p1_t, p1, p2_t, p2) \
    struct Do_Call_Params_##fn params = {p0, p1, p2}; \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        if ( _Py_Coroutine_CanStartCoroutine()) { \
            return (r_t)(intptr_t)_Py_Coroutine_Chain(Do_##fn, &params); \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return (r_t)(intptr_t)Do_##fn(&params); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    p0_t p0 = params->v0; \
    p1_t p1 = params->v1; \
    p2_t p2 = params->v2;

#endif
