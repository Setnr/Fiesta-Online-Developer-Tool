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
inline const NiPoint3& NiPSysVortexFieldModifier::GetDirection() const
{
    return m_kDirection;
}
//---------------------------------------------------------------------------
inline void NiPSysVortexFieldModifier::SetDirection(const NiPoint3& kDirection)
{
    m_kDirection = kDirection;
}
//---------------------------------------------------------------------------
inline NiPoint3 NiPSysVortexFieldModifier::VortexDirection(
    NiPoint3& kVortexPos, NiPoint3& kDirection, NiPoint3& kParticle)
{
    NiPoint3 kDiff = kParticle - kVortexPos;
    float fSegParam = kDirection.Dot(kDiff);

    kDiff -= fSegParam * kDirection;

    // Check for Near Zero
    if (kDiff == NiPoint3::ZERO)
        return kDiff;

    return kDirection.UnitCross(kDiff);
}
//---------------------------------------------------------------------------
