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
inline unsigned int NiControllerManager::GetSequenceCount() const
{
    return m_kSequenceArray.GetSize();
}
//---------------------------------------------------------------------------
inline NiControllerSequence* NiControllerManager::GetSequenceByName(
    const NiFixedString& kName)
{
    NIASSERT(kName.Exists());

    unsigned int uiIndex;
    if (m_kIndexMap.GetAt(kName, uiIndex))
    {
        return m_kSequenceArray.GetAt(uiIndex);
    }

    return NULL;
}
//---------------------------------------------------------------------------
inline NiControllerSequence* NiControllerManager::GetSequenceAt(
    unsigned int uiIndex)
{
    NIASSERT(uiIndex < m_kSequenceArray.GetSize());
    return m_kSequenceArray.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline bool NiControllerManager::ActivateSequence(
    const NiFixedString& kSequenceName,
    int iPriority, bool bStartOver, float fWeight, float fEaseInTime,
    NiControllerSequence* pkTimeSyncSeq)
{
    NIASSERT(kSequenceName.Exists());

    unsigned int uiIndex;
    if (m_kIndexMap.GetAt(kSequenceName, uiIndex))
    {
        return ActivateSequence(m_kSequenceArray.GetAt(uiIndex), iPriority,
            bStartOver, fWeight, fEaseInTime, pkTimeSyncSeq);
    }

    return false;
}
//---------------------------------------------------------------------------
inline bool NiControllerManager::ActivateSequence(
    NiControllerSequence* pkSequence, int iPriority, bool bStartOver,
    float fWeight, float fEaseInTime, NiControllerSequence* pkTimeSyncSeq)
{
    NIASSERT(pkSequence && pkSequence->GetOwner() == this);
    return pkSequence->Activate(iPriority, bStartOver, fWeight, fEaseInTime,
        pkTimeSyncSeq, false);
}
//---------------------------------------------------------------------------
inline bool NiControllerManager::DeactivateSequence(
    const NiFixedString& kSequenceName, float fEaseOutTime)
{
    NIASSERT(kSequenceName.Exists());

    unsigned int uiIndex;
    if (m_kIndexMap.GetAt(kSequenceName, uiIndex))
    {
        return DeactivateSequence(m_kSequenceArray.GetAt(uiIndex),
            fEaseOutTime);
    }

    return false;
}
//---------------------------------------------------------------------------
inline bool NiControllerManager::DeactivateSequence(
    NiControllerSequence* pkSequence, float fEaseOutTime)
{
    NIASSERT(pkSequence && pkSequence->GetOwner() == this);
    return pkSequence->Deactivate(fEaseOutTime, false);
}
//---------------------------------------------------------------------------
inline void NiControllerManager::DeactivateAll(float fEaseOutTime)
{
    for (unsigned int ui = 0; ui < m_kSequenceArray.GetSize(); ui++)
    {
        NiControllerSequence* pkSequence = m_kSequenceArray.GetAt(ui);
        if (pkSequence)
        {
            DeactivateSequence(pkSequence, fEaseOutTime);
        }
    }
}
//---------------------------------------------------------------------------
inline bool NiControllerManager::SetSequenceWeight(
    const NiFixedString& kSequenceName, float fWeight)
{
    NIASSERT(kSequenceName.Exists());

    unsigned int uiIndex;
    if (m_kIndexMap.GetAt(kSequenceName, uiIndex))
    {
        return SetSequenceWeight(m_kSequenceArray.GetAt(uiIndex), fWeight);
    }

    return false;
}
//---------------------------------------------------------------------------
inline bool NiControllerManager::SetSequenceWeight(
    NiControllerSequence* pkSequence, float fWeight)
{
    NIASSERT(pkSequence && pkSequence->GetOwner() == this);
    pkSequence->SetSequenceWeight(fWeight);
    return true;
}
//---------------------------------------------------------------------------
inline bool NiControllerManager::BlendFromSequence(
    NiControllerSequence* pkSourceSequence,
    NiControllerSequence* pkDestSequence, float fDuration,
    const NiFixedString& kDestTextKey, int iPriority, float fSourceWeight,
    float fDestWeight, NiControllerSequence* pkTimeSyncSeq)
{
    NIASSERT(pkSourceSequence && pkSourceSequence->GetOwner() == this &&
        pkDestSequence && pkDestSequence->GetOwner() == this);

    return pkSourceSequence->StartBlend(pkDestSequence, fDuration,
        kDestTextKey, iPriority, fSourceWeight, fDestWeight, pkTimeSyncSeq);
}
//---------------------------------------------------------------------------
inline bool NiControllerManager::BlendFromSequence(
    NiControllerSequence* pkSourceSequence,
    NiControllerSequence* pkDestSequence, float fDuration, float fDestFrame,
    int iPriority, float fSourceWeight, float fDestWeight,
    NiControllerSequence* pkTimeSyncSeq)
{
    NIASSERT(pkSourceSequence && pkSourceSequence->GetOwner() == this &&
        pkDestSequence && pkDestSequence->GetOwner() == this);

    return pkSourceSequence->StartBlend(pkDestSequence, fDuration, fDestFrame,
        iPriority, fSourceWeight, fDestWeight, pkTimeSyncSeq);
}
//---------------------------------------------------------------------------
inline bool NiControllerManager::GetCumulativeAnimations() const
{
    return m_bCumulative;
}
//---------------------------------------------------------------------------
inline NiAVObject* NiControllerManager::GetAccumRoot() const
{
    if (m_bCumulative && m_kSequenceArray.GetSize() > 0 &&
        m_kSequenceArray.GetAt(0) != NULL)
    {
        return m_kSequenceArray.GetAt(0)->GetAccumRoot();
    }

    return NULL;
}
//---------------------------------------------------------------------------
inline NiAVObjectPalette* NiControllerManager::GetObjectPalette() const
{
    return m_spObjectPalette;
}
//---------------------------------------------------------------------------
