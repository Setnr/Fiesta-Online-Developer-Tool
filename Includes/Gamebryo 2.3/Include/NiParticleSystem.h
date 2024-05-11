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

#ifndef NIPARTICLESYSTEM_H
#define NIPARTICLESYSTEM_H

#include "NiParticleLibType.h"
#include <NiParticles.h>
#include "NiPSysData.h"
#include "NiPSysModifier.h"
#include <NiTaskManager.h>

class NIPARTICLE_ENTRY NiParticleSystem : public NiParticles
{
    NiDeclareRTTI;
    NiDeclareClone(NiParticleSystem);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    // The constructed object is given ownership of the input arrays and
    // has the responsibility for deleting them when finished with them.
    NiParticleSystem(unsigned short usMaxNumParticles, bool bHasColors,
        bool bHasRotations, bool bWorldSpace = true);

    // The constructed object shares the input data.
    NiParticleSystem(NiPSysData* pkData, bool bWorldSpace = true);

    virtual void SetModelData(NiGeometryData* pkModelData);

    virtual void UpdateSystem(float fTime);
    virtual void Do_UpdateSystem(float fTime);
    void ResetParticleSystem();

    // Functions for adding, getting, and removing modifiers.
    void AddModifier(NiPSysModifier* pkModifier);
    void RemoveModifier(NiPSysModifier* pkModifier);
    void RemoveModifierByName(const NiFixedString& kName);
    NiPSysModifier* GetModifierByName(const NiFixedString& kName);
    unsigned int GetModifierCount() const;
    NiPSysModifier* GetModifierAt(unsigned int uiIndex);

    // Function for initializing new particles. NiPSysModifiers should
    // call this after they have added a new particle. This function calls
    // Initialize for each NiPSysModifier attached to this particle system.
    void InitializeNewParticle(unsigned short usNewParticle);

    // Get and set whether or not to compute particles in world space or
    // object space.
    bool GetWorldSpace() const;
    void SetWorldSpace(bool bWorldSpace);

    float GetLastTime() const;

    // Return number of particles data.
    unsigned short GetMaxNumParticles() const;
    unsigned short GetNumParticles() const;

    // This function returns the earliest begin key time for all of the
    // NiPSysModifierCtlr objects attached to all the particle systems
    // in the scene graph under pkScene. This value can be used to manually
    // run up the particle system.
    static float GetMinBeginKeyTime(NiAVObject* pkScene);

    static void SetParallelUpdateEnabled(bool bEnable);
    static bool GetParallelUpdateEnabled();

    static void SetParallelUpdatePriority(
        NiTaskManager::TaskPriority ePriority);
    static NiTaskManager::TaskPriority GetParallelUpdatePriority();

    // *** begin Emergent internal use only ***
    void SetDynamicBounds(bool bDynamicBounds);

    virtual void UpdateDownwardPass(float fTime, bool bUpdateControllers);
    virtual void UpdateSelectedDownwardPass(float fTime);
    virtual void UpdateRigidDownwardPass(float fTime);
    virtual void SetSelectiveUpdateFlags(bool& bSelectiveUpdate, 
        bool bSelectiveUpdateTransforms, bool& bRigid);

    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For streaming and cloning only.
    NiParticleSystem();

    static void FindMinBeginKeyTimeRecursive(NiAVObject* pkObject,
        float& fMinTime);

    virtual void UpdateWorldData();
    virtual void UpdateWorldBound();

    void PerformSystemReset();

    bool m_bWorldSpace;

    NiTPointerList<NiPSysModifierPtr> m_kModifierList;

    float m_fLastTime;
    bool m_bResetSystem;

    bool m_bDynamicBounds;
    NiTransform m_kUnmodifiedWorld;

    static bool ms_bParallelUpdateEnabled;
    static NiTaskManager::TaskPriority ms_eParallelUpdatePriority;
    
    // NiPhysX requires access to CopyMembers and ProcessClone for its
    // conversion functions
    friend class NiPhysXParticleSystem;
    friend class NiPhysXFluidSystem;
};

NiSmartPointer(NiParticleSystem);

#include "NiParticleSystem.inl"

#endif  // #ifndef NIPARTICLESYSTEM_H
