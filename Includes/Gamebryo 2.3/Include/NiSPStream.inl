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
inline void* NiSPStream::GetData()
{
    return m_pvData;
}
//---------------------------------------------------------------------------
inline void NiSPStream::SetData(void* pvData)
{
    m_pvData = pvData;
    NotifyDependentTasks();
}
//---------------------------------------------------------------------------
inline void* NiSPStream::GetAlignedData()
{
    return m_pvAlignedData;
}
//---------------------------------------------------------------------------
inline void NiSPStream::SetAlignedData(void* pvAlignedData)
{
    m_pvAlignedData = pvAlignedData;
    NotifyDependentTasks();
}
//---------------------------------------------------------------------------
inline bool NiSPStream::IsDataAligned()
{
    return m_pvData == m_pvAlignedData;
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPStream::GetDataSize()
{
    return m_uiDataSize;
}
//---------------------------------------------------------------------------
inline bool NiSPStream::IsPartitioningEnabled()
{
    return m_bPartitioningEnabled;
}
//---------------------------------------------------------------------------
inline unsigned short NiSPStream::GetStride()
{
    return m_uiStride;
}
//---------------------------------------------------------------------------
inline void NiSPStream::SetStride(unsigned short uiStride)
{
    m_uiStride = uiStride;
    NotifyDependentTasks();
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPStream::GetBlockCount()
{
    return m_uiBlockCount;
}
//---------------------------------------------------------------------------
inline void NiSPStream::SetBlockCount(NiUInt32 uiBlockCount)
{
    m_uiBlockCount = uiBlockCount;
    NotifyDependentTasks();
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPStream::GetSegmentBlockCount()
{
    return m_uiSegmentBlockCount;
}
//---------------------------------------------------------------------------
inline void NiSPStream::AddInput(NiSPTask* pkTask)
{
    m_kInputs.Add(pkTask);
}
//---------------------------------------------------------------------------
inline NiSPTask* NiSPStream::GetInputAt(NiUInt32 uiIndex) const
{
    return m_kInputs.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPStream::GetInputSize() const
{
    return m_kInputs.GetSize();
}
//---------------------------------------------------------------------------
inline void NiSPStream::AddOutput(NiSPTask* pkTask)
{
    m_kOutputs.Add(pkTask);
}
//---------------------------------------------------------------------------
inline NiSPTask*  NiSPStream::GetOutputAt(NiUInt32 uiIndex) const
{
    return m_kOutputs.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPStream::GetOutputSize() const
{
    return m_kOutputs.GetSize();
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPStream::GetPadding(NiUInt32 uiBlockIndex)
{
    void* pData = GetBlockStartPointer(uiBlockIndex);
    void* pDataA = GetBlockStartPointerAligned(uiBlockIndex);
    
    return reinterpret_cast<unsigned char*>(pData) - 
           reinterpret_cast<unsigned char*>(pDataA);
}

