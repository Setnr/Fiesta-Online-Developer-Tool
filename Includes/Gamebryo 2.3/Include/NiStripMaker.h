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

#ifndef NISTRIPMAKER_H
#define NISTRIPMAKER_H

#include "NiStripifyLibType.h"
#include <NiMemObject.h>
#include <NiTSet.h>

class NiStripifyEdge;
class NiStripifyTriangle;

// Note: This class does not delete the arrays that are available by calling 
// GetStripData. The assumption is that these arrays will be passed to the 
// constructor for NiTriStripsData, which will assume possession of the arrays.

class NISTRIPIFY_ENTRY NiStripMaker : public NiMemObject
{
public:
    NiStripMaker(const unsigned short* pusTriList, unsigned int uiTriangles);

    // The usTriangles value returned by GetStripData may not equal the 
    // uiTriangles value passed to the constructor. Degenerate triangles are
    // deleted.

    void GetStripData(unsigned short& usTriangles, unsigned short& usStrips,
        unsigned short*& pusStripLengths, unsigned short*& pusStripLists)
        const;

    ~NiStripMaker();

protected:
    void BuildConnectivity(const unsigned short* pusTriList, 
        unsigned int uiTriangles);
    void BuildStrips(const unsigned short* pusTriList, 
        unsigned int uiTriangles);
    unsigned short GetVertexIndex(const unsigned short* pusTriList,
        unsigned short usVertex, unsigned short usTriangle);
    unsigned short GetExcludedVertex(const unsigned short* pusTriList, 
        NiStripifyEdge* pkEdge, unsigned short usTriangle);

    NiStripifyTriangle* m_pkTriangles;

    // Use this array to hang on to pointers to all edges when they are 
    // allocated, so we have a way to delete them all.
    NiTPrimitiveSet<NiStripifyEdge*> m_kEdges;

    unsigned short m_usTriangles;
    unsigned short m_usStrips;
    unsigned short* m_pusStripLengths;
    unsigned short* m_pusStripLists;
};

//---------------------------------------------------------------------------
inline void
NiStripMaker::GetStripData(unsigned short& usTriangles,
                           unsigned short& usStrips,
                           unsigned short*& pusStripLengths,
                           unsigned short*& pusStripLists) const
{
    usTriangles = m_usTriangles;
    usStrips = m_usStrips;
    pusStripLengths = m_pusStripLengths;
    pusStripLists = m_pusStripLists;
}

#endif // NISTRIPMAKER_H
