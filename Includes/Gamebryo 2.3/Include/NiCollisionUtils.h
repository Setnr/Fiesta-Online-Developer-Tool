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

#ifndef NICOLLISIONUTILS_H
#define NICOLLISIONUTILS_H

#include "NiCollisionGroup.h"

class NiAVObject;

namespace NiCollisionUtils
{
    // bounding volume intersection routines
    bool NICOLLISION_ENTRY BoundingVolumeTestIntersect(float fTime,
        NiAVObject* pkObj1, NiAVObject* pkObj2);
    bool NICOLLISION_ENTRY BoundingVolumeFindIntersect(float fTime,
        NiAVObject* pkObj1, NiAVObject* pkObj2, float& fIntrTime,
        NiPoint3& kIntrPoint, bool bCalcNormals, NiPoint3& kNormal0,
        NiPoint3& kNormal1);

    // triangle-[triangle | bv] intersection routines
    int NICOLLISION_ENTRY TriTriTestIntersect(float fTime, 
        NiAVObject* pkGeom1, NiAVObject* pkGeom2, bool& bCollision);
    int NICOLLISION_ENTRY TriTriFindIntersect(float fTime,
        NiAVObject* pkGeom1, NiAVObject* pkGeom2, bool bCalcNormals, 
        NiCollisionGroup::Intersect& kIntr, bool& bCollision);
    bool NICOLLISION_ENTRY TriToBndVolTestIntersect(float fTime,
        NiAVObject* pkTBG, NiAVObject* pkOther, bool& bCollision);
    int NICOLLISION_ENTRY TriToBndVolFindIntersect(float fTime, 
        NiAVObject* pkTBG, NiAVObject* pkOther, bool bCalcNormals, 
        NiCollisionGroup::Intersect& kIntr, bool& bCollision);

    // picking intersection routines
    bool NICOLLISION_ENTRY FindBoundIntersect(NiAVObject* pkObj, 
        const NiPoint3& kOrigin, const NiPoint3& kDir);
    bool NICOLLISION_ENTRY IntersectTriangle(const NiPoint3& kOrigin,
        const NiPoint3& kDir, const NiPoint3& kV1, const NiPoint3& kV2,
        const NiPoint3& kV3, bool bCull, NiPoint3& kIntersect, float& r,
        float& s, float& t);
}

#include "NiCollisionUtils.inl"

#endif
