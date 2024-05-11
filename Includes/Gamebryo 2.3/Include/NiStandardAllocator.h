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

#ifndef NISTANDARDALLOCATOR_H
#define NISTANDARDALLOCATOR_H

#include "NiAllocator.h"

#define NI_USE_SMALL_OBJECT_ALLOCATOR
class NiSmallObjectAllocator;

class NISYSTEM_ENTRY NiStandardAllocator : public NiAllocator
{
public:
    virtual void* Allocate(size_t& stSize,
        size_t& stAlignment,
        NiMemEventType eEventType,
        bool bProvideAccurateSizeOnDeallocate,
        const char* pcFile,
        int iLine,
        const char* pcFunction);

    virtual void Deallocate(void* pvMemory,
        NiMemEventType eEventType,
        size_t stSizeinBytes);

    virtual void* Reallocate(void* pvMemory,
        size_t& stSize,
        size_t& stAlignment,
        NiMemEventType eEventType,
        bool bProvideAccurateSizeOnDeallocate,
        size_t stSizeCurrent,
        const char* pcFile,
        int iLine,
        const char* pcFunction);

    virtual bool TrackAllocate(
        const void* const pvMemory, 
        size_t stSizeInBytes, 
        NiMemEventType eEventType, 
        const char* pcFile, 
        int iLine,
        const char* pcFunction);

    virtual bool TrackDeallocate(
        const void* const pvMemory, 
        NiMemEventType eEventType);

    // Called by the memory manager during NiInit()
    virtual void Initialize();
    // Called by the memory manager during NiShutdown()
    virtual void Shutdown();

    virtual bool VerifyAddress(const void* pvMemory);


#ifdef NI_USE_SMALL_OBJECT_ALLOCATOR
    static NiSmallObjectAllocator* GetSmallAllocator();
#endif

protected:
#ifdef NI_USE_SMALL_OBJECT_ALLOCATOR
    static NiSmallObjectAllocator* ms_pkSmallAlloc; 
#endif
    static size_t GetSizeFromAddress(void* pMemory);
    static void SetSizeToAddress(void* pMemory, size_t stSize);
};

#endif
