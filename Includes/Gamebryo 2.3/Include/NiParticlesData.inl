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
//  NiParticlesData inline functions

//---------------------------------------------------------------------------
inline float* NiParticlesData::GetRadii()
{
    return m_pfRadii;
}
//---------------------------------------------------------------------------
inline const float* NiParticlesData::GetRadii() const
{
    return m_pfRadii;
}
//---------------------------------------------------------------------------
inline float* NiParticlesData::GetSizes()
{
    return m_pfSizes;
}
//---------------------------------------------------------------------------
inline const float* NiParticlesData::GetSizes() const
{
    return m_pfSizes;
}
//---------------------------------------------------------------------------
inline NiQuaternion* NiParticlesData::GetRotations()
{
    return m_pkRotations;
}
//---------------------------------------------------------------------------
inline const NiQuaternion* NiParticlesData::GetRotations() const
{
    return m_pkRotations;
}
//---------------------------------------------------------------------------
inline float* NiParticlesData::GetRotationAngles()
{
    return m_pfRotationAngles;
}
//---------------------------------------------------------------------------
inline const float* NiParticlesData::GetRotationAngles() const
{
    return m_pfRotationAngles;
}
//---------------------------------------------------------------------------
inline NiPoint3* NiParticlesData::GetRotationAxes()
{
    return m_pkRotationAxes;
}
//---------------------------------------------------------------------------
inline const NiPoint3* NiParticlesData::GetRotationAxes() const
{
    return m_pkRotationAxes;
}
//---------------------------------------------------------------------------
inline void NiParticlesData::SetRadiiData(float* pfRadii)
{
    m_pfRadii = pfRadii;
}
//---------------------------------------------------------------------------
inline void NiParticlesData::SetSizesData(float* pfSizes)
{
    m_pfSizes = pfSizes;
}
//---------------------------------------------------------------------------
inline void NiParticlesData::SetRotationsData(NiQuaternion* pkRotations)
{
    m_pkRotations = pkRotations;
}
//---------------------------------------------------------------------------
inline void NiParticlesData::SetRotationAnglesData(float* pfRotationAngles)
{
    m_pfRotationAngles = pfRotationAngles;
}
//---------------------------------------------------------------------------
inline void NiParticlesData::SetRotationAxesData(NiPoint3* pkRotationAxes)
{
    m_pkRotationAxes = pkRotationAxes;
}
//---------------------------------------------------------------------------
inline void NiParticlesData::SetActiveVertexCount(unsigned short usActive)
{
    if (usActive <= m_usVertices)
        m_usActiveVertices = usActive;
    else
        m_usActiveVertices = m_usVertices;
}
//---------------------------------------------------------------------------
inline void NiParticlesData::CalculateNormals()
{
    // NiGeometryData::CalculateNormals is pure virtual.  Particles do not
    // know how to compute their normal vectors.
}
//---------------------------------------------------------------------------
