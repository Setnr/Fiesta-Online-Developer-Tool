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
inline D3D10_DRIVER_TYPE NiD3D10Renderer::GetD3D10DriverType(DriverType eType)
    const
{
    return (D3D10_DRIVER_TYPE)eType;
}
//---------------------------------------------------------------------------
inline unsigned int NiD3D10Renderer::GetD3D10CreateDeviceFlags(
    unsigned int uiCreateDeviceFlags) const
{
    return uiCreateDeviceFlags;
}
//---------------------------------------------------------------------------
inline unsigned int NiD3D10Renderer::GetD3D10WindowAssociationFlags(
    unsigned int uiWindowAssociationFlags) const
{
    return uiWindowAssociationFlags;
}
//---------------------------------------------------------------------------
inline void NiD3D10Renderer::SetRenderShadowCasterBackfaces(
    bool bRenderBackfaces)
{
    NiD3D10ShadowWriteShader::SetRenderBackfaces(bRenderBackfaces);
}
//---------------------------------------------------------------------------
inline const NiD3D10ShaderLibraryVersion* 
    NiD3D10Renderer::GetShaderLibraryVersion() const
{
    return &m_kShaderLibraryVersion;
}
//---------------------------------------------------------------------------
