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
inline NiTransformationComponent::NiTransformationComponent() :
    m_kTranslation(NiPoint3::ZERO), m_kRotation(NiMatrix3::IDENTITY),
    m_fScale(1.0f)
{
}
//---------------------------------------------------------------------------
inline NiTransformationComponent::NiTransformationComponent(
    const NiPoint3& kTranslation, const NiMatrix3& kRotation, float fScale) :
    m_kTranslation(kTranslation), m_kRotation(kRotation), m_fScale(fScale)
{
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiTransformationComponent::GetTranslation() const
{
    return m_kTranslation;
}
//---------------------------------------------------------------------------
inline void NiTransformationComponent::SetTranslation(
    const NiPoint3& kTranslation)
{
    m_kTranslation = kTranslation;
}
//---------------------------------------------------------------------------
inline const NiMatrix3& NiTransformationComponent::GetRotation() const
{
    return m_kRotation;
}
//---------------------------------------------------------------------------
inline void NiTransformationComponent::SetRotation(const NiMatrix3& kRotation)
{
    m_kRotation = kRotation;
}
//---------------------------------------------------------------------------
inline float NiTransformationComponent::GetScale() const
{
    return m_fScale;
}
//---------------------------------------------------------------------------
inline void NiTransformationComponent::SetScale(float fScale)
{
    m_fScale = fScale;
}
//---------------------------------------------------------------------------
