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

#ifndef NIPHYSXSCENE_H
#define NIPHYSXSCENE_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include "NiPhysXDest.h"
#include "NiPhysXProp.h"
#include "NiPhysXSrc.h"

#include <NxPhysics.h>

class NiPhysXManager;

NiSmartPointer(NiPhysXSceneDesc);
NiSmartPointer(NiPhysXScene);

class NIPHYSX_ENTRY NiPhysXScene : public NiObjectNET
{
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXScene);

public:
    NiPhysXScene();
    ~NiPhysXScene();

    // Get and set the PhysX scene. It is an error to set the scene
    // when a scene has already been set. Release the old scene first.
    // The SetPhysXScene function DOES NOT cause existing props to be
    // created in the scene. Use the snapshot in each prop to explicitly
    // create the props.
    NxScene* GetPhysXScene() const;
    void SetPhysXScene(NxScene* pkScene);
    void ReleaseScene();
    
    // Get and set the global tranform for the PhysX scene we are part of.
    const NiTransform& GetSceneXform() const;
    void SetSceneXform(const NiTransform& kXform);
    
    // Get and set the scale. World size is scale * physics size. All
    // props attached to the scene must match this scale.
    float GetScaleFactor() const;
    void SetScaleFactor(const float fScale);

    // Prop management
    unsigned int GetPropCount() const;
    unsigned int AddProp(NiPhysXProp* pkProp, unsigned int uiState = 0);
    NiPhysXProp* GetPropAt(const unsigned int uiIndex);
    void RemoveProp(NiPhysXProp* pkTarget);
    void RemovePropAt(const unsigned int uiIndex);
    
    // Compartment management. Can't remove compartments because PhysX
    // doesn't allow it. Compartments are removed if the scene is reset.
    NxCompartment* GetCompartmentAt(unsigned int uiID);
    bool AddCompartment(unsigned int uiID, NxCompartment* pkCompartment);
    bool AddCompartment(unsigned int uiID, NxCompartmentDesc& pkDescriptor);

    // Update control
    bool GetUpdateSrc() const;
    void SetUpdateSrc(const bool bActive);
    bool GetUpdateDest() const;
    void SetUpdateDest(const bool bActive);

    // Time step control. This behavior changed in Gamebryo 2.3, so that now
    // this NiPhysXScene completely controls timing: Any settings on the
    // NxScene object will be destoryed as this class does its work.
    bool GetDoFixedStep() const;
    void SetDoFixedStep(const bool bActive);
    float GetTimestep() const;
    void SetTimestep(const float fStep);
    unsigned int GetNumSubSteps() const;
    void SetNumSubSteps(const unsigned int uiNumSteps);
    unsigned int GetMaxSubSteps() const;
    void SetMaxSubSteps(const unsigned int uiMaxSteps);
    
    // Debug visualization
    bool GetDebugRender() const;
    void SetDebugRender(const bool bActive, NiNode* pkDebugParent = 0);
    void ClearDebugGeometry();
    
    // Simulation and results fetching.
    bool GetInSimFetch() const;
    bool Simulate(const float fTargetTime, const bool bForceSim = false);
    bool FetchResults(const float fTargetTime, const bool bBlock = false,
        NxU32* puiErrorState = 0);
    void SetFetchTime(const float fCurrentTime);
    float GetPrevFetchTime() const;
    float GetNextFetchTime() const; // Only valid if simulating
    
    // Update functions. Times are Gamebryo application times
    void UpdateSources(const float fTime, bool bForce = false);
    void UpdateDestinations(const float fTime, bool bForce = false);
    void UpdateSource(NiPhysXSrc* pkSrc, const float fTime,
        const bool bForce = false);
    void UpdateDestination(NiPhysXDest* pkDest, const float fTime,
        const bool bForce = false);
    
    // Snapshots. If you create from snapshot, all props are created
    // in the scene too.
    NiPhysXSceneDesc* GetSnapshot();
    void SetSnapshot(NiPhysXSceneDesc* pkSnapshot);
    bool CreateSceneFromSnapshot(const unsigned int uiIndex = 0);
    void ReleaseSnapshot();

    // These functions add a snapshot state for each prop.
    unsigned int AddSnapshotState();
    void UpdateSnapshotState(const unsigned int uiIndex);
    void RestoreSnapshotState(const unsigned int uiIndex);
        
    // *** begin Emergent internal use only ***
    
    // Sources and destinations. These functions should never be called
    // by the application. All source and destination operations should
    // use the functions in NiPhysXProp.
    void AddSource(NiPhysXSrc* pkSrc);
    void DeleteSource(NiPhysXSrc* pkTarget);
    void AddDestination(NiPhysXDest* pkDest);
    void DeleteDestination(NiPhysXDest* pkTarget);

    // The node is skippable when streaming
    virtual void PostLinkObject(NiStream& kStream);
    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
            
protected:
    // Update
    void UpdateSourcesToActors(const float fTBegin, const float fTEnd);
    void UpdateActorsToDests(const bool bForce);

    // Debug geometry
    void SetDebugGeometry(const NxDebugRenderable* pkRenderState);
    void SetDebugTriangleGeometry(const NxDebugRenderable* pkRenderState);
    void SetDebugLineGeometry(const NxDebugRenderable* pkRenderState);
    void SetDebugPointGeometry(const NxDebugRenderable* pkRenderState);

    // Back compatability for streaming
    void LoadOldBinary(NiStream& kStream);

    // A local pointer to the manager
    NiPhysXManager* m_pkManager;

    // The transform from Gamebryo world coordinates to the PhysX world
    // coordinates. Only valid if not slaved, otherwise the master's
    // transform is used, with the m_kSlavedXform used when caching and
    // restoring state.
    NiTransform m_kSceneXform;
    
    // Scale factor
    float m_fScalePToW;

    // PhysX scene object
    NxScene* m_pkPhysXScene;
    
    // Compartments
    NiTPointerMap<unsigned int, NxCompartment*> m_kCompartmentMap;

    // Props
    NiTObjectArray<NiPhysXPropPtr> m_kProps;

    // Lists of objects to update
    NiTObjectArray<NiPhysXSrcPtr> m_kSources;
    NiTObjectArray<NiPhysXDestPtr> m_kDestinations;
   
    // Management of time for PhysX simulation
    float m_fTimestep; // Only used if fixed timestep
    unsigned int m_uiNumSubSteps; // Num PhysX steps per GB step
    unsigned int m_uiMaxSubSteps; // Max num PhysX steps

    float m_fPrevFetchTime; // The time of the most recent FetchResults.
    float m_fNextFetchTime; // The time of any upcoming FecthResults.
    
    // Protect the simulate/fetchResult pair.
    bool m_bInSimFetch;

    // Debug rendering
    NiNodePtr m_spDebugRoot;
    NiTPrimitiveArray<unsigned short> m_kDebugGeomSizes;
    
    // Snapshots
    NiPhysXSceneDescPtr m_spSnapshot;
    
    // Flags
    enum {
        // Control of simulation and update
        UPDATE_SRC_MASK         = 0x0002,
        UPDATE_DEST_MASK        = 0x0004,
        
        // Time step control
        FIXED_TIMESTEP_MASK     = 0x0010,
        
        // Debug rendering enabled
        DEBUG_RENDER_MASK       = 0x0400
    };
    
    NiDeclareFlags(unsigned short);
    
    //
    // For back compatable streaming only!!! Remove when we drop support
    // for streaming versions prior to Gamebryo 2.3.
    //
    bool m_bKeepMeshesBackCompatable;
};

#include "NiPhysXScene.inl"

#endif  // #ifndef NIPHYSXSCENE_H

