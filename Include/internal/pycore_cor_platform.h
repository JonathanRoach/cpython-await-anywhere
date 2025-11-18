#ifndef Py_INTERNAL_COR_PLATFORM_H
#define Py_INTERNAL_COR_PLATFORM_H
// platform specific parts collected together
#include "Python.h"
#include "pycore_lock.h"

#define _Cor_thread_local _Py_thread_local

#define COROUTINE_HAVE_ALLOCA_H HAVE_ALLOCA_H

#define _Cor_Mutex PyMutex
static inline void _Cor_Mutex_ctor(_Cor_Mutex *mut){ *mut = (PyMutex){0};}
static inline void _Cor_Mutex_dtor(_Cor_Mutex *mut){(void)mut;}
#define _Cor_Mutex_Lock PyMutex_Lock
#define _Cor_Mutex_Unlock _PyMutex_TryUnlock

#endif
