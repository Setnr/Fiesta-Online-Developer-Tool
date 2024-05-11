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

#ifndef NIPSYSINITIALROTSPEEDVARCTLR_H
#define NIPSYSINITIALROTSPEEDVARCTLR_H

#include "NiPSysModifierFloatCtlr.h"

class NIPARTICLE_ENTRY NiPSysInitialRotSpeedVarCtlr :
    public NiPSysModifierFloatCtlr
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysInitialRotSpeedVarCtlr);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysInitialRotSpeedVarCtlr(const char* pcModifierName);

protected:
    // For cloning and streaming only.
    NiPSysInitialRotSpeedVarCtlr();

    virtual bool InterpTargetIsCorrectType(NiObjectNET* pkTarget) const;
    virtual void GetTargetFloatValue(float& fValue);
    virtual void SetTargetFloatValue(float fValue);
};

NiSmartPointer(NiPSysInitialRotSpeedVarCtlr);

#endif  // #ifndef NIPSYSINITIALROTSPEEDVARCTLR_H
