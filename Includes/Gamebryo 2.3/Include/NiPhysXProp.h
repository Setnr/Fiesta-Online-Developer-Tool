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

#ifndef NIPHYSXPROP_H
#define NIPHYSXPROP_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include "NiPhysXDest.h"
#include "NiPhysXPropDesc.h"
#include "NiPhysXSrc.h"

#include <NxPhysics.h>

NiSmartPointer(NiPhysXPropDesc);
NiSmartPointer(NiPhysXProp);
class NiPhysXScene;

class NIPHYSX_ENTRY NiPhysXProp : public NiObjectNET
{
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXProp);

public:
    NiPhysXProp();
    ~NiPhysXProp();
    
    // Get the scene we are attached to. Cannot be set - that is done
    // auotmatically when the prop is attached to or detached from a scene.
    NiPhysXScene* GetScene();
    
    // Get and set the initial conditions transform for this prop.
    NxMat34 GetXform() const;
    void SetXform(const NxMat34& kXform);
    
    // Get and set the scale. World size is scale * physics size. The scale
    // factor on all props in a scene must be the same.
    float GetScaleFactor() const;
    void SetScaleFactor(const float fScale);
    
    // Functions called upon attachment and detachment from a scene
    virtual void AttachSceneCallback(NiPhysXScene* pkScene);
    virtual void DetachSceneCallback(NiPhysXScene* pkScene);

    // Interpolation control
    void SetAllSrcInterp(const bool bActive);
    void SetAllDestInterp(const bool bActive);
    
    // Access to the arrays of sources and destinations. After the prop
    // has been added to a scene, the pointer to a NiPhysXScene is required
    // to be sure that all pointers are added or released correctly.
    unsigned int GetSourcesCount() const;
    unsigned int AddSource(NiPhysXSrc* pkSrc);
    NiPhysXSrc* GetSourceAt(const unsigned int uiIndex);
    bool DeleteSource(NiPhysXSrc* pkTarget);
    void DeleteSourceAt(const unsigned int uiIndex);    
    unsigned int GetDestinationsCount() const;
    unsigned int AddDestination(NiPhysXDest* pkDest);
    NiPhysXDest* GetDestinationAt(const unsigned int uiIndex);
    bool DeleteDestination(NiPhysXDest* pkTarget);
    void DeleteDestinationAt(const unsigned int uiIndex);
    
    // Snapshots and reset
    NiPhysXPropDesc* GetSnapshot();
    void SetSnapshot(NiPhysXPropDesc* pkSnapshot);
    void ReleaseSnapshot();

    // Whether or not snapshots keep their mesh data around. You would keep
    // meshes if you planned on removing meshes from the NiPhysXManager and
    // recreating them later, because otherwise mesh data is shared and once
    // you have one copy, you never need another.
    bool GetKeepMeshes() const;
    void SetKeepMeshes(const bool bVal);
        
    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
            
protected:
    // The scene we are attached to, or NULL
    NiPhysXScene* m_pkScene;
    
    // The transform that takes this scene's PhysX coordinate system to the
    // master scene's coordinate system. Used in snapshots.
    NxMat34 m_kXform;
    
    // Scale factor
    float m_fScalePToW;

    // Lists of objects to update
    NiTObjectArray<NiPhysXSrcPtr> m_kSources;
    NiTObjectArray<NiPhysXDestPtr> m_kDestinations;
    
    // Snapshots
    NiPhysXPropDescPtr m_spSnapshot;
    bool m_bKeepMeshes;
};

#include "NiPhysXProp.inl"

#endif  // #ifndef NIPHYSXPROP_H

