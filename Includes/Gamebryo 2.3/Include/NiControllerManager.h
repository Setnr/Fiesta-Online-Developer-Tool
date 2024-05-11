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

#ifndef NICONTROLLERMANAGER_H
#define NICONTROLLERMANAGER_H

#include <NiTimeController.h>
#include "NiAnimationLibType.h"
#include <NiAVObject.h>
#include "NiControllerSequence.h"
#include "NiBlendTransformInterpolator.h"

class NiAVObjectPalette;
class NiQuatTransform;

class NIANIMATION_ENTRY NiControllerManager : public NiTimeController
{
public:
    NiDeclareRTTI;
    NiDeclareClone(NiControllerManager);
    NiDeclareStream;

    // Constructor and destructor.
    NiControllerManager(NiAVObject* pkTarget,
        bool bCumulativeAnimations = false);
    virtual ~NiControllerManager();

    // Virtual functions inherited from NiTimeController.
    virtual void Start(float fTime){}
    virtual void Start(){}
    virtual void Stop(){}
    virtual void SetTarget(NiObjectNET* pkTarget){}
    virtual void Update(float fTime);

    // Function to return the object palette.
    NiAVObjectPalette* GetObjectPalette() const;

    // Functions for adding sequences to the manager.
    NiControllerSequence* AddSequenceFromFile(const char* pcFilename,
        unsigned int uiIndex, const NiFixedString& kName = NULL);
    NiControllerSequence* AddSequenceFromFile(const NiStream& kStream,
        unsigned int uiIndex, const NiFixedString& kName = NULL);
    bool AddAllSequencesFromFile(const char* pcFilename);
    bool AddSequence(NiControllerSequence* pkSequence,
        const NiFixedString& kName = NULL, bool bStoreTargets = true);

    // Functions for removing sequences from the manager.
    NiControllerSequencePtr RemoveSequence(const NiFixedString& kName);
    NiControllerSequencePtr RemoveSequence(NiControllerSequence* pkSequence);
    void RemoveAllSequences();

    // Functions for retrieving sequences from the manager.
    unsigned int GetSequenceCount() const;
    NiControllerSequence* GetSequenceByName(const NiFixedString& pcName);
    NiControllerSequence* GetSequenceAt(unsigned int uiIndex);

    // Functions for activating sequences.
    bool ActivateSequence(const NiFixedString& kSequenceName,
        int iPriority = 0, bool bStartOver = false, float fWeight = 1.0f,
        float fEaseInTime = 0.0f, NiControllerSequence* pkTimeSyncSeq = NULL);
    bool ActivateSequence(NiControllerSequence* pkSequence,
        int iPriority = 0, bool bStartOver = false, float fWeight = 1.0f,
        float fEaseInTime = 0.0f, NiControllerSequence* pkTimeSyncSeq = NULL);

    // Functions for deactivating sequences.
    bool DeactivateSequence(const NiFixedString& kSequenceName,
        float fEaseOutTime = 0.0f);
    bool DeactivateSequence(NiControllerSequence* pkSequence,
        float fEaseOutTime = 0.0f);
    void DeactivateAll(float fEaseOutTime = 0.0f);

    // Functions for setting sequence weight.
    bool SetSequenceWeight(const NiFixedString& kSequenceName, float fWeight);
    bool SetSequenceWeight(NiControllerSequence* pkSequence, float fWeight);

    // Functions for cross-fading two sequences.
    bool CrossFade(const NiFixedString& kSourceSequenceName,
        const NiFixedString& kDestSequenceName, float fDuration,
        int iPriority = 0, bool bStartOver = false, float fWeight = 1.0f,
        const NiFixedString& kTimeSyncSeqName = NULL);
    bool CrossFade(NiControllerSequence* pkSourceSequence,
        NiControllerSequence* pkDestSequence, float fDuration,
        int iPriority = 0, bool bStartOver = false, float fWeight = 1.0f,
        NiControllerSequence* pkTimeSyncSeq = NULL);

    // Functions for performing blend transitions from one sequence to
    // another sequence.
    bool BlendFromSequence(const NiFixedString& kSourceSequenceName,
        const NiFixedString& kDestSequenceName, float fDuration,
        const NiFixedString& kDestTextKey, int iPriority = 0,
        float fSourceWeight = 1.0f, float fDestWeight = 1.0f,
        const NiFixedString& kTimeSyncSeqName = NULL);
    bool BlendFromSequence(const NiFixedString& kSourceSequenceName,
        const NiFixedString& kDestSequenceName, float fDuration,
        float fDestFrame = 0.0f, int iPriority = 0,
        float fSourceWeight = 1.0f, float fDestWeight = 1.0f,
        const NiFixedString& kTimeSyncSeqName = NULL);
    bool BlendFromSequence(NiControllerSequence* pkSourceSequence,
        NiControllerSequence* pkDestSequence, float fDuration,
        const NiFixedString& kDestTextKey, int iPriority = 0,
        float fSourceWeight = 1.0f, float fDestWeight = 1.0f,
        NiControllerSequence* pkTimeSyncSeq = NULL);
    bool BlendFromSequence(NiControllerSequence* pkSourceSequence,
        NiControllerSequence* pkDestSequence, float fDuration,
        float fDestFrame = 0.0f, int iPriority = 0,
        float fSourceWeight = 1.0f, float fDestWeight = 1.0f,
        NiControllerSequence* pkTimeSyncSeq = NULL);
    bool StopBlendFromSequence(const NiFixedString& kSourceSequenceName,
        const NiFixedString& kDestSequenceName, float fEaseOutTime = 0.0f);
    bool StopBlendFromSequence(NiControllerSequence* pkSourceSequence,
        NiControllerSequence* pkDestSequence, float fEaseOutTime = 0.0f);

