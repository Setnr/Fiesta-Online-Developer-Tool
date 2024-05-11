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

//---------------------------------------------------------------------------
//  NiPick member functions
//---------------------------------------------------------------------------
inline void NiPick::SetPickType(PickType ePickType)
{
    m_ePickType = ePickType;
}
//---------------------------------------------------------------------------
inline NiPick::PickType NiPick::GetPickType() const
{
    return m_ePickType;
}
//---------------------------------------------------------------------------
inline void NiPick::SetSortType(SortType eSortType)
{
    m_eSortType = eSortType;
}
//---------------------------------------------------------------------------
inline NiPick::SortType NiPick::GetSortType() const
{
    return m_eSortType;
}
//---------------------------------------------------------------------------
inline void NiPick::SetIntersectType(IntersectType eIntersectType)
{
    m_eIntersectType = eIntersectType;
}
//---------------------------------------------------------------------------
inline NiPick::IntersectType NiPick::GetIntersectType() const
{
    return m_eIntersectType;
}
//---------------------------------------------------------------------------
inline void NiPick::SetCoordinateType(CoordinateType eType)
{
    m_eCoordinateType = eType;
}
//---------------------------------------------------------------------------
inline NiPick::CoordinateType NiPick::GetCoordinateType() const
{
    return m_eCoordinateType;
}
//---------------------------------------------------------------------------
inline void NiPick::SetFrontOnly(bool bFrontOnly)
{
    m_bFrontOnly = bFrontOnly;
}
//---------------------------------------------------------------------------
inline bool NiPick::GetFrontOnly() const
{
    return m_bFrontOnly;
}
//---------------------------------------------------------------------------
inline void NiPick::SetObserveAppCullFlag(bool bObserveAppCullFlag)
{
    m_bObserveAppCullFlag = bObserveAppCullFlag;
}
//---------------------------------------------------------------------------
inline bool NiPick::GetObserveAppCullFlag() const
{
    return m_bObserveAppCullFlag;
}
//---------------------------------------------------------------------------
inline void NiPick::SetTarget(NiAVObject* pObject)
{
    m_spRoot = pObject;
}
//---------------------------------------------------------------------------
inline void NiPick::RemoveTarget()
{
    m_spRoot = 0;
}
//---------------------------------------------------------------------------
inline void NiPick::SetReturnTexture(bool bReturnTexture)
{
    m_bReturnTexture = bReturnTexture;
}
//---------------------------------------------------------------------------
inline bool NiPick::GetReturnTexture() const
{
    return m_bReturnTexture;
}
//---------------------------------------------------------------------------
inline void NiPick::SetReturnNormal(bool bReturnNormal)
{
    m_bReturnNormal = bReturnNormal;
}
//---------------------------------------------------------------------------
inline bool NiPick::GetReturnNormal() const
{
    return m_bReturnNormal;
}
//---------------------------------------------------------------------------
inline void NiPick::SetReturnSmoothNormal(bool bReturnSmoothNormal)
{
    m_bReturnSmoothNormal = bReturnSmoothNormal;
}
//---------------------------------------------------------------------------
inline bool NiPick::GetReturnSmoothNormal() const
{
    return m_bReturnSmoothNormal;
}
//---------------------------------------------------------------------------
inline void NiPick::SetReturnColor(bool bReturnColor)
{
    m_bReturnColor = bReturnColor;
}
//---------------------------------------------------------------------------
inline bool NiPick::GetReturnColor() const
{
    return m_bReturnColor;
}
//---------------------------------------------------------------------------
inline NiPick::Results& NiPick::GetResults()
{
    return m_pickResults;
}
//---------------------------------------------------------------------------
inline const NiPick::Results& NiPick::GetResults() const
{
    return m_pickResults;
}
//---------------------------------------------------------------------------
inline NiPick::Record* NiPick::Add(NiAVObject* pkObject)
{
    return m_pickResults.AddNew(pkObject);
}

