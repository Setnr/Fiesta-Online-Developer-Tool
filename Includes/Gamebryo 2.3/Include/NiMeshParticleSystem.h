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

#ifndef NIMESHPARTICLESYSTEM_H
#define NIMESHPARTICLESYSTEM_H

#include "NiParticleSystem.h"
#include <NiNode.h>
#include "NiMeshPSysData.h"


class NIPARTICLE_ENTRY NiMeshParticleSystem : public NiParticleSystem
{
    NiDeclareRTTI;
    NiDeclareClone(NiMeshParticleSystem);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiMeshParticleSystem(unsigned short usMaxNumParticles, bool bHasColors,
        bool bHasRotations, bool bWorldSpace = true,
        unsigned int uiPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        unsigned int uiNumGenerations = 1, bool bFillPoolsOnLoad = false);

    NiMeshParticleSystem(NiMeshPSysData* pkData, bool bWorldSpace = true);

    virtual void SetModelData(NiGeometryData* pkModelData);

    // *** begin Emergent internal use only ***

    virtual void UpdateDownwardPass(float fTime, bool bUpdateControllers);
    virtual void UpdateSelectedDownwardPass(float fTime);
    virtual void UpdateRigidDownwardPass(float fTime);
    virtual void UpdatePropertiesDownward(NiPropertyState* pkParentState);
    virtual void UpdateEffectsDownward(NiDynamicEffectState* pkParentState);

    // culling (separate from display)
    virtual void OnVisible(NiCullingProcess& kCuller);

    virtual void SetSelectiveUpdateFlags(bool& bSelectiveUpdate, 
        bool bSelectiveUpdateTransforms, bool& bRigid);

    // renderer data
    virtual void PurgeRendererData(NiRenderer* pkRenderer);

    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiMeshParticleSystem();

    virtual void UpdateWorldData();
    virtual void UpdateWorldBound();

    float m_fTime;
    bool m_bUpdateControllers;
    
    // NiPhysX requires access to CopyMembers and ProcessClone for its
    // conversion functions
    friend class NiPhysXMeshParticleSystem;
    friend class NiPhysXMeshFluidSystem;
};

NiSmartPointer(NiMeshParticleSystem);

#endif  // #ifndef NIMESHPARTICLESYSTEM_H
