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


#ifndef NIPHYSXFLUIDDESC_H
#define NIPHYSXFLUIDDESC_H

#include "NiPhysXFluidLibType.h"

#include <NiMain.h>
#include <NiSystem.h>
#include <NiParticle.h>

#include <NxPhysics.h>
#include <fluids/NxFluid.h>

#include "NiPhysXFluidEmitterDesc.h"

class NIPHYSXFLUID_ENTRY NiPhysXFluidDesc : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXFluidDesc);

public:
    NiPhysXFluidDesc();
    virtual ~NiPhysXFluidDesc();

    // The Get... function does not get the emitters, name, userData or
    // particles (anything that is a pointer).
    virtual void SetNxFluidDesc(const NxFluidDesc& kFluidDesc);
    virtual NxFluidDesc GetNxFluidDesc() const;

    NxFluid* GetFluid() const;
    virtual void SetFluid(NxFluid* pkFluid, const NxMat34& kXform,
        NiTMap<NxShape*, NiPhysXShapeDescPtr>& kShapeMap);

    NiTObjectArray<NiPhysXFluidEmitterDescPtr>& GetEmitters();
    void SetEmitters(
        const NiTObjectArray<NiPhysXFluidEmitterDescPtr>& kEmitters);
        
    NiFixedString GetName() const;
    void SetName(const NiFixedString& kName);

    NiParticleSystemPtr GetParticleSystem() const;
    void SetParticleSystem(NiParticleSystem* pkPSys);
    
    // *** begin Emergent internal use only ***

    virtual NxFluid* CreateFluid(NxScene* pkScene,
        NxCompartment* pkCompartment, const NxMat34& kXform);
    virtual void RemoveFluid(NxScene* pkScene);
    
    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
        
protected:
    NiFixedString m_kName;
    NxFluidDesc m_kDesc;
    NiTObjectArray<NiPhysXFluidEmitterDescPtr> m_kEmitters;
    
    // Pointer to the PhysX object
    NxFluid* m_pkFluid;
    
    // Particle system object
    NiParticleSystem* m_pkPSys;
};

NiSmartPointer(NiPhysXFluidDesc);

#include "NiPhysXFluidDesc.inl"

#endif // NIPHYSXFLUIDDESC_H
