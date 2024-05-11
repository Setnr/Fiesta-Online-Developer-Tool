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

#ifndef NIPHYSXRIGIDBODYDEST_H
#define NIPHYSXRIGIDBODYDEST_H

#include "NiPhysXLibType.h"

#include <NxPhysics.h>
#include <NiAVObject.h>
#include <NiPoint3.h>
#include <NiQuaternion.h>
#include <NiColor.h>
#include <NiPhysXTypes.h>

#include "NiPhysXDest.h"

class NIPHYSX_ENTRY NiPhysXRigidBodyDest : public NiPhysXDest
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareAbstractClone(NiPhysXRigidBodyDest);

public:
    NiPhysXRigidBodyDest(NxActor* pkActor, NxActor* pkActorParent);
    ~NiPhysXRigidBodyDest();

    NxActor* GetActor() const;
    void SetActor(NxActor* pkActor);
    NxActor* GetActorParent() const;
    void SetActorParent(NxActor* pkParent);
    
    // Optimizing for sleeping
    bool GetOptimizeSleep() const;
    void SetOptimizeSleep(const bool bOptimize);
    
    // For printf debugging access
    float GetTime(unsigned int uiIndex) const;
    const NxMat34& GetPose(unsigned int uiIndex) const;
    bool GetSleeping() const;
    
    // Update functions
    virtual void UpdateFromActors(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce);
        
protected:
    NiPhysXRigidBodyDest();
    
    void GetTransforms(float fTime, NiMatrix3& kRotation,
        NiPoint3& kTranslation, const NiTransform& kRootTransform);
    
    // We need the parent of the controlled scene graph node to
    // compute the local transformation, but what that is depends on
    // the type of thing we are driving. So it's virtual.
    virtual NiNode* GetSceneGraphParent() const = 0;
    
    // Provide interpolation functionality for use by sub-classes
    void Interpolate(const float fT, NiQuaternion& kRotation,
        NiPoint3& kTranslate);
    
    NxActor* m_pkActor;    
    NxActor* m_pkActorParent;
    
    // A rotating buffer of two transforms. For actors with no scene graph
    // actor parent, these are world transforms. If there is a parent, then
    // these are local transforms to the parent.
    unsigned char m_aucIndices[2];
    float m_afTimes[2];
    NxMat34 m_akPoses[2];

    bool m_bOptimizeSleep;
    bool m_bSleeping;    
};

NiSmartPointer(NiPhysXRigidBodyDest);

#include "NiPhysXRigidBodyDest.inl"

#endif  // #ifndef NIPHYSXRIGIDBODYDEST_H
