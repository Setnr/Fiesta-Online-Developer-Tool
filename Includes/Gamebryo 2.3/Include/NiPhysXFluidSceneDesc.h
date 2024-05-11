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

//************************************************************************
//
// This class will be deprecated at some future date. It is retained ONLY
// to enable loading of older NIF files. It MUST NOT be used by new
// applications.
//
//************************************************************************
#ifndef NIPHYSXFLUIDSCENEDESC_H
#define NIPHYSXFLUIDSCENEDESC_H

#include "NiPhysXFluidLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXSceneDesc.h"
#include "NiPhysXFluidDesc.h"

class NIPHYSXFLUID_ENTRY NiPhysXFluidSceneDesc : public NiPhysXSceneDesc
{
    NiDeclareStream;
    NiDeclareRTTI;

public:
    NiPhysXFluidSceneDesc();
    virtual ~NiPhysXFluidSceneDesc();
    
    // Convert this thing to a NiPhysXFluidProp in the given scene.
    // The scene must be the same one that this is the descriptor for.
    void ConvertToProp(NiPhysXScene* pkScene);

    // *** begin Emergent internal use ***
        
    // There is no state caching for fluids, because the PhysX API does not
    // support the user removal of particles from a simulation.
    
    virtual bool StreamCanSkip();

    // *** end Emergent internal use ***
            
protected:
    // This class adds a fluid descriptor
    NiTObjectArray<NiPhysXFluidDescPtr> m_kFluids;
};

NiSmartPointer(NiPhysXFluidSceneDesc);

#include "NiPhysXFluidSceneDesc.inl"

#endif NIPHYSXFLUIDSCENEDESC_H
