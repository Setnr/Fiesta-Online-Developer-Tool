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

#ifndef NIPSYSROTATIONMODIFIER_H
#define NIPSYSROTATIONMODIFIER_H

#include "NiPSysModifier.h"
#include <NiPoint3.h>

class NIPARTICLE_ENTRY NiPSysRotationModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysRotationModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysRotationModifier(const char* pcName, float fInitialRotSpeed = 0.0f,
        float fInitialRotSpeedVar = 0.0f, bool bRandomRotSpeedSign = false,
        float fInitialRotAngle = 0.0f, float fInitialRotAngleVar = 0.0f,
        bool bRandomInitialAxis = true,
        NiPoint3 kInitialAxis = NiPoint3::UNIT_X);

    virtual void Update(float fTime, NiPSysData* pkData);
    virtual void Initialize(NiPSysData* pkData, unsigned short usNewParticle);

    float GetInitialRotSpeed() const;
    void SetInitialRotSpeed(float fInitialRotSpeed);

    float GetInitialRotSpeedVar() const;
    void SetInitialRotSpeedVar(float fInitialRotSpeedVar);

    bool GetRandomRotSpeedSign() const;
    void SetRandomRotSpeedSign(bool bRandomRotSpeedSign);

    float GetInitialRotAngle() const;
    void SetInitialRotAngle(float fInitialRotAngle);

    float GetInitialRotAngleVar() const;
    void SetInitialRotAngleVar(float fInitialRotAngleVar);

    bool GetRandomInitialAxis() const;
    void SetRandomInitialAxis(bool bRandomInitialAxis);

    const NiPoint3& GetInitialAxis() const;
    void SetInitialAxis(const NiPoint3& kInitialAxis);

    // *** begin Emergent internal use only ***
    virtual void PostLinkObject(NiStream& kStream);
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysRotationModifier();

    float m_fInitialRotSpeed;
    float m_fInitialRotSpeedVar;
    float m_fInitialRotAngle;
    float m_fInitialRotAngleVar;
    NiPoint3 m_kInitialAxis;
    bool m_bRandomInitialAxis;
    bool m_bRandomRotSpeedSign;
};

NiSmartPointer(NiPSysRotationModifier);

#include "NiPSysRotationModifier.inl"

#endif  // #ifndef NIPSYSROTATIONMODIFIER_H
