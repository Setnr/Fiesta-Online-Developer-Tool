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
inline NiScreenTextureRenderClick::NiScreenTextureRenderClick() :
    m_uiNumObjectsDrawn(0), m_fRenderTime(0.0f)
{
}
//---------------------------------------------------------------------------
inline void NiScreenTextureRenderClick::AppendScreenTexture(
    NiScreenTexture* pkScreenTexture)
{
    NIASSERT(pkScreenTexture);
    m_kScreenTextures.AddTail(pkScreenTexture);
}
//---------------------------------------------------------------------------
inline void NiScreenTextureRenderClick::PrependScreenTexture(
    NiScreenTexture* pkScreenTexture)
{
    NIASSERT(pkScreenTexture);
    m_kScreenTextures.AddHead(pkScreenTexture);
}
//---------------------------------------------------------------------------
inline void NiScreenTextureRenderClick::RemoveScreenTexture(
    NiScreenTexture* pkScreenTexture)
{
    NIASSERT(pkScreenTexture);
    m_kScreenTextures.Remove(pkScreenTexture);
}
//---------------------------------------------------------------------------
inline void NiScreenTextureRenderClick::RemoveAllScreenTextures()
{
    m_kScreenTextures.RemoveAll();
}
//---------------------------------------------------------------------------
inline NiTPointerList<NiScreenTexturePtr>&
    NiScreenTextureRenderClick::GetScreenTextures()
{
    return m_kScreenTextures;
}
//---------------------------------------------------------------------------
inline const NiTPointerList<NiScreenTexturePtr>&
    NiScreenTextureRenderClick::GetScreenTextures() const
{
    return m_kScreenTextures;
}
//---------------------------------------------------------------------------
