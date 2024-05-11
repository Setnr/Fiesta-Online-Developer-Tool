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
inline void NiPSysMeshEmitter::RemoveGeometryEmitter(
    unsigned int uiWhichEmitter)
{
    if (uiWhichEmitter < GetGeometryEmitterCount())
    {
        m_kGeomEmitterArray.RemoveAtAndFill(uiWhichEmitter);
        m_kSkinnedEmitterData.RemoveAtAndFill(uiWhichEmitter);
    }
}
//---------------------------------------------------------------------------
inline void NiPSysMeshEmitter::RemoveAllGeometryEmitters()
{
    m_kGeomEmitterArray.RemoveAll();
    m_kSkinnedEmitterData.RemoveAll();
}
//---------------------------------------------------------------------------
inline NiGeometry* NiPSysMeshEmitter::GetGeometryEmitter(
    unsigned int uiWhichEmitter)
{
    if (uiWhichEmitter < GetGeometryEmitterCount())
        return m_kGeomEmitterArray.GetAt(uiWhichEmitter);
    else 
        return NULL;
}
//---------------------------------------------------------------------------
inline unsigned int NiPSysMeshEmitter::GetGeometryEmitterCount()
{
    return m_kGeomEmitterArray.GetEffectiveSize();
}
//---------------------------------------------------------------------------
inline unsigned int NiPSysMeshEmitter::GetGeometryEmitterArrayCount()
{
    return m_kGeomEmitterArray.GetSize();
}
//---------------------------------------------------------------------------
inline NiPSysMeshEmitter::NiSkinnedEmitterData*
    NiPSysMeshEmitter::GetSkinnedEmitterData(unsigned int uiWhichEmitter)
{
    if (uiWhichEmitter < GetSkinnedEmitterDataCount())
        return m_kSkinnedEmitterData.GetAt(uiWhichEmitter);
    else 
        return NULL;
    
}
//---------------------------------------------------------------------------
inline unsigned int NiPSysMeshEmitter::GetSkinnedEmitterDataCount()
{
    return m_kSkinnedEmitterData.GetEffectiveSize();
}

//---------------------------------------------------------------------------
inline NiPSysMeshEmitter::MeshEmissionType
    NiPSysMeshEmitter::GetMeshEmissionType()
{
    return m_eEmissionType;
}
//---------------------------------------------------------------------------
inline void NiPSysMeshEmitter::SetMeshEmissionType(
    NiPSysMeshEmitter::MeshEmissionType eMeshEmissionType)
{
    m_eEmissionType = eMeshEmissionType;
}
//---------------------------------------------------------------------------
inline NiPSysMeshEmitter::InitialVelocityType 
    NiPSysMeshEmitter::GetInitialVelocityType()
{
    return m_eInitVelocityType;
}
//---------------------------------------------------------------------------
inline void NiPSysMeshEmitter::SetInitialVelocityType(
    NiPSysMeshEmitter::InitialVelocityType eInitialVelocityType)
{
    m_eInitVelocityType = eInitialVelocityType;
}
//---------------------------------------------------------------------------
inline const NiPoint3& NiPSysMeshEmitter::GetEmitAxis()
{
    return m_kEmitAxis;
}
//---------------------------------------------------------------------------
inline void NiPSysMeshEmitter::SetEmitAxis(const NiPoint3& kEmitAxis)
{
    m_kEmitAxis = kEmitAxis;
}
//---------------------------------------------------------------------------
