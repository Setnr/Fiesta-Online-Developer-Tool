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
inline bool NiPSysAgeDeathModifier::GetSpawnOnDeath() const
{
    return m_bSpawnOnDeath;
}
//---------------------------------------------------------------------------
inline void NiPSysAgeDeathModifier::SetSpawnOnDeath(bool bSpawnOnDeath)
{
    m_bSpawnOnDeath = bSpawnOnDeath;
}
//---------------------------------------------------------------------------
inline NiPSysSpawnModifier* NiPSysAgeDeathModifier::GetSpawnModifier() const
{
    return m_pkSpawnModifier;
}
//---------------------------------------------------------------------------
inline void NiPSysAgeDeathModifier::SetSpawnModifier(
    NiPSysSpawnModifier* pkSpawnModifier)
{
    m_pkSpawnModifier = pkSpawnModifier;
}
//---------------------------------------------------------------------------
