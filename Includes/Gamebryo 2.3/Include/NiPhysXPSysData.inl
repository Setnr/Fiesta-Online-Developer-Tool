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
inline NiPhysXScenePtr NiPhysXPSysData::GetScene() const
{
    return m_spScene;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPSysData::GetCompartmentID() const
{
    return m_uiCompartmentID;
}
//---------------------------------------------------------------------------
inline void NiPhysXPSysData::SetCompartmentID(
    const unsigned int uiCompartmentID)
{
    m_uiCompartmentID = uiCompartmentID;
}
//---------------------------------------------------------------------------
inline NiPhysXParticleInfo* NiPhysXPSysData::GetPhysXParticleInfo()
{
    return m_pkPhysXParticleInfo;
}
//---------------------------------------------------------------------------
inline const NiPhysXParticleInfo* NiPhysXPSysData::GetPhysXParticleInfo() const
{
    return m_pkPhysXParticleInfo;
}
//---------------------------------------------------------------------------
inline void NiPhysXPSysData::SetActorAt(unsigned short usIndex,
    NxActor* pkActor)
{
    NIASSERT(usIndex < m_usVertices);
    m_pkPhysXParticleInfo[usIndex].m_pkActor = pkActor;
}
//---------------------------------------------------------------------------
inline int NiPhysXPSysData::GetActorPoolSize()
{
    return m_kActorPool.GetAllocatedSize();
}
//---------------------------------------------------------------------------
inline void NiPhysXPSysData::SetActorPoolSize(unsigned int uiSize)
{
    m_kActorPool.SetSize(uiSize);
}
//---------------------------------------------------------------------------
inline void NiPhysXPSysData::GetActorPoolRegion(NiPoint3& kCenter,
    NiPoint3& kDim)
{
    kCenter = m_kActorPoolCenter;
    kDim = m_kActorPoolDim;
}
//---------------------------------------------------------------------------
inline void NiPhysXPSysData::SetActorPoolRegion(const NiPoint3 kCenter,
    const NiPoint3 kDim)
{
    m_kActorPoolDim = kDim;
    m_kActorPoolCenter = kCenter;
}
//---------------------------------------------------------------------------
inline int NiPhysXPSysData::GetActorPoolCount()
{
    return m_kActorPool.GetSize();
}
//---------------------------------------------------------------------------
inline bool NiPhysXPSysData::IsActorPoolEmpty()
{
    return (m_kActorPool.GetSize() == 0);
}
//---------------------------------------------------------------------------
inline NxActor* NiPhysXPSysData::RemoveActorFromPool()
{
    return m_kActorPool.RemoveEnd();
}
//---------------------------------------------------------------------------
inline bool NiPhysXPSysData::GetFillActorPoolOnLoad()
{
    return m_bFillActorPoolOnLoad;
}
//---------------------------------------------------------------------------
inline void NiPhysXPSysData::SetKeepsActorMeshes(bool bKeep)
{
    m_bKeepsActorMeshes = bKeep;
}
//---------------------------------------------------------------------------
inline bool NiPhysXPSysData::GetKeepsActorMeshes()
{
    return m_bKeepsActorMeshes;
}
//---------------------------------------------------------------------------
inline void NiPhysXPSysData::SetFillActorPoolOnLoad(bool bFill)
{
    m_bFillActorPoolOnLoad = bFill;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPSysData::GetDefaultActorPoolSize()
{
    return m_uiDefaultActorPoolSize;
}
//---------------------------------------------------------------------------
inline void NiPhysXPSysData::SetDefaultActorPoolSize(unsigned int uiSize)
{
    m_uiDefaultActorPoolSize = uiSize;
}
//---------------------------------------------------------------------------
inline NiPhysXActorDesc* NiPhysXPSysData::GetActorDesc() const
{
    return m_spActorDescriptor;
}
//---------------------------------------------------------------------------
inline NiPhysXActorDescPtr NiPhysXPSysData::SetActorDesc(
    NiPhysXActorDesc* pkActor)
{
    NiPhysXActorDescPtr spOldPtr = m_spActorDescriptor;
    
    m_spActorDescriptor = pkActor;

    return spOldPtr;
}
//---------------------------------------------------------------------------
