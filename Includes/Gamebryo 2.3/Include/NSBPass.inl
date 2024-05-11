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
inline const char* NSBPass::GetName() const
{
    return m_pcName;
}
//---------------------------------------------------------------------------
inline const char* NSBPass::GetVertexShaderProgramEntryPoint() const
{
    return m_pcVSProgramEntryPoint;
}
//---------------------------------------------------------------------------
inline const char* NSBPass::GetVertexShaderProgramShaderTarget() const
{
    return m_pcVSProgramTarget;
}
//---------------------------------------------------------------------------
inline bool NSBPass::GetVertexShaderPresent() const
{
    return (m_pcVertexShaderProgramFilePS3 != NULL ||
        m_pcVertexShaderProgramFileXenon != NULL ||
        m_pcVertexShaderProgramFileDX9 != NULL ||
        m_pcVertexShaderProgramFileD3D10 != NULL);
}
//---------------------------------------------------------------------------
inline const char* NSBPass::GetGeometryShaderProgramEntryPoint() const
{
    return m_pcGSProgramEntryPoint;
}
//---------------------------------------------------------------------------
inline const char* NSBPass::GetGeometryShaderProgramShaderTarget() const
{
    return m_pcGSProgramTarget;
}
//---------------------------------------------------------------------------
inline bool NSBPass::GetGeometryShaderPresent() const
{
    return (m_pcGeometryShaderProgramFilePS3 != NULL ||
        m_pcGeometryShaderProgramFileXenon != NULL ||
        m_pcGeometryShaderProgramFileDX9 != NULL ||
        m_pcGeometryShaderProgramFileD3D10 != NULL);
}
//---------------------------------------------------------------------------
inline const char* NSBPass::GetPixelShaderProgramEntryPoint() const
{
    return m_pcPSProgramEntryPoint;
}
//---------------------------------------------------------------------------
inline const char* NSBPass::GetPixelShaderProgramShaderTarget() const
{
    return m_pcPSProgramTarget;
}
//---------------------------------------------------------------------------
inline bool NSBPass::GetPixelShaderPresent() const
{
    return (m_pcPixelShaderProgramFilePS3 != NULL ||
        m_pcPixelShaderProgramFileXenon != NULL ||
        m_pcPixelShaderProgramFileDX9 != NULL ||
        m_pcPixelShaderProgramFileD3D10 != NULL);
}
//---------------------------------------------------------------------------
inline bool NSBPass::GetSoftwareVertexProcessing() const
{
    return m_bSoftwareVP;
}
//---------------------------------------------------------------------------
inline void NSBPass::SetSoftwareVertexProcessing(bool bSoftwareVP)
{
    m_bSoftwareVP = bSoftwareVP;
}
//---------------------------------------------------------------------------
inline NSBUserDefinedDataSet* NSBPass::GetUserDefinedDataSet()
{
    return m_spUserDefinedDataSet;
}
//---------------------------------------------------------------------------
inline void NSBPass::SetUserDefinedDataSet(NSBUserDefinedDataSet* pkUDDSet)
{
    m_spUserDefinedDataSet = pkUDDSet;
}
//---------------------------------------------------------------------------
inline unsigned int NSBPass::GetVertexConstantMapCount()
{
    return m_kVertexShaderConstantMaps.GetSize();
}
//---------------------------------------------------------------------------
inline unsigned int NSBPass::GetGeometryConstantMapCount()
{
    return m_kGeometryShaderConstantMaps.GetSize();
}
//---------------------------------------------------------------------------
inline unsigned int NSBPass::GetPixelConstantMapCount()
{
    return m_kPixelShaderConstantMaps.GetSize();
}
//---------------------------------------------------------------------------
