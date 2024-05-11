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

#ifndef NIPSYSEMITTERDECLINATIONCTLR_H
#define NIPSYSEMITTERDECLINATIONCTLR_H

#include "NiPSysModifierFloatCtlr.h"

class NIPARTICLE_ENTRY NiPSysEmitterDeclinationCtlr :
    public NiPSysModifierFloatCtlr
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysEmitterDeclinationCtlr);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysEmitterDeclinationCtlr(const char* pcModifierName);

protected:
    // For cloning and streaming only.
    NiPSysEmitterDeclinationCtlr();

    virtual bool InterpTargetIsCorrectType(NiObjectNET* pkTarget) const;
    virtual void GetTargetFloatValue(float& fValue);
    virtual void SetTargetFloatValue(float fValue);
};

NiSmartPointer(NiPSysEmitterDeclinationCtlr);

#endif  // #ifndef NIPSYSEMITTERDECLINATIONCTLR_H
