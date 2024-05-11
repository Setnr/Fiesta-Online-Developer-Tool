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
inline NiD3DShaderProgramCreator::NiD3DShaderProgramCreator()
{ /* */ }
//---------------------------------------------------------------------------
inline NiD3DShaderProgramCreator::~NiD3DShaderProgramCreator()
{ /* */ }
//---------------------------------------------------------------------------
inline unsigned int 
    NiD3DShaderProgramCreator::GetUniversalShaderCreationFlags()
{
    return ms_uiUniversalShaderCreationFlags;
}
//---------------------------------------------------------------------------
inline void NiD3DShaderProgramCreator::SetUniversalShaderCreationFlags(
    unsigned int uiFlags)
{
    ms_uiUniversalShaderCreationFlags = uiFlags;
}
//---------------------------------------------------------------------------
inline void NiD3DShaderProgramCreator::_SDMInit()
{ /* */ }
//---------------------------------------------------------------------------
inline void NiD3DShaderProgramCreator::_SDMShutdown()
{
    SetD3DRenderer(NULL);
}
//---------------------------------------------------------------------------
