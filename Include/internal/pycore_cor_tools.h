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
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN0_B(nomemret, r_t, fn) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_COSTACK_MIN_HEADROOM ) { \
        void *res; \
        if (!_Py_Coroutine_Chain(PYOS_COSTACK_STD_SIZE, PYOS_COSTACK_CHAIN_HEADROOM, Do_Call_##fn, NULL, &res)) { \
            return (r_t)(uintptr_t)res; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
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
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN1_B(nomemret, r_t, fn, p0) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_COSTACK_MIN_HEADROOM ) { \
        void *res; \
        struct Do_Call_Params_##fn params = {p0}; \
        if (!_Py_Coroutine_Chain(PYOS_COSTACK_STD_SIZE, PYOS_COSTACK_CHAIN_HEADROOM, Do_Call_##fn, (void *)&params, &res)) { \
            return (r_t)(uintptr_t)res; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
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
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN2_B(nomemret, r_t, fn, p0, p1) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_COSTACK_MIN_HEADROOM ) { \
        void *res; \
        struct Do_Call_Params_##fn params = {p0, p1}; \
        if (!_Py_Coroutine_Chain(PYOS_COSTACK_STD_SIZE, PYOS_COSTACK_CHAIN_HEADROOM, Do_Call_##fn, (void *)&params, &res)) { \
            return (r_t)(uintptr_t)res; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
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
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN3_B(nomemret, r_t, fn, p0, p1, p2) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_COSTACK_MIN_HEADROOM ) { \
        void *res; \
        struct Do_Call_Params_##fn params = {p0, p1, p2}; \
        if (!_Py_Coroutine_Chain(PYOS_COSTACK_STD_SIZE, PYOS_COSTACK_CHAIN_HEADROOM, Do_Call_##fn, (void *)&params, &res)) { \
            return (r_t)(uintptr_t)res; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
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
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN4_B(nomemret, r_t, fn, p0, p1, p2, p3) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_COSTACK_MIN_HEADROOM ) { \
        void *res; \
        struct Do_Call_Params_##fn params = {p0, p1, p2, p3}; \
        if (!_Py_Coroutine_Chain(PYOS_COSTACK_STD_SIZE, PYOS_COSTACK_CHAIN_HEADROOM, Do_Call_##fn, (void *)&params, &res)) { \
            return (r_t)(uintptr_t)res; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
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
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN5_B(nomemret, r_t, fn, p0, p1, p2, p3, p4) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_COSTACK_MIN_HEADROOM ) { \
        void *res; \
        struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4}; \
        if (!_Py_Coroutine_Chain(PYOS_COSTACK_STD_SIZE, PYOS_COSTACK_CHAIN_HEADROOM, Do_Call_##fn, (void *)&params, &res)) { \
            return (r_t)(uintptr_t)res; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    }

#define _PY_ENSURE_COSTACK_FOR_FN1_A(fn, p0_t, p0) \
struct Do_Call_Params_##fn { \
    p0_t p0; \
}; \
static void *Do_Call_##fn(void *param);
#define _PY_ENSURE_COSTACK_FOR_FN1_B(nonmemret, r_t, fn, p0) \
    struct Do_Call_Params_##fn params = {p0}; \
    void *res; \
    if (_PyThreadStack_CallInsideCoroutine(Do_Call_##fn, (void *)&params, &res)) { \
        return nonmemret; \
    } \
    return (r_t)(uintptr_t)res; \
} \
static void *Do_Call_##fn(void *_params){ \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params;

#define _PY_ENSURE_COSTACK_FOR_FN2_A(fn, p0_t, p0, p1_t, p1) \
struct Do_Call_Params_##fn { \
    p0_t p0; \
    p1_t p1; \
}; \
static void *Do_Call_##fn(void *param);
#define _PY_ENSURE_COSTACK_FOR_FN2_B(nonmemret, r_t, fn, p0, p1) \
    struct Do_Call_Params_##fn params = {p0, p1}; \
    void *res; \
    if (_PyThreadStack_CallInsideCoroutine(Do_Call_##fn, (void *)&params, &res)) { \
        return nonmemret; \
    } \
    return (r_t)(uintptr_t)res; \
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
#define _PY_ENSURE_COSTACK_FOR_FN3_B(nonmemret, r_t, fn, p0, p1, p2) \
    struct Do_Call_Params_##fn params = {p0, p1, p2}; \
    void *res; \
    if (_PyThreadStack_CallInsideCoroutine(Do_Call_##fn, (void *)&params, &res)) { \
        return nonmemret; \
    } \
    return (r_t)(uintptr_t)res; \
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
#define _PY_ENSURE_COSTACK_FOR_FN4_B(nonmemret, r_t, fn, p0, p1, p2, p3) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3}; \
    void *res; \
    if (_PyThreadStack_CallInsideCoroutine(Do_Call_##fn, (void *)&params, &res)) { \
        return nonmemret; \
    } \
    return (r_t)(uintptr_t)res; \
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
#define _PY_ENSURE_COSTACK_FOR_FN5_B(nonmemret, r_t, fn, p0, p1, p2, p3, p4) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4}; \
    void *res; \
    if (_PyThreadStack_CallInsideCoroutine(Do_Call_##fn, (void *)&params, &res)) { \
        return nonmemret; \
    } \
    return (r_t)(uintptr_t)res; \
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
#define _PY_ENSURE_COSTACK_FOR_FN6_B(nonmemret, r_t, fn, p0, p1, p2, p3, p4, p5) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4, p5}; \
    void *res; \
    if (_PyThreadStack_CallInsideCoroutine(Do_Call_##fn, (void *)&params, &res)) { \
        return nonmemret; \
    } \
    return (r_t)(uintptr_t)res; \
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
#define _PY_ENSURE_COSTACK_FOR_FN7_B(nonmemret, r_t, fn, p0, p1, p2, p3, p4, p5, p6) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4, p5, p6}; \
    void *res; \
    if (_PyThreadStack_CallInsideCoroutine(Do_Call_##fn, (void *)&params, &res)) { \
        return nonmemret; \
    } \
    return (r_t)(uintptr_t)res; \
} \
static void *Do_Call_##fn(void *_params){ \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params;


#define _PY_ENSURE_STACK_FOR_FN0_A(r_t, fn) \
static void *Do_##fn(void *); \
static r_t Actual_##fn(void); \
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN0_B(space, nomemret, r_t, fn) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        void *result; \
        if ( !_Py_Coroutine_Chain(space, PYOS_COSTACK_CHAIN_HEADROOM, Do_##fn, NULL, &result)) { \
            return (r_t)(intptr_t)result; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return Actual_##fn(); \
} \
static void *Do_##fn(void *_params) { \
    return (void *)(uintptr_t)Actual_##fn(); \
} \
static r_t Actual_##fn(void) {


#define _PY_ENSURE_STACK_FOR_FN1_A(r_t, fn, p0_t) \
static void *Do_##fn(void *); \
static r_t Actual_##fn(p0_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN1_B(space, nomemret, r_t, fn, p0_t, p0) \
    struct Do_Call_Params_##fn params = {p0}; \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        void *result; \
        if ( !_Py_Coroutine_Chain(space, PYOS_COSTACK_CHAIN_HEADROOM, Do_##fn, &params, &result)) { \
            return (r_t)(intptr_t)result; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return Actual_##fn(p0); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    return (void *)(uintptr_t)Actual_##fn(params->v0); \
} \
static r_t Actual_##fn(p0_t p0) {


#define _PY_ENSURE_STACK_FOR_FN2_A(r_t, fn, p0_t, p1_t) \
static void *Do_##fn(void *); \
static r_t Actual_##fn(p0_t, p1_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN2_B(space, nomemret, r_t, fn, p0_t, p0, p1_t, p1) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        struct Do_Call_Params_##fn params = {p0, p1}; \
        void *result; \
        if ( !_Py_Coroutine_Chain(space, PYOS_COSTACK_CHAIN_HEADROOM, Do_##fn, &params, &result)) { \
            return (r_t)(intptr_t)result; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return Actual_##fn(p0, p1); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    return (void *)(uintptr_t)Actual_##fn(params->v0, params->v1); \
} \
static r_t Actual_##fn(p0_t p0, p1_t p1) {


#define _PY_ENSURE_STACK_FOR_FN3_A(r_t, fn, p0_t, p1_t, p2_t) \
static void *Do_##fn(void *); \
static r_t Actual_##fn(p0_t, p1_t, p2_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN3_B(space, nomemret, r_t, fn, p0_t, p0, p1_t, p1, p2_t, p2) \
    struct Do_Call_Params_##fn params = {p0, p1, p2}; \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        void *result; \
        if ( !_Py_Coroutine_Chain(space, PYOS_COSTACK_CHAIN_HEADROOM, Do_##fn, &params, &result)) { \
            return (r_t)(intptr_t)result; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return Actual_##fn(p0, p1, p2); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    return (void *)(uintptr_t)Actual_##fn(params->v0, params->v1, params->v2); \
} \
static r_t Actual_##fn(p0_t p0, p1_t p1, p2_t p2) {


#define _PY_ENSURE_STACK_FOR_FN4_A(r_t, fn, p0_t, p1_t, p2_t, p3_t) \
static void *Do_##fn(void *); \
static r_t Actual_##fn(p0_t, p1_t, p2_t, p3_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
    p3_t v3; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN4_B(space, nomemret, r_t, fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3}; \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        void *result; \
        if ( !_Py_Coroutine_Chain(space, PYOS_COSTACK_CHAIN_HEADROOM, Do_##fn, &params, &result)) { \
            return (r_t)(intptr_t)result; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return Actual_##fn(p0, p1, p2, p3); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    return (void *)(uintptr_t)Actual_##fn(params->v0, params->v1, params->v2, params->v3); \
} \
static r_t Actual_##fn(p0_t p0, p1_t p1, p2_t p2, p3_t p3) {


#define _PY_ENSURE_STACK_FOR_FN5_A(r_t, fn, p0_t, p1_t, p2_t, p3_t, p4_t) \
static void *Do_##fn(void *); \
static r_t Actual_##fn(p0_t, p1_t, p2_t, p3_t, p4_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
    p3_t v3; \
    p4_t v4; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN5_B(space, nomemret, r_t, fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3, p4_t, p4) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4}; \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        void *result; \
        if ( !_Py_Coroutine_Chain(space, PYOS_COSTACK_CHAIN_HEADROOM, Do_##fn, &params, &result)) { \
            return (r_t)(intptr_t)result; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return Actual_##fn(p0, p1, p2, p3, p4); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    return (void *)(uintptr_t)Actual_##fn(params->v0, params->v1, params->v2, params->v3, params->v4); \
} \
static r_t Actual_##fn(p0_t p0, p1_t p1, p2_t p2, p3_t p3, p4_t p4) {


#define _PY_ENSURE_STACK_FOR_FN6_A(r_t, fn, p0_t, p1_t, p2_t, p3_t, p4_t, p5_t) \
static void *Do_##fn(void *); \
static r_t Actual_##fn(p0_t, p1_t, p2_t, p3_t, p4_t, p5_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
    p3_t v3; \
    p4_t v4; \
    p5_t v5; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN6_B(space, nomemret, r_t, fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3, p4_t, p4, p5_t, p5) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4, p5}; \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        void *result; \
        if ( !_Py_Coroutine_Chain(space, PYOS_COSTACK_CHAIN_HEADROOM, Do_##fn, &params, &result)) { \
            return (r_t)(intptr_t)result; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return Actual_##fn(p0, p1, p2, p3, p4, p5); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    return (void *)(uintptr_t)Actual_##fn(params->v0, params->v1, params->v2, params->v3, params->v4, params->v5); \
} \
static r_t Actual_##fn(p0_t p0, p1_t p1, p2_t p2, p3_t p3, p4_t p4, p5_t p5) {


#define _PY_ENSURE_STACK_FOR_FN7_A(r_t, fn, p0_t, p1_t, p2_t, p3_t, p4_t, p5_t, p6_t) \
static void *Do_##fn(void *); \
static r_t Actual_##fn(p0_t, p1_t, p2_t, p3_t, p4_t, p5_t, p6_t); \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
    p3_t v3; \
    p4_t v4; \
    p5_t v5; \
    p6_t v6; \
};
// Function definition start goes here
#define _PY_ENSURE_STACK_FOR_FN7_B(space, nomemret, r_t, fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3, p4_t, p4, p5_t, p5, p6_t, p6) \
    struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4, p5, p6}; \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)(space) ){ \
        void *result; \
        if ( !_Py_Coroutine_Chain(space, PYOS_COSTACK_CHAIN_HEADROOM, Do_##fn, &params, &result)) { \
            return (r_t)(intptr_t)result; \
        } else { \
            PyErr_NoMemory(); \
            return nomemret; \
        } \
    } \
    return Actual_##fn(p0, p1, p2, p3, p4, p5, p6); \
} \
static void *Do_##fn(void *_params) { \
    struct Do_Call_Params_##fn *params = (struct Do_Call_Params_##fn *)_params; \
    return (void *)(uintptr_t)Actual_##fn(params->v0, params->v1, params->v2, params->v3, params->v4, params->v5, params->v6); \
} \
static r_t Actual_##fn(p0_t p0, p1_t p1, p2_t p2, p3_t p3, p4_t p4, p5_t p5, p6_t p6) {

#endif
