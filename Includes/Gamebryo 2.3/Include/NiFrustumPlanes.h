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

#ifndef NIFRUSTUMPLANES_H
#define NIFRUSTUMPLANES_H

#include "NiCamera.h"
#include "NiPlane.h"

class NIMAIN_ENTRY NiFrustumPlanes : public NiMemObject
{
public:
    // The culling planes.
    enum
    {
        // bits for the active planes
        NEAR_PLANE     = 0,
        FAR_PLANE      = 1,
        LEFT_PLANE     = 2,
        RIGHT_PLANE    = 3,
        TOP_PLANE      = 4,
        BOTTOM_PLANE   = 5,
        MAX_PLANES     = 6
    };

    NiFrustumPlanes();
    NiFrustumPlanes(const NiCamera& kCam);
    NiFrustumPlanes(const NiFrustum& kFrust, 
        const NiTransform& kXform);

    void Set(const NiCamera& kCam);
    void Set(const NiFrustum& kFrust, 
        const NiTransform& kXform);

    const NiPlane& GetPlane(unsigned int ePlane) const;
    void SetPlane(unsigned int ePlane, const NiPlane& kPlane);

    bool IsPlaneActive(unsigned int ePlane) const;
    bool IsAnyPlaneActive() const;
    bool AreAllPlanesActive() const;
    void EnablePlane(unsigned int ePlane);
    void DisablePlane(unsigned int ePlane);
    void EnableAllPlanes();
    void DisableAllPlanes();

    unsigned int GetActivePlaneState() const;
    void SetActivePlaneState(unsigned int uiState);
protected:
    enum
    {
        ALL_PLANES_SET = 0x3F  // 6 low-order bits of active planes
    };

    NiPlane m_akCullingPlanes[MAX_PLANES];
    unsigned int m_uiActivePlanes;
};

#include "NiFrustumPlanes.inl"

#endif