    // Functions for performing morph transitions between two sequences.
    bool Morph(const NiFixedString& kSourceSequenceName,
        const NiFixedString& kDestSequenceName,
        float fDuration, int iPriority = 0,
        float fSourceWeight = 1.0f, float fDestWeight = 1.0f);
    bool Morph(NiControllerSequence* pkSourceSequence,
        NiControllerSequence* pkDestSequence, float fDuration,
        int iPriority = 0, float fSourceWeight = 1.0f,
        float fDestWeight = 1.0f);
    bool StopMorph(const NiFixedString& kSourceSequenceName,
        const NiFixedString& kDestSequenceName, float fEaseOutTime = 0.0f);
    bool StopMorph(NiControllerSequence* pkSourceSequence,
        NiControllerSequence* pkDestSequence, float fEaseOutTime = 0.0f);

    // Functions pertaining to transformation accumulation.
    bool GetCumulativeAnimations() const;
    NiAVObject* GetAccumRoot() const;
    void ClearCumulativeAnimations();
    bool GetAccumulatedTransform(NiQuatTransform& kTransform) const;
    void SetAccumulatedTransform(const NiQuatTransform& kTransform);

    // *** Begin deprecated functions ***
    // Functions for performing blend transitions from the current state of
    // the scene graph to a destination sequence. This function is the
    // equivalent of the old Blend function in previous Gamebryo versions.
    // When possible, BlendFromSequence should be used instead of this, as
    // BlendFromPose is not as efficient at runtime.
    bool BlendFromPose(const NiFixedString& kSequenceName, 
        const NiFixedString& kDestTextKey,
        float fDuration, int iPriority = 0,
        const NiFixedString& kSequenceToSynchronize = NULL);
    bool BlendFromPose(const NiFixedString& kSequenceName, float fDestFrame,
        float fDuration, int iPriority = 0,
        const NiFixedString& kSequenceToSynchronize = NULL);
    bool BlendFromPose(NiControllerSequence* pkSequence,
        const NiFixedString& kDestTextKey, float fDuration, int iPriority = 0,
        NiControllerSequence* pkSequenceToSynchronize = NULL);
    bool BlendFromPose(NiControllerSequence* pkSequence, float fDestFrame,
        float fDuration, int iPriority = 0,
        NiControllerSequence* pkSequenceToSynchronize = NULL);
    bool StopBlendFromPose(const NiFixedString& kSequenceName);
    bool StopBlendFromPose(NiControllerSequence* pkSequence);

    // Functions for performing sums between two animations. ActivateSequence
    // should instead be called on both sequences with the appropriate weight
    // values.
    bool Sum(const NiFixedString& kSourceSequenceName,
        const NiFixedString& kDestSequenceName,
        float fWeight, int iPriority = 0, float fEaseInTime = 0.0f);
    bool Sum(NiControllerSequence* pkSourceSequence,
        NiControllerSequence* pkDestSequence, float fWeight,
        int iPriority = 0, float fEaseInTime = 0.0f);
    bool SetSumWeight(const NiFixedString& kSourceSequenceName,
        const NiFixedString& kDestSequenceName, float fWeight);
    bool SetSumWeight(NiControllerSequence* pkSourceSequence,
        NiControllerSequence* pkDestSequence, float fWeight);
    bool StopSum(const NiFixedString& kSourceSequenceName,
        const NiFixedString& kDestSequenceName, float fEaseOutTime = 0.0f);
    bool StopSum(NiControllerSequence* pkSourceSequence,
        NiControllerSequence* pkDestSequence, float fEaseOutTime = 0.0f);
    // *** End deprecated functions ***

    // *** Begin Emergent internal use only ***
    // cloning support
    virtual void ProcessClone(
        NiCloningProcess& kCloning);

    // streaming support
    virtual void PostLinkObject(NiStream& kStream);
    // *** End Emergent internal use only ***

protected:
    // Default constructor for use in cloning and streaming only.
    NiControllerManager();

    // Virtual functions inherited from NiTimeController.
    virtual bool TargetIsRequiredType() const;

    NiControllerSequence* CreateTempBlendSequence(
        NiControllerSequence* pkSequence,
        NiControllerSequence* pkSequenceToSynchronize);

    NiControllerSequencePointerArray m_kSequenceArray;
    NiTStringPointerMap<unsigned int> m_kIndexMap;
    bool m_bCumulative;

    // Set of temporary sequences to support Blend.
    NiControllerSequencePointerSet m_kTempBlendSeqs;

    // NiAVObject palette for aiding in target lookup.
    NiAVObjectPalettePtr m_spObjectPalette;
};

NiSmartPointer(NiControllerManager);

#include "NiControllerManager.inl"

#endif  // #ifndef NICONTROLLERMANAGER_H
