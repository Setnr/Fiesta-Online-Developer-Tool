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

#ifndef NIPHYSXMESHPSYSDATA_H
#define NIPHYSXMESHPSYSDATA_H

#include "NiPhysXParticleLibType.h"

#include <NiMeshPSysData.h>
#include <NxPhysics.h>
#include <NiPhysX.h>

#include "NiPhysXParticleInfo.h"
#include "NiPhysXActorDesc.h"

class NIPHYSXPARTICLE_ENTRY NiPhysXMeshPSysData : public NiMeshPSysData
{
    NiDeclareClone(NiPhysXMeshPSysData);
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    // The arguments just get passed to the superclass
    NiPhysXMeshPSysData(unsigned short usMaxNumParticles, bool bHasColors,
        bool bHasRotations, unsigned int uiNumGenerations = 1,
        unsigned int uiMeshPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillMeshPoolsOnLoad = false, NiPhysXScene* pkScene = 0,
        unsigned int uiCompartmentID = 0,
        unsigned int uiActorPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillActorPoolsOnLoad = false, bool bKeepsMeshes = false);
    
    virtual ~NiPhysXMeshPSysData();

    // Managing the PhysX scene this system is attached to. The second
    // operation could be expensive.
    NiPhysXScenePtr GetScene() const;
    void SetScene(NiPhysXScene* pkScene);
    unsigned int GetCompartmentID() const;
    void SetCompartmentID(const unsigned int uiCompartmentID);

    NiPhysXParticleInfo* GetPhysXParticleInfo();
    const NiPhysXParticleInfo* GetPhysXParticleInfo() const;

    bool GetFillActorPoolsOnLoad();
    void SetFillActorPoolsOnLoad(bool bFill);

    void GetActorPoolRegion(NiPoint3& kCenter, NiPoint3& kDim);
    void SetActorPoolRegion(const NiPoint3 kCenter, const NiPoint3 kDim);

    bool GetKeepsActorMeshes();
    void SetKeepsActorMeshes(bool bKeep);

    unsigned int GetDefaultActorPoolsSize();
    void SetDefaultActorPoolsSize(unsigned int uiSize);

    // This class stores the prototypical actors
    NiPhysXActorDesc* GetActorDescAt(unsigned int uiIndex) const;
    NiPhysXActorDescPtr SetActorDescAt(unsigned int uiIndex,
        NiPhysXActorDesc* pkActor);
    unsigned int GetActorDescCount() const;

    // Removes a particle from the simulation.
    virtual void RemoveParticle(unsigned short usParticle);

    // Removes all particles and clears any new particles waiting to be added.
    void ClearAllParticles();
    
    // *** begin Emergent internal use only ***

    // A "conversion" constructor that turns a NiMeshPSysData object into
    // a new NiPhysXPSysData object.
    NiPhysXMeshPSysData(NiMeshPSysData* pkPSysData, NiPhysXScene* pkScene = 0, 
        unsigned int uiCompartmentID = 0, unsigned int uiNumGenerations = 1,
        unsigned int uiActorPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillActorPoolsOnLoad = false, bool bKeepsMeshes = false);
        
    void SetActorAt(unsigned short usIndex, NxActor* pkActor);

    virtual void SetActiveVertexCount(unsigned short usActive);

    unsigned int GetNumGenerations();
    void SetNumGenerations(unsigned int uiNumGenerations);

    int GetActorPoolSize(unsigned int uiGeneration);
    void SetActorPoolSize(unsigned int uiGeneration, unsigned int uiSize);

    int GetActorPoolCount(unsigned int uiGeneration);
    bool IsActorPoolEmpty(unsigned int uiGeneration);
    void EmptyActorPool(unsigned int uiGeneration);
    void FillActorPool(unsigned int uiGeneration, 
        NiPhysXActorDesc* pkOriginal);

    NxActor* RemoveActorFromPool(unsigned int uiGeneration);
    void AddActorToPool(unsigned int uiGeneration, NxActor* pkActor);

    void MapActorPools(NiTMap<NxActor*, bool>& kMap);

    void ProcessClone(NiCloningProcess& kCloning);
   
    void PostLinkObject(NiStream& kStream);

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPhysXMeshPSysData();
    
    // Fill in all the data upon construction
    void SetOnConstruction(unsigned short usMaxNumParticles,
        unsigned int uiNumGenerations, NiPhysXScene* pkScene,
        unsigned int uiCompartmentID, unsigned int uiActorPoolSize,
        bool bFillActorPoolsOnLoad, bool bKeepsMeshes);

    NiPhysXParticleInfo* m_pkPhysXParticleInfo;
    
    NiPhysXScenePtr m_spScene;
    unsigned int m_uiCompartmentID;

    bool m_bFillActorPoolsOnLoad;
    bool m_bKeepsActorMeshes;
    unsigned int m_uiDefaultActorPoolSize;
    typedef NiTPrimitiveArray<NxActor*> ActorArray;
    NiTPrimitiveArray< ActorArray*>  m_kActorPools;
    NiPoint3 m_kActorPoolDim;
    NiPoint3 m_kActorPoolCenter;

    NiTObjectArray<NiPhysXActorDescPtr> m_kActorDescriptors;
};

NiSmartPointer(NiPhysXMeshPSysData);

#include "NiPhysXMeshPSysData.inl"

#endif  // #ifndef NIPHYSXMESHPSYSDATA_H
