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


#ifndef NIPHYSXD6JOINTDESC_H
#define NIPHYSXD6JOINTDESC_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXJointDesc.h"

class NIPHYSX_ENTRY NiPhysXD6JointDesc : public NiPhysXJointDesc
{
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXD6JointDesc);

public:
    NiPhysXD6JointDesc();
    
    virtual NxJoint* CreateJoint(NxScene* pkScene,
        const NxMat34& kXform = NiPhysXTypes::NXMAT34_ID);
    virtual void RemoveJoint(NxScene* pkScene);
    
    // Sets actors
    virtual void FromJoint(NxD6Joint* pkJoint,
        NiTMap<NxActor*, NiPhysXActorDescPtr>& kActorMap,
        const NxMat34& kXform = NiPhysXTypes::NXMAT34_ID);
        
    // Does not set actors
    virtual void SetD6JointDesc(const NxD6JointDesc& kJointDesc);
    virtual void GetD6JointDesc(NxD6JointDesc& kJointDesc,
        const NxMat34& kXform = NiPhysXTypes::NXMAT34_ID) const;
    
    // Get the joint
    NxD6Joint* GetJoint();

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:        
    void SaveNxJointLimitSoftDesc(NiStream& kStream,
        const NxJointLimitSoftDesc& kDesc);
    void SaveNxJointDriveDesc(NiStream& kStream,
        const NxJointDriveDesc& kDesc);
    void LoadNxJointLimitSoftDesc(NiStream& kStream,
        NxJointLimitSoftDesc& kDesc);
    void LoadNxJointDriveDesc(NiStream& kStream,
        NxJointDriveDesc& kDesc);

    NxD6JointMotion m_eXMotion;
    NxD6JointMotion m_eYMotion;
    NxD6JointMotion m_eZMotion;
    NxD6JointMotion m_eSwing1Motion;
    NxD6JointMotion m_eSwing2Motion;
    NxD6JointMotion m_eTwistMotion;
    
    NxJointLimitSoftDesc m_kLinearLimit;
    NxJointLimitSoftDesc m_kSwing1Limit;
    NxJointLimitSoftDesc m_kSwing2Limit;
    NxJointLimitSoftPairDesc m_kTwistLimit;
 
    NxJointDriveDesc  m_kXDrive;
    NxJointDriveDesc  m_kYDrive;
    NxJointDriveDesc  m_kZDrive;
    NxJointDriveDesc  m_kSwingDrive;
    NxJointDriveDesc  m_kTwistDrive;
    NxJointDriveDesc  m_kSlerpDrive;
     
    NxVec3 m_kDrivePosition;
    NxQuat m_kDriveOrientation;
    NxVec3 m_kDriveLinearVelocity;
    NxVec3 m_kDriveAngularVelocity;
     
    NxJointProjectionMode m_eProjectionMode;
    NxReal m_fProjectionDistance; 
    NxReal m_fProjectionAngle;
     
    NxReal  m_fGearRatio;
     
    NxU32  m_uiFlags;

    NxJoint* m_pkJoint;
};

NiSmartPointer(NiPhysXD6JointDesc);

#endif // #ifdef NIPHYSXD6JOINTDESC_H

