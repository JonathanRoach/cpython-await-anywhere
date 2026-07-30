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



#define _PY_MAX_STACK_FOR_CALL_IF_3(decl, r_t, fn, p0_t, p0, p1_t, p1, p2_t, p2) \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
}; \
static r_t Do_##fn(struct Do_Call_Params_##fn *params); \
decl r_t fn(bool frugal, p0_t v0, p1_t v1, p2_t v2) { \
    void *ret; \
 \
    struct Do_Call_Params_##fn params = { v0, v1, v2 }; \
    if (frugal || \
        _Py_Coroutine_CallWithMaxStack((Coroutine_Start)Do_##fn, &params, &ret)){ \
        ret = (void *)(uintptr_t)Do_##fn(&params); \
    } \
    return (r_t)(uintptr_t)ret; \
} \
static r_t Do_##fn(struct Do_Call_Params_##fn *params) { \
    p0_t p0 = params->v0; \
    p1_t p1 = params->v1; \
    p2_t p2 = params->v2;

#define _PY_MAX_STACK_FOR_CALL_IF_4(decl, r_t, fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3) \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
    p3_t v3; \
}; \
static r_t Do_##fn(struct Do_Call_Params_##fn *params); \
decl r_t fn(bool frugal, p0_t v0, p1_t v1, p2_t v2, p3_t v3) { \
    void *ret; \
 \
    struct Do_Call_Params_##fn params = { v0, v1, v2, v3 }; \
    if (frugal || \
        _Py_Coroutine_CallWithMaxStack((Coroutine_Start)Do_##fn, &params, &ret)){ \
        ret = (void *)(uintptr_t)Do_##fn(&params); \
    } \
    return (r_t)(uintptr_t)ret; \
} \
static r_t Do_##fn(struct Do_Call_Params_##fn *params) { \
    p0_t p0 = params->v0; \
    p1_t p1 = params->v1; \
    p2_t p2 = params->v2; \
    p3_t p3 = params->v3;

#define _PY_MAX_STACK_FOR_CALL_IF_7(decl, r_t, fn, p0_t, p0, p1_t, p1, p2_t, p2, p3_t, p3, p4_t, p4, p5_t, p5, p6_t, p6) \
struct Do_Call_Params_##fn { \
    p0_t v0; \
    p1_t v1; \
    p2_t v2; \
    p3_t v3; \
    p4_t v4; \
    p5_t v5; \
    p6_t v6; \
}; \
static r_t Do_##fn(struct Do_Call_Params_##fn *params); \
decl r_t fn(bool frugal, p0_t v0, p1_t v1, p2_t v2, p3_t v3, p4_t v4, p5_t v5, p6_t v6) { \
    void *ret; \
 \
    struct Do_Call_Params_##fn params = { v0, v1, v2, v3, v4, v5, v6 }; \
    if (frugal || \
        _Py_Coroutine_CallWithMaxStack((Coroutine_Start)Do_##fn, &params, &ret)){ \
        ret = (void *)(uintptr_t)Do_##fn(&params); \
    } \
    return (r_t)(uintptr_t)ret; \
} \
static r_t Do_##func(struct Do_Call_Params_##fn *params) { \
    p0_t p0 = params->v0; \
    p1_t p1 = params->v1; \
    p2_t p2 = params->v2; \
    p3_t p3 = params->v3; \
    p4_t p4 = params->v4; \
    p5_t p5 = params->v5; \
    p6_t p6 = params->v6;

#endif
