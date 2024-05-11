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

#ifndef NIPHYSXMESHFLUIDSYSTEM_H
#define NIPHYSXMESHFLUIDSYSTEM_H

#include "NiPhysXFluidLibType.h"

#include <NiMeshParticleSystem.h>
#include <fluids/NxFluid.h>

#include "NiPhysXFluidDest.h"

class NIPHYSXFLUID_ENTRY NiPhysXMeshFluidSystem : public NiMeshParticleSystem
{
    NiDeclareRTTI;
    NiDeclareStream;

public:
    // The constructed object is given ownership of the input arrays and
    // has the responsibility for deleting them when finished with them.
    NiPhysXMeshFluidSystem(NxParticleData& kFluidData,
        unsigned short usMaxNumParticles, bool bHasColors,
        bool bHasRotations, bool bWorldSpace = true, 
        unsigned int uiPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillPoolsOnLoad = false, NiPhysXProp* pkProp = 0,
        bool bPhysXSpace = false);

    virtual void SetModelData(NiGeometryData* pkModelData,
        NiPhysXProp* pkProp);

    virtual void SetModelData(NiGeometryData* pkModelData);

    // Managing the scene we are attached to
    void AttachToProp(NiPhysXProp* pkProp);
    void DetachFromProp(NiPhysXProp* pkProp);

    bool GetPhysXSpace() const;
    void SetPhysXSpace(const bool bPhysXSpace);

    // *** begin Emergent internal use only ***

    // Constructor that works from an existing Gamebryo PSys
    NiPhysXMeshFluidSystem(NiMeshParticleSystem *pkPSys,
        NiPhysXProp* pkProp, bool bPhysXSpace,
        NiTMap<NiAVObject*, NiPhysXActorDescPtr>& kActorMap);
    
    // The transformation to take PhysX space to the particle system space.
    NiTransform& GetPhysXToFluid();

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For streaming and cloning only.
    NiPhysXMeshFluidSystem();
    
    NiPhysXFluidDestPtr m_spDestination;

    bool m_bPhysXSpace;
};

NiSmartPointer(NiPhysXMeshFluidSystem);

#include "NiPhysXMeshFluidSystem.inl"

#endif  // #ifndef NIPHYSXMESHFLUIDSYSTEM_H
