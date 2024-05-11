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
// NiOldString2D inline functions
//---------------------------------------------------------------------------
inline void NiOldString2D::SetPosition(unsigned int uiX, unsigned int uiY)
{
    if ((m_uiX != uiX) || (m_uiY != uiY))
    {
        m_uiX = uiX;
        m_uiY = uiY;
        m_bModified = true;
    }
}
//---------------------------------------------------------------------------
inline unsigned int NiOldString2D::GetLimitWidth()
{
    return m_uiLimitWidth;
}
//---------------------------------------------------------------------------
