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

//---------------------------------------------------------------------------
inline const NiFixedString& NiActorManager::GetDefaultEndTextKey()
{
    return ms_kEndTextKey;
}
//---------------------------------------------------------------------------
inline const NiFixedString& NiActorManager::GetDefaultStartTextKey()
{
    return ms_kStartTextKey;
}
//---------------------------------------------------------------------------
inline void NiActorManager::SetCallbackObject(
    CallbackObject* pkCallbackObject)
{
    m_pkCallbackObject = pkCallbackObject;
}
//---------------------------------------------------------------------------
inline NiActorManager::CallbackObject* NiActorManager::GetCallbackObject()
    const
{
    return m_pkCallbackObject;
}
//---------------------------------------------------------------------------
inline NiActorManager::SequenceID NiActorManager::GetTargetAnimation() const
{
    return m_eTargetID;
}
//---------------------------------------------------------------------------
inline bool NiActorManager::SetTargetAnimation(SequenceID eSequenceID)
{
    if (m_eTargetID == eSequenceID)
    {
        return true;
    }

    if (eSequenceID != INVALID_SEQUENCE_ID)
    {
        NiControllerSequence* pkSequence;
        if (!m_kSequenceMap.GetAt(eSequenceID, pkSequence))
        {
            return false;
        }
    }

    m_eTargetID = eSequenceID;
    m_bBuildTimeline = true;

    return true;
}
//---------------------------------------------------------------------------
inline void NiActorManager::Reset()
{
    if (m_spManager)
    {
        m_spManager->DeactivateAll();
    }
    m_eTargetID = INVALID_SEQUENCE_ID;
    m_fTime = INVALID_TIME;
    m_bBuildTimeline = false;
    m_fTimelineRefreshTime = NI_INFINITY;
    m_eTransitionState = NO_TRANSITION;
    m_bStartTransition = false;
    m_fTransStartTime = NI_INFINITY;
    m_fTransEndTime = -NI_INFINITY;
    m_fTransStartFrameTime = INVALID_TIME;
    m_pcTargetKey = NULL;
    m_eCurID = INVALID_SEQUENCE_ID;
    m_pkCurSequence = NULL;
    m_eNextID = INVALID_SEQUENCE_ID;
    m_pkNextSequence = NULL;
    m_bChainActive = false;
    m_fNextChainTime = INVALID_TIME;
    m_kChainIDs.RemoveAll();
    m_kChainDurations.RemoveAll();
    m_kExtraSequences.RemoveAll();
    m_kTimeline.RemoveAll();
}
//---------------------------------------------------------------------------
inline NiActorManager::SequenceID NiActorManager::GetCurAnimation() const
{
    return m_eCurID;
}
//---------------------------------------------------------------------------
inline NiActorManager::TransitionState NiActorManager::GetTransitionState()
    const
{
    return m_eTransitionState;
}
//---------------------------------------------------------------------------
inline NiActorManager::SequenceID NiActorManager::GetNextAnimation() const
{
    return m_eNextID;
}
//---------------------------------------------------------------------------
inline NiAVObject* NiActorManager::GetNIFRoot() const
{
    return m_spNIFRoot;
}
//---------------------------------------------------------------------------
inline NiAVObject* NiActorManager::GetActorRoot() const
{
    NIASSERT(m_spManager && NiIsKindOf(NiAVObject, m_spManager->GetTarget()));
    return (NiAVObject*) m_spManager->GetTarget();
}
//---------------------------------------------------------------------------
inline NiKFMTool* NiActorManager::GetKFMTool() const
{
    return m_spKFMTool;
}
//---------------------------------------------------------------------------
inline NiControllerManager* NiActorManager::GetControllerManager() const
{
    return m_spManager;
}
//---------------------------------------------------------------------------
inline NiBoneLODController* NiActorManager::GetBoneLODController() const
{
    return m_spBoneLOD;
}
//---------------------------------------------------------------------------
inline NiControllerSequence* NiActorManager::GetSequence(
    SequenceID eSequenceID) const
{
    NiControllerSequence* pkSequence;
    if (m_kSequenceMap.GetAt(eSequenceID, pkSequence))
    {
        return pkSequence;
    }

    return NULL;
}
//---------------------------------------------------------------------------
inline NiAVObject* NiActorManager::GetAccumRoot() const
{
    NIASSERT(m_spManager);
    return m_spManager->GetAccumRoot();
}
//---------------------------------------------------------------------------
inline void NiActorManager::ClearAllRegisteredCallbacks()
{
    m_kCallbacks.RemoveAll();
    m_bBuildTimeline = true;
}
//---------------------------------------------------------------------------
inline float NiActorManager::GetNextEventTime(EventType eEventType,
    SequenceID eSequenceID, const NiFixedString& kTextKey)
{
    if (eEventType == TEXT_KEY_EVENT && kTextKey.Exists())
    {
        NiTextKeyMatch kMatchObject(kTextKey);
        return GetNextEventTime(eEventType, eSequenceID, &kMatchObject);
    }
    else
    {
        return GetNextEventTime(eEventType, eSequenceID);
    }
}
//---------------------------------------------------------------------------
inline float NiActorManager::GetNextEventTime(EventType eEventType,
    SequenceID eSequenceID, NiTextKeyMatch* pkMatchObject)
{
    switch (eEventType)
    {
        case ANIM_ACTIVATED:
            return GetNextAnimActivatedTime(eSequenceID);
        case ANIM_DEACTIVATED:
            return GetNextAnimDeactivatedTime(eSequenceID);
        case TEXT_KEY_EVENT:
            return GetNextTextKeyEventTime(eSequenceID, pkMatchObject );
        case END_OF_SEQUENCE:
            return GetNextEndOfSequenceTime(eSequenceID);
        default:
            return INVALID_TIME;
    }
}
//---------------------------------------------------------------------------
inline NiActorManager::CallbackData::CallbackData(EventType eEventType,
    SequenceID eSequenceID, NiTextKeyMatch * pkMatchObject ) : 
    m_eEventType(eEventType), m_eSequenceID(eSequenceID),
    m_spMatchObject(NULL)
{
    SetMatchObject( pkMatchObject );
}
//---------------------------------------------------------------------------
inline NiActorManager::CallbackData::~CallbackData()
{
    m_spMatchObject = 0;
}
//---------------------------------------------------------------------------
inline void NiActorManager::CallbackData::SetMatchObject( 
    NiTextKeyMatch* pkMatchObject )
{
    m_spMatchObject = pkMatchObject;
}
//---------------------------------------------------------------------------
inline NiTextKeyMatchPtr NiActorManager::CallbackData::GetMatchObject() const
{
    return m_spMatchObject;
}
//---------------------------------------------------------------------------
inline NiActorManager::TimelineData::~TimelineData()
{
    m_spMatchObject = 0;
}
//---------------------------------------------------------------------------
inline void NiActorManager::TimelineData::SetValues(float fEventTime,
    EventType eEventType, SequenceID eSequenceID, 
    NiTextKeyMatch* pkMatchObject, const NiFixedString& kTextKeyMatched)
{
    m_fEventTime = fEventTime;
    m_eEventType = eEventType;
    m_eSequenceID = eSequenceID;
    m_spMatchObject = pkMatchObject;
    m_kTextKeyMatched = kTextKeyMatched;
}
//---------------------------------------------------------------------------
inline float NiActorManager::GetNextEndOfSequenceTime(SequenceID eSequenceID)
{
    // This function returns the next time that the "end" text key will occur
    // in the specified sequence. This is a convenience function for a
    // commonly desired text key. If the time of the next "end" text key
    // cannot be determined, INVALID_TIME is returned.

    NiTextKeyMatch kKeyMatch( GetDefaultEndTextKey() );
    return GetNextTextKeyEventTime(eSequenceID, &kKeyMatch );
}
//---------------------------------------------------------------------------
inline NiActorManager::ChainCompletionInfo::ChainCompletionInfo()
{
}
//---------------------------------------------------------------------------
inline NiActorManager::ChainCompletionInfo::~ChainCompletionInfo()
{
}
//---------------------------------------------------------------------------
inline void NiActorManager::ChainCompletionInfo::SetName(
    const NiFixedString& kName)
{
    m_kName = kName;
}
//---------------------------------------------------------------------------
inline void NiActorManager::ChainCompletionInfo::SetNextName(
    const NiFixedString& kNextName)
{
    m_kNextName = kNextName;
}
//---------------------------------------------------------------------------
inline void NiActorManager::RefreshControllerManager()
{
    m_spManager = FindControllerManager(m_spNIFRoot);
    NIASSERT(m_spManager);
}
//---------------------------------------------------------------------------
inline void NiActorManager::RebuildTimeline()
{
    m_bBuildTimeline = true;
}
//---------------------------------------------------------------------------
