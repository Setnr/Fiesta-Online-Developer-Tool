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
inline NiPhysXScenePtr NiPhysXMeshPSysData::GetScene() const
{
    return m_spScene;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXMeshPSysData::GetCompartmentID() const
{
    return m_uiCompartmentID;
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshPSysData::SetCompartmentID(
    const unsigned int uiCompartmentID)
{
    m_uiCompartmentID = uiCompartmentID;
}
//---------------------------------------------------------------------------
inline NiPhysXParticleInfo* NiPhysXMeshPSysData::GetPhysXParticleInfo()
{
    return m_pkPhysXParticleInfo;
}
//---------------------------------------------------------------------------
inline const NiPhysXParticleInfo* NiPhysXMeshPSysData::GetPhysXParticleInfo()
    const
{
    return m_pkPhysXParticleInfo;
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshPSysData::SetActorAt(unsigned short usIndex,
    NxActor* pkActor)
{
    NIASSERT(usIndex < m_usVertices);
    m_pkPhysXParticleInfo[usIndex].m_pkActor = pkActor;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXMeshPSysData::GetNumGenerations()
{
    return m_kActorPools.GetSize();
}
//---------------------------------------------------------------------------
inline int NiPhysXMeshPSysData::GetActorPoolSize(unsigned int uiGeneration)
{
    return m_kActorPools.GetAt(uiGeneration)->GetAllocatedSize();
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshPSysData::SetActorPoolSize(unsigned int uiGeneration, 
    unsigned int uiSize)
{
    NIASSERT(uiGeneration < m_kActorPools.GetSize());

    m_kActorPools.GetAt(uiGeneration)->SetSize(uiSize);
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshPSysData::GetActorPoolRegion(NiPoint3& kCenter,
    NiPoint3& kDim)
{
    kCenter = m_kActorPoolCenter;
    kDim = m_kActorPoolDim;
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshPSysData::SetActorPoolRegion(const NiPoint3 kCenter,
    const NiPoint3 kDim)
{
    m_kActorPoolDim = kDim;
    m_kActorPoolCenter = kCenter;
}
//---------------------------------------------------------------------------
inline int NiPhysXMeshPSysData::GetActorPoolCount(unsigned int uiGeneration)
{
    NIASSERT(uiGeneration < m_kActorPools.GetSize());

    return m_kActorPools.GetAt(uiGeneration)->GetSize();
}
//---------------------------------------------------------------------------
inline bool NiPhysXMeshPSysData::IsActorPoolEmpty(unsigned int uiGeneration)
{
    NIASSERT(uiGeneration < m_kActorPools.GetSize());

    return (m_kActorPools.GetAt(uiGeneration)->GetSize() == 0);
}
//---------------------------------------------------------------------------
inline bool NiPhysXMeshPSysData::GetFillActorPoolsOnLoad()
{
    return m_bFillActorPoolsOnLoad;
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshPSysData::SetKeepsActorMeshes(bool bKeep)
{
    m_bKeepsActorMeshes = bKeep;
}
//---------------------------------------------------------------------------
inline bool NiPhysXMeshPSysData::GetKeepsActorMeshes()
{
    return m_bKeepsActorMeshes;
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshPSysData::SetFillActorPoolsOnLoad(bool bFill)
{
    m_bFillActorPoolsOnLoad = bFill;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXMeshPSysData::GetDefaultActorPoolsSize()
{
    return m_uiDefaultActorPoolSize;
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshPSysData::SetDefaultActorPoolsSize(unsigned int uiSize)
{
    m_uiDefaultActorPoolSize = uiSize;
}
//---------------------------------------------------------------------------
inline NiPhysXActorDesc* NiPhysXMeshPSysData::GetActorDescAt(
    unsigned int uiIndex) const
{
    if (uiIndex < m_kActorDescriptors.GetSize())
    {
        return m_kActorDescriptors.GetAt(uiIndex);
    }

    return NULL;
}
//---------------------------------------------------------------------------
inline NiPhysXActorDescPtr NiPhysXMeshPSysData::SetActorDescAt(
    unsigned int uiIndex, NiPhysXActorDesc* pkActor)
{
    if (uiIndex < m_kActorDescriptors.GetSize())
    {
        NiPhysXActorDescPtr spFormerActor = m_kActorDescriptors.GetAt(uiIndex);
        m_kActorDescriptors.SetAt(uiIndex, pkActor);
        return spFormerActor;
    }

    m_kActorDescriptors.SetAtGrow(uiIndex, pkActor);
    return NULL;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXMeshPSysData::GetActorDescCount() const
{
    return m_kActorDescriptors.GetSize();
}
//---------------------------------------------------------------------------
