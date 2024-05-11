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
inline unsigned short NiPSysSpawnModifier::GetNumSpawnGenerations() const
{
    return m_usNumSpawnGenerations;
}
//---------------------------------------------------------------------------
inline void NiPSysSpawnModifier::SetNumSpawnGenerations(
    unsigned short usNumSpawnGenerations)
{
    m_usNumSpawnGenerations = usNumSpawnGenerations;
}
//---------------------------------------------------------------------------
inline float NiPSysSpawnModifier::GetPercentageSpawned() const
{
    return m_fPercentageSpawned;
}
//---------------------------------------------------------------------------
inline void NiPSysSpawnModifier::SetPercentageSpawned(
    float fPercentageSpawned)
{
    m_fPercentageSpawned = fPercentageSpawned;
}
//---------------------------------------------------------------------------
inline unsigned short NiPSysSpawnModifier::GetMinNumToSpawn() const
{
    return m_usMinNumToSpawn;
}
//---------------------------------------------------------------------------
inline void NiPSysSpawnModifier::SetMinNumToSpawn(
    unsigned short usMinNumToSpawn)
{
    m_usMinNumToSpawn = usMinNumToSpawn;
}
//---------------------------------------------------------------------------
inline unsigned short NiPSysSpawnModifier::GetMaxNumToSpawn() const
{
    return m_usMaxNumToSpawn;
}
//---------------------------------------------------------------------------
inline void NiPSysSpawnModifier::SetMaxNumToSpawn(
    unsigned short usMaxNumToSpawn)
{
    m_usMaxNumToSpawn = usMaxNumToSpawn;
}
//---------------------------------------------------------------------------
inline float NiPSysSpawnModifier::GetSpawnSpeedChaos() const
{
    return m_fSpawnSpeedChaos;
}
//---------------------------------------------------------------------------
inline void NiPSysSpawnModifier::SetSpawnSpeedChaos(float fSpawnSpeedChaos)
{
    m_fSpawnSpeedChaos = fSpawnSpeedChaos;
}
//---------------------------------------------------------------------------
inline float NiPSysSpawnModifier::GetSpawnDirChaos() const
{
    return m_fSpawnDirChaos;
}
//---------------------------------------------------------------------------
inline void NiPSysSpawnModifier::SetSpawnDirChaos(float fSpawnDirChaos)
{
    m_fSpawnDirChaos = fSpawnDirChaos;
}
//---------------------------------------------------------------------------
inline float NiPSysSpawnModifier::GetLifeSpan() const
{
    return m_fLifeSpan;
}
//---------------------------------------------------------------------------
inline void NiPSysSpawnModifier::SetLifeSpan(float fLifeSpan)
{
    m_fLifeSpan = fLifeSpan;
}
//---------------------------------------------------------------------------
inline float NiPSysSpawnModifier::GetLifeSpanVar() const
{
    return m_fLifeSpanVar;
}
//---------------------------------------------------------------------------
inline void NiPSysSpawnModifier::SetLifeSpanVar(float fLifeSpanVar)
{
    m_fLifeSpanVar = fLifeSpanVar;
}
//---------------------------------------------------------------------------
