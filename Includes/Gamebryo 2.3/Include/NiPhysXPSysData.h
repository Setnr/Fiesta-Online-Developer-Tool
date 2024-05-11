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

#ifndef NIPHYSXPSYSDATA_H
#define NIPHYSXPSYSDATA_H

#include "NiPhysXParticleLibType.h"

#include <NiMeshPSysData.h>
#include <NxPhysics.h>
#include <NiPhysX.h>

#include "NiPhysXParticleInfo.h"
#include "NiPhysXActorDesc.h"

class NIPHYSXPARTICLE_ENTRY NiPhysXPSysData : public NiPSysData
{
    NiDeclareClone(NiPhysXPSysData);
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    // The arguments just get passed to the superclass
    NiPhysXPSysData(unsigned short usMaxNumParticles, bool bHasColors,
        bool bHasRotations, NiPhysXScene* pkScene = 0,
        unsigned int uiCompartmentID = 0,
        unsigned int uiActorPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillActorPoolOnLoad = false, bool bKeepsMeshes = false);

    virtual ~NiPhysXPSysData();

    // Managing the PhysX scene this system is attached to. The second
    // operation could be expensive.
    NiPhysXScenePtr GetScene() const;
    void SetScene(NiPhysXScene* pkScene);
    unsigned int GetCompartmentID() const;
    void SetCompartmentID(const unsigned int uiCompartmentID);

    NiPhysXParticleInfo* GetPhysXParticleInfo();
    const NiPhysXParticleInfo* GetPhysXParticleInfo() const;

    bool GetFillActorPoolOnLoad();
    void SetFillActorPoolOnLoad(bool bFill);

    void GetActorPoolRegion(NiPoint3& kCenter, NiPoint3& kDim);
    void SetActorPoolRegion(const NiPoint3 kCenter, const NiPoint3 kDim);

    bool GetKeepsActorMeshes();
    void SetKeepsActorMeshes(bool bKeep);

    unsigned int GetDefaultActorPoolSize();
    void SetDefaultActorPoolSize(unsigned int uiSize);

    // This class stores the prototypical actors
    NiPhysXActorDesc* GetActorDesc() const;
    NiPhysXActorDescPtr SetActorDesc(NiPhysXActorDesc* pkActor);

    // Removes a particle from the simulation.
    virtual void RemoveParticle(unsigned short usParticle);

    // Removes all particles and clears any new particles waiting to be added.
    void ClearAllParticles();
    
    // *** begin Emergent internal use only ***

    // A conversion constructor
    NiPhysXPSysData(NiPSysData* pkPSysData, NiPhysXScene* pkScene = 0,
        unsigned int uiCompartmentID = 0,
        unsigned int uiActorPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillActorPoolOnLoad = false, bool bKeepsMeshes = false);
    
    void SetActorAt(unsigned short usIndex, NxActor* pkActor);

    virtual void SetActiveVertexCount(unsigned short usActive);

    int GetActorPoolSize();
    void SetActorPoolSize(unsigned int uiSize);

    int GetActorPoolCount();
    bool IsActorPoolEmpty();
    void EmptyActorPool();
    void FillActorPool(NiPhysXActorDesc* pkOriginal);

    NxActor* RemoveActorFromPool();
    void AddActorToPool(NxActor* pkActor);

    void MapActorPool(NiTMap<NxActor*, bool>& kMap);

    void ProcessClone(NiCloningProcess& kCloning);

    void PostLinkObject(NiStream& kStream);

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPhysXPSysData();
    
    void SetOnConstruction(unsigned short usMaxNumParticles,
        NiPhysXScene* pkScene, unsigned int uiCompartmentID,
        unsigned int uiActorPoolSize,
        bool bFillActorPoolOnLoad, bool bKeepsMeshes);

    NiPhysXParticleInfo* m_pkPhysXParticleInfo;
    
    NiPhysXScenePtr m_spScene;
    unsigned int m_uiCompartmentID;

    bool m_bFillActorPoolOnLoad;
    bool m_bKeepsActorMeshes;
    unsigned int m_uiDefaultActorPoolSize;
    NiTPrimitiveArray<NxActor*> m_kActorPool;
    NiPoint3 m_kActorPoolCenter;
    NiPoint3 m_kActorPoolDim;

    NiPhysXActorDescPtr m_spActorDescriptor;
};

NiSmartPointer(NiPhysXPSysData);

#include "NiPhysXPSysData.inl"

#endif  // #ifndef NIPHYSXPSYSDATA_H
