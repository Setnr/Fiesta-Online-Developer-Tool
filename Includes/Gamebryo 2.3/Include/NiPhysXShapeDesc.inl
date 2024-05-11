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
inline NxShapeType NiPhysXShapeDesc::GetType() const
{
    return m_eType;
}
//---------------------------------------------------------------------------
inline NxShape* NiPhysXShapeDesc::GetShape() const
{
    return m_pkShape;
}
//---------------------------------------------------------------------------
inline void NiPhysXShapeDesc::SetShape(NxShape* pkShape)
{
    m_pkShape = pkShape;
}
//---------------------------------------------------------------------------
inline NxMat34 NiPhysXShapeDesc::GetLocalPose() const
{
    return m_kLocalPose;
}
//---------------------------------------------------------------------------
inline void NiPhysXShapeDesc::SetLocalPose(const NxMat34& kPose)
{
    m_kLocalPose = kPose;
}
//---------------------------------------------------------------------------
inline void NiPhysXShapeDesc::SetMeshDesc(NiPhysXMeshDesc* pkMeshDesc)
{
    m_spMeshDesc = pkMeshDesc;
}
//---------------------------------------------------------------------------
inline NiPhysXMeshDescPtr NiPhysXShapeDesc::GetMeshDesc()
{
    return m_spMeshDesc;
}   
//---------------------------------------------------------------------------
inline bool NiPhysXShapeDesc::StreamCanSkip()
{
    return true;
}
//---------------------------------------------------------------------------
