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
inline NiScene::NiScene(const NiFixedString& kName,
    unsigned int uiEntityArraySize, unsigned int uiSelectionSetArraySize) :
    m_kName(kName), m_kEntities(uiEntityArraySize),
    m_kSelectionSets(uiSelectionSetArraySize)
{
}
//---------------------------------------------------------------------------
inline const NiFixedString& NiScene::GetName() const
{
    return m_kName;
}
//---------------------------------------------------------------------------
inline void NiScene::SetName(const NiFixedString& kName)
{
    m_kName = kName;
}
//---------------------------------------------------------------------------
inline unsigned int NiScene::GetEntityCount() const
{
    return m_kEntities.GetSize();
}
//---------------------------------------------------------------------------
inline NiEntityInterface* NiScene::GetEntityAt(unsigned int uiIndex) const
{
    NIASSERT(uiIndex < m_kEntities.GetSize());
    return m_kEntities.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline NiEntityInterface* NiScene::GetEntityByName(const NiFixedString& kName) 
    const
{
    unsigned int uiSize = m_kEntities.GetSize();
    for (unsigned int ui = 0; ui < uiSize; ui++)
    {
        NiEntityInterface* pkEntity = m_kEntities.GetAt(ui);
        if (pkEntity->GetName() == kName)
        {
            return pkEntity;
        }
    }

    return NULL;
}
//---------------------------------------------------------------------------
inline NiBool NiScene::AddEntity(NiEntityInterface* pkEntity)
{
    NIASSERT(pkEntity);

    NIASSERT(GetEntityByName(pkEntity->GetName()) == NULL);

    m_kEntities.Add(pkEntity);
    m_kQuickEntityLookUpMap.SetAt(pkEntity, true);
    return true;
}
//---------------------------------------------------------------------------
inline void NiScene::RemoveEntity(NiEntityInterface* pkEntity)
{
    NIASSERT(pkEntity);
    int iIndex = m_kEntities.Find(pkEntity);
    if (iIndex > -1)
    {
        m_kEntities.RemoveAt(iIndex);
        m_kQuickEntityLookUpMap.RemoveAt(pkEntity);
    }
}
//---------------------------------------------------------------------------
inline void NiScene::RemoveEntityAt(unsigned int uiIndex)
{
    NIASSERT(uiIndex < m_kEntities.GetSize());
    m_kQuickEntityLookUpMap.RemoveAt(m_kEntities.GetAt(uiIndex));
    m_kEntities.RemoveAt(uiIndex);
}
//---------------------------------------------------------------------------
inline void NiScene::RemoveAllEntities()
{
    m_kEntities.RemoveAll();
    m_kQuickEntityLookUpMap.RemoveAll();
}
//---------------------------------------------------------------------------
inline NiBool NiScene::IsEntityInScene(NiEntityInterface* pkEntity) const
{
    bool InScene;
    return m_kQuickEntityLookUpMap.GetAt(pkEntity, InScene);
}
//---------------------------------------------------------------------------
inline unsigned int NiScene::GetSelectionSetCount() const
{
    return m_kSelectionSets.GetSize();
}
//---------------------------------------------------------------------------
inline NiEntitySelectionSet* NiScene::GetSelectionSetAt(unsigned int uiIndex)
    const
{
    NIASSERT(uiIndex < m_kSelectionSets.GetSize());
    return m_kSelectionSets.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline NiEntitySelectionSet* NiScene::GetSelectionSetByName(
    const NiFixedString& kName) const
{
    unsigned int uiSize = m_kSelectionSets.GetSize();
    for (unsigned int ui = 0; ui < uiSize; ui++)
    {
        NiEntitySelectionSet* pkSelectionSet = m_kSelectionSets.GetAt(ui);
        if (pkSelectionSet->GetName() == kName)
        {
            return pkSelectionSet;
        }
    }

    return NULL;
}
//---------------------------------------------------------------------------
inline NiBool NiScene::AddSelectionSet(NiEntitySelectionSet* pkSelectionSet)
{
    NIASSERT(pkSelectionSet);

    if (GetSelectionSetByName(pkSelectionSet->GetName()) != NULL)
    {
        return false;
    }

    m_kSelectionSets.Add(pkSelectionSet);
    return true;
}
//---------------------------------------------------------------------------
inline void NiScene::RemoveSelectionSet(NiEntitySelectionSet* pkSelectionSet)
{
    NIASSERT(pkSelectionSet);
    int iIndex = m_kSelectionSets.Find(pkSelectionSet);
    if (iIndex > -1)
    {
        m_kSelectionSets.RemoveAt(iIndex);
    }
}
//---------------------------------------------------------------------------
inline void NiScene::RemoveSelectionSetAt(unsigned int uiIndex)
{
    NIASSERT(uiIndex < m_kSelectionSets.GetSize());
    m_kSelectionSets.RemoveAt(uiIndex);
}
//---------------------------------------------------------------------------
inline void NiScene::RemoveAllSelectionSets()
{
    m_kSelectionSets.RemoveAll();
}
//---------------------------------------------------------------------------
inline void NiScene::GetBound(NiBound& kBound)
{
    NiTPrimitiveArray<const NiBound*> kSelectionBounds;

    unsigned int uiSize = m_kEntities.GetSize();
    for (unsigned int ui = 0; ui < uiSize; ui++)
    {
        NiEntityInterface* pkEntity = m_kEntities.GetAt(ui);
        if (pkEntity != NULL)
        {
            NiAVObject* pkScene;
            NiObject* pkObject;
            if (pkEntity->GetPropertyData(ms_kSceneRootName, pkObject, 0))
            {
                pkScene = NiDynamicCast(NiAVObject, pkObject);
                if (pkScene)
                {
                    pkScene->UpdateNodeBound();
                    kSelectionBounds.Add(&(pkScene->GetWorldBound()));
                }
            }
        }
    }
    if (kSelectionBounds.GetEffectiveSize() == 0)
    {
        kBound.SetCenterAndRadius(NiPoint3::ZERO, 0.0f);
    }
    else
    {
        kBound.ComputeMinimalBound(kSelectionBounds);
    }
}
//---------------------------------------------------------------------------
inline void NiScene::Update(float fTime, NiEntityErrorInterface* pkErrors,
    NiExternalAssetManager* pkAssetManager)
{
    unsigned int uiSize = m_kEntities.GetSize();
    for (unsigned int ui = 0; ui < uiSize; ui++)
    {
        m_kEntities.GetAt(ui)->Update(NULL, fTime, pkErrors, pkAssetManager);
    }
}
//---------------------------------------------------------------------------
inline void NiScene::BuildVisibleSet(NiEntityRenderingContext*
    pkRenderingContext, NiEntityErrorInterface* pkErrors)
{
    unsigned int uiSize = m_kEntities.GetSize();
    for (unsigned int ui = 0; ui < uiSize; ui++)
    {
        m_kEntities.GetAt(ui)->BuildVisibleSet(pkRenderingContext, pkErrors);
    }
}
//---------------------------------------------------------------------------
