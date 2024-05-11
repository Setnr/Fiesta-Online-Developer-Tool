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

//---------------------------------------------------------------------------
inline NiMemManager& NiMemManager::Get()
{
    return *(ms_pkMemManager);
}
//---------------------------------------------------------------------------
inline void* NiMemManager::Allocate(
    size_t stSizeInBytes, 
    size_t stAlignment, 
    NiMemEventType eEventType,
    bool bProvideAccurateSizeOnDeallocate,
    const char *pcSourceFile,
    int iSourceLine, 
    const char* pcFunction)
{
    NIASSERT(m_pkAllocator);

    void* pvMem = m_pkAllocator->Allocate(stSizeInBytes, stAlignment, 
        eEventType, bProvideAccurateSizeOnDeallocate, 
        pcSourceFile, iSourceLine, pcFunction);
    
    return pvMem;
}
//---------------------------------------------------------------------------
inline void* NiMemManager::Reallocate(
    void* pvMem, 
    size_t stSizeInBytes, 
    size_t stAlignment, 
    NiMemEventType eEventType,
    bool bProvideAccurateSizeOnDeallocate,
    size_t stSizeCurrent,
    const char *pcSourceFile,
    int iSourceLine, 
    const char* pcFunction)
{
    NIASSERT(m_pkAllocator);

    void* pvNewMem = m_pkAllocator->Reallocate(pvMem, stSizeInBytes,
        stAlignment, eEventType, bProvideAccurateSizeOnDeallocate,
        stSizeCurrent, pcSourceFile, iSourceLine, 
        pcFunction);
    return pvNewMem;
}
//---------------------------------------------------------------------------
inline  void NiMemManager::Deallocate(void* pvMem,
    NiMemEventType eEventType, size_t stSizeinBytes)
{
    NIASSERT(m_pkAllocator);

    m_pkAllocator->Deallocate(pvMem, eEventType, stSizeinBytes);
 
}
//---------------------------------------------------------------------------
inline bool NiMemManager::TrackAllocate(const void* const pvMem,  
    size_t stSizeInBytes, NiMemEventType eEventType, 
    const char *pcSourceFile, int iSourceLine, const char* pcFunction)
{
    NIASSERT(m_pkAllocator);
    return m_pkAllocator->TrackAllocate(pvMem, stSizeInBytes, eEventType,
        pcSourceFile, iSourceLine, pcFunction);
}
//---------------------------------------------------------------------------
inline  bool NiMemManager::TrackDeallocate(
    const void* const pvMem, NiMemEventType eEventType)
{
    NIASSERT(m_pkAllocator);
    return m_pkAllocator->TrackDeallocate(pvMem, eEventType);
}
//---------------------------------------------------------------------------
inline const char* NiMemManager::NiMemEventTypeToString(
    NiMemEventType eEventType)
{
    switch (eEventType)
    {
    case NI_UNKNOWN:
        return "NI_UNKNOWN";
    case NI_OPER_NEW:
        return "NI_OPER_NEW";
    case NI_OPER_NEW_ARRAY:
        return "NI_OPER_NEW_ARRAY";
    case NI_OPER_DELETE:
        return "NI_OPER_DELETE";
    case NI_OPER_DELETE_ARRAY:
        return "NI_OPER_DELETE_ARRAY";
    case NI_MALLOC:
        return "NI_MALLOC";
    case NI_REALLOC:
        return "NI_REALLOC";
    case NI_ALIGNEDMALLOC:
        return "NI_ALIGNEDMALLOC";
    case NI_ALIGNEDREALLOC:
        return "NI_ALIGNEDREALLOC";
    case NI_FREE:
        return "NI_FREE";
    case NI_ALIGNEDFREE:
        return "NI_ALIGNEDFREE";
    case NI_EXTERNALALLOC:
        return "NI_EXTERNALALLOC";
    case NI_EXTERNALFREE:
        return "NI_EXTERNALFREE";
    default:
        NIASSERT(!"Unknown NiMemEventType  value!");
    };

    return NULL;
}
//---------------------------------------------------------------------------
