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

#ifndef NIOLDANIMATIONCONVERTER_H
#define NIOLDANIMATIONCONVERTER_H

#include <NiMain.h>
#include <NiAnimation.h>


class NIANIMATION_ENTRY NiOldAnimationConverter
{
public:
    static void Convert(NiStream& kStream, NiObject* pkObject);

protected:
    static void RecurseScene(NiStream& kStream, NiObjectNET* pkObject);
    static void ConvertLookAtController(NiLookAtController* pkController);
    static void ConvertRollController(NiRollController* pkController);
    static void ConvertPathController(NiPathController* pkController);
    static void SynchronizeTimeControllers(const NiTimeController* pkSrc,
        NiTimeController* pkDest);
};

#endif  // #ifndef NIOLDANIMATIONCONVERTER_H
