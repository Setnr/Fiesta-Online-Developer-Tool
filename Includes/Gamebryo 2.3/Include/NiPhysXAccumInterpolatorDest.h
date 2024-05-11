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

#ifndef NIPHYSXACCUMINTERPOLATORDEST_H
#define NIPHYSXACCUMINTERPOLATORDEST_H

#include "NiPhysXLibType.h"

#include <NiTransformInterpolator.h>
#include <NiBlendAccumTransformInterpolator.h>

#include "NiPhysXAccumRigidBodyDest.h"

class NIPHYSX_ENTRY NiPhysXAccumInterpolatorDest :
    public NiPhysXAccumRigidBodyDest
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXAccumInterpolatorDest);

public:
    NiPhysXAccumInterpolatorDest(NiInterpolator* pkAccumTarget,
        NiInterpolator* pkNonAccumTarget,
        NiBlendAccumTransformInterpolator* pkBlendInterplator,
        NiNode* pkSceneParent, NxActor *pkActor, NxActor* pkActorParent);
    ~NiPhysXAccumInterpolatorDest();

    NiInterpolator* GetAccumTarget() const;
    void SetAccumTarget(NiInterpolator* pkTarget, NiNode* pkParent);
    
    NiInterpolator* GetNonAccumTarget() const;
    void SetNonAccumTarget(NiInterpolator* pkTarget);

    NiBlendAccumTransformInterpolator* GetBlendInterpolator() const;
    void SetBlendInterpolator(NiBlendAccumTransformInterpolator* pkInterp);

    // Update functions
    virtual void UpdateSceneGraph(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce);

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
    
protected:
    NiPhysXAccumInterpolatorDest();
    
    virtual NiNode* GetSceneGraphParent() const;
    
    NiInterpolator* m_pkAccumTarget;
    NiInterpolator* m_pkNonAccumTarget;
    NiBlendAccumTransformInterpolator* m_pkBlendInterp;
    NiNode* m_pkSceneParent;
};

NiSmartPointer(NiPhysXAccumInterpolatorDest);

#include "NiPhysXAccumInterpolatorDest.inl"

#endif  // #ifndef NIPHYSXACCUMINTERPOLATORDEST_H
