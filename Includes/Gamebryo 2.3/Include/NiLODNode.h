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

#ifndef NILODNODE_H
#define NILODNODE_H

#include "NiPoint3.h"
#include "NiStream.h"
#include "NiSwitchNode.h"
#include "NiTArray.h"
#include "NiLODData.h"

class NIMAIN_ENTRY NiLODNode : public NiSwitchNode
{
    NiDeclareRTTI;
    NiDeclareClone(NiLODNode);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    // construction and destruction
    NiLODNode();
    virtual ~NiLODNode();

    void SetLODActive(bool bLODActive);
    bool GetLODActive() const;

    NiLODData* GetLODData() const;
    void SetLODData(NiLODData* pkLODData);

    void UpdateAllChildren();

    // This is a global debug setting used to set all LOD Levels 
    static int GetGlobalLOD();
    static void SetGlobalLOD(int iNewLOD);

protected:

    // update during scene tree traversal
    virtual void UpdateWorldData();

    // culling (separate from drawing)
    virtual void OnVisible(NiCullingProcess& kCuller);

    NiLODDataPtr m_spLODData;

    bool m_bLODActive;

    static int ms_iGlobalLOD;
};


typedef NiPointer<NiLODNode> NiLODNodePtr;

//---------------------------------------------------------------------------
//  Inline include
#include "NiLODNode.inl"

//---------------------------------------------------------------------------

#endif  // NILODNODE_H

