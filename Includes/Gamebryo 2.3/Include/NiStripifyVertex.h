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

#ifndef NISTRIPIFYVERTEX_H
#define NISTRIPIFYVERTEX_H

#include "NiStripifyLibType.h"
#include <NiMemObject.h>
#include <NiTSet.h>

class NiStripifyEdge;

class NISTRIPIFY_ENTRY NiStripifyVertex : public NiMemObject
{
public:
    NiStripifyEdge* GetEdge(unsigned short usVert);
    void AddEdge(NiStripifyEdge* pkEdge);
    
#ifdef _DEBUG
    unsigned short m_usVert;
#endif
    
protected:
    // list of edges that originate at this vertex.
    NiTPrimitiveSet<NiStripifyEdge*> m_kEdges;
};

//---------------------------------------------------------------------------
inline void NiStripifyVertex::AddEdge(NiStripifyEdge* pkEdge)
{
    m_kEdges.Add(pkEdge);
}

#endif // NISTRIPIFYVERTEX_H