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

#ifndef NIPHYSXFLUIDEMITTER_H
#define NIPHYSXFLUIDEMITTER_H

#include "NiPSysEmitter.h"

class NIPHYSXFLUID_ENTRY NiPhysXFluidEmitter : public NiPSysEmitter
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPhysXFluidEmitter(const char* pcName,
        NiColorA kInitialColor = NiColorA::WHITE,
        float fInitialRadius = 1.0f, float fRadiusVar = 0.0f);

    virtual void Update(float fTime, NiPSysData* pkData);

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPhysXFluidEmitter();

    virtual void ComputeInitialPositionAndVelocity(
        NiPoint3& kPosition, NiPoint3& kVelocity);
};

NiSmartPointer(NiPhysXFluidEmitter);

#endif  // #ifndef NIPHYSXFLUIDEMITTER_H
