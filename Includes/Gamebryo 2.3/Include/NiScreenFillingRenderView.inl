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
inline void NiScreenFillingRenderView::AttachProperty(NiProperty* pkProperty)
{
    NIASSERT(pkProperty);
    NIASSERT(m_spScreenFillingQuad);
    m_spScreenFillingQuad->AttachProperty(pkProperty);
    m_bPropertiesChanged = true;
}
//---------------------------------------------------------------------------
inline void NiScreenFillingRenderView::DetachProperty(NiProperty* pkProperty)
{
    NIASSERT(pkProperty);
    NIASSERT(m_spScreenFillingQuad);
    m_spScreenFillingQuad->DetachProperty(pkProperty);
    m_bPropertiesChanged = true;
}
//---------------------------------------------------------------------------
inline void NiScreenFillingRenderView::DetachAllProperties()
{
    NIASSERT(m_spScreenFillingQuad);
    m_spScreenFillingQuad->DetachAllProperties();
    m_bPropertiesChanged = true;
}
//---------------------------------------------------------------------------
inline NiProperty* NiScreenFillingRenderView::GetProperty(int iType) const
{
    NIASSERT(m_spScreenFillingQuad);
    return m_spScreenFillingQuad->GetProperty(iType);
}
//---------------------------------------------------------------------------
inline NiPropertyPtr NiScreenFillingRenderView::RemoveProperty(int iType)
{
    NIASSERT(m_spScreenFillingQuad);
    m_bPropertiesChanged = true;
    return m_spScreenFillingQuad->RemoveProperty(iType);
}
//---------------------------------------------------------------------------
inline void NiScreenFillingRenderView::AttachEffect(NiDynamicEffect* pkEffect)
{
    NIASSERT(pkEffect);

    // Ensure that the effect is not already in list.
    if (m_kEffectList.FindPos(pkEffect))
    {
        return;
    }

    m_kEffectList.AddHead(pkEffect);
    m_bEffectsChanged = true;
}
//---------------------------------------------------------------------------
inline void NiScreenFillingRenderView::DetachEffect(NiDynamicEffect* pkEffect)
{
    NIASSERT(pkEffect);

    // Ensure that the effect is already in the list.
    if (!m_kEffectList.FindPos(pkEffect))
    {
        return;
    }

    m_kEffectList.Remove(pkEffect);
    m_bEffectsChanged = true;
}
//---------------------------------------------------------------------------
inline void NiScreenFillingRenderView::DetachAllEffects()
{
    m_kEffectList.RemoveAll();
    m_bEffectsChanged = true;
}
//---------------------------------------------------------------------------
inline NiGeometry& NiScreenFillingRenderView::GetScreenFillingQuad() const
{
    NIASSERT(m_spScreenFillingQuad);
    return *m_spScreenFillingQuad;
}
//---------------------------------------------------------------------------
