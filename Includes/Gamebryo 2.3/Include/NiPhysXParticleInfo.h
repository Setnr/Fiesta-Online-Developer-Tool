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

#ifndef NIPHYSXPARTICLEINFO_H
#define NIPHYSXPARTICLEINFO_H

#include "NiParticleLibType.h"

#include <NiMemObject.h>
#include <NiPoint3.h>
#include <NxPhysics.h>

class NIPHYSXPARTICLE_ENTRY NiPhysXParticleInfo : public NiMemObject
{
public:
    NiPhysXParticleInfo();

    NxActor* m_pkActor;
    NiPoint3 m_kLastVelocity;
};

#endif // NIPHYSXPARTICLEINFO_H
