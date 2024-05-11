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
inline const NiScreenTexture::ScreenRect& NiScreenTexture::GetScreenRect(
    unsigned int uiIndex) const
{
    NIASSERT(uiIndex < m_kScreenRects.GetSize());

    return m_kScreenRects.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline NiScreenTexture::ScreenRect& NiScreenTexture::GetScreenRect(
    unsigned int uiIndex)
{
    NIASSERT(uiIndex < m_kScreenRects.GetSize());

    return m_kScreenRects.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline unsigned int NiScreenTexture::GetNumScreenRects() const
{
    return m_kScreenRects.GetSize();
}
//---------------------------------------------------------------------------
inline NiTexture* NiScreenTexture::GetTexture() const
{
    return m_spTexProp->GetBaseTexture();
}
//---------------------------------------------------------------------------
inline void NiScreenTexture::SetTexture(NiTexture* pkTexture)
{
    m_spTexProp->SetBaseTexture(pkTexture);
}
//---------------------------------------------------------------------------
inline NiTexturingProperty::ClampMode NiScreenTexture::GetClampMode() const
{
    return m_spTexProp->GetBaseClampMode();
}
//---------------------------------------------------------------------------
inline void NiScreenTexture::SetClampMode(
    NiTexturingProperty::ClampMode eClampMode)
{
    m_spTexProp->SetBaseClampMode(eClampMode);
}
//---------------------------------------------------------------------------
inline NiTexturingProperty::ApplyMode NiScreenTexture::GetApplyMode() const
{
    return m_spTexProp->GetApplyMode();
}
//---------------------------------------------------------------------------
inline void NiScreenTexture::SetApplyMode(
    NiTexturingProperty::ApplyMode eApplyMode)
{
    m_spTexProp->SetApplyMode(eApplyMode);
}
//---------------------------------------------------------------------------
inline unsigned short NiScreenTexture::GetRevisionID() const
{
    return m_usDirtyFlags;
}
//---------------------------------------------------------------------------
inline void NiScreenTexture::MarkAsChanged(unsigned short usFlags)
{
    m_usDirtyFlags |= usFlags;
}
//---------------------------------------------------------------------------
inline void NiScreenTexture::ClearRevisionID()
{
    m_usDirtyFlags = 0;
}
//---------------------------------------------------------------------------
inline NiTexturingProperty* NiScreenTexture::GetTexturingProperty() const
{
    return m_spTexProp;
}
//---------------------------------------------------------------------------
inline NiGeometryData::RendererData* NiScreenTexture::GetRendererData() const
{
    return m_pkBuffData;
}
//---------------------------------------------------------------------------
inline void NiScreenTexture::SetRendererData(
    NiGeometryData::RendererData* pkRendererData)
{
    m_pkBuffData = pkRendererData;
}
//---------------------------------------------------------------------------
