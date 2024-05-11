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
inline NxActor* NiPhysXRigidBodyDest::GetActor() const
{
    return m_pkActor;
}
//---------------------------------------------------------------------------
inline void NiPhysXRigidBodyDest::SetActor(NxActor* pkActor)
{
    m_pkActor = pkActor;
}
//---------------------------------------------------------------------------
inline NxActor* NiPhysXRigidBodyDest::GetActorParent() const
{
    return m_pkActorParent;
}
//---------------------------------------------------------------------------
inline void NiPhysXRigidBodyDest::SetActorParent(NxActor* pkParent)
{
    m_pkActorParent = pkParent;
}
//---------------------------------------------------------------------------
inline bool NiPhysXRigidBodyDest::GetOptimizeSleep() const
{
    return m_bOptimizeSleep;
}
//---------------------------------------------------------------------------
inline void NiPhysXRigidBodyDest::SetOptimizeSleep(const bool bOptimize)
{
    m_bOptimizeSleep = bOptimize;
}
//---------------------------------------------------------------------------
inline float NiPhysXRigidBodyDest::GetTime(unsigned int uiIndex) const
{
    return m_afTimes[m_aucIndices[uiIndex]];
}
//---------------------------------------------------------------------------
inline const NxMat34& NiPhysXRigidBodyDest::GetPose(unsigned int uiIndex) const
{
    return m_akPoses[m_aucIndices[uiIndex]];
}
//---------------------------------------------------------------------------
inline bool NiPhysXRigidBodyDest::GetSleeping() const
{
    return m_bSleeping;
}
//---------------------------------------------------------------------------
