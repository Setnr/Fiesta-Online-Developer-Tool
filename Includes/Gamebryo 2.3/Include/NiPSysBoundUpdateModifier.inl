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
inline short NiPSysBoundUpdateModifier::GetUpdateSkip() const
{
    return m_sUpdateSkip;
}
//---------------------------------------------------------------------------
inline void NiPSysBoundUpdateModifier::SetUpdateSkip(short sUpdateSkip)
{
    m_sUpdateSkip = sUpdateSkip;

    NiDelete[] m_pkSkipBounds;

    // Always have at least 1
    sUpdateSkip = NiMax(sUpdateSkip, 1);

    m_pkSkipBounds = NiNew NiBound[sUpdateSkip];

    // Initialize the bounds
    for(int iLoop = 0; iLoop < sUpdateSkip; iLoop++)
    {
        m_pkSkipBounds[iLoop].SetCenterAndRadius(NiPoint3::ZERO, 0.0f);
    }
}
//---------------------------------------------------------------------------
inline void NiPSysBoundUpdateModifier::ResetUpdateSkipBounds()
{
    short sUpdateSkip = NiMax(m_sUpdateSkip, 1);

    // Initialize the bounds
    for(int iLoop = 0; iLoop < sUpdateSkip; iLoop++)
    {
        m_pkSkipBounds[iLoop].SetCenterAndRadius(NiPoint3::ZERO, 0.0f);
    }
}
//---------------------------------------------------------------------------
