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
inline NiFixedString NiPhysXClothDesc::GetClothName() const
{
    return m_kClothName;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetClothName(NiFixedString& kClothName)
{
    m_kClothName = kClothName;
}
//---------------------------------------------------------------------------
inline NiPhysXMeshDescPtr NiPhysXClothDesc::GetMeshDesc()
{
    return m_spClothMesh;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetMeshDesc(NiPhysXMeshDesc* pkMeshDesc)
{
    m_spClothMesh = pkMeshDesc;
}
//---------------------------------------------------------------------------
inline NxMat34 NiPhysXClothDesc::GetGlobalPose() const
{
    return m_kGlobalPose;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetGlobalPose(const NxMat34& kPose)
{
    m_kGlobalPose = kPose;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetThickness() const
{
    return m_fThickness;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetThickness(const NxReal fThickness)
{
    m_fThickness = fThickness;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetDensity() const
{
    return m_fDensity;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetDensity(const NxReal fDensity)
{
    m_fDensity = fDensity;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetBendingStiffness() const
{
    return m_fBendingStiffness;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetBendingStiffness(const NxReal fStiffness)
{
    m_fBendingStiffness = fStiffness;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetStretchingStiffness() const
{
    return m_fStretchingStiffness;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetStretchingStiffness(const NxReal fStiffness)
{
    m_fStretchingStiffness = fStiffness;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetDampingCoefficient() const
{
    return m_fDampingCoefficient;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetDampingCoefficient(const NxReal fCoefficient)
{
    m_fDampingCoefficient = fCoefficient;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetFriction() const
{
    return m_fFriction;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetFriction(const NxReal fFriction)
{
    m_fFriction = fFriction;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetPressure() const
{
    return m_fPressure;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetPressure(const NxReal fPressure)
{
    m_fPressure = fPressure;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetTearFactor() const
{
    return m_fTearFactor;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetTearFactor(const NxReal fTearFactor)
{
    m_fTearFactor = fTearFactor;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetCollisionResponseCoefficient() const
{
    return m_fCollisionResponseCoefficient;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetCollisionResponseCoefficient(
    const NxReal fCoefficient)
{
    m_fCollisionResponseCoefficient = fCoefficient;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetAttachmentResponseCoefficient() const
{
    return m_fAttachmentResponseCoefficient;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetAttachmentResponseCoefficient(
    const NxReal fCoefficient)
{
    m_fAttachmentResponseCoefficient = fCoefficient;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetAttachmentTearFactor() const
{
    return m_fAttachmentTearFactor;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetAttachmentTearFactor(const NxReal fFactor)
{
    m_fAttachmentTearFactor = fFactor;
}
//---------------------------------------------------------------------------
inline NxU32 NiPhysXClothDesc::GetSolverIterations() const
{
    return m_uiSolverIterations;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetSolverIterations(const NxU32 uiIterations)
{
    m_uiSolverIterations = uiIterations;
}
//---------------------------------------------------------------------------
inline NxVec3 NiPhysXClothDesc::GetExternalAcceleration() const
{
    return m_kExternalAcceleration;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetExternalAcceleration(
    const NxVec3 kAcceleration)
{
    m_kExternalAcceleration = kAcceleration;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetWakeUpCounter() const
{
    return m_fWakeUpCounter;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetWakeUpCounter(const NxReal fCounter)
{
    m_fWakeUpCounter = fCounter;
}
//---------------------------------------------------------------------------
inline NxReal NiPhysXClothDesc::GetSleepLinearVelocity() const
{
    return m_fSleepLinearVelocity;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetSleepLinearVelocity(const NxReal fVelocity)
{
    m_fSleepLinearVelocity = fVelocity;
}
//---------------------------------------------------------------------------
inline NxCollisionGroup NiPhysXClothDesc::GetCollisionGroup() const
{
    return m_uiCollisionGroup;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetCollisionGroup(const NxCollisionGroup uiGroup)
{
    m_uiCollisionGroup = uiGroup;
}
//---------------------------------------------------------------------------
inline NxGroupsMask NiPhysXClothDesc::GetCollisionMask() const
{
    NxGroupsMask kMask;
    kMask.bits0 = m_auiCollisionBits[0];
    kMask.bits1 = m_auiCollisionBits[1];
    kMask.bits2 = m_auiCollisionBits[2];
    kMask.bits3 = m_auiCollisionBits[3];
    return kMask;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetCollisionMask(const NxGroupsMask kMask)
{
    m_auiCollisionBits[0] = kMask.bits0;
    m_auiCollisionBits[1] = kMask.bits1;
    m_auiCollisionBits[2] = kMask.bits2;
    m_auiCollisionBits[3] = kMask.bits3;
}
//---------------------------------------------------------------------------
inline NxU32 NiPhysXClothDesc::GetClothFlags() const
{
    return m_uiFlags;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetClothFlags(const NxU32 uiFlags)
{
    m_uiFlags = uiFlags;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::AddAttachment(
    NiPhysXClothDesc::ClothAttachment* pkAttachment)
{
    m_kAttachments.AddFirstEmpty(pkAttachment);
}
//---------------------------------------------------------------------------
inline NxCloth* NiPhysXClothDesc::GetCloth() const
{
    return m_pkCloth;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetCloth(NxCloth* pkCloth)
{
    m_pkCloth = pkCloth;
}
//---------------------------------------------------------------------------
inline NiPhysXActorDesc* NiPhysXClothDesc::GetParentActor() const
{
    return m_pkParentActor;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetParentActor(NiPhysXActorDesc* pkActorDesc)
{
    m_pkParentActor = pkActorDesc;
}
//---------------------------------------------------------------------------
inline NiPhysXClothDest* NiPhysXClothDesc::GetDest() const
{
    return m_spDest;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDesc::SetDest(NiPhysXClothDest* pkDest)
{
    m_spDest = pkDest;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXClothDesc::GetAttachmentCount() const
{
    return m_kAttachments.GetSize();
}
//---------------------------------------------------------------------------
inline NiPhysXClothDesc::ClothAttachment* NiPhysXClothDesc::GetAttachmentAt(
    const unsigned int uiIndex)
{
    return m_kAttachments.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline bool NiPhysXClothDesc::RemoveAttachment(ClothAttachment* pkAttachment)
{
    unsigned int uiResult = m_kAttachments.Remove(pkAttachment);
    if (uiResult != ((unsigned int)~0))
    {
        NiDelete pkAttachment;
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
inline bool NiPhysXClothDesc::RemoveAttachmentAt(const unsigned int uiIndex)
{
    ClothAttachment* pkAttachment = m_kAttachments.RemoveAt(uiIndex);
    if (pkAttachment)
    {
        NiDelete pkAttachment;
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
inline bool NiPhysXClothDesc::StreamCanSkip()
{
    return true;
}
//---------------------------------------------------------------------------
