// NUMERICAL DESIGN LIMITED PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Numerical Design Limited and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2007 Numerical Design Limited.
//      All Rights Reserved.
//
// Numerical Design Limited, Chapel Hill, North Carolina 27514
// http://www.ndl.com
//---------------------------------------------------------------------------
// Ni2DString inline functions
//---------------------------------------------------------------------------
inline void Ni2DString::GetPosition(short& sX, short& sY)
{
    sX = m_sX;
    sY = m_sY;
}
//---------------------------------------------------------------------------
inline void Ni2DString::SetPosition(short sX, short sY)
{
    if ((m_sX != sX) || (m_sY != sY))
    {
        m_sX = sX;
        m_sY = sY;
        m_bModified = true;
    }
}
//---------------------------------------------------------------------------
inline unsigned short Ni2DString::GetPointSize()
{
    // Default to the point size of the font
    if (m_usPointSize == 0)
        return m_spFont->GetHeight();

    return m_usPointSize;
}
//---------------------------------------------------------------------------
inline void Ni2DString::SetPointSize(unsigned short usPointSize)
{
    if (usPointSize == m_usPointSize)
        return;

    // Set the new point size and set the modified flag so the underlining
    // NiScreenElements can be rebuild before the next draw.
    m_usPointSize = usPointSize;
    m_bModified = true;
}
//---------------------------------------------------------------------------
inline const NiRenderTargetGroup* Ni2DString::GetRenderTargetGroup()
{
    return m_pkTarget;
}
//---------------------------------------------------------------------------
inline void Ni2DString::SetRenderTargetGroup(
    NiRenderTargetGroup* pkTarget)
{
    if (pkTarget == m_pkTarget)
        return;

    // Set the new point size and set the modified flag so the underlining
    // NiScreenElements can be rebuild before the next draw.
    m_pkTarget = pkTarget;
    m_bModified = true;
}
//---------------------------------------------------------------------------
inline NiScreenElements* Ni2DString::GetScreenElements(unsigned char ucIndex)
{
    NIASSERT(m_spScreenElements.GetSize() < ucIndex);
    NIASSERT(m_spScreenElements.GetAt(ucIndex) != NULL);

    return m_spScreenElements.GetAt(ucIndex);
}
//---------------------------------------------------------------------------
inline void Ni2DString::CreateScreenPolygon(NiWChar ch, short& sCurrX, 
    short& sCurrY, short sW, short sH, float& fScreenWidth, 
    float& fScreenHeight, NiColorA& kColor, float* pfUVSets)
{
    // Get the texture and there by determine which Screen Elements to add to
    unsigned char ucTexture = m_spFont->GetTextureIndex(ch);

    // Create the new letter / polygon
    int iPolygon = m_spScreenElements.GetAt(ucTexture)->Insert(4);

    m_spScreenElements.GetAt(ucTexture)->SetRectangle(iPolygon, 
        (float)sCurrX / fScreenWidth, (float)sCurrY / fScreenHeight,
        sW / fScreenWidth, sH / fScreenHeight);

    // Set the normals to point at the screen
    m_spScreenElements.GetAt(ucTexture)->SetNormals(iPolygon, 
        -NiPoint3::UNIT_Z, -NiPoint3::UNIT_Z, -NiPoint3::UNIT_Z, 
        -NiPoint3::UNIT_Z);

    // Set the vertex colors
    m_spScreenElements.GetAt(ucTexture)->SetColors(iPolygon, kColor, 
        kColor, kColor, kColor);

    // Get the UV coordinates from the souce texture
    m_spScreenElements.GetAt(ucTexture)->SetTextures(iPolygon, 0, 
        pfUVSets[0], pfUVSets[1], pfUVSets[2], pfUVSets[3]); 
}
//---------------------------------------------------------------------------
