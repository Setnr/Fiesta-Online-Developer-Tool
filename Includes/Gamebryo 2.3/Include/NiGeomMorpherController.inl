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

#include <NiGeometry.h>

//---------------------------------------------------------------------------
inline NiGeomMorpherController::InterpItem::InterpItem() : m_fWeight(0.0f)
{
}
//---------------------------------------------------------------------------
inline void NiGeomMorpherController::SetMorphData(NiMorphData* pkData,
    bool bCalculateMorphBound)
{
    m_spMorphData = pkData;
    AllocateInterpArray();

    if (bCalculateMorphBound)
    {
        CalculateMorphBound();
    }
}
//---------------------------------------------------------------------------
inline NiMorphData* NiGeomMorpherController::GetMorphData() const
{
    return m_spMorphData;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeomMorpherController::GetNumTargets() const
{
    return m_spMorphData ? m_spMorphData->GetNumTargets() : 0;
}
//---------------------------------------------------------------------------
inline unsigned int NiGeomMorpherController::GetNumVertsPerTarget() const
{
    return m_spMorphData ? m_spMorphData->GetNumVertsPerTarget() : 0;
}
//---------------------------------------------------------------------------
inline void NiGeomMorpherController::SetUpdateNormals(bool bUpdateNormals)
{
    SetBit(bUpdateNormals, UPDATENORMAL_MASK);
}
//---------------------------------------------------------------------------
inline bool NiGeomMorpherController::GetUpdateNormals() const
{
    return GetBit(UPDATENORMAL_MASK);
}
//---------------------------------------------------------------------------
inline void NiGeomMorpherController::SetAlwaysUpdate(bool bAlwaysUpdate)
{
    m_bAlwaysUpdate = bAlwaysUpdate;
}
//---------------------------------------------------------------------------
inline void NiGeomMorpherController::SetAlwaysUpdateClone(
    bool bAlwaysUpdateClone)
{
    ms_bAlwaysUpdateClone = bAlwaysUpdateClone;
}
//---------------------------------------------------------------------------
inline NiMorphData::MorphTarget* NiGeomMorpherController::GetTargets()
    const
{
    return m_spMorphData->GetTargets();
}
//---------------------------------------------------------------------------
inline NiMorphData::MorphTarget* NiGeomMorpherController::GetTarget(
    unsigned int uiIndex) const
{
    return m_spMorphData->GetTarget(uiIndex);
}
//---------------------------------------------------------------------------
inline void NiGeomMorpherController::SetIncrementalUpdate(bool bIncrement)
{
    if (m_bIgnoreBaseTarget != bIncrement)
    {
        if (bIncrement)
            m_spMorphData->SetAndAdjustRelativeTargets(bIncrement);

        m_bIgnoreBaseTarget = bIncrement;
    }
}
//---------------------------------------------------------------------------
inline bool NiGeomMorpherController::GetIncrementalUpdate() const
{
    return m_bIgnoreBaseTarget;
}
//---------------------------------------------------------------------------
inline bool NiGeomMorpherController::IsVertexController() const
{
    return true;
}
//---------------------------------------------------------------------------
inline bool NiGeomMorpherController::TargetIsRequiredType() const
{
    return NiIsKindOf(NiGeometry,m_pkTarget);
}
//---------------------------------------------------------------------------
inline float NiGeomMorpherController::GetWeight(unsigned int uiIndex)
{
    float fWeight = 0.0f;

    if (m_pkInterpItems)
    {
        NIASSERT(uiIndex < GetNumTargets());
        fWeight = m_pkInterpItems[uiIndex].m_fWeight;
    }

    return fWeight;
}
//---------------------------------------------------------------------------
inline void NiGeomMorpherController::SetWeight(unsigned int uiIndex,
    float fWeight)
{
    if (m_pkInterpItems)
    {
        NIASSERT(uiIndex < GetNumTargets());
        m_pkInterpItems[uiIndex].m_fWeight = fWeight;
    }
}
//---------------------------------------------------------------------------
inline NiPoint3* NiGeomMorpherController::GetVertexData()
{
    if (m_pkTarget)
    {
        return ((NiGeometry*)m_pkTarget)->GetVertices();
    }

    return NULL;
}
//---------------------------------------------------------------------------
inline void NiGeomMorpherController::AllocateInterpArray()
{
    NiDelete[] m_pkInterpItems;
    m_pkInterpItems = NULL;

    if (m_spMorphData && m_spMorphData->GetNumTargets() > 0)
    {
        m_pkInterpItems = NiNew InterpItem[m_spMorphData->GetNumTargets()];
    }
}
//---------------------------------------------------------------------------
inline void NiGeomMorpherController::SetParallelUpdateEnabled(bool bEnable)
{
    ms_bParallelUpdateEnabled = bEnable;
}
//---------------------------------------------------------------------------
inline bool NiGeomMorpherController::GetParallelUpdateEnabled()
{
    return ms_bParallelUpdateEnabled;
}
//---------------------------------------------------------------------------
inline void NiGeomMorpherController::SetParallelUpdatePriority(
    NiTaskManager::TaskPriority ePriority)
{
    ms_eParallelUpdatePriority = ePriority;
}
//---------------------------------------------------------------------------
inline NiTaskManager::TaskPriority 
    NiGeomMorpherController::GetParallelUpdatePriority()
{
    return ms_eParallelUpdatePriority;
}
//---------------------------------------------------------------------------


