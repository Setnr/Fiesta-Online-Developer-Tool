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
inline const float NiPSysTurbulenceFieldModifier::GetFrequency() const
{
    return m_fFrequency;
}
//---------------------------------------------------------------------------
inline void NiPSysTurbulenceFieldModifier::SetFrequency(float fFrequency)
{
    m_fFrequency = fFrequency;

    // Check for near zero frequency
    if (m_fFrequency < 0.0001f)
    {
        m_fFrequencyUpdateTime = 1.0f / 0.0001f;
    }
    else
    {
        m_fFrequencyUpdateTime = 1.0f / m_fFrequency;
    }
}
//---------------------------------------------------------------------------
