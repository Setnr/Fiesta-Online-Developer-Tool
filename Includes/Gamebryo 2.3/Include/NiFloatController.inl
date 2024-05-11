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
inline NiFloatController::NiFloatController(NiFloatData* pData)
    :
    m_spFloatData(pData)
{ 
    m_uiLastIdx = 0; 
}
//---------------------------------------------------------------------------
inline void NiFloatController::SetFloatData(NiFloatData* pData)
{
    m_spFloatData = pData;
}
//---------------------------------------------------------------------------
inline NiFloatData* NiFloatController::GetFloatData()
{
    return m_spFloatData;
}
//---------------------------------------------------------------------------
