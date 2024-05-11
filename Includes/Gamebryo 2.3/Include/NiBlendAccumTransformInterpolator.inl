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
inline NiBlendAccumTransformInterpolator::AccumArrayItem::AccumArrayItem()
{
    ClearValues();
}
//---------------------------------------------------------------------------
inline void NiBlendAccumTransformInterpolator::AccumArrayItem::ClearValues()
{
    m_fLastTime = -NI_INFINITY;
    m_kLastValue.MakeInvalid();
    m_kDeltaValue.MakeInvalid();
    m_kRefFrame = NiMatrix3::IDENTITY;
}
//---------------------------------------------------------------------------
inline void NiBlendAccumTransformInterpolator::AccumArrayItem::LoadBinary(
    NiStream& kStream)
{
    m_kLastValue.LoadBinary(kStream);
    m_kDeltaValue.LoadBinary(kStream);
    m_kRefFrame.LoadBinary(kStream);
}
//---------------------------------------------------------------------------
inline void NiBlendAccumTransformInterpolator::AccumArrayItem::SaveBinary(
    NiStream& kStream)
{
    m_kLastValue.SaveBinary(kStream);
    m_kDeltaValue.SaveBinary(kStream);
    m_kRefFrame.SaveBinary(kStream);
}
//---------------------------------------------------------------------------
inline NiBlendAccumTransformInterpolator::NiBlendAccumTransformInterpolator(
    bool bManagerControlled, float fWeightThreshold, 
    unsigned char ucArraySize) : 
    NiBlendInterpolator(bManagerControlled,
    fWeightThreshold, ucArraySize), 
    m_pkAccumArray(NULL),
    m_bReset(false)
{
    if (m_ucArraySize > 0)
    {
        // Allocate memory for array.
        m_pkAccumArray = NiNew AccumArrayItem[m_ucArraySize];
    }
}
//---------------------------------------------------------------------------
inline NiBlendAccumTransformInterpolator::NiBlendAccumTransformInterpolator()
    : m_pkAccumArray(NULL),
    m_bReset(false)
{
}
//---------------------------------------------------------------------------
inline void NiBlendAccumTransformInterpolator::ClearAccumulatedTransform()
{
    if (m_ucInterpCount == 1)
    {
        NiQuatTransform kEmpty;
        m_kAccumulatedTransformValue = kEmpty;
    }
    else
    {
        m_kAccumulatedTransformValue.SetTranslate(NiPoint3::ZERO);
        m_kAccumulatedTransformValue.SetRotate(NiQuaternion::IDENTITY);
        m_kAccumulatedTransformValue.SetScale(1.0f);
    }
    m_bReset = true;
}
//---------------------------------------------------------------------------
inline void NiBlendAccumTransformInterpolator::SetAccumulatedTransform(
    const NiQuatTransform& kTransform)
{
    if (kTransform.IsTranslateValid())
    {
        m_kAccumulatedTransformValue.SetTranslate(kTransform.GetTranslate());
    }
    if (kTransform.IsRotateValid())
    {
        m_kAccumulatedTransformValue.SetRotate(kTransform.GetRotate());
    }
    if (kTransform.IsScaleValid())
    {
        m_kAccumulatedTransformValue.SetScale(kTransform.GetScale());
    }
    m_bReset = true;
}
//---------------------------------------------------------------------------
inline const NiQuatTransform&
    NiBlendAccumTransformInterpolator::GetAccumulatedTransformValue() const
{
    return m_kAccumulatedTransformValue;
}
//---------------------------------------------------------------------------
