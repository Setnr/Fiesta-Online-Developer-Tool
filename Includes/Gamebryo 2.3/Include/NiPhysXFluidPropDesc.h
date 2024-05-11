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


#ifndef NIPHYSXFLUIDPROPDESC_H
#define NIPHYSXFLUIDPROPDESC_H

#include "NiPhysXFluidLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXSceneDesc.h"
#include "NiPhysXFluidDesc.h"

class NIPHYSXFLUID_ENTRY NiPhysXFluidPropDesc : public NiPhysXPropDesc
{
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXFluidPropDesc);

public:
    NiPhysXFluidPropDesc(unsigned int uiMaterialHashSize = 521);
    virtual ~NiPhysXFluidPropDesc();

    unsigned int GetNumFluids() const;
    NiPhysXFluidDescPtr GetFluidDesc(unsigned int uiIndex);
    void AddFluidDesc(NiPhysXFluidDesc* pkFluidDesc);
    NiPhysXFluidDescPtr RemoveFluidDesc(unsigned int uiIndex);

    // *** begin Emergent internal use ***
    
    // Instantiates the contents of this object in the given PhysX scene.
    virtual bool CreateInScene(NxScene* pkScene, const NxMat34& kXform,
        NxCompartment* pkRBCompartment, NxCompartment* pkClothCompartment,
        NxCompartment* pkFluidCompartment, const bool bKeepMeshes = false,
        const unsigned int uiIndex = 0);
    
    // Removes the fluid, in addition to anything in it's base class.
    virtual void RemoveFromScene(NxScene* pkScene);
    
    // There is no state caching for fluids, because the PhysX API does not
    // support the user removal of particles from a simulation.
    
    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);

    // *** end Emergent internal use ***
            
protected:
    // This class adds a fluid descriptor
    NiTObjectArray<NiPhysXFluidDescPtr> m_kFluids;
};

NiSmartPointer(NiPhysXFluidPropDesc);

#include "NiPhysXFluidPropDesc.inl"

#endif // NIPHYSXFLUIDPROPDESC_H
