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

#include <NiAVObject.h>

//---------------------------------------------------------------------------
inline NiControllerSequence::InterpArrayItem::InterpArrayItem() :
    m_spInterpolator(NULL), m_spInterpCtlr(NULL), m_pkBlendInterp(NULL),
    m_ucBlendIdx(NiBlendInterpolator::INVALID_INDEX)
{
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::InterpArrayItem::ClearValues()
{
    m_spInterpolator = NULL;
    m_spInterpCtlr = NULL;
    m_pkBlendInterp = NULL;
    m_ucBlendIdx = NiBlendInterpolator::INVALID_INDEX;
}
//---------------------------------------------------------------------------
inline NiControllerSequence::IDTag::IDTag(
    const NiFixedString& kAVObjectName, const NiFixedString& kPropertyType,
    const NiFixedString& kCtlrType, const NiFixedString& kCtlrID,
    const NiFixedString& kInterpolatorID) 
{
    ClearValues();
    SetAVObjectName(kAVObjectName);
    SetPropertyType(kPropertyType);
    SetCtlrType(kCtlrType);
    SetCtlrID(kCtlrID);
    SetInterpolatorID(kInterpolatorID);
}
//---------------------------------------------------------------------------
inline NiControllerSequence::IDTag::IDTag(const IDTag& kOther)
{
    SetAVObjectName(kOther.m_kAVObjectName);
    SetPropertyType(kOther.m_kPropertyType);
    SetCtlrType(kOther.m_kCtlrType);
    SetCtlrID(kOther.m_kCtlrID);
    SetInterpolatorID(kOther.m_kInterpolatorID);
}
//---------------------------------------------------------------------------
inline NiControllerSequence::IDTag& 
NiControllerSequence::IDTag::operator=(const IDTag& kTag)
{
    SetAVObjectName(kTag.m_kAVObjectName);
    SetPropertyType(kTag.m_kPropertyType);
    SetCtlrType(kTag.m_kCtlrType);
    SetCtlrID(kTag.m_kCtlrID);
    SetInterpolatorID(kTag.m_kInterpolatorID);
    return *this;
}
//---------------------------------------------------------------------------
inline const NiFixedString& NiControllerSequence::IDTag::GetAVObjectName()
    const
{
    return m_kAVObjectName;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::IDTag::SetAVObjectName(
    const NiFixedString& kAVObjectName)
{
    m_kAVObjectName = kAVObjectName;
}
//---------------------------------------------------------------------------
inline const NiFixedString& NiControllerSequence::IDTag::GetPropertyType()
    const
{
    return m_kPropertyType;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::IDTag::SetPropertyType(
    const NiFixedString& kPropertyType)
{
    m_kPropertyType = kPropertyType;
}
//---------------------------------------------------------------------------
inline const NiFixedString& NiControllerSequence::IDTag::GetCtlrType() const
{
    return m_kCtlrType;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::IDTag::SetCtlrType(
    const NiFixedString& kCtlrType)
{
    m_kCtlrType = kCtlrType;
}
//---------------------------------------------------------------------------
inline const NiFixedString& NiControllerSequence::IDTag::GetCtlrID() const
{
    return m_kCtlrID;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::IDTag::SetCtlrID(
    const NiFixedString& kCtlrID)
{
    m_kCtlrID = kCtlrID;
}
//---------------------------------------------------------------------------
inline const NiFixedString& 
NiControllerSequence::IDTag::GetInterpolatorID() const
{
    return m_kInterpolatorID;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::IDTag::SetInterpolatorID(
    const NiFixedString& kInterpolatorID)
{
    m_kInterpolatorID = kInterpolatorID;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::IDTag::ClearValues()
{
    m_kAVObjectName = NULL;
    m_kPropertyType = NULL;
    m_kCtlrType = NULL;
    m_kCtlrID = NULL;
    m_kInterpolatorID = NULL;
}
//---------------------------------------------------------------------------
inline const NiFixedString& NiControllerSequence::GetName() const
{
    return m_kName;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetName(const NiFixedString& kName)
{
    m_kName = kName;
}
//---------------------------------------------------------------------------
inline NiTextKeyExtraData* NiControllerSequence::GetTextKeys() const
{
    return m_spTextKeys;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetTextKeys(NiTextKeyExtraData* pkKeys)
{
    m_spTextKeys = pkKeys;
}
//---------------------------------------------------------------------------
inline unsigned int NiControllerSequence::GetArraySize() const
{
    return m_uiArraySize;
}
//---------------------------------------------------------------------------
inline unsigned int NiControllerSequence::GetArrayGrowBy() const
{
    return m_uiArrayGrowBy;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetArrayGrowBy(unsigned int uiArrayGrowBy)
{
    m_uiArrayGrowBy = uiArrayGrowBy;
}
//---------------------------------------------------------------------------
inline NiInterpController* NiControllerSequence::GetControllerAt(
    unsigned int uiIndex) const
{
    NIASSERT(uiIndex < m_uiArraySize);
    return m_pkInterpArray[uiIndex].m_spInterpCtlr;
}
//---------------------------------------------------------------------------
inline NiObjectNET* NiControllerSequence::GetTargetAt(unsigned int uiIndex)
    const
{
    NIASSERT(uiIndex < m_uiArraySize);
    if (m_pkInterpArray[uiIndex].m_spInterpCtlr)
    {
        return m_pkInterpArray[uiIndex].m_spInterpCtlr->GetTarget();
    }

    return NULL;
}
//---------------------------------------------------------------------------
inline NiInterpolator* NiControllerSequence::GetInterpolatorAt(
    unsigned int uiIndex) const
{
    NIASSERT(uiIndex < m_uiArraySize);
    return m_pkInterpArray[uiIndex].m_spInterpolator;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::GetInterpInfoAt(unsigned int uiIndex,
    const char*& pcAVObjectName, const char*& pcPropertyType,
    const char*& pcCtlrType, const char*& pcCtlrID,
    const char*& pcInterpolatorID)
{
    NIASSERT(uiIndex < m_uiArraySize);
    IDTag& kTag = m_pkIDTagArray[uiIndex];
    pcAVObjectName = kTag.GetAVObjectName();
    pcPropertyType = kTag.GetPropertyType();
    pcCtlrType = kTag.GetCtlrType();
    pcCtlrID = kTag.GetCtlrID();
    pcInterpolatorID = kTag.GetInterpolatorID();
}
//---------------------------------------------------------------------------
inline NiBlendInterpolator* NiControllerSequence::GetBlendInterpolatorAt(
    unsigned int uiIndex) const
{
    NIASSERT(uiIndex < m_uiArraySize);
    return m_pkInterpArray[uiIndex].m_pkBlendInterp;
}
//---------------------------------------------------------------------------
inline unsigned char NiControllerSequence::GetBlendIdxAt(
    unsigned int uiIndex)
{
    NIASSERT(uiIndex < m_uiArraySize);
    return m_pkInterpArray[uiIndex].m_ucBlendIdx;
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetSequenceWeight() const
{
    return m_fSeqWeight;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetSequenceWeight(float fWeight)
{
    m_fSeqWeight = fWeight;
    if (m_fSeqWeight < 0.0f)
    {
        m_fSeqWeight = 0.0f;
    }
}
//---------------------------------------------------------------------------
inline NiTimeController::CycleType NiControllerSequence::GetCycleType()
    const
{
    return m_eCycleType;
}
//---------------------------------------------------------------------------
inline bool NiControllerSequence::SetCycleType(
    NiTimeController::CycleType eCycleType)
{
    if (eCycleType == NiTimeController::CLAMP ||
        eCycleType == NiTimeController::LOOP)
    {
        m_eCycleType = eCycleType;
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetFrequency() const
{
    return m_fFrequency;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetFrequency(float fFrequency)
{
    m_fFrequency = fFrequency;
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::TimeDivFreq(float fTime) const
{
    return (fTime / m_fFrequency);
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::TimeMultFreq(float fTime) const
{
    return (fTime * m_fFrequency);
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetBeginKeyTimeDivFreq() const
{
    return TimeDivFreq(m_fBeginKeyTime);
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetBeginKeyTime() const
{
    return m_fBeginKeyTime;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetBeginKeyTime(float fBeginKeyTime)
{
    m_fBeginKeyTime = fBeginKeyTime;
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetEndKeyTimeDivFreq() const
{
    return TimeDivFreq(m_fEndKeyTime);
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetEndKeyTime() const
{
    return m_fEndKeyTime;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetEndKeyTime(float fEndKeyTime)
{
    m_fEndKeyTime = fEndKeyTime;
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetLength()
{
    NIASSERT(m_fFrequency != 0.0f);
    return ((m_fEndKeyTime - m_fBeginKeyTime) / m_fFrequency);
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetLastTime() const
{
    return m_fLastTime;
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetLastScaledTime() const
{
    return m_fLastScaledTime;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::ResetSequence()
{
    m_fOffset = -NI_INFINITY;
}
//---------------------------------------------------------------------------
inline NiControllerSequence::AnimState NiControllerSequence::GetState() const
{
    return m_eState;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::AddActivationCallbackObject(
    CallbackObject* pkCallback)
{
    m_kActivationCallbacks.AddFirstEmpty(pkCallback);
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::RemoveActivationCallbackObject(
    CallbackObject* pkCallback)
{
    m_kActivationCallbacks.Remove(pkCallback);
    m_kActivationCallbacks.Compact();
}
//---------------------------------------------------------------------------
inline unsigned int NiControllerSequence::GetActivationCallbackCount() const
{
    return m_kActivationCallbacks.GetSize();
}
//---------------------------------------------------------------------------
inline NiControllerSequence::CallbackObject*
    NiControllerSequence::GetActivationCallbackObjectAt(unsigned int uiIndex)
{
    return m_kActivationCallbacks.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetOffset() const
{
    return m_fOffset;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetOffset(float fOffset)
{
    m_fOffset = fOffset;
}
//---------------------------------------------------------------------------
inline NiControllerManager* NiControllerSequence::GetOwner() const
{
    return m_pkOwner;
}
//---------------------------------------------------------------------------
inline bool NiControllerSequence::SetOwner(NiControllerManager* pkOwner)
{
    if (pkOwner && m_pkOwner)
    {
        return false;
    }

    m_pkOwner = pkOwner;
    return true;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetAccumRootName(const NiFixedString&
    kAccumRootName)
{   
    m_kAccumRootName = kAccumRootName;
}
//---------------------------------------------------------------------------
inline const NiFixedString& NiControllerSequence::GetAccumRootName() const
{
    return m_kAccumRootName;
}
//---------------------------------------------------------------------------
inline NiAVObject* NiControllerSequence::GetAccumRoot() const
{
    return m_pkAccumRoot;
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetAccumTranslation(
        const bool bAccumX, const bool bAccumY, const bool bAccumZ)
{
    SetBit(bAccumX, ACCUM_X_TRANS);
    SetBit(bAccumY, ACCUM_Y_TRANS);
    SetBit(bAccumZ, ACCUM_Z_TRANS);
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::GetAccumTranslation(
        bool& bAccumX, bool& bAccumY, bool& bAccumZ) const
{
    bAccumX = GetBit(ACCUM_X_TRANS);
    bAccumY = GetBit(ACCUM_Y_TRANS);
    bAccumZ = GetBit(ACCUM_Z_TRANS);
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetAccumRotation(
        const bool bAccumX, const bool bAccumY, const bool bAccumZ)
{
    SetBit(bAccumX, ACCUM_X_ROT);
    SetBit(bAccumY, ACCUM_Y_ROT);
    SetBit(bAccumZ, ACCUM_Z_ROT);
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::GetAccumRotation(
        bool& bAccumX, bool& bAccumY, bool& bAccumZ) const
{
    bAccumX = GetBit(ACCUM_X_ROT);
    bAccumY = GetBit(ACCUM_Y_ROT);
    bAccumZ = GetBit(ACCUM_Z_ROT);
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetAccumFrontFacing(
    const unsigned char ucAxis, const bool bIsNegative)
{
    switch (ucAxis)
    {
        case 0:
            SetBit(true, ACCUM_X_FRONT);
            SetBit(false, ACCUM_Y_FRONT);
            SetBit(false, ACCUM_Z_FRONT);
            break;
        case 1:
            SetBit(false, ACCUM_X_FRONT);
            SetBit(true, ACCUM_Y_FRONT);
            SetBit(false, ACCUM_Z_FRONT);
            break;
        case 2:
            SetBit(false, ACCUM_X_FRONT);
            SetBit(false, ACCUM_Y_FRONT);
            SetBit(true, ACCUM_Z_FRONT);
            break;
    }
    SetBit(bIsNegative, ACCUM_NEG_FRONT);
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::GetAccumFrontFacing(
    unsigned char& ucAxis, bool& bIsNegative) const
{
    if (GetBit(ACCUM_X_FRONT))
    {
        ucAxis = 0;
    }
    else if (GetBit(ACCUM_Y_FRONT))
    {
        ucAxis = 1;
    }
    else
    {
        ucAxis = 2;
    }
    bIsNegative = GetBit(ACCUM_NEG_FRONT);
}
//---------------------------------------------------------------------------
inline void NiControllerSequence::SetControllerAt(
    NiInterpController* pkInterpCtlr, unsigned int uiIndex)
{
    NIASSERT(uiIndex < m_uiArraySize);
    m_pkInterpArray[uiIndex].m_spInterpCtlr = pkInterpCtlr;
}
//---------------------------------------------------------------------------
inline unsigned int NiControllerSequence::GetIDTagCount()
{
    return m_uiArraySize;
}
//---------------------------------------------------------------------------
inline NiControllerSequence::IDTag* NiControllerSequence::GetIDTag(
    unsigned int uiTagIndex)
{
    NIASSERT(uiTagIndex < m_uiArraySize);
    return &m_pkIDTagArray[uiTagIndex];
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetEaseEndTime() const
{
    return m_fEndTime;
}
//---------------------------------------------------------------------------
inline float NiControllerSequence::GetDestFrame() const
{
    return m_fDestFrame;
}
//---------------------------------------------------------------------------
