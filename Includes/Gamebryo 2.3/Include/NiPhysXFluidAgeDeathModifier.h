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

#ifndef NIPHYSXFLUIDAGEDEATHMODIFIER_H
#define NIPHYSXFLUIDAGEDEATHMODIFIER_H

#include "NiPSysModifier.h"

class NIPHYSXFLUID_ENTRY NiPhysXFluidAgeDeathModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPhysXFluidAgeDeathModifier(const char* pcName);

    virtual void Update(float fTime, NiPSysData* pkData);

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPhysXFluidAgeDeathModifier();
};

NiSmartPointer(NiPhysXFluidAgeDeathModifier);


#endif  // #ifndef NIPHYSXFLUIDAGEDEATHMODIFIER_H
