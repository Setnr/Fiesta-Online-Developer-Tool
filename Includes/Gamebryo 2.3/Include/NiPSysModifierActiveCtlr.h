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

#ifndef NIPSYSMODIFIERACTIVECTLR_H
#define NIPSYSMODIFIERACTIVECTLR_H

#include "NiPSysModifierBoolCtlr.h"
#include <NiBoolData.h>

class NIPARTICLE_ENTRY NiPSysModifierActiveCtlr : public NiPSysModifierBoolCtlr
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysModifierActiveCtlr);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysModifierActiveCtlr(const char* pcModifierName);

protected:
    // For cloning and streaming only.
    NiPSysModifierActiveCtlr();

    // Virtual function overrides from base classes.
    virtual bool InterpTargetIsCorrectType(NiObjectNET* pkTarget) const;

    virtual void GetTargetBoolValue(bool& bValue);
    virtual void SetTargetBoolValue(bool bValue);
};

NiSmartPointer(NiPSysModifierActiveCtlr);

#endif  // #ifndef NIPSYSMODIFIERACTIVECTLR_H
