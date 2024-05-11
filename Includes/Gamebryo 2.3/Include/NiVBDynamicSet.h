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
#ifndef NIVBDYNAMICSET_H
#define NIVBDYNAMICSET_H

#include "NiVBChip.h"

// NiVBDynamicSet - a managed set of vertex buffer blocks for a given FVF
//  
class NiVBDynamicSet : public NiMemObject
{
public:
    enum
    {
        // The default size of a created (shared) vertex buffer - in bytes
        NIVBDYNAMICSET_DEF_BLOCKSIZE       = 2048, // 2k
    };

protected:
    NiVBDynamicSet();
public:
    ~NiVBDynamicSet();

    // *** begin Emergent internal use only ***
    static NiVBDynamicSet* Create(unsigned int uiBlockSize, 
        D3DDevicePtr pkD3DDevice, unsigned int uiFVF, 
        unsigned int uiIndex);

    unsigned int GetBlockSize(void);
    unsigned int GetFVF(void);
    unsigned int GetIndex(void);
    unsigned int GetCurrOffset(void);

    //  Grab a section of this buffer...
    D3DVertexBufferPtr AllocateBufferSpace(unsigned int uiSize, 
        unsigned int& uiOffset, unsigned int& uiFlags, 
        bool bForceDiscard = false, bool bSoftwareVP = false);

    //  Free a section of buffer
    void FreeBufferSpace(D3DVertexBufferPtr pkVB);

    // *** end Emergent internal use only ***

protected:
    void SetBlockSize(unsigned int uiBlockSize);
    void SetD3DDevice(D3DDevicePtr pkD3DDevice);
    void SetFVF(unsigned int uiFVF);
    void SetIndex(unsigned int uiIndex);

    void CreateVertexBuffer(bool bSoftwareVP = false);
    void ReleaseVertexBuffer(void);

    unsigned int m_uiIndex;
    unsigned int m_uiFVF;

    //******************************
    //***  Internal use only - Start
    unsigned int m_uiBlockSize;             // Size of VBs in this set
    D3DDevicePtr m_pkD3DDevice;             // D3DDevice pointer
    D3DVertexBufferPtr m_pkVB;              // D3D Vertex Buffer

    unsigned int m_uiCurrOffset;

    // Static members...
    static unsigned int ms_uiDefBlockSize;
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

#include "NiVBDynamicSet.inl"

#endif  //#ifndef NIVBDYNAMICSET_H