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

#ifndef NIPHYSXPSYSDEST_H
#define NIPHYSXPSYSDEST_H

#include "NiPhysXParticleLibType.h"

#include <NiParticle.h>
#include <NxPhysics.h>

#include "NiPhysXDest.h"

class NiPhysXParticleSystem;

class NIPHYSXPARTICLE_ENTRY NiPhysXPSysDest : public NiPhysXDest
{
    NiDeclareRTTI;
    NiDeclareStream;

public:
    NiPhysXPSysDest(NiParticleSystem* pkTarget);
    ~NiPhysXPSysDest();

    NiParticleSystem* GetTarget();

    // Update functions
    virtual void UpdateFromActors(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce);
    virtual void UpdateSceneGraph(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce);

    bool GetUpdated() const;

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    NiPhysXPSysDest();
    
    NiParticleSystem* m_pkTarget;
    
    // The scale the last time the simulation was stepped.
    float m_fScalePToW;
    
    // true if we have updated the particles since the last simulation step.
    bool m_bUpdated;
};

NiSmartPointer(NiPhysXPSysDest);

#include "NiPhysXPSysDest.inl"

#endif  // #ifndef NIPHYSXPSYSDEST_H
