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
inline void NiPhysXMeshFluidSystem::SetModelData(NiGeometryData* pkModelData)
{
    // Does nothng. Would like to assert but this gets called when
    // creating a fluid system from a regular particle system. 
}
//---------------------------------------------------------------------------
inline bool NiPhysXMeshFluidSystem::GetPhysXSpace() const
{
    return m_bPhysXSpace;
}
//---------------------------------------------------------------------------
inline void NiPhysXMeshFluidSystem::SetPhysXSpace(const bool bPhysXSpace)
{
    m_bPhysXSpace = bPhysXSpace;
}
//---------------------------------------------------------------------------
inline NiTransform& NiPhysXMeshFluidSystem::GetPhysXToFluid()
{
    return m_spDestination->GetPhysXToFluid();
}
//---------------------------------------------------------------------------
