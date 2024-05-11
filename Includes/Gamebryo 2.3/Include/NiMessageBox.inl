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
inline void NiMessageBox::SetMessageBoxFunction(
    MessageBoxFunction pfnMessageBox)
{
    ms_pfnMessageBox = pfnMessageBox;
}
//---------------------------------------------------------------------------
inline MessageBoxFunction NiMessageBox::GetMessageBoxFunction()
{
    return ms_pfnMessageBox;
}
//---------------------------------------------------------------------------
inline unsigned int NiMessageBox::DisplayMessage(const char* pcText, 
    const char* pcCaption, void* pvExtraData)
{
    if (ms_pfnMessageBox == NULL)
        return 0;

    return (*ms_pfnMessageBox)(pcText, pcCaption, pvExtraData);
}
//---------------------------------------------------------------------------
inline unsigned int NiMessageBox(const char* pcText, const char* pcCaption, 
    void* pvExtraData)
{
    return NiMessageBox::DisplayMessage(pcText, pcCaption, pvExtraData);
}
//---------------------------------------------------------------------------
