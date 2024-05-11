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

#ifndef NIPSYSRESETONLOOPCTLR_H
#define NIPSYSRESETONLOOPCTLR_H

#include "NiParticleLibType.h"
#include <NiTimeController.h>

class NIPARTICLE_ENTRY NiPSysResetOnLoopCtlr : public NiTimeController
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysResetOnLoopCtlr);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysResetOnLoopCtlr();

    virtual void Update(float fTime);

protected:
    virtual bool TargetIsRequiredType() const;

    float m_fLastScaledTime;
};

#endif  // #ifndef NIPSYSRESETONLOOPCTLR_H
