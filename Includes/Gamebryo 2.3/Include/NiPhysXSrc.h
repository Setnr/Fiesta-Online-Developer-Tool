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

#ifndef NIPHYSXSRC_H
#define NIPHYSXSRC_H

#include "NiPhysXLibType.h"

#include <NiObject.h>

class NIPHYSX_ENTRY NiPhysXSrc : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareAbstractClone(NiPhysXSrc);

public:
    NiPhysXSrc();
    ~NiPhysXSrc();

    bool GetActive() const;
    void SetActive(const bool bActive);
   
    bool GetInterpolate() const;
    void SetInterpolate(const bool bActive);

    // Update functions
    virtual void UpdateFromSceneGraph(const float fT,
        const NiTransform& kInvRootTransform, const float fScaleWToP,
        const bool bForce = false) = 0;
    virtual void UpdateToActors(const float fTBegin, const float fTEnd) = 0;

protected:
    bool m_bActive;
    bool m_bInterp;
};

NiSmartPointer(NiPhysXSrc);

#include "NiPhysXSrc.inl"

#endif  // #ifndef NIPHYSXSRC_H
