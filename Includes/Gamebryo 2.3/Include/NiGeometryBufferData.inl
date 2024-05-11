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
//  NiGeometryBufferData Inline Functions
//---------------------------------------------------------------------------
inline NiGeometryBufferData::NiGeometryBufferData() :
    m_uiFlags(0),
    m_pkGeometryGroup(NULL),
    m_uiFVF(0),
    m_hDeclaration(0),
    m_bSoftwareVP(false),
    m_uiVertCount(0),
    m_uiMaxVertCount(0),
    m_uiStreamCount(0),
    m_puiVertexStride(NULL),
    m_ppkVBChip(NULL),
    m_uiIndexCount(0),
    m_uiIBSize(0),
    m_pkIB(NULL),
    m_uiBaseVertexIndex(0),
    m_eType(D3DPT_TRIANGLELIST),
    m_uiTriCount(0),
    m_uiMaxTriCount(0),
    m_uiNumArrays(0),
    m_pusArrayLengths(NULL),
    m_pusIndexArray(NULL)
{ /* */ }
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetFlags() const
{
    return m_uiFlags;
}
//---------------------------------------------------------------------------
inline NiGeometryGroup* NiGeometryBufferData::GetGeometryGroup() const
{
    return m_pkGeometryGroup;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetFVF() const
{
    return m_uiFVF;
}
//---------------------------------------------------------------------------
inline NiD3DVertexDeclaration NiGeometryBufferData::GetVertexDeclaration() 
    const
{
    return m_hDeclaration;
}
//---------------------------------------------------------------------------
inline bool NiGeometryBufferData::GetSoftwareVP() const
{
    return m_bSoftwareVP;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetSoftwareVP(bool bSoftwareVP)
{
    m_bSoftwareVP = bSoftwareVP;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetVertCount() const
{
    return m_uiVertCount;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetMaxVertCount() const
{
    return m_uiMaxVertCount;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetStreamCount() const
{
    return m_uiStreamCount;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetVertexStride(
    unsigned int uiIndex) const
{
    if (uiIndex < m_uiStreamCount)
        return m_puiVertexStride[uiIndex];
    else
        return 0;
}
//---------------------------------------------------------------------------
inline NiVBChip* NiGeometryBufferData::GetVBChip(unsigned int uiIndex) const
{
    if (uiIndex < m_uiStreamCount)
        return m_ppkVBChip[uiIndex];
    else
        return NULL;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::RemoveVBChip(unsigned int uiIndex)
{
    if (uiIndex < m_uiStreamCount)
        m_ppkVBChip[uiIndex] = NULL;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::RemoveAllVBChips()
{
    if (m_ppkVBChip)
        memset(m_ppkVBChip, 0, m_uiStreamCount * sizeof(*m_ppkVBChip));
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetIndexCount() const
{
    return m_uiIndexCount;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetIBSize() const
{
    return m_uiIBSize;
}
//---------------------------------------------------------------------------
inline D3DIndexBufferPtr NiGeometryBufferData::GetIB() const
{
    return m_pkIB;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetBaseVertexIndex() const
{
    return m_uiBaseVertexIndex;
}
//---------------------------------------------------------------------------
inline D3DPRIMITIVETYPE NiGeometryBufferData::GetType() const
{
    return m_eType;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetTriCount() const
{
    return m_uiTriCount;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetMaxTriCount() const
{
    return m_uiMaxTriCount;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryBufferData::GetNumArrays() const
{
    return m_uiNumArrays;
}
//---------------------------------------------------------------------------
inline const unsigned short* NiGeometryBufferData::GetArrayLengths() const
{
    return m_pusArrayLengths;
}
//---------------------------------------------------------------------------
inline const unsigned short* NiGeometryBufferData::GetIndexArray() const
{
    return m_pusIndexArray;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetFlags(unsigned int uiFlags)
{
    m_uiFlags = uiFlags;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetGeometryGroup(NiGeometryGroup* pkGroup)
{
    m_pkGeometryGroup = pkGroup;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetVertCount(unsigned int uiVertCount, 
    unsigned int uiMaxVertCount)
{
    NIASSERT(uiMaxVertCount >= uiVertCount);
    m_uiVertCount = uiVertCount;
    m_uiMaxVertCount = uiMaxVertCount;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetBaseVertexIndex(
    unsigned int uiBaseVertexIndex)
{
    m_uiBaseVertexIndex = uiBaseVertexIndex;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetVBChip(unsigned int uiStream, 
    NiVBChip* pkChip)
{
    if (uiStream < m_uiStreamCount)
        m_ppkVBChip[uiStream] = pkChip;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetVertexStride(unsigned int uiStream, 
    unsigned int uiStride)
{
    if (uiStream < m_uiStreamCount)
        m_puiVertexStride[uiStream] = uiStride;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetIB(D3DIndexBufferPtr pkIB, 
    unsigned int uiIndexCount, unsigned int uiIBSize)
{
    m_pkIB = pkIB;
    m_uiIndexCount = uiIndexCount;
    m_uiIBSize = uiIBSize;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetType(D3DPRIMITIVETYPE ePrimType)
{
    m_eType = ePrimType;
}
//---------------------------------------------------------------------------
inline void NiGeometryBufferData::SetIndices(unsigned int uiTriCount, 
    unsigned int uiMaxTriCount, const unsigned short* pusIndexArray, 
    const unsigned short* pusArrayLengths, unsigned int uiNumArrays)
{
    NIASSERT(uiMaxTriCount >= uiTriCount);
    m_uiTriCount = uiTriCount;
    m_uiMaxTriCount = uiMaxTriCount;
    m_pusIndexArray = pusIndexArray;
    m_pusArrayLengths = pusArrayLengths;
    m_uiNumArrays = uiNumArrays;
}
//---------------------------------------------------------------------------
