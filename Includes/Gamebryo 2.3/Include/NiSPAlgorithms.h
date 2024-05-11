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
#ifndef NISPALGORITHMS_H
#define NISPALGORITHMS_H
//---------------------------------------------------------------------------
#include "NiFloodgateLibType.h"
//---------------------------------------------------------------------------
class NIFLOODGATE_ENTRY NiSPAlgorithms
{
public:

    // Sorts the collection using quick sort
    template <class C, class T>
    static void QuickSort(C* pkCollection, NiInt32 iLeft, NiInt32 iRight);

    // Aligns the input to the next 128 bytes
    inline static NiUInt64 Align128(NiUInt64 uiSize);

    // Aligns the input to the previous 128 bytes
    inline static void* PreAlign128(void* uiSize);

protected:

    // Partitions the collection
    template <class C, class T>
    static NiInt32 Partition(C* pkCollection, NiInt32 iLeft, NiInt32 iRight);

    // Swaps elements given the indicies
    template <class C, class T>
    static void Swap(C* pkCollection, NiInt32 iIndex1, NiInt32 iIndex2);

    // Hidden constructor / destructor
    NiSPAlgorithms(){}
    ~NiSPAlgorithms(){}
    
};
//---------------------------------------------------------------------------
#include "NiSPAlgorithms.inl"
//---------------------------------------------------------------------------
#endif
