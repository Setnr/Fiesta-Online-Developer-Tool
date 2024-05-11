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
inline NiLookAtController::NiLookAtController()
{
    m_uFlags = 0;
    m_pkLookAt = 0; 
    SetFlip(false);
    SetAxis(X);
    SetComputeScaledTime(false);
}
//---------------------------------------------------------------------------
inline void NiLookAtController::SetLookAt(NiAVObject* pkLookAt)
{
    m_pkLookAt = pkLookAt;
}
//---------------------------------------------------------------------------
inline NiAVObject* NiLookAtController::GetLookAt() const
{
    return m_pkLookAt;
}
//---------------------------------------------------------------------------
inline void NiLookAtController::SetAxis(Axis eAxis)
{
    SetField(eAxis, AXIS_MASK, AXIS_POS);
}
//---------------------------------------------------------------------------
inline NiLookAtController::Axis NiLookAtController::GetAxis() const
{
    return (Axis)GetField(AXIS_MASK, AXIS_POS);
}
//---------------------------------------------------------------------------
inline void NiLookAtController::SetFlip(bool bFlip)
{
    SetBit(bFlip,FLIP_MASK);
}
//---------------------------------------------------------------------------
inline bool NiLookAtController::GetFlip() const
{
    return GetBit(FLIP_MASK);
}
//---------------------------------------------------------------------------
inline bool NiLookAtController::TargetIsRequiredType() const
{
    return NiIsKindOf(NiAVObject, m_pkTarget);
}
//---------------------------------------------------------------------------
inline bool NiLookAtController::IsTransformController() const
{
    return true;
}
//---------------------------------------------------------------------------
