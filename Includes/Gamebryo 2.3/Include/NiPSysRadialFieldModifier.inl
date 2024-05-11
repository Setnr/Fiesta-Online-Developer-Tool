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
inline float NiPSysRadialFieldModifier::GetRadialType() const
{
    return m_fRadialType;
}
//---------------------------------------------------------------------------
inline void NiPSysRadialFieldModifier::SetRadialType(const float fRadialType)
{
    m_fRadialType = fRadialType;

    // insure the range
    NIASSERT((m_fRadialType >= 0.0f) && (m_fRadialType <= 1.0f));

    m_fRadialType = NiMax(0.0f, m_fRadialType);
    m_fRadialType = NiMin(1.0f, m_fRadialType);
}
//---------------------------------------------------------------------------
