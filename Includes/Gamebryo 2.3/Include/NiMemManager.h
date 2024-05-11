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

#ifndef NIMEMMANAGER_H
#define NIMEMMANAGER_H

#include "NiSystemLibType.h"
#include "NiRTLib.h"
#include "NiOS.h"
#include "NiAllocator.h"
#include "NiCriticalSection.h"

class NISYSTEM_ENTRY NiMemManager
{
public:
    // Singleton access
    static NiMemManager& Get();
    static bool IsInitialized();

    // 3 funnel points for all memory operations:
    void* Allocate(
        size_t stSize, 
        size_t stAlignment, 
        NiMemEventType eEventType,
        bool bProvideAccurateSizeOnDeallocate = false,
        const char *pcSourceFile = NI_MEM_FILE_DEFAULT,
        int iSourceLine = NI_MEM_LINE_DEFAULT,
        const char* pcFunction = NI_MEM_FUNCTION_DEFAULT);

    void* Reallocate(
        void* pvMem, 
        size_t stSize, 
        size_t stAlignment, 
        NiMemEventType eEventType, 
        bool bProvideAccurateSizeOnDeallocate = false,
        size_t stSizeCurrent = NI_MEM_DEALLOC_SIZE_DEFAULT,
        const char *pcSourceFile = NI_MEM_FILE_DEFAULT,
        int iSourceLine = NI_MEM_LINE_DEFAULT,
        const char* pcFunction = NI_MEM_FUNCTION_DEFAULT);

    void Deallocate(
        void* pvMem, 
        NiMemEventType eEventType,
        size_t stSizeinBytes= NI_MEM_DEALLOC_SIZE_DEFAULT);

    bool TrackAllocate(
        const void* const pvMemory, 
        size_t stSizeInBytes, 
        NiMemEventType eEventType, 
        const char *pcSourceFile = NI_MEM_FILE_DEFAULT,
        int iSourceLine = NI_MEM_LINE_DEFAULT,
        const char* pcFunction = NI_MEM_FUNCTION_DEFAULT);

    bool TrackDeallocate(
        const void* const pvMemory, 
        NiMemEventType eEventType);

    static void _SDMInit();
    static void _SDMShutdown();

    static bool VerifyAddress(const void* pvMemory);

    // Utility functions
    static const char* NiMemEventTypeToString(NiMemEventType eEventType);
protected:
    NiMemManager() {};
    NiMemManager(const NiMemManager&) {};
    static NiMemManager* ms_pkMemManager;
    NiAllocator* m_pkAllocator;
    friend class NiMemObject;
};

#include "NiMemManager.inl"

#endif // #ifndef NIMEMMANAGER_H
