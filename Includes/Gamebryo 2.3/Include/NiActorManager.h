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

#ifndef NIACTORMANAGER_H
#define NIACTORMANAGER_H

#include <NiRefObject.h>
#include <NiNode.h>
#include <NiTObjectList.h>
#include "NiKFMTool.h"
#include "NiControllerManager.h"
#include "NiBoneLODController.h"
#include "NiTextKeyMatch.h"

NiSmartPointer(NiTextKeyMatch);

class NIANIMATION_ENTRY NiActorManager : public NiRefObject
{
public:
    typedef unsigned int SequenceID;

    // Deprecated typedef.
    typedef SequenceID EventCode;

    class NIANIMATION_ENTRY CallbackObject
    {
    public:
        virtual ~CallbackObject();
        virtual void AnimActivated(NiActorManager* pkManager,
            SequenceID eSequenceID, float fCurrentTime, float fEventTime) = 0;
        virtual void AnimDeactivated(NiActorManager* pkManager,
            SequenceID eSequenceID, float fCurrentTime, float fEventTime) = 0;
        virtual void TextKeyEvent(NiActorManager* pkManager,
            SequenceID eSequenceID, const NiFixedString& kTextKey,
            const NiTextKeyMatch* pkMatchObject,
            float fCurrentTime, float fEventTime) = 0;
        virtual void EndOfSequence(NiActorManager* pkManager,
            SequenceID eSequenceID, float fCurrentTime, float fEventTime) = 0;
    };

    enum EventType
    {
        ANIM_ACTIVATED,
        ANIM_DEACTIVATED,
        TEXT_KEY_EVENT,
        END_OF_SEQUENCE
    };

    enum TransitionState
    {
        NO_TRANSITION,
        BLENDING,
        MORPHING,
        CROSSFADING
    };

    // Static constants.
    static const SequenceID ANY_SEQUENCE_ID;
    static const SequenceID INVALID_SEQUENCE_ID;
    static const float INVALID_TIME;
    static const NiFixedString& GetDefaultEndTextKey();
    static const NiFixedString& GetDefaultStartTextKey();
    
    // Deprecated static constants.
    static const SequenceID ANY_EVENT_CODE;
    static const SequenceID INVALID_EVENT_CODE;

    // Static creation functions.
    static NiActorManager* Create(const char* pcKFMFilename,
        bool bCumulativeAnimations = false, bool bLoadFilesFromDisk = true,
        NiStream* pkStream = NULL);
    static NiActorManager* Create(NiKFMTool* pkKFMTool,
        const char* pcKFMFilePath, bool bCumulativeAnimations = false,
        bool bLoadFilesFromDisk = true, NiStream* pkStream = NULL);

    // Destructor.
    ~NiActorManager();

    // Cloning function. Creates a clone of the NiActorManager and all of
    // its contents.
    NiActorManager* Clone(NiCloningProcess* pkCloningProcess = NULL);

    // Reloads the NIF file using the info in the NiKFMTool class.
    // If bLoadNIFFile is false, the NIF root will be loaded directly from
    // pkStream without loading a NIF file. pkStream cannot be NULL if
    // bLoadNIFFile is false.
    bool ReloadNIFFile(NiStream* pkStream = NULL, bool bLoadNIFFile = true);

    // Changes the NIF root and actor root that are being used for the
    // NiActorManager. This function does the same thing as ReloadNIFFile but
    // does not require the root objects to reside in an NiStream object or
    // to be loaded from disk.
    bool ChangeNIFRoot(NiAVObject* pkNIFRoot);

    // Loads the specified sequence using the info in the NiKFMTool class.
    // If bLoadKFFile is false, the sequence will be loaded directly from
    // pkStream without loading a KF file. pkStream cannot be NULL if
    // bLoadKFFile is false.
    bool LoadSequence(SequenceID eSequenceID, bool bLoadKFFile = true,
        NiStream* pkStream = NULL);
    
    // Loads all the sequences in the NiKFMTool class.
    // If pkStream is NULL this function creates and deletes its own
    // stream, otherwise it uses the given stream.
    bool LoadSequences(NiStream* pkStream = NULL);

    // Changes the NiControllerSequence that is used for the specified
    // SequenceID. This function does the same thing as LoadSequence but does
    // not require the NiControllerSequence object to reside in an NiStream
    // object or to be loaded from disk.
    bool ChangeSequence(SequenceID eSequenceID,
        NiControllerSequence* pkSequence);

    // Unloads the specified sequence from the actor manager, removing it
    // from the NiControllerManager in the process. If there is no other
    // smart pointer referencing this sequence, it will be deleted.
    void UnloadSequence(SequenceID eSequenceID);

    // Update function. Must be called with the current time periodically to
    // update the state machine.
    void Update(float fTime);

