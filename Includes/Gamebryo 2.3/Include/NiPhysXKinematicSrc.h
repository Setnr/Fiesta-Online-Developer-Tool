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

#ifndef NIPHYSXKINEMATICSRC_H
#define NIPHYSXKINEMATICSRC_H

#include "NiPhysXLibType.h"

#include "NiPhysXRigidBodySrc.h"

class NIPHYSX_ENTRY NiPhysXKinematicSrc : public NiPhysXRigidBodySrc
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXKinematicSrc);

public:
    NiPhysXKinematicSrc(NiAVObject* pkSource, NxActor* pkActor);
    ~NiPhysXKinematicSrc();
    
    // Update functions
    virtual void UpdateToActors(const float fTBegin, const float fTEnd);

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();
    
    virtual void ProcessClone(NiCloningProcess& kCloning);

    // *** end Emergent internal use only ***
    
protected:
    NiPhysXKinematicSrc();
};

NiSmartPointer(NiPhysXKinematicSrc);

#endif  // #ifndef NIPHYSXKINEMATICSRC_H
