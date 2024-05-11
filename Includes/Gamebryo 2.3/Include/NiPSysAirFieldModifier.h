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

#ifndef NIPSYSAIRFIELDMODIFIER_H
#define NIPSYSAIRFIELDMODIFIER_H

#include "NiPSysFieldModifier.h"
#include "NiPoint3.h"

class NiAVObject;

class NIPARTICLE_ENTRY NiPSysAirFieldModifier : public NiPSysFieldModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysAirFieldModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysAirFieldModifier(const char* pcName, NiAVObject* pkFieldObj, 
        float fMagnitude, float fAttenuation, bool bUseMaxDistance,
        float fMaxDistance, NiPoint3 kDirection, float fAirFriction, 
        float fInheritVelocity, bool bInheritRotation, bool bComponentOnly,
        bool bEnableSpread, float fSpread);

    virtual void Update(float fTime, NiPSysData* pkData);

    const NiPoint3& GetLastFieldPosition() const;
    void SetLastFieldPosition(const NiPoint3& kLastPosition);
    void ClearLastFieldPosition();

    const NiPoint3& GetDirection() const;
    void SetDirection(const NiPoint3& kDirection);

    float GetAirFriction() const;
    void SetAirFriction(float fAirFriction);

    float GetInheritVelocity() const;
    void SetInheritVelocity(float fInherit);

    bool GetInheritRotation() const;
    void SetInheritRotation(bool bInherit);

    bool GetComponentOnly() const;
    void SetComponentOnly(bool bComponent);

    bool GetEnableSpread() const;
    void SetEnableSpread(bool bEnable);

    float GetSpread() const;
    void SetSpread(float fSpread);

    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysAirFieldModifier();

    // Optimized Update Functions
    void UpdateNoAttenuationNoInheritNoSpread(float fTime,
        NiPSysData* pkData);
    void UpdateNoAttenuationNoInheritNoSpreadNoMaxDistance(float fTime,
        NiPSysData* pkData);

    NiPoint3 m_kLastFieldPosition;
    float m_fLastUpdateTime;

    NiPoint3 m_kDirection;
    NiPoint3 m_kUnitDirection;
    float m_fAirFriction;
    float m_fInheritVelocity;
    bool m_bInheritRotation;
    bool m_bComponentOnly;
    bool m_bEnableSpread;
    float m_fSpread;

    static NiPoint3 m_ckUninitializePosition;
};

NiSmartPointer(NiPSysAirFieldModifier);

#include "NiPSysAirFieldModifier.inl"

#endif  // #ifndef NIPSYSAIRFIELDMODIFIER_H
