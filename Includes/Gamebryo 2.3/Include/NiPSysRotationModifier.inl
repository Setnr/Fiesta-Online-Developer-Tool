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
inline float NiPSysRotationModifier::GetInitialRotSpeed() const
{
    return m_fInitialRotSpeed;
}
//---------------------------------------------------------------------------
inline void NiPSysRotationModifier::SetInitialRotSpeed(float fInitialRotSpeed)
{
    m_fInitialRotSpeed = fInitialRotSpeed;
}
//---------------------------------------------------------------------------
inline float NiPSysRotationModifier::GetInitialRotSpeedVar() const
{
    return m_fInitialRotSpeedVar;
}
//---------------------------------------------------------------------------
inline void NiPSysRotationModifier::SetInitialRotSpeedVar(
    float fInitialRotSpeedVar)
{
    m_fInitialRotSpeedVar = fInitialRotSpeedVar;
}
//---------------------------------------------------------------------------
inline bool NiPSysRotationModifier::GetRandomRotSpeedSign() const
{
    return m_bRandomRotSpeedSign;
}
//---------------------------------------------------------------------------
inline void NiPSysRotationModifier::SetRandomRotSpeedSign(
    bool bRandomRotSpeedSign)
{
    m_bRandomRotSpeedSign = bRandomRotSpeedSign;
}
//---------------------------------------------------------------------------
inline float NiPSysRotationModifier::GetInitialRotAngle() const
{
    return m_fInitialRotAngle;
}
//---------------------------------------------------------------------------
inline void NiPSysRotationModifier::SetInitialRotAngle(float fInitialRotAngle)
{
    m_fInitialRotAngle = fInitialRotAngle;
}
//---------------------------------------------------------------------------
inline float NiPSysRotationModifier::GetInitialRotAngleVar() const
{
    return m_fInitialRotAngleVar;
}
//---------------------------------------------------------------------------
inline void NiPSysRotationModifier::SetInitialRotAngleVar(
    float fInitialRotAngleVar)
{
    m_fInitialRotAngleVar = fInitialRotAngleVar;
}
//---------------------------------------------------------------------------
inline bool NiPSysRotationModifier::GetRandomInitialAxis() const
{
    return m_bRandomInitialAxis;
}
//---------------------------------------------------------------------------
inline void NiPSysRotationModifier::SetRandomInitialAxis(
    bool bRandomInitialAxis)
{
    m_bRandomInitialAxis = bRandomInitialAxis;
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiPSysRotationModifier::GetInitialAxis() const
{
    return m_kInitialAxis;
}
//---------------------------------------------------------------------------
inline void NiPSysRotationModifier::SetInitialAxis(
    const NiPoint3& kInitialAxis)
{
    m_kInitialAxis = kInitialAxis;
}
//---------------------------------------------------------------------------
