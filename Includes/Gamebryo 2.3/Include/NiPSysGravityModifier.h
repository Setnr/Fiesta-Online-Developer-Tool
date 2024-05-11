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

#ifndef NIPSYSGRAVITYMODIFIER_H
#define NIPSYSGRAVITYMODIFIER_H

#include "NiPSysModifier.h"
#include "NiPoint3.h"

class NiAVObject;

class NIPARTICLE_ENTRY NiPSysGravityModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysGravityModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    enum ForceType
    {
        FORCE_PLANAR,
        FORCE_SPHERICAL
    };

    NiPSysGravityModifier(const char* pcName, NiAVObject* pkGravityObj,
        NiPoint3 kGravityAxis = NiPoint3::UNIT_X, float fDecay = 0.0f,
        float fStrength = 1.0f, ForceType eType = FORCE_PLANAR,
        float fTurbulence = 0.0f, float fScale = 1.0f);

    virtual void Update(float fTime, NiPSysData* pkData);

    NiAVObject* GetGravityObj() const;
    void SetGravityObj(NiAVObject* pkGravityObj);

    const NiPoint3& GetGravityAxis() const;
    void SetGravityAxis(const NiPoint3& kGravityAxis);

    float GetDecay() const;
    void SetDecay(float fDecay);

    float GetStrength() const;
    void SetStrength(float fStrength);

    ForceType GetType() const;
    void SetType(ForceType eType);

    float GetTurbulence();
    void SetTurbulence(float fTurbulence);

    float GetTurbulenceScale();
    void SetTurbulenceScale(float fScale);
    
    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysGravityModifier();

    NiAVObject* m_pkGravityObj;
    NiPoint3 m_kGravityAxis;
    float m_fDecay;
    float m_fStrength;  // specified in units/seconds^2
    ForceType m_eType;

    float m_fTurbulence;
    float m_fScale;
};

NiSmartPointer(NiPSysGravityModifier);

#include "NiPSysGravityModifier.inl"

#endif  // #ifndef NIPSYSGRAVITYMODIFIER_H
