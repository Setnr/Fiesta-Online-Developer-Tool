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

#ifndef NIPHYSXTYPES_H
#define NIPHYSXTYPES_H

#include "NiPhysXLibType.h"


#include <NiMainPCH.h>
#include <NxPhysics.h>

class NIPHYSX_ENTRY NiPhysXTypes
{
public:
    static void NxVec3ToNiPoint3(const NxVec3& kNxVSrc, NiPoint3& kNiPDest);
    static void NiPoint3ToNxVec3(const NiPoint3& kNiPSrc, NxVec3& kNxVDest);

    static void NxQuatToNiQuaternion(const NxQuat& kNxQSrc,
        NiQuaternion& kNiQDest);
    static void NiQuaternionToNxQuat(const NiQuaternion& kNiQSrc,
        NxQuat& kNxQDest);

    static void NxMat33ToNiMatrix3(const NxMat33& kNxMSrc, 
        NiMatrix3& kNiMDest);
    static void NiMatrix3ToNxMat33(const NiMatrix3& kNiMSrc, 
        NxMat33& kNxMDest);

    static void NxMat33ToNiQuaternion(const NxMat33& kNxMSrc,
        NiQuaternion& kNiQDest);
    static void NiQuaternionToNxMat33(const NiQuaternion& kNiQSrc,
        NxMat33& kNxMDest);

    static void NxMat34ToNiTransform(const NxMat34& kNxMSrc,
        NiMatrix3& kNiMDest, NiPoint3& kNiPDest);
    static void NiTransformToNxMat34(const NiMatrix3& kNiMSrc,
        const NiPoint3& kNiPSrc, NxMat34& kNxMDest);

    static void NxMat34ToNiQuatTransform(const NxMat34& kNxMSrc,
        NiQuaternion& kNiQDest, NiPoint3& kNiPDest);
    static void NiQuatTransformToNxMat34(const NiQuaternion& kNiQSrc,
        const NiPoint3& kNiPSrc, NxMat34& kNxMDest);

    static void NxColorToNiColorA(const NxU32 kNxSrc, NiColorA& kNiDest);

    static const NxVec3 NXVEC3_ZERO;
    static const NxMat33 NXMAT33_ID;
    static const NxMat34 NXMAT34_ID;

    // *** begin Emergent use only ***
    
    static void NxVec3SaveBinary(NiStream& kStream, const NxVec3& kNxVec);
    static void NxVec3LoadBinary(NiStream& kStream, NxVec3& kNxVec);

    static void NxMat34SaveBinary(NiStream& kStream, const NxMat34& kNxMat);
    static void NxMat34LoadBinary(NiStream& kStream, NxMat34& kNxMat);
    
    static void NxQuatSaveBinary(NiStream& kStream, const NxQuat& kNxQuat);
    static void NxQuatLoadBinary(NiStream& kStream, NxQuat& kNxQuat);

    static const NxPhysicsSDKDesc NXPHYSICSSDKDESC_DEFAULT;
    // *** end Emergent use only ***

protected:
   
};

#include "NiPhysXTypes.inl"

#endif  // #ifndef NIPHYSXTYPES_H

