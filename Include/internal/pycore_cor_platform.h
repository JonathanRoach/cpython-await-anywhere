#ifndef Py_INTERNAL_COR_PLATFORM_H
#define Py_INTERNAL_COR_PLATFORM_H
// platform specific parts collected together
#include "Python.h"
#include "pycore_lock.h"

#define _Cor_thread_local _Py_thread_local

#ifdef HAVE_ALLOCA_H
 #define COROUTINE_HAVE_ALLOCA_H HAVE_ALLOCA_H
#endif

#define _Cor_Mutex PyMutex
static inline int _Cor_Mutex_ctor(_Cor_Mutex *mut){ *mut = (PyMutex){0}; return 0;}
static inline int _Cor_Mutex_dtor(_Cor_Mutex *mut){(void)mut; return 0;}
static inline int _Cor_Mutex_Lock(_Cor_Mutex *mut){ PyMutex_Lock(mut); return 0;}
static inline int _Cor_Mutex_Unlock(_Cor_Mutex *mut){ _PyMutex_TryUnlock(mut); return 0;}

#endif
