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

#ifndef NIPHYSXMESHFLUIDDATA_H
#define NIPHYSXMESHFLUIDDATA_H

#include "NiPhysXFluidLibType.h"

#include <NiMeshPSysData.h>
#include <NxPhysics.h>
#include <fluids/NxFluid.h>
#include <NiPhysX.h>


class NIPHYSXFLUID_ENTRY NiPhysXMeshFluidData : public NiMeshPSysData
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXMeshFluidData);

public:
    // The arguments just get passed to the superclass
    NiPhysXMeshFluidData(NxParticleData& kFluidData,
        unsigned short usMaxNumParticles, bool bHasColors, bool bHasRotations,
        unsigned int uiPoolSize = NiMeshPSysData::DEFAULT_POOL_SIZE,
        bool bFillPoolsOnLoad = false);

    virtual ~NiPhysXMeshFluidData();

    NxParticleData& GetFluidData();
    void SetFluidData(NxParticleData& kFluidData);

    // *** begin Emergent internal use only ***

    NiPhysXMeshFluidData(NiMeshPSysData* pkPSysData);

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPhysXMeshFluidData();
    void AllocateFluidData();

    NxParticleData m_kFluidData;
};

NiSmartPointer(NiPhysXMeshFluidData);

#include "NiPhysXMeshFluidData.inl"

#endif  // #ifndef NIPHYSXMESHFLUIDDATA_H
