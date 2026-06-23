#ifndef Py_INTERNAL_COR_PLATFORM_INC_H
#define Py_INTERNAL_COR_PLATFORM_INC_H
// platform specific parts collected together
#include "Python.h"

#define Coroutine_NS(N) _Py_Coroutine_##N
#define Coroutine_API_FUNC(T) PyAPI_FUNC(T)

#endif
