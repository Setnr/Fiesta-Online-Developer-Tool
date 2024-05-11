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

#ifndef NIPSYSDRAGFIELDMODIFIER_H
#define NIPSYSDRAGFIELDMODIFIER_H

#include "NiPSysFieldModifier.h"
#include "NiPoint3.h"

class NiAVObject;

class NIPARTICLE_ENTRY NiPSysDragFieldModifier : public NiPSysFieldModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysDragFieldModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysDragFieldModifier(const char* pcName, NiAVObject* pkFieldObj, 
        float fMagnitude, float fAttenuation, bool bUseMaxDistance,
        float fMaxDistance, bool bUseDirection = false, 
        NiPoint3 kDirection = NiPoint3::UNIT_X);

    virtual void Update(float fTime, NiPSysData* pkData);

    const bool GetUseDirection() const;
    void SetUseDirection(bool bUseDirection);

    const NiPoint3& GetDirection() const;
    void SetDirection(const NiPoint3& kDirection);

    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysDragFieldModifier();

    // Optimized Update Functions
    void UpdateNoAttenuationNoDirNoDist(float fTime, NiPSysData* pkData);
    void UpdateNoAttenuationNoDir(float fTime, NiPSysData* pkData);

    bool m_bUseDirection;
    NiPoint3 m_kDirection;
};

NiSmartPointer(NiPSysDragFieldModifier);

#include "NiPSysDragFieldModifier.inl"

#endif  // #ifndef NIPSYSDRAGFIELDMODIFIER_H
