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


#ifndef NIPHYSXJOINTDESC_H
#define NIPHYSXJOINTDESC_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXActorDesc.h"

class NIPHYSX_ENTRY NiPhysXJointDesc : public NiObject
{
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXJointDesc);

public:
    // This does set the actors.
    static NiPhysXJointDesc* CreateFromJoint(NxJoint* pkJoint,
        NiTMap<NxActor*, NiPhysXActorDescPtr>& kActorMap,
        const NxMat34& kXform = NiPhysXTypes::NXMAT34_ID);

    NiPhysXActorDescPtr GetActor(const unsigned int uiIndex);
    void SetActor(const unsigned int uiIndex, NiPhysXActorDesc* kActorDesc);

    NxVec3 GetLimitPoint() const;
    void SetLimitPoint(const NxVec3& kLimitPt);
    
    unsigned int GetNumLimits() const;
    void GetLimitPlane(const unsigned int uiIndex, NxVec3& kNorm, NxReal& kD);
    void SetLimitPlane(const unsigned int uiIndex, const NxVec3& kNorm,
        const NxReal& kD);

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
    
protected:
    NiPhysXJointDesc();
    virtual ~NiPhysXJointDesc();

    virtual void SetJointDesc(const NxJointDesc& kJointDesc);
    virtual void GetJointDesc(NxJointDesc& kJointDesc,
        const NxMat34& kXform) const;

    virtual void FromJoint(NxJoint* pkJoint);
    virtual void ToJoint(NxJoint* pkJoint);

    NxJointType m_eType;

    NiFixedString m_kJointName;
    NiPhysXActorDescPtr m_aspActors[2];
    NxVec3 m_akLocalNormal[2];
    NxVec3 m_akLocalAxis[2];
    NxVec3 m_akLocalAnchor[2];
    NxReal m_fMaxForce;
    NxReal m_fMaxTorque;
    NxU32 m_uiJointFlags;
    
    NxVec3 m_kLimitPoint;
    unsigned int m_uiNumLimits;
    NxVec3* m_pkLimitPlaneNorms;
    NxReal* m_pkLimitPlaneDs;
};

NiSmartPointer(NiPhysXJointDesc);

#endif // #ifdef NIPHYSXJOINTDESC_H

