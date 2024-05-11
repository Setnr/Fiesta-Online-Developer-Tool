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


#ifndef NIPHYSXSEQUENCEMANAGER_H
#define NIPHYSXSEQUENCEMANAGER_H

#include "NiPhysXLibType.h"

#include <NiSystem.h>
#include <NiMain.h>
#include <NiAnimation.h>

#include <NxPhysics.h>

#include "NiPhysXDynamicSrc.h"
#include "NiPhysXInterpolatorDest.h"
#include "NiPhysXProp.h"
#include "NiPhysXTransformDest.h"

class NIPHYSX_ENTRY NiPhysXSequenceManager
    : public NiObject, public NiControllerSequence::CallbackObject
{
    NiDeclareRTTI;

public:
    NiPhysXSequenceManager();
    virtual ~NiPhysXSequenceManager();
    
    // Set the controller manager and prop we are working with.
    // The PhysX prop is also required because we need to add/remove
    // sources. This function adds sources for all of the actors that are
    // controlled by a sequence in the manager.
    bool SetManager(NiControllerManager* pkCM, NiPhysXProp* pkProp);
    
    // Access functions for the prop.
    NiPhysXProp* GetProp();
    
    // Add a sequence that will be key-frame controlled. SetManager must
    // have already been called before this function. This function does not
    // need to be called if the sequence was managed by the
    // NiControllerManager passed to SetManager.
    bool AddSequence(NiControllerSequence* pkSeq);
    
    // Remove a sequence from all consideration.
    bool RemoveSequence(NiControllerSequence* pkSeq);

    // Make a sequence physical. The sequence must already exist as
    // a keyframed sequence.
    bool MakeSequencePhysical(NiControllerSequence* pkSeq);

    // *** begin Emergent internal use only ***
    
    // *** end Emergent internal use only ***
        
protected:
    // Objects to hold one controller's data. An array of actor sources
    // and destinations, and activity and type information.
    class SequenceActorData : public NiMemObject
    {
    public:
        SequenceActorData();
        ~SequenceActorData();

        NiPhysXRigidBodyDestPtr m_spDestination;
        NiPhysXDynamicSrcPtr m_spSource;
    };
    typedef NiTPrimitiveArray<SequenceActorData*> ActorDataArray;

    class SequenceData : public NiMemObject
    {
    public:
        SequenceData();
        ~SequenceData();
        
        // Destinations are strictly per-sequence
        ActorDataArray m_kActorData;
        
        // Strictly per-sequence
        bool m_bIsActive;
    };

    // Clean out all the maps.
    void CleanMaps();
    
    // Find all the objects controlled by transform destinations.
    // Results are put in m_kOriginalDestMap
    void FindTransformDests(NiPhysXProp* pkProp);

    // Activation change callback
    void ActivationChanged(NiControllerSequence* pkSeq,
        NiControllerSequence::AnimState eState);

    // Deactivate/Activate sources.
    bool SetActivity(SequenceData* pkSeqData, bool bActive);

    // The PhysX Prop that is managing the PhysX objects.
    NiPhysXPropPtr m_spProp;
    
    // A map of all the destinations in the prop
    NiTMap<NiAVObject*, SequenceActorData*> m_kOriginalDestMap;
    
    // A map, indexed by sequence, of the sequence information. This includes
    // the destinations and sources used for the sequence, and whether it
    // is physical or not.
    NiTMap<NiControllerSequence*, SequenceData*> m_kSequences;
    
    // A map, keyed on actors, containing the number of active physics
    // sequences for that actor. Actors with no active physical sequences
    // are set to kinematic.
    NiTMap<NxActor*, unsigned short> m_kActorActiveCount;
};

NiSmartPointer(NiPhysXSequenceManager);

#include "NiPhysXSequenceManager.inl"

#endif // #ifdef NIPHYSXSEQUENCEMANAGER_H

