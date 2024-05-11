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
inline NiPhysXMeshPSysPropPtr NiPhysXMeshParticleSystem::GetProp() const
{
    return m_spProp;
}
//---------------------------------------------------------------------------
inline NiPhysXScenePtr NiPhysXMeshParticleSystem::GetScene() const
{
    return ((NiPhysXMeshPSysData*)GetModelData())->GetScene();
}
//---------------------------------------------------------------------------
inline bool NiPhysXMeshParticleSystem::GetUsesPhysXRotations() const
{
    return m_bPhysXRotations;
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshParticleSystem::SetUsesPhysXRotations(
    const bool bPhysXRotations)
{
    m_bPhysXRotations = bPhysXRotations;
}
//---------------------------------------------------------------------------
inline bool NiPhysXMeshParticleSystem::GetPhysXSpace() const
{
    return m_bPhysXSpace;
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshParticleSystem::SetPhysXSpace(const bool bPhysXSpace)
{
    m_bPhysXSpace = bPhysXSpace;
}
//---------------------------------------------------------------------------
