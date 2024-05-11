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
inline void NiPhysXBodyDesc::SetFromBodyDesc(
    const NxBodyDesc& kBodyDesc, const NxMat33& kXform)
{
    m_kBodyDesc = kBodyDesc;
    SetVelocities(kBodyDesc.linearVelocity, kBodyDesc.angularVelocity, 0,
        kXform);
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXBodyDesc::AddState(NxActor* pkActor,
    const NxMat33& kXform)
{
    unsigned int uiIndex = m_uiNumVels;
    SetVelocities(pkActor->getLinearVelocity(),
        pkActor->getAngularVelocity(), uiIndex, kXform);
    return uiIndex;
}
//---------------------------------------------------------------------------
inline void NiPhysXBodyDesc::UpdateState(NxActor* pkActor,
    const unsigned int uiIndex, const NxMat33& kXform)
{
    NIASSERT(uiIndex < m_uiNumVels);
    
    m_pkLinVels[uiIndex] = kXform * pkActor->getLinearVelocity();
    m_pkAngVels[uiIndex] = kXform * pkActor->getAngularVelocity();    
}
//---------------------------------------------------------------------------
inline void NiPhysXBodyDesc::RestoreState(NxActor* pkActor,
    const unsigned int uiIndex, const NxMat33& kXform)
{
    NIASSERT(uiIndex < m_uiNumVels);
    if (!pkActor->readBodyFlag(NX_BF_KINEMATIC))
    {
        pkActor->setLinearVelocity(kXform * m_pkLinVels[uiIndex]);
        pkActor->setAngularVelocity(kXform * m_pkAngVels[uiIndex]);
    }
}
//---------------------------------------------------------------------------
inline bool NiPhysXBodyDesc::StreamCanSkip()
{
    return true;
}
//---------------------------------------------------------------------------
inline void NiPhysXBodyDesc::GetVelocities(NxVec3& kLinearVelocity,
    NxVec3& kAngularVelocity, const unsigned int uiIndex) const
{
    kLinearVelocity = m_pkLinVels[uiIndex];
    kAngularVelocity = m_pkAngVels[uiIndex];
}
//---------------------------------------------------------------------------
