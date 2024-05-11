// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2007 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef NISYSTEM_H
#define NISYSTEM_H

#include "NiRTLib.h"
#include "NiOS.h"
#include "NiDebug.h"
#include "NiMemManager.h"
#include "NiMemObject.h"
#include "NiFile.h"
#include "NiPath.h"
#include "NiLog.h"
#include "NiSystemSDM.h"
#include "NiCriticalSection.h"
#include "NiMessageBox.h"
#include "NiSystemDesc.h"
#include "NiEndian.h"
#include "NiTCreate.h"
#include "NiUniversalTypes.h"

// Wrappers for some C Runtime functions
// Functions defined inline, so no need for NISYSTEM_ENTRY
char* NiStrcpy(char* pcDest, size_t stDestSize, const char* pcSrc);
char* NiStrncpy(char* pcDest, size_t stDestSize, const char* pcSrc, 
    size_t stCount);
char* NiStrcat(char* pcDest, size_t stDestSize, const char* pcSrc);
char* NiStrtok(char* pcString, const char* pcDelimit, char** ppcContext);
int NiSprintf(char* pcDest, size_t stDestSize, const char* pcFormat, ...);
int NiVsprintf(char* pcDest, size_t stDestSize, const char* pcFormat,
    va_list kArgs);
int NiSnprintf(char* pcDest, size_t stDestSize, size_t stCount,
    const char* pcFormat, ...);
int NiVsnprintf(char* pcDest, size_t stDestSize, size_t stCount,
    const char* pcFormat, va_list kArgs);
int NiMemcpy(void* pvDest, size_t stDestSize, const void* pvSrc, 
    size_t stCount);
int NiMemcpy(void* pvDest, const void* pvSrc, size_t stCount);
int NiMemmove(void* pvDest, size_t stDestSize, const void* pvSrc, 
    size_t stCount);


// Unicode/Wide functions defined inline, so no need for NISYSTEM_ENTRY
NiWChar* NiWStrcpy(NiWChar* pkDest, size_t stDestSize, const NiWChar* pkSrc);
NiWChar* NiWStrncpy(NiWChar* pkDest, size_t stDestSize, const NiWChar* pkSrc, 
    size_t stCount);

NiWChar* NiWStrcat(NiWChar* pkDest, size_t stDestSize, const NiWChar* pkSrc);
NiWChar* NiWStrtok(NiWChar* pkString, const NiWChar* pkDelimit, 
    NiWChar** ppkContext);

int NiWSprintf(NiWChar* pkDest, size_t stDestSize, 
    const NiWChar* pkFormat, ...);

// Constant for NiStrncpy function
#if defined(_MSC_VER) && _MSC_VER >= 1400
#define NI_TRUNCATE _TRUNCATE
#else //#if defined(_MSC_VER) && _MSC_VER >= 1400
#define NI_TRUNCATE ((size_t)-1)
#endif //#if defined(_MSC_VER) && _MSC_VER >= 1400

// Routines whose implementation depends on system
NISYSTEM_ENTRY int NiStricmp(const char* s1, const char* s2);
NISYSTEM_ENTRY int NiStrnicmp(const char* s1, const char* s2, size_t n);
NISYSTEM_ENTRY char* NiStrdup(const char* pcString);
NISYSTEM_ENTRY unsigned int NiRotr(unsigned int x, int n);
NISYSTEM_ENTRY void NiSleep(unsigned int uiMilliseconds);
NISYSTEM_ENTRY void NiYieldThread();
NISYSTEM_ENTRY unsigned long NiGetCurrentThreadId();
NISYSTEM_ENTRY NiInt32 NiAtomicIncrement(NiInt32 &i32Value);
NISYSTEM_ENTRY NiInt32 NiAtomicDecrement(NiInt32 &i32Value);
NISYSTEM_ENTRY NiUInt32 NiAtomicIncrement(NiUInt32 &ui32Value);
NISYSTEM_ENTRY NiUInt32 NiAtomicDecrement(NiUInt32 &ui32Value);
NISYSTEM_ENTRY NiInt64 NiAtomicIncrement(NiInt64 &i64Value);
NISYSTEM_ENTRY NiInt64 NiAtomicDecrement(NiInt64 &i64Value);
NISYSTEM_ENTRY NiUInt64 NiAtomicIncrement(NiUInt64 &ui64Value);
NISYSTEM_ENTRY NiUInt64 NiAtomicDecrement(NiUInt64 &ui64Value);

//---------------------------------------------------------------------------
// Routines that can potentially avoid branches on certain platforms
NISYSTEM_ENTRY float NiSelectGE(float fCondition, 
    float fResultIfGreaterThanEqualZero, float fResultIfLessThanZero);
NISYSTEM_ENTRY float NiSelectGE(int iCondition, 
    float fResultIfGreaterThanEqualZero, float fResultIfLessThanZero);
NISYSTEM_ENTRY int NiSelect(bool bCondition, int iResultIfTrue, 
    int iResultIfFalse);
NISYSTEM_ENTRY float NiSelect(bool bCondition, float fResultIfTrue, 
    float fResultIfFalse);
//---------------------------------------------------------------------------

// Routines that query the system
NISYSTEM_ENTRY float NiGetCurrentTimeInSec();
NISYSTEM_ENTRY unsigned int NiGetFileSize (const char* pcFilename);
NISYSTEM_ENTRY unsigned int NiGetPerformanceCounter();
// NiGetPerformanceCounter has a different number of cycles per second
// on different platforms and even different processors.  Use
// NiSystemDesc::GetSystemDesc().GetPerformanceCounterHz()
// to get the conversion divisor.

// Routines that return information to user/programmer
#if !defined(NDEBUG)
NISYSTEM_ENTRY void NiWriteToStdErr(char* pText);
#if !defined(REPLACE_NIOUTPUTDEBUGSTRING_WITH_NILOG)
NISYSTEM_ENTRY void NiOutputDebugString(const char* pcOut);
#endif //#if !defined(REPLACE_NIOUTPUTDEBUGSTRING_WITH_NILOG)
#else //#if !defined(NDEBUG)
#define NiWriteToStdErr(x) ((void)0)
#define NiOutputDebugString(x) ((void)0)
#endif //#if !defined(NDEBUG)

#include "NiSystem.inl"

#endif
