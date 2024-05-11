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

#ifndef NIPHYSXACCUMRIGIDBODYDEST_H
#define NIPHYSXACCUMRIGIDBODYDEST_H

#include "NiPhysXLibType.h"

#include <NxPhysics.h>

#include "NiPhysXRigidBodyDest.h"

class NIPHYSX_ENTRY NiPhysXAccumRigidBodyDest : public NiPhysXRigidBodyDest
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareAbstractClone(NiPhysXAccumRigidBodyDest);

public:
    NiPhysXAccumRigidBodyDest(NxActor* pkActor, NxActor* pkActorParent);
    ~NiPhysXAccumRigidBodyDest();

    // Flags controlling accumulation
    void SetAccumTranslation(
        const bool bAccumX, const bool bAccumY, const bool bAccumZ);
    void GetAccumTranslation(
        bool& bAccumX, bool& bAccumY, bool& bAccumZ) const;
    void SetAccumRotation(
        const bool bAccumX, const bool bAccumY, const bool bAccumZ);
    void GetAccumRotation(
        bool& bAccumX, bool& bAccumY, bool& bAccumZ) const;
    void SetAccumFrontFacing(const unsigned char ucAxis,
        const bool bIsNegative);
    void GetAccumFrontFacing(unsigned char& ucAxis, bool& bIsNegative) const;
        
protected:
    NiPhysXAccumRigidBodyDest();

    void GetAccumTransforms(float fTime, NiQuaternion& kAccumRotation,
        NiPoint3& kAccumTranslation, NiQuaternion& kNonAccumRotation,
        NiPoint3& kNonAccumTranslation, const NiTransform& kRootTransform);
        
    void FactorRot(const NiMatrix3& kFullRot,
        NiMatrix3& kAccumRot, NiMatrix3& kNonAccumRot);
    void FactorTrans(const NiPoint3& kFullTrans,
        NiPoint3& kAccumTrans, NiPoint3& kNonAccumTrans);
    
    enum {
        // Translations that are accumulated
        ACCUM_X_TRANS   = 0x0001,
        ACCUM_Y_TRANS   = 0x0002,
        ACCUM_Z_TRANS   = 0x0004,
        
        // Rotation that is accumulated. Either one, none or all
        // of these may be specified.
        ACCUM_X_ROT     = 0x0008,
        ACCUM_Y_ROT     = 0x0010,
        ACCUM_Z_ROT     = 0x0020,

        // Front facing axis. Default is positive X.
        ACCUM_X_FRONT   = 0x0040,
        ACCUM_Y_FRONT   = 0x0080,
        ACCUM_Z_FRONT   = 0x0100,
        ACCUM_NEG_FRONT = 0x0200
    };
    NiDeclareFlags(unsigned int);
};

NiSmartPointer(NiPhysXAccumRigidBodyDest);

#include "NiPhysXAccumRigidBodyDest.inl"

#endif  // #ifndef NIPHYSXACCUMRIGIDBODYDEST_H
