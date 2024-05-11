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

#ifndef NIPSYSVORTEXFIELDMODIFIER_H
#define NIPSYSVORTEXFIELDMODIFIER_H

#include "NiPSysFieldModifier.h"
#include "NiPoint3.h"

class NiAVObject;

class NIPARTICLE_ENTRY NiPSysVortexFieldModifier : public NiPSysFieldModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysVortexFieldModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysVortexFieldModifier(const char* pcName, NiAVObject* pkFieldObj, 
        float fMagnitude, float fAttenuation, bool bUseMaxDistance,
        float fMaxDistance, NiPoint3 kDirection = NiPoint3::UNIT_X);

    virtual void Update(float fTime, NiPSysData* pkData);

    const NiPoint3& GetDirection() const;
    void SetDirection(const NiPoint3& kDirection);

    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysVortexFieldModifier();

    NiPoint3 VortexDirection(NiPoint3& kVortexPos, NiPoint3& kDirection,
        NiPoint3& kParticle);

    // Optimized Update Functions
    void UpdateNoAttenuation(float fTime, NiPSysData* pkData);
    void UpdateNoAttenuationNoDist(float fTime, NiPSysData* pkData);

    NiPoint3 m_kDirection;
};

NiSmartPointer(NiPSysVortexFieldModifier);

#include "NiPSysVortexFieldModifier.inl"

#endif  // #ifndef NIPSYSVORTEXFIELDMODIFIER_H
