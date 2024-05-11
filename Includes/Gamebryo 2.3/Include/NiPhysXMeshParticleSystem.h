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

#ifndef NIPHYSXMESHPARTICLESYSTEM_H
#define NIPHYSXMESHPARTICLESYSTEM_H

#include "NiPhysXParticleLibType.h"

#include <NiMeshParticleSystem.h>

#include "NiPhysXMeshPSysData.h"
#include "NiPhysXPSysDest.h"
#include "NiPhysXMeshPSysSrc.h"
#include "NiPhysXMeshPSysProp.h"

class NIPHYSXPARTICLE_ENTRY NiPhysXMeshParticleSystem :
    public NiMeshParticleSystem
{
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXMeshParticleSystem);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPhysXMeshParticleSystem(unsigned short usMaxNumParticles,
        bool bHasColors, bool bHasRotations, bool bWorldSpace = true,
        unsigned int uiNumGenerations = 1,
        unsigned int uiMeshPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillMeshPoolsOnLoad = false,
        NiPhysXMeshPSysProp* pkProp = 0, unsigned int uiCompartmentID = 0, 
        bool bPhysXSpace = false, bool bPhysXRotations = false,
        unsigned int uiActorPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillActorPoolsOnLoad = false, bool bKeepsMeshes = false);
    
    virtual ~NiPhysXMeshParticleSystem();
    
    virtual void SetModelData(NiGeometryData* pkModelData);

    // Managing the PhysX prop this system is attached to.
    NiPhysXMeshPSysPropPtr GetProp() const;
    void AttachToProp(NiPhysXMeshPSysProp* pkProp);
    void DetachFromProp();

    // Managing the PhysX scene this system is attached to.
    NiPhysXScenePtr GetScene() const;
    void AttachToScene(NiPhysXScene* pkScene);
    void DetachFromScene(void);
    
    bool GetUsesPhysXRotations() const;
    void SetUsesPhysXRotations(const bool bPhysXRotations);

    bool GetPhysXSpace() const;
    void SetPhysXSpace(const bool bPhysXSpace);
    
    // *** begin Emergent internal use only ***

    // Conversion from a NiMeshParticleSystem
    NiPhysXMeshParticleSystem(NiMeshParticleSystem* pkPSys,
        NiPhysXMeshPSysProp* pkProp = 0, unsigned int uiCompartmentID = 0,
        bool bPhysXSpace = false, bool bPhysXRotations = true,
        unsigned int uiActorPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillActorPoolsOnLoad = false, bool bKeepsMeshes = false);
    
    void ProcessClone(NiCloningProcess& kCloning);

    virtual void PostLinkObject(NiStream& kStream);
    virtual bool StreamCanSkip();

    // *** end Emergent internal use only ***
    
protected:
    // For streaming and cloning only.
    NiPhysXMeshParticleSystem();

    void PerformSystemReset();
    
    NiPhysXMeshPSysPropPtr m_spProp;
    NiPhysXPSysDestPtr m_spDestination;
    NiPhysXMeshPSysSrcPtr m_spSource;

    bool m_bPhysXRotations;
    bool m_bPhysXSpace;
};

NiSmartPointer(NiPhysXMeshParticleSystem);

#include "NiPhysXMeshParticleSystem.inl"

#endif  // #ifndef NIPHYSXMESHPARTICLESYSTEM_H
