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


#ifndef NIPHYSXACTORDESC_H
#define NIPHYSXACTORDESC_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXBodyDesc.h"
#include "NiPhysXShapeDesc.h"
#include "NiPhysXRigidBodyDest.h"
#include "NiPhysXRigidBodySrc.h"

class NIPHYSX_ENTRY NiPhysXActorDesc : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXActorDesc);

public:
    NiPhysXActorDesc();
    virtual ~NiPhysXActorDesc();
    
    void SetConstants(const NiFixedString& kActorName,
        const NxReal fDensity, const NxU32 uiActorFlags,
        const NxActorGroup uiActorGroup, NiPhysXBodyDesc* pkBodyDesc,
        const NiTObjectArray<NiPhysXShapeDescPtr>& kActorShapes);

    NxMat34 GetPose(const unsigned int uiIndex) const;
    void SetPose(const NxMat34& kPose, const unsigned int uiIndex);

    NiFixedString GetActorName() const;
    void SetActorName(NiFixedString& kActorName);
    NxReal GetDensity() const;
    void SetDensity(const NxReal fDensity);
    NxU32 GetActorFlags() const;
    void SetActorFlags(const NxU32 uiActorFlags);
    NxActorGroup GetActorGroup() const;
    void SetActorGroup(const NxActorGroup uiActorGroup);
    NiPhysXBodyDesc* GetBodyDesc();
    void SetBodyDesc(NiPhysXBodyDesc* pkBodyDesc);
    NiTObjectArray<NiPhysXShapeDescPtr>& GetActorShapes();
    void SetActorShapes(
        const NiTObjectArray<NiPhysXShapeDescPtr>& kActorShapes);
        
    virtual bool IsValid();

    virtual NxActor* CreateActor(NxScene* pkScene,
        NxCompartment* pkCompartment, const unsigned int uiIndex,
        const NxMat34& kXform, const bool bKeepMeshes = false);
    virtual void RemoveActor(NxScene* pkScene);

    virtual unsigned int AddState(const NxMat34& kXform);
    virtual void UpdateState(const unsigned int uiIndex,
        const NxMat34& kXform);
    virtual void RestoreState(const unsigned int uiIndex,
        const NxMat34& kXform);

    NxActor* GetActor() const;
    void SetActor(NxActor* pkActor);
    
    NiPhysXActorDesc* GetActorParent() const;
    void SetActorParent(NiPhysXActorDesc* pkActorDesc); 
       
    NiPhysXRigidBodySrcPtr GetSource() const;
    void SetSource(NiPhysXRigidBodySrcPtr spSource);
    
    NiPhysXRigidBodyDestPtr GetDest() const;
    void SetDest(NiPhysXRigidBodyDestPtr spDest);

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
        
protected:
    virtual void ToActorDesc(NxActorDesc& kActorDesc,
        NxCompartment* pkCompartment, const unsigned int uiIndex,
        const NxMat34& kNxMat34, const bool bKeepMeshes,
        const bool bCheckPlatform = true);
    
    // Constant through the life of an actor
    NiFixedString m_kActorName;
    NxReal m_fDensity;
    NxU32 m_uiActorFlags;
    NxActorGroup m_uiActorGroup;
    unsigned int m_uiCompartmentID;
    NiPhysXBodyDescPtr m_spBodyDesc;
    NiTObjectArray<NiPhysXShapeDescPtr> m_kActorShapes;
    
    // An array of poses
    unsigned int m_uiNumPoses;
    NxMat34* m_pkPoses;

    // Pointers to the PhysX objects.
    NxActor* m_pkActor;
    NiPhysXActorDesc* m_pkActorParent;
    
    // Source and or destination for this actor. Needed so that their
    // internal actor pointers can be reset after streaming or cloning.
    NiPhysXRigidBodySrcPtr m_spSource;
    NiPhysXRigidBodyDestPtr m_spDest;

    // Temporary for use while working with PhysX descriptors.
    // Should always be freed within the same operation that creates it.
    NxBodyDesc* m_pkBodyDesc;
};

NiSmartPointer(NiPhysXActorDesc);

#include "NiPhysXActorDesc.inl"

#endif // #ifdef NIPHYSXACTORDESC_H