    // Functions for setting/getting the next target animation.
    SequenceID GetTargetAnimation() const;
    bool SetTargetAnimation(SequenceID eSequenceID);
    void Reset();

    // Functions for getting the current state of the actor manager.
    SequenceID GetCurAnimation() const;
    TransitionState GetTransitionState() const;
    SequenceID GetNextAnimation() const;

    // Functions for setting/getting the callback object.
    void SetCallbackObject(CallbackObject* pkCallbackObject);
    CallbackObject* GetCallbackObject() const;

    // Gets the root of the NIF file that was loaded.
    NiAVObject* GetNIFRoot() const;

    // Gets the actor root (the target of the NiControllerManager).
    NiAVObject* GetActorRoot() const;

    // Accessor functions.
    NiKFMTool* GetKFMTool() const;
    NiControllerManager* GetControllerManager() const;
    NiBoneLODController* GetBoneLODController() const;
    NiControllerSequence* GetSequence(SequenceID eSequenceID) const;

    // Gets the accumulation root of the actor, if there is one.
    NiAVObject* GetAccumRoot() const;

    // Finds the SequenceID for the first found sequence with a given
    // name.  Returns INVALID_SEQUENCE_ID if the name is not found.
    // pcName must be non-NULL.
    SequenceID FindSequenceID(const char* pcName) const;

    // Functions for registering and unregistering callback events to occur
    // within some advance notification time.
    bool RegisterCallback(EventType eEventType, SequenceID eSequenceID,
        const NiFixedString& kTextKey);
    void UnregisterCallback(EventType eEventType, SequenceID eSequenceID,
        const NiFixedString& kTextKey);
    bool RegisterCallback(EventType eEventType, SequenceID eSequenceID,
        NiTextKeyMatch* pkMatchObject = NULL );
    void UnregisterCallback(EventType eEventType, SequenceID eSequenceID,
        NiTextKeyMatch* pkMatchObject = NULL );
    void ClearAllRegisteredCallbacks();

    // Gets the time of the next event. If the next time cannot be predicted,
    // INVALID_TIME is returned.
    float GetNextEventTime(EventType eEventType, SequenceID eSequenceID,
        const NiFixedString& kTextKey);
    float GetNextEventTime(EventType eEventType, SequenceID eSequenceID,
        NiTextKeyMatch * pkMatchObject = NULL );

    // Activate a sequence separately from the internal state machine. This
    // is useful for layering animations on top of a base animation that is
    // controlled by the state machine. Callbacks are available for these
    // animations.
    bool ActivateSequence(SequenceID eSequenceID, int iPriority = 0,
        bool bStartOver = false, float fWeight = 1.0f,
        float fEaseInTime = 0.0f,
        SequenceID eTimeSyncSeqID = NiKFMTool::SYNC_SEQUENCE_ID_NONE);
    bool DeactivateSequence(SequenceID eSequenceID,
        float fEaseOutTime = 0.0f);

    // Refreshes the NiActorManager's internal pointer to its
    // NiControllerManager. This function is useful if you have
    // programmatically changed the NiControllerManager that the actor 
    // manager is using and need the NiActorManager to reflect that change.
    void RefreshControllerManager();

    // Causes the timeline of callback events to be rebuilt during the next
    // update.
    void RebuildTimeline();

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();

    NiKFMTool::KFM_RC ChangeSequenceID(SequenceID eOldID, SequenceID eNewID);

    class ChainCompletionInfo : public NiRefObject
    {
    public:
        ChainCompletionInfo();
        ~ChainCompletionInfo();

        void SetName(const NiFixedString& kName);
        void SetNextName(const NiFixedString& kNextName);

        float m_fSeqStart;
        float m_fTransStart;
        float m_fTransEnd;
        float m_fInSeqBeginFrame;
        NiFixedString m_kName;
        NiFixedString m_kNextName;
        NiKFMTool::Transition* m_pkTransition;
        unsigned int m_uiSrcID;
        unsigned int m_uiDesID;
    };
    typedef NiPointer<ChainCompletionInfo> ChainCompletionInfoPtr;
    typedef NiTObjectSet<ChainCompletionInfoPtr> ChainCompletionInfoPtrSet;

    class CompletionInfo : public NiRefObject
    {
    public:
        float m_fFrameInDestWhenTransitionCompletes;
        float m_fTimeToCompleteTransition;
        float m_fFrameTransitionOccursInSrc;
        float m_fTimeForChainToComplete;
        NiKFMTool::Transition::BlendPair* m_pkBlendPair;
        ChainCompletionInfoPtrSet m_kChainCompletionInfoSet;
    };
    typedef NiPointer<CompletionInfo> CompletionInfoPtr;

    bool FindTimeForAnimationToCompleteTransition(
        unsigned int uiTransSrcID, unsigned int uiTransDesID,
        NiKFMTool::Transition* pkTransition, float fBeginSQTime,
        float fDesiredTransitionSQTime, CompletionInfo* pkInfoOut);
    
