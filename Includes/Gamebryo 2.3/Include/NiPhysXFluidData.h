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

#ifndef NIPHYSXFLUIDDATA_H
#define NIPHYSXFLUIDDATA_H

#include "NiPhysXFluidLibType.h"

#include <NiPSysData.h>
#include <NxPhysics.h>
#include <fluids/NxFluid.h>
#include <NiPhysX.h>


class NIPHYSXFLUID_ENTRY NiPhysXFluidData : public NiPSysData
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXFluidData);

public:
    NiPhysXFluidData(NxParticleData& kFluidData,
        unsigned short usMaxNumParticles, bool bHasColors, bool bHasRotations);
    virtual ~NiPhysXFluidData();

    NxParticleData& GetFluidData();
    void SetFluidData(NxParticleData& pkFluidData);

    // *** begin Emergent internal use only ***

    // Constructor that creates this data based on PSysData.
    NiPhysXFluidData(NiPSysData* pkPSysData);

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPhysXFluidData();
    void AllocateFluidData();
    
    NxParticleData m_kFluidData;
};

NiSmartPointer(NiPhysXFluidData);

#include "NiPhysXFluidData.inl"

#endif  // #ifndef NIPHYSXFLUIDDATA_H
