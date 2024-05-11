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
inline NiAVObject* NiPSysFieldModifier::GetFieldObj() const
{
    return m_pkFieldObj;
}
//---------------------------------------------------------------------------
inline void NiPSysFieldModifier::SetFieldObj(NiAVObject* pkFieldObj)
{
    m_pkFieldObj = pkFieldObj;
}
//---------------------------------------------------------------------------
inline float NiPSysFieldModifier::GetMagnitude() const
{
    return m_fMagnitude;
}
//---------------------------------------------------------------------------
inline void NiPSysFieldModifier::SetMagnitude(float fMagnitude)
{
    m_fMagnitude = fMagnitude;
}
//---------------------------------------------------------------------------
inline float NiPSysFieldModifier::GetAttenuation() const
{
    return m_fAttenuation;
}
//---------------------------------------------------------------------------
inline void NiPSysFieldModifier::SetAttenuation(float fAttenuation)
{
    m_fAttenuation = fAttenuation;
}
//---------------------------------------------------------------------------
inline bool NiPSysFieldModifier::GetUseMaxDistance() const
{
    return m_bUseMaxDistance;
}
//---------------------------------------------------------------------------
inline void NiPSysFieldModifier::SetUseMaxDistance(bool bUseMaxDistance)
{
    m_bUseMaxDistance = bUseMaxDistance;
}
//---------------------------------------------------------------------------
inline float NiPSysFieldModifier::GetMaxDistance() const
{
    return m_fMaxDistance;
}
//---------------------------------------------------------------------------
inline void NiPSysFieldModifier::SetMaxDistance(float fMaxDistance)
{
    m_fMaxDistance = fMaxDistance;
    m_fMaxDistanceSqr = fMaxDistance * fMaxDistance;
}
//---------------------------------------------------------------------------
