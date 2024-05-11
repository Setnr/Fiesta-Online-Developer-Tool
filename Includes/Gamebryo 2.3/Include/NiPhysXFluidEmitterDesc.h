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


#ifndef NIPHYSXFLUIDEMITTERDESC_H
#define NIPHYSXFLUIDEMITTERDESC_H

#include "NiPhysXFluidLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>
#include <fluids/NxFluid.h>

#include "NiPhysXActorDesc.h"

class NIPHYSXFLUID_ENTRY NiPhysXFluidEmitterDesc : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXFluidEmitterDesc);

public:
    NiPhysXFluidEmitterDesc();
    virtual ~NiPhysXFluidEmitterDesc();

    // The Get... function does not get anything that is a pointer.
    virtual NxFluidEmitterDesc GetNxFluidEmitterDesc() const;
    virtual void SetNxFluidEmitterDesc(const NxFluidEmitterDesc& kEmitterDesc);

    NxFluidEmitter* GetEmitter() const;
    virtual void SetEmitter(NxFluidEmitter* pkEmitter, const NxMat34& kXform,
        NiTMap<NxShape*, NiPhysXShapeDescPtr>& kShapeMap);

    NiFixedString GetName() const;
    void SetName(const NiFixedString& kName);

    NiPhysXShapeDescPtr GetFrameShape() const;
    void SetFrameShape(NiPhysXShapeDesc* pkShapeDesc);
    
    // *** begin Emergent internal use only ***

    virtual NxFluidEmitter* CreateEmitter(NxFluid* pkFluid,
        const NxMat34& kXform);
    virtual void RemoveEmitter(NxFluid* pkFluid);
    
    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // *** end Emergent internal use only ***
        
protected:
    virtual void ToFluidEmitterDesc(
        NxFluidEmitterDesc& kEmitterDesc, const NxMat34& kXform);
    
    // Constant through the life of an fluid
    NxFluidEmitterDesc m_kDesc;
    NiPhysXShapeDescPtr m_spFrameShape;
    NiFixedString m_kName;
    
    // Pointer to the PhysX objects.
    NxFluidEmitter* m_pkEmitter;

    // Legacy loading code
    NiPhysXActorDescPtr m_spDeprecatedFrameActor;
};

NiSmartPointer(NiPhysXFluidEmitterDesc);

#include "NiPhysXFluidEmitterDesc.inl"

#endif // #ifdef NIPHYSXFLUIDEMITTERDESC_H
