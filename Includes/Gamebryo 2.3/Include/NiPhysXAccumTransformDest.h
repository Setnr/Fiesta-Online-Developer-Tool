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

#ifndef NIPHYSXACCUMTRANSFORMDEST_H
#define NIPHYSXACCUMTRANSFORMDEST_H

#include "NiPhysXLibType.h"

#include "NiPhysXAccumRigidBodyDest.h"

class NIPHYSX_ENTRY NiPhysXAccumTransformDest :
    public NiPhysXAccumRigidBodyDest
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXAccumTransformDest);

public:
    NiPhysXAccumTransformDest(NiAVObject* pkAccumTarget,
        NiAVObject* pkNonAccumTarget,
        NxActor *pkActor, NxActor* pkActorParent);
    ~NiPhysXAccumTransformDest();

    NiAVObject* GetAccumTarget() const;
    void SetAccumTarget(NiAVObject* pkTarget);

    NiAVObject* GetNonAccumTarget() const;
    void SetNonAccumTarget(NiAVObject* pkTarget);
    
    // Update functions.
    virtual void UpdateSceneGraph(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce);
    
    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***

protected:
    NiPhysXAccumTransformDest();
    
    virtual NiNode* GetSceneGraphParent() const;

    NiAVObject* m_pkAccumTarget;    
    NiAVObject* m_pkNonAccumTarget;    
};

NiSmartPointer(NiPhysXAccumTransformDest);

#include "NiPhysXAccumTransformDest.inl"

#endif  // #ifndef NIPHYSXACCUMTRANSFORMDEST_H
