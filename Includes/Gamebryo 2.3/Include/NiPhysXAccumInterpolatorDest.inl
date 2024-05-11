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
inline NiInterpolator* NiPhysXAccumInterpolatorDest::GetAccumTarget() const
{
    return m_pkAccumTarget;
}
//---------------------------------------------------------------------------
inline NiInterpolator* NiPhysXAccumInterpolatorDest::GetNonAccumTarget() const
{
    return m_pkNonAccumTarget;
}
//---------------------------------------------------------------------------
inline NiNode* NiPhysXAccumInterpolatorDest::GetSceneGraphParent() const
{
    return m_pkSceneParent;
}
//---------------------------------------------------------------------------
inline void NiPhysXAccumInterpolatorDest::SetAccumTarget(
    NiInterpolator* pkTarget, NiNode* pkParent)
{
    m_pkAccumTarget = pkTarget;
    m_pkSceneParent = pkParent;
}
//---------------------------------------------------------------------------
inline void NiPhysXAccumInterpolatorDest::SetNonAccumTarget(
    NiInterpolator* pkTarget)
{
    m_pkNonAccumTarget = pkTarget;
}
//---------------------------------------------------------------------------
inline NiBlendAccumTransformInterpolator*
    NiPhysXAccumInterpolatorDest::GetBlendInterpolator() const
{
    return m_pkBlendInterp;
}
//---------------------------------------------------------------------------
inline void NiPhysXAccumInterpolatorDest::SetBlendInterpolator(
    NiBlendAccumTransformInterpolator* pkInterp)
{
    m_pkBlendInterp = pkInterp;
}
//---------------------------------------------------------------------------
inline bool NiPhysXAccumInterpolatorDest::StreamCanSkip()
{
    return true;
}
//---------------------------------------------------------------------------
