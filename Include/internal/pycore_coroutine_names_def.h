// Maps names from their convenience name (eg Coroutine_New) to their NS names (eg Py_Coroutine_New)

// This allows you to rename all Coroutine things with your own namespace.
#ifndef Coroutine_NS
    #define Coroutine_NS(N) Coroutine_##N
#endif

#define Coroutine_SetStackLimit Coroutine_NS(SetStackLimit)
#define Coroutine_GetReport Coroutine_NS(GetReport)
#define Coroutine_CheckIntegrity Coroutine_NS(CheckIntegrity)
#define Coroutine_New Coroutine_NS(New)
#define Coroutine_RunSystem Coroutine_NS(RunSystem)
#define Coroutine_Run Coroutine_NS(Run)
#define Coroutine_Delete Coroutine_NS(Delete)
#define Coroutine_Continue Coroutine_NS(Continue)
#define Coroutine_Yield Coroutine_NS(Yield)
#define Coroutine_GetValue Coroutine_NS(GetValue)
#define Coroutine_GetActive Coroutine_NS(GetActive)
#define Coroutine_GetStackHeadroom Coroutine_NS(GetStackHeadroom)
#define Coroutine_GetStackHWM Coroutine_NS(GetStackHWM)
#define Coroutine_ClearStackForHWM Coroutine_NS(ClearStackForHWM)
#define Coroutine_CanStartCoroutine Coroutine_NS(CanStartCoroutine)
#define Coroutine_GetUsefulFreeSpace Coroutine_NS(GetUsefulFreeSpace)
#define Coroutine_GetCStackTop Coroutine_NS(GetCStackTop)
#define Coroutine_Chain Coroutine_NS(Chain)
#define Coroutine_CallWithMaxStack Coroutine_NS(CallWithMaxStack)
#define Coroutine_IsStarted Coroutine_NS(IsStarted)
#define Coroutine_IsRunning Coroutine_NS(IsRunning)
#define Coroutine_IsComplete Coroutine_NS(IsComplete)
#define Coroutine_Dump_ Coroutine_NS(Dump_)
