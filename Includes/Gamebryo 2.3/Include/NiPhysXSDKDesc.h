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


#ifndef NIPHYSXSDKDESC_H
#define NIPHYSXSDKDESC_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

class NIPHYSX_ENTRY NiPhysXSDKDesc : public NiObject
{
    NiDeclareStream;
    NiDeclareRTTI;

public:
    void FromSDK();
    void ToSDK() const;
    
    NxReal GetParameter(const NxParameter kIndex) const;
    void SetParameter(const NxParameter kIndex, const NxReal kVal);

    void SetSDKDesc(const NxPhysicsSDKDesc& kSDKDesc);
    void GetSDKDesc(NxPhysicsSDKDesc& kSDKDesc) const;

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    NxPhysicsSDKDesc m_kSDKDescriptor;
    NxReal m_afSDKParams[NX_PARAMS_NUM_VALUES];
};

NiSmartPointer(NiPhysXSDKDesc);

#endif // #ifdef NIPHYSXSDKDESC_H

