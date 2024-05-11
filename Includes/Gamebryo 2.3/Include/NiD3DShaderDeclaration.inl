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
//  NiD3DShaderDeclaration Inline Functions
//---------------------------------------------------------------------------
inline NiD3DShaderDeclaration::NiPackerEntry::NiPackerEntry()
{
    m_uiShaderParameterType = NiD3DShaderDeclaration::SPTYPE_NONE;
    m_kPackFunctions.RemoveAll();
}
//---------------------------------------------------------------------------
inline NiD3DShaderDeclaration::NiPackerEntry::NiPackerEntry(
    const NiPackerEntry& Src)
{
    m_uiShaderParameterType = Src.m_uiShaderParameterType;
    //  Clear the function array...
    m_kPackFunctions.RemoveAll();
}
//---------------------------------------------------------------------------
inline NiD3DShaderDeclaration::NiPackerEntry::~NiPackerEntry()
{
    m_kPackFunctions.RemoveAll();
}
//---------------------------------------------------------------------------
inline bool NiD3DShaderDeclaration::NiPackerEntry::operator==(
    const NiPackerEntry& Src)
{
    if (m_uiShaderParameterType != Src.m_uiShaderParameterType)
        return false;

    //  Check the array entries...

    return true;
}
//---------------------------------------------------------------------------
inline bool NiD3DShaderDeclaration::NiPackerEntry::operator!=(
    const NiPackerEntry& Src)
{
    //  Check the array entries...

    if (m_uiShaderParameterType == Src.m_uiShaderParameterType)
        return false;

    return true;
}
//---------------------------------------------------------------------------
inline NiD3DShaderDeclaration::NiPackerEntry& 
NiD3DShaderDeclaration::NiPackerEntry::operator=(const NiPackerEntry& Src)
{
    m_uiShaderParameterType = Src.m_uiShaderParameterType;
    //  Clear the function array...
    m_kPackFunctions.RemoveAll();
    //  Copy the function arrays...

    return *(this);
}
//---------------------------------------------------------------------------
inline unsigned int NiD3DShaderDeclaration::GetTypeSize(
    ShaderParameterType eType)
{
    NIASSERT(eType < SPTYPE_COUNT);

    return ms_auiTypeSizes[eType];
}
//---------------------------------------------------------------------------
inline void NiD3DShaderDeclaration::SetSoftwareVertexProcessing(bool)
{
    /* */
}
//---------------------------------------------------------------------------
inline bool NiD3DShaderDeclaration::GetSoftwareVertexProcessing() const
{
    return false;
}
//---------------------------------------------------------------------------
inline bool NiD3DShaderDeclaration::ClearAllStreams()
{
    NIASSERT(m_pkStreamEntries);
    
    bool bResult = true;

    for (unsigned int ui = 0; ui < m_uiStreamCount; ui++)
    {
        if (ClearAllEntries(ui) != true)
            bResult = false;
    }

    m_bModified = true;

    return bResult;
}
//---------------------------------------------------------------------------
