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

#ifndef NIPSYSPOSITIONMODIFIER_H
#define NIPSYSPOSITIONMODIFIER_H

#include "NiPSysModifier.h"

class NIPARTICLE_ENTRY NiPSysPositionModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysPositionModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysPositionModifier(const char* pcName);

    virtual void Update(float fTime, NiPSysData* pkData);

protected:
    // For cloning and streaming only.
    NiPSysPositionModifier();
};

NiSmartPointer(NiPSysPositionModifier);

#endif  // #ifndef NIPSYSPOSITIONMODIFIER_H
