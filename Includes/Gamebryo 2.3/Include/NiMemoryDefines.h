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

#ifndef NIMEMORYDEFINES_H
#define NIMEMORYDEFINES_H

#include "NiSystemLibType.h"
#include "NiBuildConfiguration.h"
#include "NiDebug.h"

#ifdef _PS3
#include <stddef.h>
#else
#include <malloc.h>
#endif

//---------------------------------------------------------------------------
// Determine whether or not to store the File, Line, Function data.
//---------------------------------------------------------------------------
// Uncomment this #if block in order to turn on memory debugging in
// debug builds.

// Use memory debugging only in NiMemory builds
#if NI_USE_MEMORY_MANAGEMENT && !defined(NI_MEMORY_DEBUGGER)
    #define NI_MEMORY_DEBUGGER
#endif

//---------------------------------------------------------------------------
// NiMemEventType.
//
// Determine the overall allocation type. When an object is allocated with
// an allocation routine, it should be deallocated with its corresponding
// deallocation routine.
//---------------------------------------------------------------------------

enum NiMemEventType
{
    NI_UNKNOWN = 0,                 // Unknown allocation/deallocation type.
                                    // This type will cause assertions in
                                    // the memory management interfaces.
    NI_OPER_NEW,                    // Allocated through NiNew. 
                                    // Should be deallocated with NiDelete
    NI_OPER_NEW_ARRAY,              // Allocated through NiNew[]. 
                                    // Should be deallocated with NiDelete[]
    NI_OPER_DELETE,                 // Deallocated with NiDelete.

    NI_OPER_DELETE_ARRAY,           // Deallocated with NiDelete[]
    
    NI_MALLOC,                      // Allocated through NiMalloc.
                                    // Should be deallocated with NiFree
    NI_REALLOC,                     // Allocated through NiRealloc.
                                    // Should be deallocated with NiFree.
    NI_ALIGNEDMALLOC,               // Allocated through NiAlignedAlloc.
                                    // Should be deallocated with 
                                    // NiAlignedFree
    NI_ALIGNEDREALLOC,              // Allocated through NiAlignedRealloc.
                                    // Should be deallocated with 
                                    // NiAlignedFree
    NI_FREE,                        // Deallocated with NiFree
    NI_ALIGNEDFREE,                 // Deallocated with NiAlignedFree
    NI_EXTERNALALLOC,               // Used to track external allocations
    NI_EXTERNALFREE                 // Used to track external frees
};

//---------------------------------------------------------------------------
// Allocation macros and functions.
//
// NiNew and NiDelete are ONLY used for NiMemObject-derived classes.
// This lets Gamebryo store the file, line, function information depending
// on the build target.
//
// NiAlloc/NiMalloc/NiCalloc/NiRealloc and NiFree should be used for 
// any non-NiMemObject object or intrinsic type. Note that they do not call
// global new or delete, nor will they call constructors or destructors.
//---------------------------------------------------------------------------

#define NI_MEM_FILE_DEFAULT 0
#define NI_MEM_LINE_DEFAULT -1
#define NI_MEM_FUNCTION_DEFAULT 0
#define NI_MEM_DEALLOC_SIZE_DEFAULT (size_t)-1

#ifdef _XENON
    #define NI_MEM_ALIGNMENT_DEFAULT  16
#elif defined(_PS3)
    #define NI_MEM_ALIGNMENT_DEFAULT  16
#else
    #define NI_MEM_ALIGNMENT_DEFAULT  4
#endif

// Build-independent memory management routines
NISYSTEM_ENTRY void  _NiFree(void* pvMemory);
NISYSTEM_ENTRY void  _NiAlignedFree(void* pvMemory);

