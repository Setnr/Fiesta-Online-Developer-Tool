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

#ifndef NIPHYSXMESHPARTICLEPROP_H
#define NIPHYSXMESHPARTICLEPROP_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include "NiPhysXParticle.h"
#include "NiPhysXProp.h"

class NiPhysXMeshParticleSystem;

class NIPHYSXPARTICLE_ENTRY NiPhysXMeshPSysProp : public NiPhysXProp
{
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXMeshPSysProp);

public:
    NiPhysXMeshPSysProp();
    ~NiPhysXMeshPSysProp();
    
    // Access and info about systems
    unsigned int GetSystemCount() const;
    NiPhysXMeshParticleSystem* GetSystemAt(unsigned int uiIndex);
       
    // *** begin Emergent internal use only ***

    // Functions called upon attachment and detachment from a scene
    virtual void AttachSceneCallback(NiPhysXScene* pkScene);
    virtual void DetachSceneCallback(NiPhysXScene* pkScene);
       
    // Should only be called via NiPhysXMeshParticleSystem::AttachToProp
    void AddSystem(NiPhysXMeshParticleSystem* pkSystem);
    void RemoveSystem(NiPhysXMeshParticleSystem* pkSystem);

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
            
protected:
    NiTPrimitiveArray<NiPhysXMeshParticleSystem*> m_kSystems;
};

NiSmartPointer(NiPhysXMeshPSysProp);

#endif  // #ifndef NIPHYSXMESHPARTICLEPROP_H

