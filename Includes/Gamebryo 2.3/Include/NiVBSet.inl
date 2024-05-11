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
// NiVBSet Inline Functions
//---------------------------------------------------------------------------
inline NiVBSet::NiVBSet()
{
    SetBlockSize(ms_uiDefBlockSize);
    SetFVF(0);
    SetFlags(0);
    SetIndex(0);
    
    m_pkD3DDevice = 0;
    m_uiUsage = 0;
    m_ePool = D3DPOOL_DEFAULT;
    
    m_apkBlocks.RemoveAll();

    m_pkBlockList = 0;
    m_uiFreeSpaceThreshold = ms_uiDefFreeSpaceThreshold;

#if defined(_NIVB_TRACK_STATS_)
    m_uiTotalBlockBytesAllocated = 0;
    m_uiTotalBlockCount = 0;
    m_uiRunningBlockBytesAllocated = 0;
    m_uiRunningBlockCount = 0;
    m_uiRunningChipSize = 0;
    m_uiRunningChipCount = 0;
#endif  //#if defined(_NIVB_TRACK_STATS_)

}
//---------------------------------------------------------------------------
inline NiVBSet* NiVBSet::Create(unsigned int uiBlockSize, 
    D3DDevicePtr pkD3DDevice, unsigned int uiFVF, unsigned int uiFlags, 
    unsigned int uiIndex)
{
    NiVBSet* pkSet = NiNew NiVBSet();
    if (pkSet)
    {
        pkSet->SetBlockSize(uiBlockSize);
        pkSet->SetD3DDevice(pkD3DDevice);
        pkSet->SetFVF(uiFVF);
        pkSet->SetFlags(uiFlags);
        pkSet->SetIndex(uiIndex);

        DetermineUsageAndPool(pkSet);
    }

    return pkSet;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBSet::GetBlockSize(void)
{
    return m_uiBlockSize;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBSet::GetFVF(void)
{
    return m_uiFVF;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBSet::GetFlags(void)
{
    return m_uiFlags;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBSet::GetIndex(void)
{
    return m_uiIndex;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBSet::GetFreeSpaceThreshold(void)
{
    return m_uiFreeSpaceThreshold;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBSet::GetUsage(void)
{
    return m_uiUsage;
}
//---------------------------------------------------------------------------
inline D3DPOOL NiVBSet::GetPool(void)
{
    return m_ePool;
}
//---------------------------------------------------------------------------
inline NiVBBlock* NiVBSet::GetBlock(unsigned int uiBlockIndex)
{
    return m_apkBlocks.GetAt(uiBlockIndex);
}
//---------------------------------------------------------------------------
inline NiVBChip* NiVBSet::GetChip(unsigned int uiBlockIndex, 
    unsigned int uiChipIndex)
{
    NiVBBlock* pkBlock = m_apkBlocks.GetAt(uiBlockIndex);
    if (pkBlock)
        return pkBlock->GetChip(uiChipIndex);

    return 0;
}
//---------------------------------------------------------------------------
inline void NiVBSet::SetBlockSize(unsigned int uiBlockSize)
{
    m_uiBlockSize = uiBlockSize;
}
//---------------------------------------------------------------------------
inline void NiVBSet::SetD3DDevice(D3DDevicePtr pkD3DDevice)
{
    if (pkD3DDevice)
    {
        m_pkD3DDevice = pkD3DDevice;
        D3D_POINTER_REFERENCE(m_pkD3DDevice);
    }
    else
    {
        if (m_pkD3DDevice)
            NiD3DRenderer::ReleaseDevice(m_pkD3DDevice);
        m_pkD3DDevice = pkD3DDevice;
    }
}
//---------------------------------------------------------------------------
inline void NiVBSet::SetFVF(unsigned int uiFVF)
{
    m_uiFVF = uiFVF;
}
//---------------------------------------------------------------------------
inline void NiVBSet::SetFlags(unsigned int uiFlags)
{
    m_uiFlags = uiFlags;
}
//---------------------------------------------------------------------------
inline void NiVBSet::SetIndex(unsigned int uiIndex)
{
    m_uiIndex = uiIndex;
}
//---------------------------------------------------------------------------
inline void NiVBSet::SetFreeSpaceThreshold(unsigned int uiFreeSpaceThreshold)
{
    m_uiFreeSpaceThreshold = uiFreeSpaceThreshold;
}
//---------------------------------------------------------------------------
inline void NiVBSet::AddToBlockDump(NiVBBlock* pkVBBlock)
{
    NIASSERT(pkVBBlock);

    // Remove from any existing lists
    NiVBBlock* pkNext = pkVBBlock->m_pkNextBlock;
    NiVBBlock* pkPrev = pkVBBlock->m_pkPrevBlock;

    if (pkNext)
        pkNext->m_pkPrevBlock = pkPrev;
    if (pkPrev)
        pkPrev->m_pkNextBlock = pkNext;

    if (ms_pkBlockDump)
        ms_pkBlockDump->m_pkPrevBlock = pkVBBlock;
    pkVBBlock->m_pkNextBlock = ms_pkBlockDump;
    pkVBBlock->m_pkPrevBlock = 0;
    ms_pkBlockDump = pkVBBlock;
}
//---------------------------------------------------------------------------
#if defined(_NIVB_TRACK_STATS_)
inline void NiVBSet::GetStats(unsigned int& uiTotalBlockCount, 
    unsigned int& uiTotalBlockBytesAllocated, 
    unsigned int& uiRunningBlockCount, 
    unsigned int& uiRunningBlockBytesAllocated, 
    unsigned int& uiRunningChipCount, unsigned int& uiRunningChipSize)
{
    uiTotalBlockCount = m_uiTotalBlockCount;
    uiTotalBlockBytesAllocated = m_uiTotalBlockBytesAllocated;
    uiRunningBlockCount = m_uiRunningBlockCount;
    uiRunningBlockBytesAllocated = m_uiRunningBlockBytesAllocated;
    uiRunningChipCount = m_uiRunningChipCount;
    uiRunningChipSize = m_uiRunningChipSize;
}
#endif  //#if defined(_NIVB_TRACK_STATS_)
//---------------------------------------------------------------------------
