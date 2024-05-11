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

#ifndef NIPHYSXRIGIDBODYSRC_H
#define NIPHYSXRIGIDBODYSRC_H

#include "NiPhysXLibType.h"

#include <NiAVObject.h>
#include <NxPhysics.h>
#include "NiPhysXSrc.h"

class NIPHYSX_ENTRY NiPhysXRigidBodySrc : public NiPhysXSrc
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareAbstractClone(NiPhysXRigidBodySrc);

public:
    NiPhysXRigidBodySrc(NiAVObject* pkSource, NxActor* pkActor);
    ~NiPhysXRigidBodySrc();

    NiAVObject* GetSource() const;
    void SetTarget(NxActor* pkActor);
    NxActor* GetTarget() const;
    
    // If called, velocity computations will be reset to require a couple
    // of frames of data from the scene graph before velocities are non-zero.
    void ResetVelocity();
    
    // For printf debugging access
    float GetTime(unsigned int uiIndex) const;
    NxMat34 GetPose(unsigned int uiIndex) const;
    
    // Update functions
    virtual void UpdateFromSceneGraph(const float fT,
        const NiTransform& kInvRootTransform, const float fScaleWToP,
        const bool bForce = false);

    // *** begin Emergent internal use ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use ***

protected:
    NiPhysXRigidBodySrc();
    
    // Provide interpolation functionality for use by sub-classes
    void Interpolate(const float fT, NxMat34& kPose);
    bool CanComputeVelocities() const;
    void ComputeVelocities(const NxMat34& kBodyLocalPose,
        NxVec3& kLinVel, NxVec3& kAngVel);
    void ComputeDynamicData(const float fT, const NxMat34& kBodyLocalPose,
        NxMat34& kBodyWorldPose, NxVec3& kLinVel, NxVec3& kAngVel);
        
    NiAVObject* m_pkSource;
    NxActor* m_pkTarget;
    
    // A rotating buffer of two poses. Data at m_aucIndices[1] is most recent.
    // These are the PhysX global transforms.
    NxMat34 m_akPoses[2];
    float m_afTimes[2];
    unsigned char m_aucIndices[2];
    
    // This holds the number of frames still required before we can compute
    // decent velocity information.
    unsigned char m_ucResetVelocity;

    // This is true if the target was dynamic the last time we set its state,
    // or is true if we have never set the target's state.
    bool m_bForcedUpdate;

};

NiSmartPointer(NiPhysXRigidBodySrc);

#include "NiPhysXRigidBodySrc.inl"

#endif  // #ifndef NIPHYSXRIGIDBODYSRC_H
