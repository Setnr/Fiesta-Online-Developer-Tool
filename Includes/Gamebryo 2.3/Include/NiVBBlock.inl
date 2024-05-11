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
//  NiVBBlock Inline Functions
//---------------------------------------------------------------------------
inline NiVBBlock::NiVBBlock()
{
    ClearAllData();
}
//---------------------------------------------------------------------------
inline NiVBBlock* NiVBBlock::Create(NiVBSet* pkSet, 
    D3DVertexBufferPtr pkVB, unsigned int uiSize, unsigned int uiFVF, 
    unsigned int uiFlags, D3DPOOL ePool, unsigned int uiUsage, 
    unsigned int uiIndex)
{
    NiVBBlock* pkBlock = NiNew NiVBBlock();
    if (pkBlock)
    {
        pkBlock->SetSet(pkSet);
        pkBlock->SetVB(pkVB);
        pkBlock->SetSize(uiSize);
        pkBlock->SetFVF(uiFVF);
        pkBlock->SetFlags(uiFlags);
        pkBlock->SetIndex(uiIndex);
        pkBlock->SetPool(ePool);
        pkBlock->SetUsage(uiUsage);
    }

    return pkBlock;
}
//---------------------------------------------------------------------------
inline NiVBSet* NiVBBlock::GetSet(void)
{
    return m_pkSet;
}
//---------------------------------------------------------------------------
inline D3DVertexBufferPtr NiVBBlock::GetVB(void)
{
    return m_pkVB;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBBlock::GetSize(void)
{
    return m_uiSize;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBBlock::GetFVF(void)
{
    return m_uiFVF;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBBlock::GetFlags(void)
{
    return m_uiFlags;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBBlock::GetIndex(void)
{
    return m_uiIndex;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBBlock::GetCurrOffset(void)
{
    return m_uiCurrOffset;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBBlock::GetFreeBytes(void)
{
    return m_uiFreeBytes;
}
//---------------------------------------------------------------------------
inline unsigned int NiVBBlock::GetTotalFreeBytes(void)
{
    return m_uiTotalFreeBytes;
}
//---------------------------------------------------------------------------
inline NiVBChip* NiVBBlock::GetChip(unsigned int uiChipIndex)
{
    return m_apkChips.GetAt(uiChipIndex);
}    
//---------------------------------------------------------------------------
inline unsigned int NiVBBlock::GetChipSize(unsigned int uiChipIndex)
{
    NiVBChip* pkVBChip = m_apkChips.GetAt(uiChipIndex);
    if (pkVBChip)
        return pkVBChip->GetSize();

    return 0;
}
//---------------------------------------------------------------------------
inline void NiVBBlock::SetSet(NiVBSet* pkSet)
{
    m_pkSet = pkSet;
}
//---------------------------------------------------------------------------
inline void NiVBBlock::SetVB(D3DVertexBufferPtr pkVB)
{
    m_pkVB = pkVB;
}
//---------------------------------------------------------------------------
inline void NiVBBlock::SetSize(unsigned int uiSize)
{
    m_uiSize = uiSize;

    m_uiFreeBytes = uiSize;
    m_uiTotalFreeBytes = uiSize;
}
//---------------------------------------------------------------------------
inline void NiVBBlock::SetFVF(unsigned int uiFVF)
{
    m_uiFVF = uiFVF;
}
//---------------------------------------------------------------------------
inline void NiVBBlock::SetFlags(unsigned int uiFlags)
{
    m_uiFlags = uiFlags;
}
//---------------------------------------------------------------------------
inline void NiVBBlock::SetIndex(unsigned int uiIndex)
{
    m_uiIndex = uiIndex;
}
//---------------------------------------------------------------------------
inline void NiVBBlock::SetPool(D3DPOOL ePool)
{
    m_ePool = ePool;
}
//---------------------------------------------------------------------------
inline void NiVBBlock::SetUsage(unsigned int uiUsage)
{
    m_uiUsage = uiUsage;
}
//---------------------------------------------------------------------------
inline void NiVBBlock::ClearAllData(void)
{
    SetSet(0);
    SetVB(0);
    SetSize(0);
    SetFVF(0);
    SetFlags(0);
    SetIndex(0);

    m_uiUsage = 0;
    m_ePool = D3DPOOL_DEFAULT;
    m_uiCurrOffset = 0;
    m_uiFreeBytes = 0;
    m_uiTotalFreeBytes = 0;
    m_apkChips.RemoveAll();
    m_pkNextBlock = 0;
    m_pkPrevBlock = 0;
}
//---------------------------------------------------------------------------
