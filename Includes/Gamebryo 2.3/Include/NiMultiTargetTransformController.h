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

#ifndef NIMULTITARGETTRANSFORMCONTROLLER_H
#define NIMULTITARGETTRANSFORMCONTROLLER_H

#include "NiInterpController.h"
#include "NiBlendTransformInterpolator.h"

class NiControllerSequence;

class NIANIMATION_ENTRY NiMultiTargetTransformController :
    public NiInterpController
{
    NiDeclareRTTI;
    NiDeclareClone(NiMultiTargetTransformController);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiMultiTargetTransformController();
    NiMultiTargetTransformController(unsigned short usCount);
    virtual ~NiMultiTargetTransformController();

    virtual void Update(float fTime);

    unsigned short GetArrayCount();
    NiBlendTransformInterpolator* GetBlendInterpolator(
        unsigned short usIndex);
    NiAVObject* GetInterpolatorTarget(unsigned short usIndex);

    void SetInterpolatorTarget(unsigned short usIndex, NiAVObject* pkTarget);
    unsigned short GetInterpolatorTargetIndex(NiAVObject* pkObj);
    unsigned short AddInterpolatorTarget(NiAVObject* pkTarget);
    
    virtual unsigned short GetInterpolatorCount() const;

    virtual const char* GetInterpolatorID(unsigned short usIndex = 0);
    virtual unsigned short GetInterpolatorIndex(const char* pcID) const;
    
    virtual NiInterpolator* GetInterpolator(unsigned short usIndex = 0) const;
    virtual void SetInterpolator(NiInterpolator* pkInterpolator,
        unsigned short usIndex = 0);

    // This method should be called whenever the key time range must be reset.
    // Note that this may cause the key range to be set to (0.0f, 0.0f) if it
    // contains any pose interpolators.
    virtual void ResetTimeExtrema();

    // *** begin Emergent internal use only ***
    virtual NiInterpolator* CreatePoseInterpolator(
        unsigned short usIndex = 0);
    virtual void SynchronizePoseInterpolator(NiInterpolator* pkMultiTransform, 
        unsigned short usIndex = 0);
    virtual NiBlendInterpolator* CreateBlendInterpolator(
        unsigned short usIndex = 0, bool bManagerControlled = false, 
        bool bAccumulateAnimations = false, float fWeightThreshold = 0.0f,
        unsigned char ucArraySize = 2) const;
    virtual void GuaranteeTimeRange(float fStartTime, float fEndTime);
    virtual bool TargetIsRequiredType() const;
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For debug only.
    virtual bool InterpolatorIsCorrectType(NiInterpolator* pkInterpolator,
        unsigned short usIndex) const;

    NiBlendTransformInterpolator* m_pkBlendInterps;
    NiAVObject** m_ppkTargets;
    unsigned short m_usNumInterps;

    void AllocateStorage(unsigned short usCount);
};

NiSmartPointer(NiMultiTargetTransformController);

#include "NiMultiTargetTransformController.inl"

#endif  // #ifndef NIMULTITARGETTRANSFORMCONTROLLER_H
