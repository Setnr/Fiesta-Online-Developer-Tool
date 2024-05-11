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
inline NiAVObject* NiPSysPlanarCollider::GetColliderObj() const
{
    return m_pkColliderObj;
}
//---------------------------------------------------------------------------
inline void NiPSysPlanarCollider::SetColliderObj(NiAVObject* pkColliderObj)
{
    m_pkColliderObj = pkColliderObj;
}
//---------------------------------------------------------------------------
inline float NiPSysPlanarCollider::GetWidth() const
{
    return m_fWidth;
}
//---------------------------------------------------------------------------
inline void NiPSysPlanarCollider::SetWidth(float fWidth)
{
    m_fWidth = fWidth;
}
//---------------------------------------------------------------------------
inline float NiPSysPlanarCollider::GetHeight() const
{
    return m_fHeight;
}
//---------------------------------------------------------------------------
inline void NiPSysPlanarCollider::SetHeight(float fHeight)
{
    m_fHeight = fHeight;
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiPSysPlanarCollider::GetXAxis() const
{
    return m_kXAxis;
}
//---------------------------------------------------------------------------
inline void NiPSysPlanarCollider::SetXAxis(const NiPoint3& kXAxis)
{
    m_kXAxis = kXAxis;
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiPSysPlanarCollider::GetYAxis() const
{
    return m_kYAxis;
}
//---------------------------------------------------------------------------
inline void NiPSysPlanarCollider::SetYAxis(const NiPoint3& kYAxis)
{
    m_kYAxis = kYAxis;
}
//---------------------------------------------------------------------------
