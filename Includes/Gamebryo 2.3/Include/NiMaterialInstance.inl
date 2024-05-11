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
inline NiMaterialInstance::NiMaterialInstance() :
    m_uiMaterialExtraData(NiMaterialInstance::DEFAULT_EXTRA_DATA),
    m_eNeedsUpdate(DIRTY)
{
    /* */
}
//---------------------------------------------------------------------------
inline NiMaterialInstance::NiMaterialInstance(
    const NiMaterialInstance& kMatInst) :
    m_spCachedShader(kMatInst.m_spCachedShader),
    m_spMaterial(kMatInst.m_spMaterial),
    m_uiMaterialExtraData(kMatInst.m_uiMaterialExtraData),
    m_eNeedsUpdate(DIRTY)
{
    /* */
}
//---------------------------------------------------------------------------
inline NiMaterialInstance::NiMaterialInstance(NiMaterial* pkMaterial) : 
    m_spMaterial(pkMaterial),
        m_uiMaterialExtraData(NiMaterialInstance::DEFAULT_EXTRA_DATA),
        m_eNeedsUpdate(DIRTY)
{
    /* */
}
//---------------------------------------------------------------------------
inline NiMaterialInstance::NiMaterialInstance(NiMaterial* pkMaterial, 
    unsigned int uiMaterialExtraData) :  m_spMaterial(pkMaterial),
    m_uiMaterialExtraData(uiMaterialExtraData),
    m_eNeedsUpdate(DIRTY)
{
    /* */
}
//---------------------------------------------------------------------------
inline NiMaterialInstance::~NiMaterialInstance()
{
    /* */
}
//---------------------------------------------------------------------------
inline void NiMaterialInstance::ClearCachedShader()
{
    m_spCachedShader = NULL;
}
//---------------------------------------------------------------------------
inline NiMaterial* NiMaterialInstance::GetMaterial() const
{
    return m_spMaterial;
}
//---------------------------------------------------------------------------
inline void NiMaterialInstance::Reinitialize()
{
    m_spCachedShader = NULL;
    m_spMaterial = NULL;
    m_eNeedsUpdate = DIRTY;
}
//---------------------------------------------------------------------------
inline void NiMaterialInstance::SetMaterialNeedsUpdate(bool bNeedsUpdate)
{
    m_eNeedsUpdate = (bNeedsUpdate ? DIRTY : CLEAN);
}
//---------------------------------------------------------------------------
