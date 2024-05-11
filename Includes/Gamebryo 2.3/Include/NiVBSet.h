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
#ifndef NIVBSET_H
#define NIVBSET_H

#include "NiVBBlock.h"

// NiVBSet - a managed set of vertex buffer blocks for a given FVF
//  
class NiVBSet : public NiMemObject
{
public:
    enum
    {
        // The default size of a created (shared) vertex buffer - in bytes
        NIVBSET_DEF_BLOCKSIZE       = 65536, // 64k
        // The default bytes required to have space considered 'free'
        NIVBSET_DEF_FREETHRESHOLD   = 128,  // 4 32-byte vertices
    };

    enum
    {
        NIVBSET_FLAG_SOFTWAREVP     = 0x80000000
    };

protected:
    NiVBSet();
public:
    ~NiVBSet();

    // *** begin Emergent internal use only ***
    static NiVBSet* Create(unsigned int uiBlockSize, 
        D3DDevicePtr pkD3DDevice, unsigned int uiFVF, 
        unsigned int uiFlags, unsigned int uiIndex);

    unsigned int GetBlockSize(void);
    unsigned int GetFVF(void);
    unsigned int GetFlags(void);
    unsigned int GetIndex(void);
    unsigned int GetFreeSpaceThreshold(void);

    unsigned int GetUsage(void);
    D3DPOOL GetPool(void);

    //  Get the given block
    NiVBBlock* GetBlock(unsigned int uiBlockIndex);
    //  Get the given chip from the given block
    NiVBChip* GetChip(unsigned int uiBlockIndex, unsigned int uiChipIndex);
    //  'Allocate' a chip from a block in this set
    NiVBChip* AllocateChip(unsigned int uiSize, bool bForceNewBlock = false);
//    D3DVertexBufferPtr AllocateBufferSpace(unsigned int uiSize, 
//        unsigned int& uiOffset, unsigned int& uiFlags, 
//        unsigned int& uiChipIndex);
    //  Free a chip from a block in this set
    void FreeChip(unsigned int uiBlockIndex, unsigned int uiChipIndex);
    void FreeBlock(NiVBBlock* pkBlock);

    //  Shutdown function - must be called when the renderer shuts down!!!
    static void DeleteBlocks(void);
    // *** end Emergent internal use only ***

protected:
    void SetBlockSize(unsigned int uiBlockSize);
    void SetD3DDevice(D3DDevicePtr pkD3DDevice);
    void SetFVF(unsigned int uiFVF);
    void SetFlags(unsigned int uiFlags);
    void SetIndex(unsigned int uiIndex);
    void SetFreeSpaceThreshold(unsigned int uiFreeSpaceThreshold);

    static void DetermineUsageAndPool(NiVBSet* pkSet);

    NiVBBlock* CreateBlock(unsigned int uiSize);
    void DestroyBlock(unsigned int uiBlockIndex);
    NiVBBlock* FindBlockWithFreeSpace(unsigned int uiFreeSize);
    void InsertBlockIntoFreeSpaceList(NiVBBlock* pkVBBlock);
    void RemoveBlockFromFreeSpaceList(NiVBBlock* pkVBBlock);
    NiVBBlock* GetFreeBlock(void);

    //  Allocates a collection of blocks, to minimize memory usage.
    static void AllocateBlocks(void);
    //  Put a VBBlock into the 'dump'
    static void AddToBlockDump(NiVBBlock* pkVBBlock);

    unsigned int m_uiIndex;
    unsigned int m_uiFVF;
    unsigned int m_uiFlags;

    //******************************
    //***  Internal use only - Start
    unsigned int m_uiBlockSize;             // Size of VBs in this set
    D3DDevicePtr m_pkD3DDevice;             // D3DDevice9 pointer
    unsigned int m_uiUsage;                 // D3D Usage Flags
    D3DPOOL m_ePool;                        // D3D Pool
    NiTPrimitiveArray<NiVBBlock*> m_apkBlocks; // Array of blocks in set

    NiVBBlock* m_pkBlockList;               // Blocks w/ free space left
    unsigned int m_uiFreeSpaceThreshold;    // Size req. to go on list

    // Static members...
    static NiVBBlock* ms_pkBlockDump;
    static unsigned int ms_uiDefBlockAllocSize;
    static unsigned int ms_uiDefBlockSize;
    static unsigned int ms_uiDefFreeSpaceThreshold;
    //***  Internal use only - End
    //******************************

#if defined(_NIVB_TRACK_STATS_)
    // Statistic variables...
    //  BLOCKS
    unsigned int m_uiTotalBlockBytesAllocated;
    unsigned int m_uiTotalBlockCount;
    unsigned int m_uiRunningBlockBytesAllocated;
    unsigned int m_uiRunningBlockCount;
    //  CHIPS    
    unsigned int m_uiRunningChipSize;
    unsigned int m_uiRunningChipCount;

    static unsigned int m_uiFrameBytesAllocated;
public:
    void GetStats(
        unsigned int& uiTotalBlockCount,
        unsigned int& uiTotalBlockBytesAllocated,
        unsigned int& uiRunningBlockCount,
        unsigned int& uiRunningBlockBytesAllocated,
        unsigned int& uiRunningChipCount,
        unsigned int& uiRunningChipSize);
#endif  //#if defined(_NIVB_TRACK_STATS_)
};

#include "NiVBSet.inl"

#endif  //#ifndef NIVBSET_H