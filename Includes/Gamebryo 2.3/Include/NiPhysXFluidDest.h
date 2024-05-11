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

#ifndef NIPHYSXFLUIDDEST_H
#define NIPHYSXFLUIDDEST_H

#include "NiPhysXFluidLibType.h"

#include <NiParticleSystem.h>

#include <NxPhysics.h>
#include <fluids/NxFluid.h>

#include "NiPhysXDest.h"

class NiPhysXFluidSystem;

class NIPHYSXFLUID_ENTRY NiPhysXFluidDest : public NiPhysXDest
{
    NiDeclareRTTI;
    NiDeclareStream;

public:
    NiPhysXFluidDest(NiParticleSystem* pkTarget);
    ~NiPhysXFluidDest();

    NiParticleSystem* GetTarget();
    NiTransform& GetPhysXToFluid();

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
    NiPhysXFluidDest();
    
    NiParticleSystem* m_pkTarget;
    NiTransform m_kPhysXToFluid; // Cached for emission of particles
    
    // Update management stuff
    float m_fScalePToW;
    bool m_bUpdated;
};

NiSmartPointer(NiPhysXFluidDest);

#include "NiPhysXFluidDest.inl"

#endif  // #ifndef NIPHYSXFLUIDDEST_H
