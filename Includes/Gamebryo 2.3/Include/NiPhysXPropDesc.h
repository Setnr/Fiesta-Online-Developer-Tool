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


#ifndef NIPHYSXPROPDESC_H
#define NIPHYSXPROPDESC_H

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXActorDesc.h"
#include "NiPhysXBodyDesc.h"
#include "NiPhysXClothDesc.h"
#include "NiPhysXD6JointDesc.h"
#include "NiPhysXMaterialDesc.h"
#include "NiPhysXShapeDesc.h"

#include "NiPhysXProp.h"

class NIPHYSX_ENTRY NiPhysXPropDesc : public NiObject
{
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXPropDesc);

public:
    NiPhysXPropDesc(unsigned int uiMaterialHashSize = 7);
    virtual ~NiPhysXPropDesc();

    // The compartment we will be in, using Gamebryo's ID
    unsigned int GetRBCompartmentID() const;
    void SetRBCompartmentID(const unsigned int uiID);
    unsigned int GetClothCompartmentID() const;
    void SetClothCompartmentID(const unsigned int uiID);
    unsigned int GetFluidCompartmentID() const;
    void SetFluidCompartmentID(const unsigned int uiID);
    
    // Access to the actor list
    unsigned int GetActorCount() const;
    NiPhysXActorDescPtr GetActorAt(unsigned int uiIndex);
    unsigned int AddActor(NiPhysXActorDesc* pkActorDesc);
    NiPhysXActorDescPtr RemoveActorAt(unsigned int uiIndex);

    // Access to the joint list
    unsigned int GetJointCount() const;
    NiPhysXJointDescPtr GetJointAt(unsigned int uiIndex);
    unsigned int AddJoint(NiPhysXJointDesc* pkJointDesc);
    NiPhysXJointDescPtr RemoveJointAt(unsigned int uiIndex);

    // Access to the cloth list
    unsigned int GetClothCount() const;
    NiPhysXClothDescPtr GetClothAt(unsigned int uiIndex);
    unsigned int AddCloth(NiPhysXClothDesc* pkClothDesc);
    NiPhysXClothDescPtr RemoveClothAt(unsigned int uiIndex);

    // Access to the material map
    NxMaterialIndex AddMaterial(NxMaterialDesc& kMaterial,
        const NxMaterialIndex usBaseIndex = 0);
    NiPhysXMaterialDescPtr SetMaterialAt(NxMaterialDesc& kMaterial,
        const NxMaterialIndex usIndex);
    NiPhysXMaterialDescPtr GetMaterial(const NxMaterialIndex usIndex);
    NiTMapIterator GetFirstMaterial();
    void GetNextMaterial(NiTMapIterator& kPos, NxMaterialIndex& usIndex,
        NiPhysXMaterialDescPtr& spMaterial);
    bool RemoveMaterial(const NxMaterialIndex usIndex);
        
    // *** begin Emergent internal use ***
    
    // Creates a PhysX scene based on this descriptor. If pkExistingScene
    // is non-null, the actors, joints and materials in this descriptor are
    // added into the existing scene.
    virtual bool CreateInScene(NxScene* pkScene, const NxMat34& kXform,
        NxCompartment* pkRBCompartment, NxCompartment* pkClothCompartment,
        NxCompartment* pkFluidCompartment,
        const bool bKeepMeshes = false, const unsigned int uiIndex = 0);
    
    // Removes any actors, joints and materials that this descriptor knows
    // about from the given PhysX scene.
    virtual void RemoveFromScene(NxScene* pkScene);

    void SetNumStates(const unsigned int uiNumStates);

    // Adds a new set of restore state from the scene, and returns the
    // index of the new state.
    virtual unsigned int AddState(NxScene* pkPhysXScene, 
        const NxMat34& kXform);

    // Updates the restore state from the given scene. The index tells which
    // of the stored states to update.
    virtual void UpdateState(NxScene* pkPhysXScene, const unsigned int uiIndex,
        const NxMat34& kXform);

    // Restores state to the given scene. The index tells which
    // of the stored states to restore from.
    virtual void RestoreState(NxScene* pkScene, const unsigned int uiIndex,
        const NxMat34& kXform, const bool bKeepMeshes);

    unsigned int GetNumStates() const;
    
    // Non-default streaming functions
    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);

    // *** end Emergent internal use ***
            
protected:
    // Updates the actor pointers in NiPhysXSrc and NiPhysXDest objects.
    virtual void PushToSrcDest() const;

    // Components of a scene
    NiTObjectArray<NiPhysXActorDescPtr> m_kActors;
    NiTObjectArray<NiPhysXJointDescPtr> m_kJoints;
    NiTObjectArray<NiPhysXClothDescPtr> m_kClothes;
    NiTMap<NxMaterialIndex, NiPhysXMaterialDescPtr> m_kMaterials;
    
    // The compartment to create this prop in
    unsigned int m_uiRBCompartmentID;
    unsigned int m_uiClothCompartmentID;
    unsigned int m_uiFluidCompartmentID;
    
    // The number of states we have for restoration
    unsigned int m_uiNumStates;
};

#include "NiPhysXPropDesc.inl"

#endif // NIPHYSXPROPDESC_H
