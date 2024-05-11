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

#ifndef NISWITCHNODE_H
#define NISWITCHNODE_H

#include "NiNode.h"
#include "NiStream.h"

class NIMAIN_ENTRY NiSwitchNode : public NiNode
{
    NiDeclareRTTI;
    NiDeclareClone(NiSwitchNode);
    NiDeclareStream;
    NiDeclareViewerStrings;
    NiDeclareFlags(unsigned short);

public:
    // construction and destruction
    NiSwitchNode();

    // child attaching/detaching
    virtual void AttachChild(NiAVObject* pkChild, bool bFirstAvail = false);
    virtual NiAVObjectPtr DetachChild(NiAVObject* pkChild);
    virtual NiAVObjectPtr DetachChildAt(unsigned int i);
    virtual NiAVObjectPtr SetAt(unsigned int i, NiAVObject* pkChild);

    // switch operations
    NiAVObject* GetActiveChild() const;
    void SetIndex(int iIndex);
    int GetIndex() const;

    // updates
    void UpdateOnlyActiveChild(bool bUpdateOnlyActive);
    bool GetUpdateOnlyActiveChild() const;

    // *** begin Emergent internal use only ***

    // updates
    virtual void UpdateControllers(float fTime);
    virtual void UpdateDownwardPass(float fTime, bool bUpdateControllers);
    virtual void UpdateSelectedDownwardPass(float fTime);
    virtual void UpdateRigidDownwardPass(float fTime);
    virtual void UpdateWorldBound();
    virtual void UpdateNodeBound();

    // culling (separate from drawing)
    virtual void OnVisible(NiCullingProcess& kCuller);

    // *** end Emergent internal use only ***

protected:
    // --- Begin NIF conversion code
    // Deprecated flags - use new flag code
    enum
    {
        MAX_POS = 9
    };
    // --- End NIF conversion code

    // flags
    enum
    {
        UPDATE_ONLY_ACTIVE_CHILD_MASK   = 0x0001,
        UPDATECONTROLLERS_MASK          = 0x0002
    };

    void SetUpdateControllers(bool bUpdateControllers);
    bool GetUpdateControllers() const;

    int m_iIndex;

    float m_fSavedTime;
    unsigned int m_uiRevID;
    NiTPrimitiveArray<unsigned int> m_kChildRevID;
};

typedef NiPointer<NiSwitchNode> NiSwitchNodePtr;

#include "NiSwitchNode.inl"

#endif
