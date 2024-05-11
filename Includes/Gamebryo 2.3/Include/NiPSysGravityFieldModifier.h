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

#ifndef NIPSYSGRAVITYFIELDMODIFIER_H
#define NIPSYSGRAVITYFIELDMODIFIER_H

#include "NiPSysFieldModifier.h"
#include "NiPoint3.h"

class NiAVObject;

class NIPARTICLE_ENTRY NiPSysGravityFieldModifier : public NiPSysFieldModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysGravityFieldModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysGravityFieldModifier(const char* pcName, NiAVObject* pkFieldObj, 
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
    NiPSysGravityFieldModifier();

    // Optimized Update Functions
    void UpdateNoAttenuationNoDist(float fTime, NiPSysData* pkData);
    void UpdateNoAttenuationNoDirNoDist(float fTime, NiPSysData* pkData);

    NiPoint3 m_kDirection;
    NiPoint3 m_kUnitDirection;
};

NiSmartPointer(NiPSysGravityFieldModifier);

#include "NiPSysGravityFieldModifier.inl"

#endif  // #ifndef NIPSYSGRAVITYFIELDMODIFIER_H
