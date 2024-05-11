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
inline NiAVObject* NiPSysBombModifier::GetBombObj() const
{
    return m_pkBombObj;
}
//---------------------------------------------------------------------------
inline void NiPSysBombModifier::SetBombObj(NiAVObject* pkBombObj)
{
    m_pkBombObj = pkBombObj;
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiPSysBombModifier::GetBombAxis() const
{
    return m_kBombAxis;
}
//---------------------------------------------------------------------------
inline void NiPSysBombModifier::SetBombAxis(const NiPoint3& kBombAxis)
{
    m_kBombAxis = kBombAxis;
}
//---------------------------------------------------------------------------
inline float NiPSysBombModifier::GetDecay() const
{
    return m_fDecay;
}
//---------------------------------------------------------------------------
inline void NiPSysBombModifier::SetDecay(float fDecay)
{
    m_fDecay = fDecay;
}
//---------------------------------------------------------------------------
inline float NiPSysBombModifier::GetDeltaV() const
{
    return m_fDeltaV;
}
//---------------------------------------------------------------------------
inline void NiPSysBombModifier::SetDeltaV(float fDeltaV)
{
    m_fDeltaV = fDeltaV;
}
//---------------------------------------------------------------------------
inline NiPSysBombModifier::DecayType NiPSysBombModifier::GetDecayType() const
{
    return m_eDecayType;
}
//---------------------------------------------------------------------------
inline void NiPSysBombModifier::SetDecayType(DecayType eDecayType)
{
    m_eDecayType = eDecayType;
}
//---------------------------------------------------------------------------
inline NiPSysBombModifier::SymmType NiPSysBombModifier::GetSymmType() const
{
    return m_eSymmType;
}
//---------------------------------------------------------------------------
inline void NiPSysBombModifier::SetSymmType(SymmType eSymmType)
{
    m_eSymmType = eSymmType;
}
//---------------------------------------------------------------------------
