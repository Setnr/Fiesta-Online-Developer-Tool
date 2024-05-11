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
inline const NiPoint3& NiPSysGravityFieldModifier::GetDirection() const
{
    return m_kDirection;
}
//---------------------------------------------------------------------------
inline void NiPSysGravityFieldModifier::SetDirection(
    const NiPoint3& kDirection)
{
    m_kDirection = kDirection;

    // Build the Unitized Direction
    m_kUnitDirection = kDirection;
    m_kUnitDirection.Unitize();

    // Check for Near Zero
    NIASSERT(m_kUnitDirection != NiPoint3::ZERO);
    if (m_kUnitDirection == NiPoint3::ZERO)
        m_kUnitDirection == -NiPoint3::UNIT_Y;
}
//---------------------------------------------------------------------------
