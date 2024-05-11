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

#ifndef NIPSYSMODIFIERFLOATCTLR_H
#define NIPSYSMODIFIERFLOATCTLR_H

#include "NiPSysModifierCtlr.h"

class NIPARTICLE_ENTRY NiPSysModifierFloatCtlr : public NiPSysModifierCtlr
{
    NiDeclareRTTI;
    NiDeclareAbstractClone(NiPSysModifierFloatCtlr);
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;

public:

    virtual void Update(float fTime);

    // *** begin Emergent internal use only ***
    virtual NiInterpolator* CreatePoseInterpolator(unsigned short usIndex = 0);
    virtual void SynchronizePoseInterpolator(NiInterpolator* pkInterp, 
        unsigned short usIndex = 0);
    virtual NiBlendInterpolator* CreateBlendInterpolator(
        unsigned short usIndex = 0, bool bManagerControlled = false, 
        bool bAccumulateAnimations = false, float fWeightThreshold = 0.0f, 
        unsigned char ucArraySize = 2) const;
    // *** end Emergent internal use only ***

protected:
    NiPSysModifierFloatCtlr(const char* pcModifierName);

    // For cloning and streaming only.
    NiPSysModifierFloatCtlr();

    // Virtual function overrides from base classes.
    virtual bool InterpolatorIsCorrectType(NiInterpolator* pkInterpolator,
        unsigned short usIndex) const;

    virtual void GetTargetFloatValue(float& fValue) = 0;
    virtual void SetTargetFloatValue(float fValue) = 0;
};

NiSmartPointer(NiPSysModifierFloatCtlr);

#include "NiPSysModifierFloatCtlr.inl"

#endif  // #ifndef NIPSYSMODIFIERFLOATCTLR_H
