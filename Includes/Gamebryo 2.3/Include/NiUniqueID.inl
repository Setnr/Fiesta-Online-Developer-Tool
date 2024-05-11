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
inline NiUniqueID::NiUniqueID()
{
    memset(&m_aucValue, 0, NUM_BYTES_IN_UNIQUE_ID);
}
//---------------------------------------------------------------------------
inline NiUniqueID::NiUniqueID(const unsigned char* pucID)
{
    NiMemcpy(&m_aucValue, NUM_BYTES_IN_UNIQUE_ID, pucID,
        NUM_BYTES_IN_UNIQUE_ID);
}
//---------------------------------------------------------------------------
inline NiUniqueID::NiUniqueID(unsigned char ucA, unsigned char ucB,
    unsigned char ucC, unsigned char ucD, unsigned char ucE,
    unsigned char ucF, unsigned char ucG, unsigned char ucH,
    unsigned char ucI, unsigned char ucJ, unsigned char ucK,
    unsigned char ucL, unsigned char ucM, unsigned char ucN,
    unsigned char ucO, unsigned char ucP)
{
    m_aucValue[0] = ucA;
    m_aucValue[1] = ucB;
    m_aucValue[2] = ucC;
    m_aucValue[3] = ucD;
    m_aucValue[4] = ucE;
    m_aucValue[5] = ucF;
    m_aucValue[6] = ucG;
    m_aucValue[7] = ucH;
    m_aucValue[8] = ucI;
    m_aucValue[9] = ucJ;
    m_aucValue[10] = ucK;
    m_aucValue[11] = ucL;
    m_aucValue[12] = ucM;
    m_aucValue[13] = ucN;
    m_aucValue[14] = ucO;
    m_aucValue[15] = ucP;
}
//---------------------------------------------------------------------------
inline bool NiUniqueID::operator==(const NiUniqueID& kUniqueID) const
{
    return (memcmp(&m_aucValue, &kUniqueID.m_aucValue, NUM_BYTES_IN_UNIQUE_ID)
        == 0);
}
//---------------------------------------------------------------------------
inline bool NiUniqueID::operator!=(const NiUniqueID& kUniqueID) const
{
    return !(*this == kUniqueID);
}
//---------------------------------------------------------------------------
