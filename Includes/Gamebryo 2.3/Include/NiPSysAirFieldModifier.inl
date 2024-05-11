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
inline const NiPoint3& NiPSysAirFieldModifier::GetLastFieldPosition() const
{
    return m_kLastFieldPosition;
}
//---------------------------------------------------------------------------
inline void NiPSysAirFieldModifier::SetLastFieldPosition(
    const NiPoint3& kLastPosition)
{
    m_kLastFieldPosition = kLastPosition;
}
//---------------------------------------------------------------------------
inline void NiPSysAirFieldModifier::ClearLastFieldPosition()
{
    m_kLastFieldPosition = m_ckUninitializePosition;
    m_fLastUpdateTime = FLT_MAX;
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiPSysAirFieldModifier::GetDirection() const
{
    return m_kDirection;
}
//---------------------------------------------------------------------------
inline void NiPSysAirFieldModifier::SetDirection(const NiPoint3& kDirection)
{
    m_kDirection = kDirection;

    // Create the Unitized Direction
    m_kUnitDirection = kDirection;
    m_kUnitDirection.Unitize();

    // Check for a valid direction
    NIASSERT(m_kUnitDirection != NiPoint3::ZERO);
    if (m_kUnitDirection == NiPoint3::ZERO)
    {
        m_kUnitDirection = NiPoint3::UNIT_X;
    }
}
//---------------------------------------------------------------------------
inline float NiPSysAirFieldModifier::GetAirFriction() const
{
    return m_fAirFriction;
}
//---------------------------------------------------------------------------
inline void NiPSysAirFieldModifier::SetAirFriction(float fAirFriction)
{
    NIASSERT((fAirFriction >= 0.0f) && (fAirFriction <= 1.0f));

    // Verify we are in the range 0.0 - 1.0
    fAirFriction = NiMax(0.0f, fAirFriction);
    fAirFriction = NiMin(1.0f, fAirFriction);

    m_fAirFriction = fAirFriction;
}
//---------------------------------------------------------------------------
inline float NiPSysAirFieldModifier::GetInheritVelocity() const
{
    return m_fInheritVelocity;
}
//---------------------------------------------------------------------------
inline void NiPSysAirFieldModifier::SetInheritVelocity(float fInherit)
{
    NIASSERT((fInherit >= 0.0f) && (fInherit <= 1.0f));

    // Verify we are in the range 0.0 - 1.0
    fInherit = NiMax(0.0f, fInherit);
    fInherit = NiMin(1.0f, fInherit);

    m_fInheritVelocity = fInherit;
}
//---------------------------------------------------------------------------
inline bool NiPSysAirFieldModifier::GetInheritRotation() const
{
    return m_bInheritRotation;
}
//---------------------------------------------------------------------------
inline void NiPSysAirFieldModifier::SetInheritRotation(bool bInherit)
{
    m_bInheritRotation = bInherit;
}
//---------------------------------------------------------------------------
inline bool NiPSysAirFieldModifier::GetComponentOnly() const
{
    return m_bComponentOnly;
}
//---------------------------------------------------------------------------
inline void NiPSysAirFieldModifier::SetComponentOnly(bool bComponent)
{
    m_bComponentOnly = bComponent;
}
//---------------------------------------------------------------------------
inline bool NiPSysAirFieldModifier::GetEnableSpread() const
{
    return m_bEnableSpread;
}
//---------------------------------------------------------------------------
inline void NiPSysAirFieldModifier::SetEnableSpread(bool bEnable)
{
    m_bEnableSpread = bEnable;
}
//---------------------------------------------------------------------------
inline float NiPSysAirFieldModifier::GetSpread() const
{
    return m_fSpread;
}
//---------------------------------------------------------------------------
inline void NiPSysAirFieldModifier::SetSpread(float fSpread)
{
    NIASSERT((fSpread >= 0.0f) && (fSpread <= 1.0f));

    // Verify we are in the range 0.0 - 1.0
    fSpread = NiMax(0.0f, fSpread);
    fSpread = NiMin(1.0f, fSpread);

    m_fSpread = fSpread;
}
//---------------------------------------------------------------------------
