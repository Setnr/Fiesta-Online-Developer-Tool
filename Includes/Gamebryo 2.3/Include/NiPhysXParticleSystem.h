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

#ifndef NIPHYSXPARTICLESYSTEM_H
#define NIPHYSXPARTICLESYSTEM_H

#include "NiPhysXParticleLibType.h"

#include <NiParticleSystem.h>

#include "NiPhysXPSysData.h"
#include "NiPhysXPSysDest.h"
#include "NiPhysXPSysSrc.h"
#include "NiPhysXPSysProp.h"

class NIPHYSXPARTICLE_ENTRY NiPhysXParticleSystem : public NiParticleSystem
{
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXParticleSystem);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    // The constructed object is given ownership of the input arrays and
    // has the responsibility for deleting them when finished with them.
    NiPhysXParticleSystem(unsigned short usMaxNumParticles, bool bHasColors,
        bool bHasRotations, bool bWorldSpace = true,
        NiPhysXPSysProp* pkProp = 0, unsigned int uiCompartmentID = 0,
        bool bPhysXSpace = false,
        unsigned int uiActorPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillActorPoolsOnLoad = false, bool bKeepsMeshes = false);
    virtual ~NiPhysXParticleSystem();
    
    virtual void SetModelData(NiGeometryData* pkModelData);

    // Attaching to and removing from a prop.
    NiPhysXPSysPropPtr GetProp() const;
    void AttachToProp(NiPhysXPSysProp* pkProp);
    void DetachFromProp();

    // Managing the PhysX scene this system is attached to.
    NiPhysXScenePtr GetScene() const;
    void AttachToScene(NiPhysXScene* pkScene);
    void DetachFromScene(void);

    bool GetPhysXSpace() const;
    void SetPhysXSpace(const bool bPhysXSpace);

    // *** begin Emergent internal use only ***

    // Converts a NiParticleSystem into a PhysX particle system.
    NiPhysXParticleSystem(NiParticleSystem *pkPSys,
        NiPhysXPSysProp* pkProp = 0, unsigned int uiCompartmentID = 0,
        bool bPhysXSpace = false,
        unsigned int uiActorPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillActorPoolsOnLoad = false, bool bKeepsMeshes = false);
    
    void ProcessClone(NiCloningProcess& kCloning);

    virtual void PostLinkObject(NiStream& kStream);
    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For streaming and cloning only.
    NiPhysXParticleSystem();
    
    void PerformSystemReset();
    
    NiPhysXPSysPropPtr m_spProp;
    NiPhysXPSysDestPtr m_spDestination;
    NiPhysXPSysSrcPtr m_spSource;

    bool m_bPhysXSpace;
};

NiSmartPointer(NiPhysXParticleSystem);

#include "NiPhysXParticleSystem.inl"

#endif  // #ifndef NIPHYSXPARTICLESYSTEM_H
