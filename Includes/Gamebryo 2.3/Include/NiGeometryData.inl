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
//  NiGeometryData inline functions

//---------------------------------------------------------------------------
inline unsigned short NiGeometryData::GetVertexCount() const
{
    return m_usVertices;
}
//---------------------------------------------------------------------------
inline const NiPoint3* NiGeometryData::GetVertices() const
{
    return m_pkVertex;
}
//---------------------------------------------------------------------------
inline const NiPoint3* NiGeometryData::GetNormals() const
{
    return m_pkNormal;
}
//---------------------------------------------------------------------------
inline NiPoint3* NiGeometryData::GetVertices()
{
    return m_pkVertex;
}
//---------------------------------------------------------------------------
inline NiPoint3* NiGeometryData::GetNormals()
{
    return m_pkNormal;
}
//---------------------------------------------------------------------------
inline NiGeometryData::DataFlags 
    NiGeometryData::GetNormalBinormalTangentMethod() const
{
    return (DataFlags)(m_usDataFlags & NBT_METHOD_MASK);
}
//---------------------------------------------------------------------------
inline void NiGeometryData::SetNormalBinormalTangentMethod(
    DataFlags eNBTMethod)
{
    m_usDataFlags = (m_usDataFlags & ~NBT_METHOD_MASK) | eNBTMethod;
}
//---------------------------------------------------------------------------
inline void NiGeometryData::SetBound(const NiBound& kBound)
{
    m_kBound = kBound;
}
//---------------------------------------------------------------------------
inline NiBound& NiGeometryData::GetBound() 
{
    return m_kBound;
}
//---------------------------------------------------------------------------
inline const NiBound& NiGeometryData::GetBound() const
{
    return m_kBound;
}
//---------------------------------------------------------------------------
inline NiColorA* NiGeometryData::GetColors()
{
    return m_pkColor;
}
//---------------------------------------------------------------------------
inline const NiColorA* NiGeometryData::GetColors() const
{
    return m_pkColor;
}
//---------------------------------------------------------------------------
inline unsigned short NiGeometryData::GetTextureSets() const
{
    return (m_usDataFlags & TEXTURE_SET_MASK);
}
//---------------------------------------------------------------------------
inline NiPoint2* NiGeometryData::GetTextures()
{
    return m_pkTexture;
}
//---------------------------------------------------------------------------
inline const NiPoint2* NiGeometryData::GetTextures() const
{
    return m_pkTexture;
}
//---------------------------------------------------------------------------
inline unsigned short NiGeometryData::GetRevisionID() const
{
    return (m_usDirtyFlags & DIRTY_MASK);
}
//---------------------------------------------------------------------------
inline void NiGeometryData::MarkAsChanged(unsigned short usFlags)
{
    NIASSERT(GetConsistency() != STATIC);
    m_usDirtyFlags |= (usFlags & DIRTY_MASK);
}
//---------------------------------------------------------------------------
inline void NiGeometryData::ClearRevisionID()
{
    m_usDirtyFlags &= ~DIRTY_MASK;
}
//---------------------------------------------------------------------------
inline unsigned short NiGeometryData::GetUniqueID() const
{
    return m_usID;
}
//---------------------------------------------------------------------------
inline NiGeometryData::Consistency NiGeometryData::GetConsistency() const
{
    return (Consistency)(m_usDirtyFlags & CONSISTENCY_MASK);
}
//---------------------------------------------------------------------------
inline void NiGeometryData::SetConsistency(
    NiGeometryData::Consistency eConsistency)
{
    m_usDirtyFlags = (m_usDirtyFlags & ~CONSISTENCY_MASK) | eConsistency;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryData::GetKeepFlags() const
{
    return m_ucKeepFlags;
}
//---------------------------------------------------------------------------
inline void NiGeometryData::SetKeepFlags(unsigned int uiKeepFlags)
{
    NIASSERT((uiKeepFlags & ~KEEP_ALL) == 0);
    m_ucKeepFlags = uiKeepFlags;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeometryData::GetCompressFlags() const
{
    return m_ucCompressFlags;
}
//---------------------------------------------------------------------------
inline void NiGeometryData::SetCompressFlags(unsigned int uiCompressFlags)
{
    NIASSERT((uiCompressFlags & ~COMPRESS_ALL) == 0);
    m_ucCompressFlags = uiCompressFlags;
}
//---------------------------------------------------------------------------
inline void NiGeometryData::SetNumTextureSets(unsigned short usNumTextureSets)
{
    NIASSERT(usNumTextureSets < 64);
    m_usDataFlags = (m_usDataFlags & ~TEXTURE_SET_MASK) | 
        (usNumTextureSets & TEXTURE_SET_MASK);
}
//---------------------------------------------------------------------------
inline void NiGeometryData::SetActiveVertexCount(unsigned short /*usActive*/)
{
    // stub for derived classes
}
//---------------------------------------------------------------------------
inline unsigned short NiGeometryData::GetActiveVertexCount() const
{
    return m_usActiveVertices;
}
//---------------------------------------------------------------------------
inline NiAdditionalGeometryData* NiGeometryData::GetAdditionalGeometryData()
{
    return m_spAdditionalGeomData;
}
//---------------------------------------------------------------------------
inline NiGeometryData::RendererData* NiGeometryData::GetRendererData() const
{
    return m_pkBuffData;
}
//---------------------------------------------------------------------------
inline void NiGeometryData::SetRendererData(
    NiGeometryData::RendererData* pkBuffData)
{
    m_pkBuffData = pkBuffData;
}
//---------------------------------------------------------------------------
inline void NiGeometryData::SetVertexCount(unsigned short usVertices)
{
    // Unless a derived class overrides this, all vertices are considered
    // active
    m_usVertices = usVertices;
    m_usActiveVertices = usVertices;
}
//---------------------------------------------------------------------------
