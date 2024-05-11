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
template <class T>
T* NiSPWorkload::GetInput(NiUInt32 uiInputIndex)
{
    char* pvDataAddress = NULL;
    NiSPWorkload::Header& kHeader = m_kInputs[uiInputIndex];
    if (kHeader.m_pvEffectiveAddress != 0)
    {
        // Add padding offset to address
        pvDataAddress = (char*)kHeader.m_pvEffectiveAddress;
        pvDataAddress += kHeader.m_uiDataOffset;
    }
    return (T*)pvDataAddress;
}
//---------------------------------------------------------------------------
template <class T>
T* NiSPWorkload::GetOutput(NiUInt32 uiOutputIndex)
{
    char* pvDataAddress = NULL;
    NiSPWorkload::Header& kHeader = m_kOutputs[uiOutputIndex];
    if (kHeader.m_pvEffectiveAddress != 0)
    {
        // Add padding offset to address
        pvDataAddress = (char*)kHeader.m_pvEffectiveAddress;
        pvDataAddress += kHeader.m_uiDataOffset;
    }
    return (T*)pvDataAddress;
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPWorkload::GetBlockCount()
{
    return m_uiBlockCount;
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPWorkload::GetInputCount()
{
    return m_uiInputCount;
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPWorkload::GetOutputCount()
{
    return m_uiInputCount;
}
//---------------------------------------------------------------------------
inline NiSPKernel* NiSPWorkload::GetKernel()
{
    return m_pkKernel;
}
//---------------------------------------------------------------------------
inline void NiSPWorkload::SetKernel(NiSPKernel* pkKernel)
{
    m_pkKernel = pkKernel;
}
//---------------------------------------------------------------------------
inline void NiSPWorkload::Clear()
{
    m_uiInputCount = 0;
    m_uiOutputCount = 0;
    m_uiBlockCount = 0;
    m_uiRangeStart = 0;
}
