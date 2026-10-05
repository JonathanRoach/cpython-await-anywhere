#include "pycore_coroutine.h"
#include "pycore_cor_platform.h"

#include "pycore_coroutine_names_def.h"

static uintptr_t StackTopNow(void);

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

// Unused...
// static inline unsigned char *
// StackPointerAdd
// (
//     unsigned char *a,
//     ptrdiff_t b
// ){
// #if COROUTINE_STACK_GROWS_UP
//     return a + b;
// #else
//     return a - b;
// #endif
// }

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

// static inline unsigned char *
// StackLimitEnd
// (
//     unsigned char *memlo,
//     unsigned char *memhi
// ){
// #if COROUTINE_STACK_GROWS_UP
//     (void)memlo;
//     return memhi;
// #else
//     (void)memhi;
//     return memlo-1;
// #endif
// }
// ...unused

_Cor_thread_local unsigned char *g_stack_limit;


#ifndef NDEBUG
/// @brief Check system integrity - does stack overrun check and internals check
/// @return Coroutine_OK all is OK; other something was wrong
Coroutine_Err
Coroutine_CheckIntegrity(
    void
){
    return Coroutine_OK;
}
#endif


void
Coroutine_SetStackLimit(
    void *limit
){
    g_stack_limit = limit;
}


Coroutine_Err Coroutine_Run(
    size_t min_size,
    size_t min_headroom,
    Coroutine_Start start,
    void *value,
    void **result
){
    // Pretend we are in an active coroutine, so call start() directly
    void *res = start(value);
    if (result){
        *result = res;
    }

    // no failures, so...
    return Coroutine_OK;
}


ptrdiff_t
Coroutine_GetStackHeadroom(
    void
){
    // no active coroutine
    if (g_stack_limit){
        return StackPointerDiff(g_stack_limit, (unsigned char *)StackTopNow());
    }

    // The biggest ptrdiff_t possible
    return PTRDIFF_MAX;
}


bool
Coroutine_CanStartCoroutine(
    size_t size
){
    (void)size;
    return false;
}


void *
Coroutine_GetCStackTop(
    void
){
    return (void *)StackTopNow();
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


Coroutine_Err
Coroutine_CallWithMaxStack(
    Coroutine_Start start,
    void *value,
    void **result
){
    void *ret = start(value);
    if (result){
        *result = ret;
    }
    return Coroutine_OK;
}
#include "pycore_coroutine_names_undef.h"
