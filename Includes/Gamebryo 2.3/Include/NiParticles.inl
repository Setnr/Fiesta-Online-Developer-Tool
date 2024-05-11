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
//  NiParticles inline functions

//---------------------------------------------------------------------------
inline float* NiParticles::GetRadii()
{
    NiParticlesData* pkData =
        NiSmartPointerCast(NiParticlesData,m_spModelData);
    return pkData->GetRadii();
}
//---------------------------------------------------------------------------
inline float* NiParticles::GetSizes()
{
    NiParticlesData* pkData =
        NiSmartPointerCast(NiParticlesData,m_spModelData);
    return pkData->GetSizes();
}
//---------------------------------------------------------------------------
inline NiQuaternion* NiParticles::GetRotations()
{
    NiParticlesData* pkData =
        NiSmartPointerCast(NiParticlesData,m_spModelData);
    return pkData->GetRotations();
}
//---------------------------------------------------------------------------
