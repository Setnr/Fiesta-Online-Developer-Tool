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
inline const char* NSBD3D10Pass::GetName() const
{
    return m_pcName;
}
//---------------------------------------------------------------------------
inline const char* NSBD3D10Pass::GetVertexShaderProgramEntryPoint() const
{
    return m_pcVSProgramEntryPoint;
}
//---------------------------------------------------------------------------
inline const char* NSBD3D10Pass::GetVertexShaderProgramShaderTarget() const
{
    return m_pcVSProgramTarget;
}
//---------------------------------------------------------------------------
inline bool NSBD3D10Pass::GetVertexShaderPresent() const
{
    return (m_pcVertexShaderProgramFileD3D10 != NULL);
}
//---------------------------------------------------------------------------
inline const char* NSBD3D10Pass::GetGeometryShaderProgramEntryPoint() const
{
    return m_pcGSProgramEntryPoint;
}
//---------------------------------------------------------------------------
inline const char* NSBD3D10Pass::GetGeometryShaderProgramShaderTarget() const
{
    return m_pcGSProgramTarget;
}
//---------------------------------------------------------------------------
inline bool NSBD3D10Pass::GetGeometryShaderPresent() const
{
    return (m_pcGeometryShaderProgramFileD3D10 != NULL);
}
//---------------------------------------------------------------------------
inline const char* NSBD3D10Pass::GetPixelShaderProgramEntryPoint() const
{
    return m_pcPSProgramEntryPoint;
}
//---------------------------------------------------------------------------
inline const char* NSBD3D10Pass::GetPixelShaderProgramShaderTarget() const
{
    return m_pcPSProgramTarget;
}
//---------------------------------------------------------------------------
inline bool NSBD3D10Pass::GetPixelShaderPresent() const
{
    return (m_pcPixelShaderProgramFileD3D10 != NULL);
}
//---------------------------------------------------------------------------
inline bool NSBD3D10Pass::GetSoftwareVertexProcessing() const
{
    return m_bSoftwareVP;
}
//---------------------------------------------------------------------------
inline void NSBD3D10Pass::SetSoftwareVertexProcessing(bool bSoftwareVP)
{
    m_bSoftwareVP = bSoftwareVP;
}
//---------------------------------------------------------------------------
inline NSBD3D10UserDefinedDataSet* NSBD3D10Pass::GetUserDefinedDataSet()
{
    return m_spUserDefinedDataSet;
}
//---------------------------------------------------------------------------
inline void NSBD3D10Pass::SetUserDefinedDataSet(
    NSBD3D10UserDefinedDataSet* pkUDDSet)
{
    m_spUserDefinedDataSet = pkUDDSet;
}
//---------------------------------------------------------------------------
inline unsigned int NSBD3D10Pass::GetVertexConstantMapCount()
{
    return m_kVertexShaderConstantMaps.GetSize();
}
//---------------------------------------------------------------------------
inline unsigned int NSBD3D10Pass::GetGeometryConstantMapCount()
{
    return m_kGeometryShaderConstantMaps.GetSize();
}
//---------------------------------------------------------------------------
inline unsigned int NSBD3D10Pass::GetPixelConstantMapCount()
{
    return m_kPixelShaderConstantMaps.GetSize();
}
//---------------------------------------------------------------------------
