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
inline NiScreenElementsData* NiScreenElements::Data()
{
    return NiSmartPointerCast(NiScreenElementsData,m_spModelData);
}
//---------------------------------------------------------------------------
inline const NiScreenElementsData* NiScreenElements::Data() const
{
    return NiSmartPointerCast(NiScreenElementsData,m_spModelData);
}
//---------------------------------------------------------------------------
inline int NiScreenElements::GetMaxPQuantity() const
{
    return Data()->GetMaxPQuantity();
}
//---------------------------------------------------------------------------
inline void NiScreenElements::SetPGrowBy(int iPGrowBy)
{
    Data()->SetPGrowBy(iPGrowBy);
}
//---------------------------------------------------------------------------
inline int NiScreenElements::GetPGrowBy() const
{
    return Data()->GetPGrowBy();
}
//---------------------------------------------------------------------------
inline int NiScreenElements::GetMaxVQuantity() const
{
    return Data()->GetMaxVQuantity();
}
//---------------------------------------------------------------------------
inline void NiScreenElements::SetVGrowBy(int iVGrowBy)
{
    Data()->SetVGrowBy(iVGrowBy);
}
//---------------------------------------------------------------------------
inline int NiScreenElements::GetVGrowBy() const
{
    return Data()->GetPGrowBy();
}
//---------------------------------------------------------------------------
inline int NiScreenElements::GetMaxTQuantity() const
{
    return Data()->GetMaxTQuantity();
}
//---------------------------------------------------------------------------
inline void NiScreenElements::SetTGrowBy(int iTGrowBy)
{
    Data()->SetTGrowBy(iTGrowBy);
}
//---------------------------------------------------------------------------
inline int NiScreenElements::GetTGrowBy() const
{
    return Data()->GetTGrowBy();
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::IsValid(int iPolygon) const
{
    return Data()->IsValid(iPolygon);
}
//---------------------------------------------------------------------------
inline int NiScreenElements::Insert(unsigned short usNumVertices,
    unsigned short usNumTriangles, const unsigned short* ausTriList)
{
    return Data()->Insert(usNumVertices, usNumTriangles, ausTriList);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::Remove(int iPolygon)
{
    return Data()->Remove(iPolygon);
}
//---------------------------------------------------------------------------
inline void NiScreenElements::RemoveAll()
{
    Data()->RemoveAll();
}
//---------------------------------------------------------------------------
inline int NiScreenElements::GetNumPolygons() const
{
    return Data()->GetNumPolygons();
}
//---------------------------------------------------------------------------
inline int NiScreenElements::GetNumVertices(int iPolygon) const
{
    return Data()->GetNumVertices(iPolygon);
}
//---------------------------------------------------------------------------
inline int NiScreenElements::GetNumTriangles(int iPolygon) const
{
    return Data()->GetNumTriangles(iPolygon);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetVertex(int iPolygon, int iVertex,
    const NiPoint2& kValue)
{
    return Data()->SetVertex(iPolygon, iVertex, kValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetVertex(int iPolygon, int iVertex,
    NiPoint2& kValue) const
{
    return Data()->GetVertex(iPolygon, iVertex, kValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetVertices(int iPolygon,
    const NiPoint2* akValue)
{
    return Data()->SetVertices(iPolygon, akValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetVertices(int iPolygon, NiPoint2* akValue)
    const
{
    return Data()->GetVertices(iPolygon, akValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetRectangle(int iPolygon, float fLeft,
    float fTop, float fWidth, float fHeight)
{
    return Data()->SetRectangle(iPolygon, fLeft, fTop, fWidth, fHeight);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetRectangle(int iPolygon, float& fLeft,
    float& fTop, float& fWidth, float& fHeight) const
{
    return Data()->GetRectangle(iPolygon, fLeft, fTop, fWidth, fHeight);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetNormal(int iPolygon, int iVertex,
    const NiPoint3& kValue)
{
    return Data()->SetNormal(iPolygon, iVertex, kValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetNormal(int iPolygon, int iVertex,
    NiPoint3& kValue) const
{
    return Data()->GetNormal(iPolygon, iVertex, kValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetNormals(int iPolygon,
    const NiPoint3* akValue)
{
    return Data()->SetNormals(iPolygon, akValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetNormals(int iPolygon,
    const NiPoint3& kCommonValue)
{
    return Data()->SetNormals(iPolygon, kCommonValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetNormals(int iPolygon, NiPoint3* akValue)
    const
{
    return Data()->GetNormals(iPolygon, akValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetNormals(int iPolygon,
    const NiPoint3& kUpperLeft, const NiPoint3& kLowerLeft,
    const NiPoint3& kLowerRight, const NiPoint3& kUpperRight)
{
    return Data()->SetNormals(iPolygon, kUpperLeft, kLowerLeft, kLowerRight,
        kUpperRight);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetNormals(int iPolygon, NiPoint3& kUpperLeft,
    NiPoint3& kLowerLeft, NiPoint3& kLowerRight, NiPoint3& kUpperRight) const
{
    return Data()->GetNormals(iPolygon, kUpperLeft, kLowerLeft, kLowerRight,
        kUpperRight);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetColor(int iPolygon, int iVertex,
    const NiColorA& kValue)
{
    return Data()->SetColor(iPolygon, iVertex, kValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetColor(int iPolygon, int iVertex,
    NiColorA& kValue) const
{
    return Data()->GetColor(iPolygon, iVertex, kValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetColors(int iPolygon,
    const NiColorA* akValue)
{
    return Data()->SetColors(iPolygon, akValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetColors(int iPolygon,
    const NiColorA& kCommonValue)
{
    return Data()->SetColors(iPolygon, kCommonValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetColors(int iPolygon, NiColorA* akValue)
    const
{
    return Data()->GetColors(iPolygon, akValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetColors(int iPolygon,
    const NiColorA& kUpperLeft, const NiColorA& kLowerLeft,
    const NiColorA& kLowerRight, const NiColorA& kUpperRight)
{
    return Data()->SetColors(iPolygon, kUpperLeft, kLowerLeft, kLowerRight,
        kUpperRight);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetColors(int iPolygon, NiColorA& kUpperLeft,
    NiColorA& kLowerLeft, NiColorA& kLowerRight, NiColorA& kUpperRight) const
{
    return Data()->GetColors(iPolygon, kUpperLeft, kLowerLeft, kLowerRight,
        kUpperRight);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetTexture(int iPolygon, int iVertex,
    unsigned short usSet, const NiPoint2& kValue)
{
    return Data()->SetTexture(iPolygon, iVertex, usSet, kValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetTexture(int iPolygon, int iVertex,
    unsigned short usSet, NiPoint2& kValue) const
{
    return Data()->GetTexture(iPolygon, iVertex, usSet, kValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetTextures(int iPolygon,
    unsigned short usSet, const NiPoint2* akValue)
{
    return Data()->SetTextures(iPolygon, usSet, akValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetTextures(int iPolygon, unsigned short usSet,
    NiPoint2* akValue) const
{
    return Data()->GetTextures(iPolygon, usSet, akValue);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetTextures(int iPolygon, unsigned short usSet,
    const NiPoint2& kUpperLeft, const NiPoint2& kLowerLeft,
    const NiPoint2& kLowerRight, const NiPoint2& kUpperRight)
{
    return Data()->SetTextures(iPolygon, usSet, kUpperLeft, kLowerLeft,
        kLowerRight, kUpperRight);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::SetTextures(int iPolygon, unsigned short usSet,
    float fLeft, float fTop, float fRight, float fBottom)
{
    return Data()->SetTextures(iPolygon, usSet, fLeft, fTop, fRight,
        fBottom);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::GetTextures(int iPolygon, unsigned short usSet,
    NiPoint2& kUpperLeft, NiPoint2& kLowerLeft, NiPoint2& kLowerRight,
    NiPoint2& kUpperRight) const
{
    return Data()->GetTextures(iPolygon, usSet, kUpperLeft, kLowerLeft,
        kLowerRight, kUpperRight);
}
//---------------------------------------------------------------------------
inline NiPoint3* NiScreenElements::GetVertices(int iPolygon)
{
    return Data()->GetVertices(iPolygon);
}
//---------------------------------------------------------------------------
inline const NiPoint3* NiScreenElements::GetVertices(int iPolygon) const
{
    return Data()->GetVertices(iPolygon);
}
//---------------------------------------------------------------------------
inline NiPoint3* NiScreenElements::GetNormals(int iPolygon)
{
    return Data()->GetNormals(iPolygon);
}
//---------------------------------------------------------------------------
inline const NiPoint3* NiScreenElements::GetNormals(int iPolygon) const
{
    return Data()->GetNormals(iPolygon);
}
//---------------------------------------------------------------------------
inline NiColorA* NiScreenElements::GetColors(int iPolygon)
{
    return Data()->GetColors(iPolygon);
}
//---------------------------------------------------------------------------
inline const NiColorA* NiScreenElements::GetColors(int iPolygon) const
{
    return Data()->GetColors(iPolygon);
}
//---------------------------------------------------------------------------
inline NiPoint2* NiScreenElements::GetTextures(int iPolygon,
    unsigned short usSet)
{
    return Data()->GetTextures(iPolygon, usSet);
}
//---------------------------------------------------------------------------
inline const NiPoint2* NiScreenElements::GetTextures(int iPolygon,
    unsigned short usSet) const
{
    return Data()->GetTextures(iPolygon, usSet);
}
//---------------------------------------------------------------------------
inline unsigned short* NiScreenElements::GetIndices(int iPolygon)
{
    return Data()->GetIndices(iPolygon);
}
//---------------------------------------------------------------------------
inline const unsigned short* NiScreenElements::GetIndices(int iPolygon)
    const
{
    return Data()->GetIndices(iPolygon);
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::BoundNeedsUpdate() const
{
    return Data()->BoundNeedsUpdate();
}
//---------------------------------------------------------------------------
inline void NiScreenElements::UpdateBound()
{
    Data()->UpdateBound();
}
//---------------------------------------------------------------------------
inline bool NiScreenElements::Resize(float fXScale, float fYScale)
{
    return Data()->Resize(fXScale, fYScale);
}
//---------------------------------------------------------------------------
