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
// NiD3DShaderConstantMap inline functions
//---------------------------------------------------------------------------
inline NiD3DShaderConstantMap::NiD3DShaderConstantMap(
    NiD3DRenderer* pkRenderer, NiGPUProgram::ProgramType eType) : 
    NiShaderConstantMap(eType), m_pkD3DDevice(NULL), m_pkD3DRenderer(NULL),
    m_pkD3DRenderState(NULL), m_bModified(true), m_eLastError(NISHADERERR_OK),
    m_pkLastShaderProgram(NULL)
{
    SetD3DRenderer(pkRenderer);
    m_aspEntries.RemoveAll();
}
//---------------------------------------------------------------------------
inline bool NiD3DShaderConstantMap::GetModified()
{
    return m_bModified;
}
//---------------------------------------------------------------------------
inline void NiD3DShaderConstantMap::SetModified(bool bModified)
{
    m_bModified = bModified;
}
//---------------------------------------------------------------------------
inline NiD3DError NiD3DShaderConstantMap::GetLastError()
{
    return m_eLastError;
}
//---------------------------------------------------------------------------
inline unsigned int NiD3DShaderConstantMap::GetEntryCount()
{
    return m_aspEntries.GetEffectiveSize();
}
//---------------------------------------------------------------------------
