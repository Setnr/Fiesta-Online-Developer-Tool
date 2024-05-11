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

#ifndef NIPHYSXMESHPSYSSRC_H
#define NIPHYSXMESHPSYSSRC_H

#include "NiPhysXParticleLibType.h"

#include "NiPhysXPSysSrc.h"

class NIPHYSXPARTICLE_ENTRY NiPhysXMeshPSysSrc : public NiPhysXPSysSrc
{
    NiDeclareRTTI;
    NiDeclareStream;

public:
    NiPhysXMeshPSysSrc(NiParticleSystem* pkSource);
    ~NiPhysXMeshPSysSrc();

    virtual void UpdateToActors(const float fTBegin, const float fTEnd);
    
    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For streaming only
    NiPhysXMeshPSysSrc();
};

NiSmartPointer(NiPhysXMeshPSysSrc);

#endif  // #ifndef NIPHYSXMESHPSYSSRC_H
