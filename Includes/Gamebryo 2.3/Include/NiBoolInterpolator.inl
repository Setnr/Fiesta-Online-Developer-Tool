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
inline NiBoolData* NiBoolInterpolator::GetBoolData() const
{
    return m_spBoolData;
}
//---------------------------------------------------------------------------
inline void NiBoolInterpolator::SetBoolData(NiBoolData* pkBoolData)
{
    m_spBoolData = pkBoolData;
}
//---------------------------------------------------------------------------
inline NiBoolKey* NiBoolInterpolator::GetKeys(unsigned int& uiNumKeys,
        NiBoolKey::KeyType& eType, unsigned char& ucSize) const
{
    if (m_spBoolData)
    {
        return m_spBoolData->GetAnim(uiNumKeys, eType, ucSize);
    }

    uiNumKeys = 0;
    eType = NiBoolKey::NOINTERP;
    ucSize = 0;
    return NULL;
}
//---------------------------------------------------------------------------
inline void NiBoolInterpolator::ReplaceKeys(NiBoolKey* pkKeys,
    NiBoolKey::KeyType eType, unsigned int uiNumKeys)
{
    if (uiNumKeys > 0)
    {
        if (!m_spBoolData)
        {
            m_spBoolData = NiNew NiBoolData;
        }

        m_spBoolData->ReplaceAnim(pkKeys, uiNumKeys, eType);
        m_uiLastIdx = 0;
    }
    else if (m_spBoolData)
    {
        m_spBoolData->ReplaceAnim(NULL, 0, NiAnimationKey::NOINTERP);
    }
}
//---------------------------------------------------------------------------
inline void NiBoolInterpolator::SetKeys(NiBoolKey* pkKeys,
    NiBoolKey::KeyType eType, unsigned int uiNumKeys)
{
    if (uiNumKeys > 0)
    {
        if (!m_spBoolData)
        {
            m_spBoolData = NiNew NiBoolData;
        }

        m_spBoolData->SetAnim(pkKeys, uiNumKeys, eType);
        m_uiLastIdx = 0;
    }
    else if (m_spBoolData)
    {
        m_spBoolData->SetAnim(NULL, 0, NiAnimationKey::NOINTERP);
    }
}
//---------------------------------------------------------------------------
