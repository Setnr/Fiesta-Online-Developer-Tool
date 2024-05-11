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

#ifndef NIPSYSVOLUMEEMITTER_H
#define NIPSYSVOLUMEEMITTER_H

#include "NiPSysEmitter.h"

class NiTransform;

class NIPARTICLE_ENTRY NiPSysVolumeEmitter : public NiPSysEmitter
{
    NiDeclareRTTI;
    NiDeclareAbstractClone(NiPSysVolumeEmitter);
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;

public:
    NiAVObject* GetEmitterObj() const;
    void SetEmitterObj(NiAVObject* pkEmitterObj);

    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    NiPSysVolumeEmitter(const char* pcName, NiAVObject* pkEmitterObj,
        float fSpeed, float fSpeedVar, 
        float fDeclination, float fDeclinationVar, 
        float fPlanarAngle, float fPlanarAngleVar,
        NiColorA kInitialColor, float fInitialRadius, float fLifeSpan, 
        float fLifeSpanVar, float fRadiusVar);

    // For cloning and streaming only.
    NiPSysVolumeEmitter();

    virtual void ComputeInitialPositionAndVelocity(
        NiPoint3& kPosition, NiPoint3& kVelocity);

    virtual void ComputeVolumeInitialPositionAndVelocity( 
        NiTransform& kEmitterToPSys, NiPoint3& kPosition, 
        NiPoint3& kVelocity)=0;

    // Emitter object
    NiAVObject* m_pkEmitterObj;
};

NiSmartPointer(NiPSysVolumeEmitter);

#include "NiPSysVolumeEmitter.inl"

#endif  // #ifndef NIPSYSVOLUMEEMITTER_H
