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
//  NiLinesData inline functions

//---------------------------------------------------------------------------
inline NiBool* NiLinesData::GetFlags()
{
    return m_pkFlags;
}
//---------------------------------------------------------------------------
inline const NiBool* NiLinesData::GetFlags() const
{
    return m_pkFlags;
}
//---------------------------------------------------------------------------
inline void NiLinesData::CalculateNormals()
{
    // NiGeometry::CalculateNormals is pure virtual.  Lines do not have
    // normal vectors.
}
//---------------------------------------------------------------------------
