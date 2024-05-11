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

#ifndef NIVISIBLEARRAY_H
#define NIVISIBLEARRAY_H

#include "NiTArray.h"

class NiGeometry;

class NIMAIN_ENTRY NiVisibleArray : public NiMemObject
{
public:
    NiVisibleArray();
    NiVisibleArray(unsigned int uiMaxSize, unsigned int uiGrowBy);
    ~NiVisibleArray();

    // adds to the end of the array, incrementing count by one
    void Add(NiGeometry& kObj);

    // Fast - does not clear each element
    void RemoveAll();

    // uiIndex < GetCount()
    void SetAt(unsigned int uiIndex, NiGeometry& kObj);

    // Ensures that the array stays packed, but does change the order
    // (fills empty slot with last item in array - DOES NOT slide all
    // other items over)
    NiGeometry& RemoveAtAndFill(unsigned int uiIndex);

    // uiIndex < GetCount()
    NiGeometry& GetAt(unsigned int uiIndex);
    NiGeometry& GetAt(unsigned int uiIndex) const;

    unsigned int GetCount() const;

    unsigned int GetAllocatedSize() const;
    void SetAllocatedSize(unsigned int uiSize);

    unsigned int GetGrowBy() const;

    // Grow by _must_ be positive.  Passing uiGrowBy of 0 will cause the 
    // grow by member to be set to 1
    void SetGrowBy(unsigned int uiGrowBy);

protected:

    enum { DEF_MAX_SIZE = 1024 };
    enum { DEF_GROW_BY = 1024 };

    NiGeometry** m_ppkArray;
    unsigned int m_uiCurrentSize;
    unsigned int m_uiAllocatedSize;

    unsigned int m_uiGrowBy;
};

#include "NiVisibleArray.inl"

#endif
