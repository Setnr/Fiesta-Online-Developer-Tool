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
inline NiParticleSystem* NiPhysXPSysSrc::GetSource()
{
    return m_pkSource;
}
//---------------------------------------------------------------------------
inline void NiPhysXPSysSrc::UpdateFromSceneGraph(const float fT,
        const NiTransform& kInvRootTransform, const float fScaleWToP,
        const bool bForce)
{
    m_kInvRootTransform = kInvRootTransform;
    m_fScaleWToP = fScaleWToP;
}
//---------------------------------------------------------------------------
