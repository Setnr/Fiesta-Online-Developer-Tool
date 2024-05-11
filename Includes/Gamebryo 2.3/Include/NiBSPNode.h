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

#ifndef NIBSPNODE_H
#define NIBSPNODE_H

#include "NiNode.h"
#include "NiPlane.h"
#include "NiStream.h"

class NIMAIN_ENTRY NiBSPNode : public NiNode
{
    NiDeclareRTTI;
    NiDeclareClone(NiBSPNode);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
        // construction
    NiBSPNode();

    // tree operations
    void AttachLeft(NiAVObject* pkChild);
    void AttachRight(NiAVObject* pkChild);
    NiAVObjectPtr DetachLeft();
    NiAVObjectPtr DetachRight();
    NiAVObjectPtr GetLeft();
    NiAVObjectPtr GetRight();

    // plane methods
    void SetModelPlane(const NiPlane& kPlane);
    const NiPlane& GetModelPlane() const;
    const NiPlane& GetWorldPlane() const;
    int WhichSide(const NiPoint3& kP) const;

    // transforms
    virtual void ApplyTransform(const NiMatrix3& kMat, const NiPoint3& kTrn,
        bool bOnLeft);

    // *** begin Emergent internal use only ***

    // culling (separate from display)
    virtual void OnVisible(NiCullingProcess& kCuller);

    // *** end Emergent internal use only ***

protected:
    // update during scene tree traversal
    virtual void UpdateWorldData();
    void UpdateWorldPlane();

    NiPlane m_kModelPlane;
    NiPlane m_kWorldPlane;
};


typedef NiPointer<NiBSPNode> NiBSPNodePtr;

//---------------------------------------------------------------------------
//  Inline include
#include "NiBSPNode.inl"

//---------------------------------------------------------------------------


#endif

