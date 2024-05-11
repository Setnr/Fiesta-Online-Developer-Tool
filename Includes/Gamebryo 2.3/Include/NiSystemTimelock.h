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

#ifndef NISYSTEMTIMELOCK_H
#define NISYSTEMTIMELOCK_H

void ReadNDLDevLock(char* pResult);
void NormalizeExpireTime(unsigned long& ulTime);
void DialogBoxExit(const char* pText, const char* pCaption);
bool CheckForTimeReset(unsigned int uiDecExpire);

#endif // NISYSTEMTIMELOCK_H
