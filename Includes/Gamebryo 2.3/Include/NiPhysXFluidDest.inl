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
inline NiParticleSystem* NiPhysXFluidDest::GetTarget()
{
    return m_pkTarget;
}
//---------------------------------------------------------------------------
inline NiTransform& NiPhysXFluidDest::GetPhysXToFluid()
{
    return m_kPhysXToFluid;
}
//---------------------------------------------------------------------------
inline void NiPhysXFluidDest::UpdateSceneGraph(const float fT,
    const NiTransform& kRootTransform, const float fScalePToW,
    const bool bForce)
{
    // This function deliberately left blank
}
//---------------------------------------------------------------------------