// Build-dependent memory management routines
#ifdef NI_MEMORY_DEBUGGER
    NISYSTEM_ENTRY void* _NiMalloc(size_t stSizeInBytes, 
        const char* pcSourceFile, int iSourceLine, const char* pcFunction);
    NISYSTEM_ENTRY void* _NiAlignedMalloc(size_t stSizeInBytes, 
        size_t stAlignment, const char* pcSourceFile, int iSourceLine,
        const char* pcFunction);
    NISYSTEM_ENTRY void* _NiRealloc(void *memblock, size_t stSizeInBytes, 
        const char* pcSourceFile, int iSourceLine, const char* pcFunction); 
    NISYSTEM_ENTRY void* _NiAlignedRealloc(void *memblock, 
        size_t stSizeInBytes, size_t stAlignment, const char* pcSourceFile,
        int iSourceLine, const char* pcFunction); 
    NISYSTEM_ENTRY bool NiVerifyAddress(const void* pvMemory);
    NISYSTEM_ENTRY bool _NiTrackAlloc(const void* pvMemory, 
        size_t stSizeInBytes, const char* pcSourceFile, int iSourceLine, 
        const char* pcFunction); 
    NISYSTEM_ENTRY bool _NiTrackFree(const void* pvMemory); 

    #define NiNew new(__FILE__,__LINE__,__FUNCTION__)
    #define NiDelete delete
    #define NiAlloc(T, count) ((T*)_NiMalloc(sizeof(T)*(count), \
        __FILE__, __LINE__, __FUNCTION__)) 
    #define NiAlignedAlloc(T, count, alignment) ((T*)_NiAlignedMalloc( \
        sizeof(T)*(count), alignment, __FILE__, __LINE__, __FUNCTION__)) 
    #define NiAlignedMalloc(size, alignment) (_NiAlignedMalloc(size, \
        alignment, __FILE__, __LINE__, __FUNCTION__)) 
    #define NiMalloc(size) (_NiMalloc(size, \
        __FILE__, __LINE__, __FUNCTION__)) 
    #define NiRealloc(memblock, size) (_NiRealloc(memblock, size, \
        __FILE__, __LINE__, __FUNCTION__)) 
    #define NiAlignedRealloc(memblock, size, alignment) (_NiAlignedRealloc( \
        memblock, size, alignment, __FILE__, __LINE__, __FUNCTION__))
    #define NiFree(p) _NiFree(p)
    #define NiAlignedFree(p) _NiAlignedFree(p)
    #define NiTrackAlloc(pointer, sizeInBytes) (_NiTrackAlloc(pointer,  \
        sizeInBytes,__FILE__, __LINE__, __FUNCTION__)) 
    #define NiTrackFree(pointer)  (_NiTrackFree(pointer)) 
#else
    NISYSTEM_ENTRY void* _NiMalloc(size_t stSizeInBytes);
    NISYSTEM_ENTRY void* _NiAlignedMalloc(size_t stSizeInBytes,
        size_t stAlignment);
    NISYSTEM_ENTRY void* _NiRealloc(void *memblock, size_t stSizeInBytes); 
    NISYSTEM_ENTRY void* _NiAlignedRealloc(void *memblock, 
        size_t stSizeInBytes, size_t stAlignment); 

    #define NiVerifyAddress(x) false
    
    #define NiNew new
    #define NiDelete delete
    #define NiAlloc(T, count) ((T*)_NiMalloc(sizeof(T)*(count)))
    #define NiAlignedAlloc(T, count, alignment) ((T*)_NiAlignedMalloc( \
        sizeof(T)*(count), alignment))
    #define NiAlignedMalloc(size, alignment) (_NiAlignedMalloc(size, \
        alignment)) 
    #define NiMalloc(size) (_NiMalloc(size)) 
    #define NiRealloc(memblock, size) (_NiRealloc(memblock, size))
    #define NiAlignedRealloc(memblock, size, alignment) (_NiAlignedRealloc( \
        memblock, size, alignment))
    #define NiFree(p) _NiFree(p)
    #define NiAlignedFree(p) _NiAlignedFree(p)
    #define NiTrackAlloc(p, sizeInBytes) false
    #define NiTrackFree(p) false
#endif // NI_MEMORY_DEBUGGER

// In many cases, external resources will need to be allocated.
// In this case, use the following macros:
#define NiExternalNew new
#define NiExternalDelete delete
#define NiExternalAlloc(T, count) ((T*)malloc(sizeof(T)*(count)))
#define NiExternalMalloc malloc
#define NiExternalRealloc realloc
#define NiExternalFree free
#define NiExternalCalloc calloc

