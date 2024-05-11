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


#ifndef NIPHYSXCLOTHDESC_H
#define NIPHYSXCLOTHDESC_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXActorDesc.h"
#include "NiPhysXClothDest.h"
#include "NiPhysXMeshDesc.h"
#include "NiPhysXShapeDesc.h"

class NIPHYSX_ENTRY NiPhysXClothDesc : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXClothDesc);

public:
    // Class for attachments
    class NIPHYSX_ENTRY ClothAttachment : public NiMemObject
    {
    public:
        ClothAttachment();
        ~ClothAttachment();
        
        // The shape to which the attachment is made, or NULL if it is
        // a global attachment.
        NiPhysXShapeDescPtr m_spShape;
        
        // The number of vertices that are attached or 0 if
        // it is to be a “collision” type attachment.
        unsigned int m_uiVertexCount;
        
        // The vertices that are attached.
        unsigned int* m_puiVertexIDs;
        
        // The locations of the attachment on the shape or in the world.
        NxVec3* m_pkPositions;
        
        // Attachment flags.
        NxU32* m_puiFlags;
    };

    // Constructor/Destructor
    NiPhysXClothDesc();
    virtual ~NiPhysXClothDesc();
    
    // Get/Set parameters
    NiFixedString GetClothName() const;
    void SetClothName(NiFixedString& kClothName);
    NiPhysXMeshDescPtr GetMeshDesc();
    void SetMeshDesc(NiPhysXMeshDesc* pkMeshDesc);
    NxMat34 GetGlobalPose() const;
    void SetGlobalPose(const NxMat34& kPose);
    NxReal GetThickness() const;
    void SetThickness(const NxReal fThickness);
    NxReal GetDensity() const;
    void SetDensity(const NxReal fDensity);
    NxReal GetBendingStiffness() const;
    void SetBendingStiffness(const NxReal fStiffness);
    NxReal GetStretchingStiffness() const;
    void SetStretchingStiffness(const NxReal fStiffness);
    NxReal GetDampingCoefficient() const;
    void SetDampingCoefficient(const NxReal fCoefficient);
    NxReal GetFriction() const;
    void SetFriction(const NxReal fFriction);
    NxReal GetPressure() const;
    void SetPressure(const NxReal fPressure);
    NxReal GetTearFactor() const;
    void SetTearFactor(const NxReal fTearFactor);
    NxReal GetCollisionResponseCoefficient() const;
    void SetCollisionResponseCoefficient(const NxReal fCoefficient);
    NxReal GetAttachmentResponseCoefficient() const;
    void SetAttachmentResponseCoefficient(const NxReal fCoefficient);
    NxReal GetAttachmentTearFactor() const;
    void SetAttachmentTearFactor(const NxReal fFactor);
    NxU32 GetSolverIterations() const;
    void SetSolverIterations(const NxU32 uiIterations);
    NxVec3 GetExternalAcceleration() const;
    void SetExternalAcceleration(const NxVec3 kAcceleration);
    NxReal GetWakeUpCounter() const;
    void SetWakeUpCounter(const NxReal fCounter);
    NxReal GetSleepLinearVelocity() const;
    void SetSleepLinearVelocity(const NxReal fVelocity);
    NxCollisionGroup GetCollisionGroup() const;
    void SetCollisionGroup(const NxCollisionGroup uiGroup);
    NxGroupsMask GetCollisionMask() const;
    void SetCollisionMask(const NxGroupsMask kMask);
    NxU32 GetClothFlags() const;
    void SetClothFlags(const NxU32 uiFlags);
    
    void SetParameters(NiFixedString& kClothName, NiPhysXMeshDesc* pkMeshDesc,
        const NxMat34& kPose, const NxReal fThickness, const NxReal fDensity,
        const NxReal fBendingStiffness, const NxReal fStretchingStiffness,
        const NxReal fDampingCoefficient, const NxReal fFriction,
        const NxReal fPressure, const NxReal fTearFactor,
        const NxReal fCollisionResponseCoefficient,
        const NxReal fAttachmentResponseCoefficient,
        const NxReal fAttachmentTearFactor, const NxU32 uiSolverIterations,
        const NxVec3 kExternalAcceleration, const NxReal fWakeUpCounter,
        const NxReal fSleepLinearVelocity,
        const NxCollisionGroup uiCollisionGroup,
        const NxGroupsMask kCollisionMask, const NxU32 uiFlags);

    NxCloth* GetCloth() const;
    void SetCloth(NxCloth* pkCloth);
    
    NiPhysXActorDesc* GetParentActor() const;
    void SetParentActor(NiPhysXActorDesc* pkActorDesc); 
       
    NiPhysXClothDest* GetDest() const;
    void SetDest(NiPhysXClothDest* pkDest);

    //
    // Attachment functions
    //
    // Get the number of attachments for this cloth.
    unsigned int GetAttachmentCount() const;

    // Get a pointer to the attachment at a given index. Could return NULL.
    ClothAttachment* GetAttachmentAt(const unsigned int uiIndex);
    
    // Add an attachment for this cloth. This function will add the
    // attachment in PhysX if the scene has already been created. The cloth
    // descriptor ons the memory for the passed object and will free it.
    // The object passed in shoudl have been allocated with NiNew.
    void AddAttachment(ClothAttachment* pkAttachment);
    
    // Remove an attachment of this cloth. This will remove the attachment
    // in the scene if it has already been created. Returns true if the
    // attachment was found and removed.
    bool RemoveAttachment(ClothAttachment* pkAttachment);

    // Remove an attachment of this cloth at a specified index.
    // This will remove the attachment in the scene if it has already
    // been created. Returns true if there was an attachment at the
    // given index.
    bool RemoveAttachmentAt(const unsigned int uiIndex);

    // Creation/removal in PhysX scene
    virtual bool IsValid();
    virtual NxCloth* CreateCloth(NxScene* pkScene,
        NxCompartment* pkCompartment, const NxMat34& kXform,
        const bool bKeepMeshes = false);
    virtual void RemoveCloth(NxScene* pkScene);

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
        
protected:
    virtual bool ToClothDesc(NxClothDesc& kClothDesc, const NxMat34& kNxMat34,
        const bool bKeepMeshes, const bool bCheckPlatform = true);
    
    // Parameters from NxClothDesc class
    NiFixedString m_kClothName;
    NiPhysXMeshDescPtr m_spClothMesh;
    NxMat34 m_kGlobalPose;
    NxReal m_fThickness;
    NxReal m_fDensity;
    NxReal m_fBendingStiffness;
    NxReal m_fStretchingStiffness;
    NxReal m_fDampingCoefficient;
    NxReal m_fFriction;
    NxReal m_fPressure;
    NxReal m_fTearFactor;
    NxReal m_fCollisionResponseCoefficient;
    NxReal m_fAttachmentResponseCoefficient;
    NxReal m_fAttachmentTearFactor;
    NxU32 m_uiSolverIterations;
    NxVec3 m_kExternalAcceleration;
    NxReal m_fWakeUpCounter;
    NxReal m_fSleepLinearVelocity;
    NxCollisionGroup m_uiCollisionGroup;
    NxU32 m_auiCollisionBits[4];
    NxU32 m_uiFlags;
    
    NiTPrimitiveArray<ClothAttachment*> m_kAttachments;
    
    NxCloth* m_pkCloth;
    NiPhysXActorDesc* m_pkParentActor;
    
    NiPhysXClothDestPtr m_spDest;
};

NiSmartPointer(NiPhysXClothDesc);

#include "NiPhysXClothDesc.inl"

#endif // #ifdef NIPHYSXCLOTHDESC_H

