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
//  NiVBChip Inline Functions
//---------------------------------------------------------------------------
inline NiVBChip::NiVBChip()
{
    SetIndex(0);
    SetBlock(0);
    SetVB(0);
    SetOffset(0);
    SetSize(0);
    SetLockFlags(0);

    m_pkNextChip = 0;
    m_pkPrevChip = 0;
}
//---------------------------------------------------------------------------
inline NiVBChip::~NiVBChip()
{
    m_pkVB = 0;
}
//---------------------------------------------------------------------------
inline NiVBChip* NiVBChip::Create(NiVBBlock* pkBlock, 
    D3DVertexBufferPtr pkVB, unsigned int uiOffset, unsigned int uiSize, 
    unsigned int uiIndex)
{
    NiVBChip* pkChip = NiNew NiVBChip();
    if (pkChip)
    {
        pkChip->SetIndex(uiIndex);
        pkChip->SetBlock(pkBlock);
        pkChip->SetVB(pkVB);
        pkChip->SetOffset(uiOffset);
        pkChip->SetSize(uiSize);
        pkChip->SetLockFlags(0);
    }

    return pkChip;
}
//---------------------------------------------------------------------------
inline NiVBBlock* NiVBChip::GetBlock(void)
{
    return m_pkBlock;
}
//---------------------------------------------------------------------------
inline NiVBDynamicSet* NiVBChip::GetDynamicSet()
{
    return m_pkDynamicSet;
}
//---------------------------------------------------------------------------
inline D3DVertexBufferPtr NiVBChip::GetVB(void)
{
    return m_pkVB;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBChip::GetOffset(void)
{
    return m_uiOffset;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBChip::GetLockFlags(void)
{
    return m_uiLockFlags;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBChip::GetSize(void)
{
    return m_uiSize;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBChip::GetIndex(void)
{
    return m_uiIndex;
}
//---------------------------------------------------------------------------
inline NiVBChip* NiVBChip::GetNext()
{
    return m_pkNextChip;
}
//---------------------------------------------------------------------------
inline void NiVBChip::SetNext(NiVBChip* pkChip)
{
    m_pkNextChip = pkChip;
}
//---------------------------------------------------------------------------
inline NiVBChip* NiVBChip::GetPrev()
{
    return m_pkPrevChip;
}
//---------------------------------------------------------------------------
inline void NiVBChip::SetPrev(NiVBChip* pkChip)
{
    m_pkPrevChip = pkChip;
}
//---------------------------------------------------------------------------
inline void NiVBChip::SetBlock(NiVBBlock* pkBlock)
{
    m_pkBlock = pkBlock;
}
//---------------------------------------------------------------------------
inline void NiVBChip::SetDynamicSet(NiVBDynamicSet* pkSet)
{
    m_pkDynamicSet = pkSet;
}
//---------------------------------------------------------------------------
inline void NiVBChip::SetVB(D3DVertexBufferPtr pkVB)
{
    m_pkVB = pkVB;
}
//---------------------------------------------------------------------------
inline void NiVBChip::SetOffset(unsigned int uiOffset)
{
    m_uiOffset = uiOffset;
}
//---------------------------------------------------------------------------
inline void NiVBChip::SetLockFlags(unsigned int uiLockFlags)
{
    m_uiLockFlags = uiLockFlags;
}
//---------------------------------------------------------------------------
inline void NiVBChip::SetSize(unsigned int uiSize)
{
    m_uiSize = uiSize;
}
//---------------------------------------------------------------------------
inline void NiVBChip::SetIndex(unsigned int uiIndex)
{
    m_uiIndex = uiIndex;
}
//---------------------------------------------------------------------------
