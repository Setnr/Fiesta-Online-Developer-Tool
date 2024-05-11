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

#ifndef NIPHYSXFLUIDSYSTEM_H
#define NIPHYSXFLUIDSYSTEM_H

#include "NiPhysXFluidLibType.h"

#include <NiParticleSystem.h>
#include <fluids/NxFluid.h>

#include "NiPhysXFluidDest.h"
#include "NiPhysXFluidDesc.h"

class NIPHYSXFLUID_ENTRY NiPhysXFluidSystem : public NiParticleSystem
{
    NiDeclareRTTI;
    NiDeclareStream;

public:
    // The constructed object is given ownership of the input arrays and
    // has the responsibility for deleting them when finished with them.
    NiPhysXFluidSystem(NxParticleData& pkFluidData,
        unsigned short usMaxNumParticles, bool bHasColors,
        bool bHasRotations, bool bWorldSpace = true, NiPhysXProp* pkProp = 0,
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

    // Constructor that converts from a Gamebryo PSys    
    NiPhysXFluidSystem(NiParticleSystem *pkPSys,
        NiPhysXProp* pkProp, bool bPhysXSpace,
        NiTMap<NiAVObject*, NiPhysXActorDescPtr>& kActorMap);

    // The transformation to take PhysX space to the particle system space.
    NiTransform& GetPhysXToFluid();
    
    // Used by both kinds of fluid systems to convert from regular PSys.
    static void ConvertModifiers(NiParticleSystem* pkTarget,
        NiPhysXProp* pkProp, NiPhysXFluidDescPtr spFluidDesc,
        NiTMap<NiAVObject*, NiPhysXActorDescPtr>& kActorMap);

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For streaming and cloning only.
    NiPhysXFluidSystem();
    
    static void ConvertEmitter(NiParticleSystem* pkTarget,
        NiPSysEmitter* pkEmitter, float fInvScale,
        NiTMap<NiAVObject*, NiPhysXActorDescPtr>& kActorMap,
        NiTObjectArray<NiPhysXFluidEmitterDescPtr>& kEmitters);
    
    NiPhysXFluidDestPtr m_spDestination;

    bool m_bPhysXSpace;
};

NiSmartPointer(NiPhysXFluidSystem);

#include "NiPhysXFluidSystem.inl"

#endif  // #ifndef NIPHYSXFLUIDSYSTEM_H
