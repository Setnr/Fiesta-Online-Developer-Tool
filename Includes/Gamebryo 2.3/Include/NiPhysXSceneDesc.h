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


#ifndef NIPHYSXSCENEDESC_H
#define NIPHYSXSCENEDESC_H

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXActorDesc.h"
#include "NiPhysXBodyDesc.h"
#include "NiPhysXD6JointDesc.h"
#include "NiPhysXMaterialDesc.h"
#include "NiPhysXShapeDesc.h"

#include "NiPhysXScene.h"

class NIPHYSX_ENTRY NiPhysXSceneDesc : public NiObject
{
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXSceneDesc);

public:
    NiPhysXSceneDesc();
    virtual ~NiPhysXSceneDesc();

    // Obtain all the data one can find in a PhysX scene descriptor.
    virtual NxSceneDesc& GetNxSceneDesc();
    virtual void SetNxSceneDesc(const NxSceneDesc& kSceneDesc);
    
    // Set things not in a NxSceneDesc
    void SetGroupCollisionFlag(const unsigned char ucGroup1,
        const unsigned char ucGroup2, const bool bVal);
    void SetFilterOps(NxFilterOp eOp0, NxFilterOp eOp1, NxFilterOp eOp2); 
    void SetFilterBool(bool bFlag);
    void SetFilterConstant0(const NxGroupsMask &kMask);
    void SetFilterConstant1(const NxGroupsMask &kMask);
    
    // Get functions for collision flags
    bool GetGroupCollisionFlag(const unsigned char ucGroup1,
        const unsigned char ucGroup2) const;
    void GetFilterOps(
        NxFilterOp& eOp0, NxFilterOp& eOp1, NxFilterOp& eOp2) const; 
    bool GetFilterBool() const;
    void GetFilterConstant0(NxGroupsMask &kMask) const;
    void GetFilterConstant1(NxGroupsMask &kMask) const;
    
    // Access to compartments. Can edit this reference to set up compartments.
    NiTMap<unsigned int, NxCompartmentDesc*>& GetCompartmentMap();

    // Creates a PhysX scene based on this descriptor.
    virtual NxScene* CreateScene();
        
    // *** begin Emergent internal use ***
    
    // Non-default streaming functions
    virtual bool StreamCanSkip();

    //
    // These functions for back-compatable streaming ONLY.
    //
    // Access to the actor list
    unsigned int GetActorCountBackCompatable() const;
    NiPhysXActorDescPtr GetActorAtBackCompatable(unsigned int uiIndex);

    // Access to the joint list
    unsigned int GetJointCountBackCompatable() const;
    NiPhysXJointDescPtr GetJointAtBackCompatable(unsigned int uiIndex);

    // Access to the material map
    NiTMapIterator GetFirstMaterialBackCompatable();
    void GetNextMaterialBackCompatable(NiTMapIterator& kPos,
        NxMaterialIndex& usIndex, NiPhysXMaterialDescPtr& spMaterial);

    unsigned int GetNumStatesBackCompatable() const;

    void ClearBackCompatable();

    // *** end Emergent internal use ***
            
protected:
    //
    // For streaming of deprecated PhysX member variables.
    //
    enum NiSceneDescNxBroadPhaseType
    {
        NX_BROADPHASE_QUADRATIC,
        NX_BROADPHASE_FULL,
        NX_BROADPHASE_COHERENT,

        NX_BROADPHASE_FORCE_DWORD = 0x7fffffff
    };
    enum NiSceneDescNxHwPipelineSpec
    {
        NX_HW_RB_PIPELINE_HLP_ONLY  = 0,
        NX_HW_PIPELINE_FULL         = 1,
        NX_HW_PIPELINE_DEBUG        = 2,

        NX_PSP_FORCE_DWORD = 0x7fffffff
    };
    enum NiSceneDescNxHwSceneType
    {
        NX_HW_SCENE_TYPE_RB             = 0,
        NX_HW_SCENE_TYPE_FLUID          = 1,
        NX_HW_SCENE_TYPE_FLUID_SOFTWARE = 2,
        NX_HW_SCENE_TYPE_CLOTH          = 3,

        NX_SCT_FORCE_DWORD = 0x7fffffff
    };

    // Most data is in a PhysX scene descriptor
    NxSceneDesc m_kNxSceneDesc;
    
    // These things are queryable from a scene, but not in a descriptor.
    bool m_abGroupCollisionFlags[32][32];
    NxFilterOp m_aeFilterOps[3];
    bool m_bFilterBool;
    NxGroupsMask m_auiFilterConstants[2];
    
    // These things are because we need to stream them ourselves.
    NiTMap<unsigned int, NxCompartmentDesc*> m_kCompartmentMap;

    // Components of a scene, from an old incarnation. These things are now
    // stored in NiPhysXPropDesc objects, and should not be here. We keep
    // them only to support streaming of prior versions. Remove when we
    // drop support for streaming versions prior to Gamebryo 2.3.
    NiTObjectArray<NiPhysXActorDescPtr> m_kActorsBackCompatable;
    NiTObjectArray<NiPhysXJointDescPtr> m_kJointsBackCompatable;
    NiTMap<NxMaterialIndex, NiPhysXMaterialDescPtr> m_kMaterialsBackCompatable;
    unsigned int m_uiNumStatesBackCompatable;
};

#include "NiPhysXSceneDesc.inl"

#endif // NIPHYSXSCENEDESC_H
