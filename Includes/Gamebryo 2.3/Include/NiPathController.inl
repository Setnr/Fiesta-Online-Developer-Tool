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
inline NiPosData* NiPathController::GetPathData()
{
    return m_spPathData;
}
//---------------------------------------------------------------------------
inline void NiPathController::SetPathData(NiPosData* pPathData)
{
    m_spPathData = pPathData;
}
//---------------------------------------------------------------------------
inline NiFloatData* NiPathController::GetPctData()
{
    return m_spPctData;
}
//---------------------------------------------------------------------------
inline void NiPathController::SetPctData(NiFloatData* pPctData)
{
    m_spPctData = pPctData;
}
//---------------------------------------------------------------------------
inline void NiPathController::GetPathData(NiPosKey*& pkPathData, 
    unsigned int& uiNumKeys, NiPosKey::KeyType& eType,
    unsigned char& ucSize) const
{
    if (m_spPathData)
    {
        pkPathData = m_spPathData->GetAnim(uiNumKeys, eType, ucSize);
    }
    else
    {
        uiNumKeys = 0;
        eType = NiAnimationKey::NOINTERP;
        pkPathData = NULL;
        ucSize = 0;
    }
}
//---------------------------------------------------------------------------
inline void NiPathController::GetPctData(NiFloatKey*& pkPctData,
    unsigned int& uiNumKeys, NiFloatKey::KeyType& eType,
    unsigned char& ucSize) const
{
    if (m_spPctData)
    {
        pkPctData = m_spPctData->GetAnim(uiNumKeys, eType, ucSize);
    }
    else
    {
        uiNumKeys = 0;
        eType = NiAnimationKey::NOINTERP;
        pkPctData = 0;
        ucSize = 0;
    }
}
//---------------------------------------------------------------------------
inline void NiPathController::SetAllowFlip(bool bAllowFlip)
{
    SetBit(bAllowFlip, ALLOWFLIP_MASK);
}
//---------------------------------------------------------------------------
inline bool NiPathController::GetAllowFlip() const
{
    return GetBit(ALLOWFLIP_MASK);
}
//---------------------------------------------------------------------------
inline void NiPathController::SetBank(bool bBank)
{
    SetBit(bBank, BANK_MASK);
}
//---------------------------------------------------------------------------
inline bool NiPathController::GetBank() const
{
    return GetBit(BANK_MASK);
}
//---------------------------------------------------------------------------
inline void NiPathController::SetBankDir(BankDir eDir)
{
    m_eBankDir = eDir;
}
//---------------------------------------------------------------------------
inline NiPathController::BankDir NiPathController::GetBankDir() const
{
    return m_eBankDir;
}
//---------------------------------------------------------------------------
inline void NiPathController::SetConstVelocity(bool bConstVelocity)
{
    SetBit(bConstVelocity, CONSTVELOCITY_MASK);
    if (bConstVelocity)
        SetConstantVelocityData();
}
//---------------------------------------------------------------------------
inline bool NiPathController::GetConstVelocity() const
{
    return GetBit(CONSTVELOCITY_MASK);
}
//---------------------------------------------------------------------------
inline void NiPathController::SetFollow(bool bFollow)
{
    SetBit(bFollow, FOLLOW_MASK);
}
//---------------------------------------------------------------------------
inline bool NiPathController::GetFollow() const
{
    return GetBit(FOLLOW_MASK);
}
//---------------------------------------------------------------------------
inline void NiPathController::SetMaxBankAngle(float fAngle)
{
    m_fMaxBankAngle = fAngle;
}
//---------------------------------------------------------------------------
inline float NiPathController::GetMaxBankAngle() const
{
    return m_fMaxBankAngle;
}
//---------------------------------------------------------------------------
inline void NiPathController::SetSmoothing(float fSmooth)
{
    m_fSmoothing = fSmooth;
}
//---------------------------------------------------------------------------
inline float NiPathController::GetSmoothing() const
{
    return m_fSmoothing;
}
//---------------------------------------------------------------------------
inline void NiPathController::SetFollowAxis(short sAxis)
{
    m_sFollowAxis = sAxis;
}
//---------------------------------------------------------------------------
inline short NiPathController::GetFollowAxis() const
{
    return m_sFollowAxis;
}
//---------------------------------------------------------------------------
inline void NiPathController::SetFlip(bool bFlip)
{
    SetBit(bFlip, FLIP_MASK);
}
//---------------------------------------------------------------------------
inline bool NiPathController::GetFlip() const
{
    return GetBit(FLIP_MASK);
}
//---------------------------------------------------------------------------
inline void NiPathController::SetCurveTypeOpen(bool bOpen)
{
    SetBit(bOpen, OPENCURVE_MASK);
}
//---------------------------------------------------------------------------
inline bool NiPathController::GetCurveTypeOpen() const
{
    return GetBit(OPENCURVE_MASK);
}
//---------------------------------------------------------------------------
inline void NiPathController::SetCVDataNeedsUpdate(bool bUpdate)
{
    SetBit(bUpdate, CVDATAUPDATE_MASK);
}
//---------------------------------------------------------------------------
inline bool NiPathController::GetCVDataNeedsUpdate() const
{
    return GetBit(CVDATAUPDATE_MASK);
}
//---------------------------------------------------------------------------
inline bool NiPathController::TargetIsRequiredType() const
{
    return NiIsKindOf(NiAVObject, m_pkTarget);
}
//---------------------------------------------------------------------------
inline bool NiPathController::IsTransformController() const
{
    return true;
}
//---------------------------------------------------------------------------
