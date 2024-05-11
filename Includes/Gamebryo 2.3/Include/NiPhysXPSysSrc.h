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

#ifndef NIPHYSXPSYSSRC_H
#define NIPHYSXPSYSSRC_H

#include "NiPhysXParticleLibType.h"

#include "NiPhysXSrc.h"

class NiPhysXPSysData;
class NiPhysXParticleSystem;

class NIPHYSXPARTICLE_ENTRY NiPhysXPSysSrc : public NiPhysXSrc
{
    NiDeclareRTTI;
    NiDeclareStream;

public:
    NiPhysXPSysSrc(NiParticleSystem* pkSource);
    ~NiPhysXPSysSrc();

    NiParticleSystem* GetSource();
    
    // Update functions
    virtual void UpdateFromSceneGraph(const float fT,
        const NiTransform& kInvRootTransform, const float fScaleWToP,
        const bool bForce = false);
    virtual void UpdateToActors(const float fTBegin, const float fTEnd);

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For streaming only
    NiPhysXPSysSrc();

    NiParticleSystem* m_pkSource;

    NiTransform m_kInvRootTransform;
    float m_fScaleWToP;
};

NiSmartPointer(NiPhysXPSysSrc);

#include "NiPhysXPSysSrc.inl"

#endif  // #ifndef NIPHYSXPSYSSRC_H
