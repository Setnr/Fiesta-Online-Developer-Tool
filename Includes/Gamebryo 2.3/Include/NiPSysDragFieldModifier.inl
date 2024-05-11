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
inline const bool NiPSysDragFieldModifier::GetUseDirection() const
{
    return m_bUseDirection;
}
//---------------------------------------------------------------------------
inline void NiPSysDragFieldModifier::SetUseDirection(bool bUseDirection)
{
    m_bUseDirection = bUseDirection;
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiPSysDragFieldModifier::GetDirection() const
{
    return m_kDirection;
}
//---------------------------------------------------------------------------
inline void NiPSysDragFieldModifier::SetDirection(const NiPoint3& kDirection)
{
    m_kDirection = kDirection;
}
//---------------------------------------------------------------------------