//---------------------------------------------------------------------------
inline NiPick::Record* NiPick::GetLastAddedRecord()
{
    return m_pLastAddedRecord;
}
//---------------------------------------------------------------------------
inline unsigned int NiPick::GetSize()
{ 
    return m_pickResults.GetEffectiveSize();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// NiPick::Record member functions
//---------------------------------------------------------------------------
inline NiPick::Record::Record(NiAVObject* pObject)
{
    Initialize(pObject);
}
//---------------------------------------------------------------------------
inline NiAVObject* NiPick::Record::GetAVObject()
{
    return m_spObject;
}
//---------------------------------------------------------------------------
inline void NiPick::Record::Initialize (NiAVObject* pObject)
{
    m_spObject = pObject;
    m_spProxyParent = 0;
}
//---------------------------------------------------------------------------
inline NiNode* NiPick::Record::GetParent()
{
    if ( m_spProxyParent )
        return m_spProxyParent;
    else
        return m_spObject->GetParent();
}
//---------------------------------------------------------------------------
inline void NiPick::Record::SetProxyParent(NiNode* pProxyParent)
{
    m_spProxyParent = pProxyParent;
}
//---------------------------------------------------------------------------
inline void NiPick::Record::SetIntersection(const NiPoint3& intersect)
{
    m_intersect = intersect;
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiPick::Record::GetIntersection() const
{
    return m_intersect;
}
//---------------------------------------------------------------------------
inline void NiPick::Record::SetDistance(float fDistance)
{
    m_fDistance = fDistance;
}
//---------------------------------------------------------------------------
inline float NiPick::Record::GetDistance() const
{
    return m_fDistance;
}
//---------------------------------------------------------------------------
inline void NiPick::Record::SetTriangleIndex(unsigned short usIndex)
{
    m_usTriangleIndex = usIndex;
}
//---------------------------------------------------------------------------
inline unsigned short NiPick::Record::GetTriangleIndex() const
{
    return m_usTriangleIndex;
}
//---------------------------------------------------------------------------
inline void NiPick::Record::SetVertexIndices(unsigned short usIndex0,
    unsigned short usIndex1, unsigned short usIndex2)

{
    m_usVertexIndex[0] = usIndex0;
    m_usVertexIndex[1] = usIndex1;
    m_usVertexIndex[2] = usIndex2;
}
//---------------------------------------------------------------------------
inline void NiPick::Record::GetVertexIndices(unsigned short& usIndex0,
    unsigned short& usIndex1, unsigned short& usIndex2) const
{
    usIndex0 = m_usVertexIndex[0];
    usIndex1 = m_usVertexIndex[1];
    usIndex2 = m_usVertexIndex[2];
}
//---------------------------------------------------------------------------
inline void NiPick::Record::SetTexture(const NiPoint2& texture)
{
    m_texture = texture;
}
//---------------------------------------------------------------------------
inline const NiPoint2& NiPick::Record::GetTexture() const
{
    return m_texture;
}
//---------------------------------------------------------------------------
inline void NiPick::Record::SetNormal(const NiPoint3& normal)
{
    m_normal = normal;
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiPick::Record::GetNormal() const
{
    return m_normal;
}
//---------------------------------------------------------------------------
inline void NiPick::Record::SetColor(const NiColorA& color)
{
    m_color = color;
}
//---------------------------------------------------------------------------
inline const NiColorA& NiPick::Record::GetColor() const
{
    return m_color;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// NiPick::Results member functions
//---------------------------------------------------------------------------
inline NiPick::Results::Results(unsigned int uiInitialSize, 
    unsigned int uiGrowBy) :
    NiTPrimitiveArray<Record*>(uiInitialSize, uiGrowBy),
    m_uiValidResults(0)
{
}
//---------------------------------------------------------------------------
inline NiPick::Results::~Results()
{
    for (unsigned int ui = 0; ui < NiTPrimitiveArray<Record*>::GetSize(); 
        ui++)
    {
        NiDelete NiTPrimitiveArray<Record*>::GetAt(ui);
    }
}
//---------------------------------------------------------------------------
inline unsigned int NiPick::Results::GetSize() const
{
    return m_uiValidResults;
}
//---------------------------------------------------------------------------
inline unsigned int NiPick::Results::GetEffectiveSize() const
{
    return m_uiValidResults;
}
//---------------------------------------------------------------------------
inline NiPick::Record* NiPick::Results::GetAt (unsigned int uiIndex) const
{
    return NiTPrimitiveArray<Record*>::GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline NiPick::Record* NiPick::Results::RemoveAtAndFill(unsigned int uiIndex)
{
    // calling remove on an invalid index?
    NIASSERT(m_uiValidResults && uiIndex < m_uiValidResults);

    // remove at uiIndex and fill from the last valid result (decrementing
    // the number of valid results)

    Record* pkRec = NiTPrimitiveArray<Record*>::RemoveAt(uiIndex);
    m_uiValidResults--;

    // Since array is packed tightly with valid records and then invalid
    // records, we need to do two swaps to maintain this condition.

    // swap last valid record with uiIndex
    NiTPrimitiveArray<Record*>::SetAt(uiIndex, GetAt(m_uiValidResults));
    // swap last invalid record with last record
    const unsigned int uiLast = NiTPrimitiveArray<Record*>::GetSize() - 1;
    NiTPrimitiveArray<Record*>::SetAt(m_uiValidResults,
        NiTPrimitiveArray<Record*>::GetAt(uiLast));
    // set last invalid record to zero
    NiTPrimitiveArray<Record*>::SetAt(uiLast, NULL);

    // decrement number of used array elements
    m_usSize--;

    return pkRec;
}
//---------------------------------------------------------------------------
inline void NiPick::Results::ClearArray()
{
    m_uiValidResults = 0;
}
//---------------------------------------------------------------------------
inline NiPick::Record* NiPick::Results::AddNew(NiAVObject* pObject)
{
    Record* pkRecord;
    if (m_uiValidResults < NiTPrimitiveArray<Record*>::GetSize())
    {
        // Previously allocated record.  Use it.
        pkRecord = NiTPrimitiveArray<Record*>::GetAt(m_uiValidResults);
        NIASSERT(pkRecord);
        pkRecord->Initialize(pObject);
    }
    else
    {
        // There doesn't exist any records for us to use.  Allocate a new
        // one and put it on the end of the array.
        pkRecord = NiNew NiPick::Record(pObject);
        NIASSERT(pkRecord);
        Add(pkRecord);
    }

    m_uiValidResults++;
    return pkRecord;
}
//---------------------------------------------------------------------------