#ifdef _PS3
    #define NiExternalAlignedMalloc(size, alignment) memalign(alignment, size)
    #define NiExternalAlignedRealloc reallocalign
    #define NiExternalAlignedFree free
    #define NiAlignOf(T) __alignof__(T)
#else
    #define NiExternalAlignedMalloc _aligned_malloc
    #define NiExternalAlignedRealloc _aligned_realloc
    #define NiExternalAlignedFree _aligned_free
    #define NiAlignOf(T) __alignof(T)
#endif

#ifdef _PS3
    class NiAllocUnit;
    void NiLogMemAssert(const char* pcCondition, NiAllocUnit* pkUnit, 
        const char* pcFile, int iLine, const char* pcFunction);

    #if defined(NI_MEMORY_DEBUGGER) && defined(_DEBUG)
        #define NIMEMASSERT(value) {NIASSERT(value);}
        #define NIMEMASSERTUNIT(value, unit) {NIASSERT(value);}
    #elif defined(NI_MEMORY_DEBUGGER) && defined(NDEBUG)
        #define NIMEMASSERT(value) {bool bValue = 0 != (value); \
            if (!bValue) {NiLogMemAssert(#value, 0, __FILE__, __LINE__, \
            __FUNCTION__);}}
        #define NIMEMASSERTUNIT(value, unit) {bool bValue = (bool)(value); \
            if (!bValue) {NiLogMemAssert(#value, pkUnit, __FILE__, __LINE__, \
            __FUNCTION__);}}
    #else
        #define NIMEMASSERT(value)
        #define NIMEMASSERTUNIT(value, unit)
    #endif // if defined(NI_MEMORY_DEBUGGER)
#else
    #if defined(NI_MEMORY_DEBUGGER) && defined(_DEBUG)
        #define NIMEMASSERT(value) {NIASSERT(value);}
        #define NIMEMASSERTUNIT(value, unit) {NIASSERT(value);}
    #elif defined(NI_MEMORY_DEBUGGER) && defined(NDEBUG)
        #define NIMEMASSERT(value) {bool bValue = 0 != (value); \
            if (!bValue) {NiMessageBox(#value, "NIMEMASSERT!");}}
        #define NIMEMASSERTUNIT(value, unit) {bool bValue = (bool)(value); \
            if (!bValue) {NiMessageBox(#value, "NIMEMASSERT!");}}
    #else
        #define NIMEMASSERT(value) (0)
        #define NIMEMASSERTUNIT(value, unit) (0)
    #endif // #if defined(NI_MEMORY_DEBUGGER) && defined(_DEBUG)
#endif // #ifdef _PS3

// Platform-specific allocation routines
#ifdef _XENON
#ifdef NI_MEMORY_DEBUGGER
    NISYSTEM_ENTRY void* _NiPhysicalAlloc(size_t stSizeInBytes, 
        size_t stDesiredPhysicalAddress, size_t stAlignment,
        unsigned int uiFlags, const char* pcSourceFile, int iSourceLine,
        const char* pcFunction);
    NISYSTEM_ENTRY void _NiPhysicalFree(void* p);
    #define NiPhysicalAlloc(size, desiredAddress, alignment, flags) \
        (_NiPhysicalAlloc(size, desiredAddress, alignment, flags, \
        __FILE__, __LINE__, __FUNCTION__))
    #define NiPhysicalFree(p) (_NiPhysicalFree(p))

#else
    NISYSTEM_ENTRY void* _NiPhysicalAlloc(size_t stSizeInBytes, 
        size_t stDesiredPhysicalAddress, size_t stAlignment,
        unsigned int uiFlags);
    NISYSTEM_ENTRY void _NiPhysicalFree(void* p);
    #define NiPhysicalAlloc(size, desiredAddress, alignment, flags) \
        (_NiPhysicalAlloc(size, desiredAddress, alignment, flags))
    #define NiPhysicalFree(p) (_NiPhysicalFree(p))

#endif // NI_MEMORY_DEBUGGER
#endif // _XENON
#endif // NIMEMORYDEFINES_H
