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
inline unsigned short NiPhysXMaterialDesc::GetIndex() const
{
    return m_usIndex;
}
//---------------------------------------------------------------------------
inline void NiPhysXMaterialDesc::SetIndex(const unsigned short usIndex)
{
    m_usIndex = usIndex;
}
//---------------------------------------------------------------------------
inline void NiPhysXMaterialDesc::ToMaterialDesc(NxMaterialDesc& kMaterialDesc,
    const unsigned int uiIndex) const
{
    kMaterialDesc = m_pkMaterialDescs[uiIndex];
}
//---------------------------------------------------------------------------
inline void NiPhysXMaterialDesc::ToMaterial(NxMaterial* pkMaterial,
    const unsigned int uiIndex) const
{
    NxMaterialDesc kMatDesc = m_pkMaterialDescs[uiIndex];
    pkMaterial->loadFromDesc(kMatDesc);
}
//---------------------------------------------------------------------------
inline void NiPhysXMaterialDesc::FromMaterial(NxMaterial* pkMaterial,
    const unsigned int uiIndex)
{
    NxMaterialDesc kMatDesc;
    pkMaterial->saveToDesc(kMatDesc);
    FromMaterialDesc(kMatDesc, uiIndex);
}
//---------------------------------------------------------------------------
inline bool NiPhysXMaterialDesc::StreamCanSkip()
{
    return true;
}
//---------------------------------------------------------------------------
