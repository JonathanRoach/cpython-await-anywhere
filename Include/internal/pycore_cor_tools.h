#ifndef Py_INTERNAL_COR_TOOLS_H
#define Py_INTERNAL_COR_TOOLS_H
#include "pycore_coroutine.h"
#include "pycore_pystate.h"

#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN0_A(decl, r_t, fn) \
decl r_t fn(void); \
static void *Do_Call_##fn(void *param) { \
    (void)param; \
    return (void *)(uintptr_t)fn(); \
}
#define _PY_ENSURE_COSTACK_HEADROOM_FOR_FN0_B(r_t, fn) \
    if (_Py_Coroutine_GetStackHeadroom() < (intptr_t)PYOS_STACK_MARGIN_BYTES && \
        _Py_Coroutine_CanStartCoroutine((void *)((_PyThreadStateImpl *)_PyThreadState_GET())->c_stack_hard_limit)) { \
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
        _Py_Coroutine_CanStartCoroutine((void *)((_PyThreadStateImpl *)_PyThreadState_GET())->c_stack_hard_limit)) { \
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
        _Py_Coroutine_CanStartCoroutine((void *)((_PyThreadStateImpl *)_PyThreadState_GET())->c_stack_hard_limit)) { \
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
        _Py_Coroutine_CanStartCoroutine((void *)((_PyThreadStateImpl *)_PyThreadState_GET())->c_stack_hard_limit)) { \
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
        _Py_Coroutine_CanStartCoroutine((void *)((_PyThreadStateImpl *)_PyThreadState_GET())->c_stack_hard_limit)) { \
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
        _Py_Coroutine_CanStartCoroutine((void *)((_PyThreadStateImpl *)_PyThreadState_GET())->c_stack_hard_limit)) { \
        struct Do_Call_Params_##fn params = {p0, p1, p2, p3, p4}; \
        return (r_t)(uintptr_t)_Py_Coroutine_Chain(Do_Call_##fn, (void *)&params); \
    }

#endif
