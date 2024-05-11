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

#ifndef NIPHYSXTRANSFORMDEST_H
#define NIPHYSXTRANSFORMDEST_H

#include "NiPhysXLibType.h"

#include "NiPhysXRigidBodyDest.h"

class NIPHYSX_ENTRY NiPhysXTransformDest : public NiPhysXRigidBodyDest
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXTransformDest);

public:
    NiPhysXTransformDest(NiAVObject* pkTarget, NxActor *pkActor,
        NxActor* pkActorParent);
    ~NiPhysXTransformDest();

    NiAVObject* GetTarget() const;
    void SetTarget(NiAVObject* pkTarget);
    
    // Update functions.
    virtual void UpdateSceneGraph(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce);
            
    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***

protected:
    NiPhysXTransformDest();
    
    virtual NiNode* GetSceneGraphParent() const;

    NiAVObject* m_pkTarget;    
};

NiSmartPointer(NiPhysXTransformDest);

#include "NiPhysXTransformDest.inl"

#endif  // #ifndef NIPHYSXTRANSFORMDEST_H
