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
inline void NiCullingProcess::SetVisibleSet(
    NiVisibleArray* pkVisibleSet)
{
    m_pkVisibleSet = pkVisibleSet;
}
//---------------------------------------------------------------------------
inline NiVisibleArray* NiCullingProcess::GetVisibleSet()
{
    return m_pkVisibleSet;
}
//---------------------------------------------------------------------------
inline const NiCamera* NiCullingProcess::GetCamera() const
{
    return m_pkCamera;
}
//---------------------------------------------------------------------------
inline const NiFrustum& NiCullingProcess::GetFrustum() const
{
    return m_kFrustum;
}
//---------------------------------------------------------------------------
inline const NiFrustumPlanes& NiCullingProcess::GetFrustumPlanes() const
{
    return m_kPlanes;
}
//---------------------------------------------------------------------------
inline  void NiCullingProcess::Append(NiGeometry& kVisible)
{
    NIASSERT(m_pkVisibleSet);
    if (m_bUseVirtualAppend) // need to use the virtual append
        AppendVirtual(kVisible);
    else
        m_pkVisibleSet->Add(kVisible);
}
//---------------------------------------------------------------------------
