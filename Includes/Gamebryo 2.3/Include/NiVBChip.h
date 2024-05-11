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
#ifndef NIVBCHIP_H
#define NIVBCHIP_H

#include "NiD3DDefines.h"
#include "NiD3DMacros.h"
#include <NiRTLib.h>

// NiVBChip represent a chunk of vertex buffer memory.
// A chip may be either a whole vertex buffer, or just a section of a much 
//   larger one. 
// A NiVBChip is ALWAYS linked to a NiVBBlock in the static case - it can not
// exist without one.  In the dynamic case, it is linked directly to its
// NiVBDynamicSet.  Dynamic cases should just set pkBlock to NULL in the
// create call and then use SetDynamicSet() to set its parent.

class NiVBBlock;

class NiVBChip : public NiMemObject
{
protected:
    NiVBChip();
public:
    ~NiVBChip();

    // *** begin Emergent internal use only ***
    static NiVBChip* Create(NiVBBlock* pkBlock, D3DVertexBufferPtr pkVB,
        unsigned int uiOffset, unsigned int uiSize, unsigned int uiIndex);

    // static get parent
    NiVBBlock* GetBlock();
    // dynamic get parent
    NiVBDynamicSet* GetDynamicSet();

    D3DVertexBufferPtr GetVB();
    unsigned int GetOffset();
    unsigned int GetLockFlags();
    unsigned int GetSize();
    unsigned int GetIndex();

    NiVBChip* GetNext();
    void SetNext(NiVBChip* pkChip);
    NiVBChip* GetPrev();
    void SetPrev(NiVBChip* pkChip);

    // Set functions should only ever be called by geometry groups.
    void SetBlock(NiVBBlock* pkBlock);
    void SetDynamicSet(NiVBDynamicSet* pkSet);
    void SetVB(D3DVertexBufferPtr pkVB);
    void SetOffset(unsigned int uiOffset);
    void SetLockFlags(unsigned int uiLockFlags);
    void SetSize(unsigned int uiSize);
    void SetIndex(unsigned int uiIndex);

    // *** end Emergent internal use only ***

protected:

    unsigned int m_uiIndex;         // Internally referenced ID
    union
    {
        // Parent block (static)
        NiVBBlock* m_pkBlock;
        // Parent set (dynamic)
        NiVBDynamicSet* m_pkDynamicSet; 
    };
    D3DVertexBufferPtr m_pkVB;      // Pointer to the VB
    unsigned int m_uiOffset;        // Offset into the VB
    unsigned int m_uiLockFlags;     // D3D Flags to use when locking
    unsigned int m_uiSize;          // Size of data in VB (bytes)

    NiVBChip* m_pkNextChip;
    NiVBChip* m_pkPrevChip;

    friend class NiVBBlock;
};

#include "NiVBChip.inl"

#endif  //#ifndef NIVBCHIP_H
