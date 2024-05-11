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

#ifndef NINEIGHBORMANAGER_H
#define NINEIGHBORMANAGER_H

#include "NiStripifyLibType.h"
#include <NiMemObject.h>
#include "NiNeighborManager.h"

class NiStripifyTriangle;

class NISTRIPIFY_ENTRY NiNeighborManager : public NiMemObject
{
public:
    NiNeighborManager();
    void Add(NiStripifyTriangle* pkTriangle);
    void Remove(NiStripifyTriangle* pkTriangle);
    void Adjust(NiStripifyTriangle* pkTriangle);
    NiStripifyTriangle* GetMinimum() const;
    void AdjustNeighbors(NiStripifyTriangle* pkTriangle);
    
protected:
    static unsigned int CountNeighbors(const NiStripifyTriangle* pkTriangle);

    // m_pNeighborList[i] is the triangle at the head of a list of triangles 
    // which all have neighbors adjacent to i edges.
    NiStripifyTriangle* m_apkNeighborLists[4];
};

#endif // NINEIGHBORMANAGER_H