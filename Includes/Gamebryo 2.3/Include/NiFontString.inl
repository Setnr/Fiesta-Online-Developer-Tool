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
// NiFontString inline functions
//---------------------------------------------------------------------------
inline const char* NiFontString::GetText()
{
    return m_pcString;
}
//---------------------------------------------------------------------------
inline const NiWChar* NiFontString::GetUnicodeText()
{
    return m_pkString;
}
//---------------------------------------------------------------------------
inline const NiColorA& NiFontString::GetColor() const
{
    return m_kColor;
}
//---------------------------------------------------------------------------
inline unsigned char NiFontString::GetDirection() const
{
    return m_ucDirection;
}
//---------------------------------------------------------------------------
inline void NiFontString::SetDirection(unsigned char ucDirection)
{
    if (m_ucDirection == ucDirection)
        return;

    m_ucDirection = ucDirection;

    m_bModified = true;
}
//---------------------------------------------------------------------------
