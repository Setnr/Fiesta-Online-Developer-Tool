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

#ifndef NIPSYSEMITTERCTLR_H
#define NIPSYSEMITTERCTLR_H

#include "NiPSysModifierCtlr.h"
#include <NiTSet.h>
#include <NiFloatInterpolator.h>
#include <NiBoolInterpolator.h>

class NIPARTICLE_ENTRY NiPSysEmitterCtlr : public NiPSysModifierCtlr
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysEmitterCtlr);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysEmitterCtlr(const char* pcModifierName);

    virtual void Stop();
    virtual void Update(float fTime);

    NiFloatInterpolator* GetBirthRateInterpolator() const;
    NiBoolInterpolator* GetEmitterActiveInterpolator() const;

    void SetBirthRateInterpolator(NiFloatInterpolator* pkInterpolator);
    void SetEmitterActiveInterpolator(NiBoolInterpolator* pkInterpolator);
    
    // *** begin Emergent internal use only ***
    virtual void GuaranteeTimeRange(float fStartTime, float fEndTime);
    virtual void ResetTimeExtrema();

    virtual unsigned short GetInterpolatorCount() const;
    virtual const char* GetInterpolatorID(unsigned short usIndex = 0);
    virtual unsigned short GetInterpolatorIndex(const char* pcID) const;
    
    virtual NiInterpolator* GetInterpolator(unsigned short usIndex = 0) const;
    virtual void SetInterpolator(NiInterpolator* pkInterpolator,
        unsigned short usIndex = 0);

    virtual NiInterpolator* CreatePoseInterpolator(unsigned short usIndex = 0);
    virtual void SynchronizePoseInterpolator(NiInterpolator* pkInterp, 
        unsigned short usIndex = 0);
    virtual NiBlendInterpolator* CreateBlendInterpolator(
        unsigned short usIndex = 0, bool bManagerControlled = false, 
        bool bAccumulateAnimations = false, float fWeightThreshold = 0.0f, 
        unsigned char ucArraySize = 2) const;

    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysEmitterCtlr();

    bool InterpolatorIsCorrectType(NiInterpolator *,unsigned short) const;
    bool InterpTargetIsCorrectType(NiObjectNET *) const;

    static const char* ms_pcBirthRate;
    static const char* ms_pcEmitterActive;

    virtual bool TargetIsRequiredType() const;

    void EmitParticles(float fTime, float fCurTime, float fLastTime,
        float fEmitStart, float fEmitStop, float fBirthRate);
    
    virtual void SetTargetData();

    NiInterpolatorPtr m_spEmitterActiveInterpolator;
    NiInterpolator* m_pkLastBirthRateInterpolator;

    float m_fLastScaledTime;
    bool m_bLastActive;
    NiFloatSet m_kParticleAges;
};

NiSmartPointer(NiPSysEmitterCtlr);

#include "NiPSysEmitterCtlr.inl"

#endif  // #ifndef NIPSYSEMITTERCTLR_H
