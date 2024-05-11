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
inline void NiPSysModifier::UpdateModifier(float fTime, NiPSysData* pkData)
{
    if (m_bActive)
    {
        Update(fTime, pkData);
    }
}
//---------------------------------------------------------------------------
inline const NiFixedString& NiPSysModifier::GetName() const
{
    return m_kName;
}
//---------------------------------------------------------------------------
inline void NiPSysModifier::SetName(const NiFixedString& kName)
{
    NIASSERT(kName.Exists());
    m_kName = kName;
}
//---------------------------------------------------------------------------
inline unsigned int NiPSysModifier::GetOrder() const
{
    return m_uiOrder;
}
//---------------------------------------------------------------------------
inline NiParticleSystem* NiPSysModifier::GetSystemPointer() const
{
    return m_pkTarget;
}
//---------------------------------------------------------------------------
inline bool NiPSysModifier::GetActive() const
{
    return m_bActive;
}
//---------------------------------------------------------------------------
inline void NiPSysModifier::SetActive(bool bActive)
{
    m_bActive = bActive;
}
//---------------------------------------------------------------------------
