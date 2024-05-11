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

#ifndef NIPSYSINITIALROTANGLECTLR_H
#define NIPSYSINITIALROTANGLECTLR_H

#include "NiPSysModifierFloatCtlr.h"

class NIPARTICLE_ENTRY NiPSysInitialRotAngleCtlr :
    public NiPSysModifierFloatCtlr
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysInitialRotAngleCtlr);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysInitialRotAngleCtlr(const char* pcModifierName);

protected:
    // For cloning and streaming only.
    NiPSysInitialRotAngleCtlr();

    virtual bool InterpTargetIsCorrectType(NiObjectNET* pkTarget) const;
    virtual void GetTargetFloatValue(float& fValue);
    virtual void SetTargetFloatValue(float fValue);
};

NiSmartPointer(NiPSysInitialRotAngleCtlr);

#endif  // #ifndef NIPSYSINITIALROTANGLECTLR_H
