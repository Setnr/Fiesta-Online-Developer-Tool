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
#ifndef NIVBBLOCK_H
#define NIVBBLOCK_H

#include "NiVBChip.h"
#include <NiTArray.h>

class NiVBSet;

// NiVBBlock - a complete vertex buffer representation.
//   May be broken up into chips...
class NiVBBlock : public NiMemObject
{
private:
    NiVBBlock();
public:
    ~NiVBBlock();

    // *** begin Emergent internal use only ***
    static NiVBBlock* Create(NiVBSet* pkSet, D3DVertexBufferPtr pkVB,
        unsigned int uiSize, unsigned int uiFVF, unsigned int uiFlags, 
        D3DPOOL ePool, unsigned int uiUsage, unsigned int uiIndex);

    NiVBSet* GetSet(void);
    D3DVertexBufferPtr GetVB(void);
    unsigned int GetSize(void);
    unsigned int GetFVF(void);
    unsigned int GetFlags(void);
    unsigned int GetIndex(void);

    unsigned int GetCurrOffset(void);
    unsigned int GetFreeBytes(void);
    unsigned int GetTotalFreeBytes(void);

    //  Get the given chip
    NiVBChip* GetChip(unsigned int uiChipIndex);
    unsigned int GetChipSize(unsigned int uiChipIndex);
    //  'Memory' management of chips
    NiVBChip* AllocateChip(unsigned int uiSize);
    unsigned int FreeChip(unsigned int uiChipIndex);
    // *** end Emergent internal use only ***

protected:
    void SetSet(NiVBSet* pkSet);
    void SetVB(D3DVertexBufferPtr pkVB);
    void SetSize(unsigned int uiSize);
    void SetFVF(unsigned int uiFVF);
    void SetFlags(unsigned int uiFlags);
    void SetIndex(unsigned int uiIndex);
    void SetPool(D3DPOOL ePool);
    void SetUsage(unsigned int uiUsage);

    void ClearAllData(void);

    NiVBChip* GetFreeChip(void);
    //  Allocates a collection of chips.
    static void AllocateChips(void);
    //  Shutdown function
    static void DeleteChips(void);

    NiVBSet* m_pkSet;               // Parent set
    unsigned int m_uiIndex;         // Internally referenced ID
    D3DVertexBufferPtr m_pkVB;      // Pointer to the VB
    unsigned int m_uiSize;          // Total size of the VB (bytes)
    unsigned int m_uiFVF;           // FVF flags (format of the VB)
    unsigned int m_uiFlags;         // NI flags (static, etc.)

    //******************************
    //***  Internal use only - Start
    unsigned int m_uiUsage;         // D3D Usage Flags
    D3DPOOL m_ePool;                // D3D Pool
    unsigned int m_uiCurrOffset;    // Current offset (next available)
    unsigned int m_uiFreeBytes;     // Free bytes remaining (continuous)
    unsigned int m_uiTotalFreeBytes;// Free bytes remaining (total)
    NiTPrimitiveArray<NiVBChip*> m_apkChips; // Array of chips in block

    NiVBBlock* m_pkNextBlock;       // Ptrs for creating linked list in
    NiVBBlock* m_pkPrevBlock;       //   the sets...

    //  Static members
    static NiVBChip* ms_pkChipDump;
    static unsigned int ms_uiDefClipAllocSize;
    //***  Internal use only - End
    //******************************

    friend class NiVBSet;
};

#include "NiVBBlock.inl"

#endif  //#ifndef NIVBBLOCK_H