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

#ifndef NIPHYSXDEST_H
#define NIPHYSXDEST_H

#include "NiPhysXLibType.h"

#include <NiObject.h>
#include <NiAVObject.h>

class NIPHYSX_ENTRY NiPhysXDest : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareAbstractClone(NiPhysXDest);

public:
    NiPhysXDest();
    ~NiPhysXDest();

    bool GetActive() const;
    void SetActive(const bool bActive);

    bool GetInterpolate() const;
    void SetInterpolate(const bool bActive);

    // Update functions
    virtual void UpdateFromActors(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce) = 0;
    virtual void UpdateSceneGraph(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce) = 0;
    
protected:
    bool m_bActive;
    bool m_bInterp;
};

NiSmartPointer(NiPhysXDest);

#include "NiPhysXDest.inl"

#endif  // #ifndef NIPHYSXDEST_H
