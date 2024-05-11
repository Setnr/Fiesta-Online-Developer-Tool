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

#ifndef NIPHYSXDYNAMICSRC_H
#define NIPHYSXDYNAMICSRC_H

#include "NiPhysXLibType.h"

#include "NiPhysXRigidBodySrc.h"
#include <NiControllerSequence.h>

class NIPHYSX_ENTRY NiPhysXDynamicSrc : public NiPhysXRigidBodySrc
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXDynamicSrc);

public:
    NiPhysXDynamicSrc(NiAVObject* pkSource, NxActor* pkActor);
    ~NiPhysXDynamicSrc();
    
    // Update functions
    virtual void UpdateToActors(const float fTBegin, const float fTEnd);
    
    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
    
protected:
    NiPhysXDynamicSrc();
};

NiSmartPointer(NiPhysXDynamicSrc);

#endif  // #ifndef NIPHYSXDYNAMICSRC_H