    protected:
    ChainCompletionInfo* FillChainComplete( unsigned int uiSrcID,
        unsigned int uiDesID, float fDuration, 
        float& fSrcBeginFrame, float& fTotalTime);

    // *** end Emergent internal use only ***

protected:
    static NiFixedString ms_kEndTextKey;
    static NiFixedString ms_kStartTextKey;

    class CallbackData : public NiRefObject
    {
    public:
        CallbackData(EventType eEventType, SequenceID eSequenceID,
            NiTextKeyMatch * pkMatchObject = NULL);
        ~CallbackData();

        EventType m_eEventType;
        SequenceID m_eSequenceID;

        NiTextKeyMatchPtr GetMatchObject( ) const;
        void SetMatchObject( NiTextKeyMatch * pkMatchObject );

    protected:
        NiTextKeyMatchPtr m_spMatchObject;
    };
    typedef NiPointer<CallbackData> CallbackDataPtr;

public:
    class TimelineData : public NiMemObject
    {
    public:
        TimelineData(){};
        ~TimelineData();

        void SetValues(float fEventTime, EventType eEventType,
            SequenceID eSequenceID, NiTextKeyMatch* pkMatchObject,
             const NiFixedString& kTextKeyMatched);

        float m_fEventTime;
        EventType m_eEventType;
        SequenceID m_eSequenceID;
        NiTextKeyMatchPtr m_spMatchObject;
        NiFixedString m_kTextKeyMatched;
    };

protected:
    NiActorManager(NiKFMTool* pkKFMTool, bool bCumulativeAnimations);

    bool Initialize(NiStream& kStream);
    bool LoadNIFFile(NiStream& kStream, bool bLoadNIFFile);
    bool AddSequence(SequenceID eSequenceID, NiStream& kStream,
        bool bLoadKFFile);

    NiControllerManager* FindControllerManager(NiAVObject* pkObject);
    NiBoneLODController* FindBoneLODController(NiAVObject* pkObject);

    float GetNextAnimActivatedTime(SequenceID eSequenceID);
    float GetNextAnimDeactivatedTime(SequenceID eSequenceID);
    float GetNextTextKeyEventTime(SequenceID eSequenceID,
        NiTextKeyMatch* pkMatchObject);
    float GetNextTextKeyEventTime(SequenceID eSequenceID,
        const NiFixedString& kTextKey);

    float GetNextEndOfSequenceTime(SequenceID eSequenceID);

    void BuildTimeline();
    void AddTimelineDataObject(EventType eEventType, SequenceID eSequenceID,
        NiTextKeyMatch* pkMatchObject);
    void RemoveTimelineDataObjects(EventType eEventType,
        SequenceID eSequenceID, NiTextKeyMatch* pkMatchObject);
    void ProcessCallbacks();
    void RaiseAnimActivatedEvents(SequenceID eEventSeqID);
    void RaiseAnimDeactivatedEvents(SequenceID eEventSeqID);

    NiKFMTool::Transition::BlendPair* GetNextBlendPair(
        unsigned int uiTransSrcID, unsigned int uiTransDesID,
        NiKFMTool::Transition* pkTransition, float fFrameTime,
        float& fActualUnboundedFrame);

    void RemoveInactiveExtraSequences();

    NiKFMToolPtr m_spKFMTool;
    NiAVObjectPtr m_spNIFRoot;
    NiControllerManagerPtr m_spManager;

    NiBoneLODControllerPtr m_spBoneLOD;
    CallbackObject* m_pkCallbackObject;

    SequenceID m_eTargetID;
    float m_fTime;
    float m_fTimelineRefreshTime;
    bool m_bBuildTimeline;
    
    bool m_bStartTransition;
    TransitionState m_eTransitionState;
    float m_fTransStartTime;
    float m_fTransEndTime;
    float m_fTransStartFrameTime;
    const char* m_pcTargetKey;
    SequenceID m_eCurID;
    NiControllerSequence* m_pkCurSequence;
    SequenceID m_eNextID;
    NiControllerSequence* m_pkNextSequence;
    float m_fNextChainTime;
    NiTPointerList<unsigned int> m_kChainIDs;
    NiTPointerList<float> m_kChainDurations;
    bool m_bChainActive;

    bool m_bCumulativeAnimations;

    NiTPointerMap<SequenceID, NiControllerSequence*> m_kSequenceMap;

    NiTObjectArray<CallbackDataPtr> m_kCallbacks;
    NiTObjectList<TimelineData> m_kTimeline;

    typedef NiTPrimitiveSet<SequenceID> SequenceIDSet;
    SequenceIDSet m_kExtraSequences;
};

NiSmartPointer(NiActorManager);

#include "NiActorManager.inl"

#endif  // #ifndef NIACTORMANAGER_H
