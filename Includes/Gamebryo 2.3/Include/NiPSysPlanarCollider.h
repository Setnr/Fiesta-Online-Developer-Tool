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

#ifndef NIPSYSPLANARCOLLIDER_H
#define NIPSYSPLANARCOLLIDER_H

#include "NiPSysCollider.h"
#include <NiPlane.h>
#include <NiTransform.h>

class NIPARTICLE_ENTRY NiPSysPlanarCollider : public NiPSysCollider
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysPlanarCollider);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysPlanarCollider(float fBounce = 1.0f, bool bSpawnOnCollide = false,
        bool bDieOnCollide = false,
        NiPSysSpawnModifier* pkSpawnModifier = NULL,
        NiAVObject* pkColliderObj = NULL, float fWidth = 1.0f,
        float fHeight = 1.0f, NiPoint3 kXAxis = NiPoint3::UNIT_X,
        NiPoint3 kYAxis = NiPoint3::UNIT_Y);

    virtual NiPSysCollider* Resolve(float fInitialTime, float& fCollisionTime,
        NiPSysData* pkData, unsigned short usCurParticle);
    virtual void Update(float fTime, NiPSysData* pkData,
        unsigned short usCurParticle);
    virtual void SetResolveConstants();


    NiAVObject* GetColliderObj() const;
    void SetColliderObj(NiAVObject* pkColliderObj);

    float GetWidth() const;
    void SetWidth(float fWidth);

    float GetHeight() const;
    void SetHeight(float fHeight);

    const NiPoint3& GetXAxis() const;
    void SetXAxis(const NiPoint3& kXAxis);

    const NiPoint3& GetYAxis() const;
    void SetYAxis(const NiPoint3& kYAxis);

    // ** begin Emergent internal use only ***
    void ProcessClone(NiCloningProcess& kCloning);
    // ** end Emergent internal use only ***

protected:
    NiAVObject* m_pkColliderObj;
    float m_fWidth;
    float m_fHeight;
    NiPoint3 m_kXAxis;
    NiPoint3 m_kYAxis;

    float m_fHalfSqScaledWidth;
    float m_fHalfSqScaledHeight;
    NiPlane m_kPlane;
    NiPoint3 m_kPosition;
    NiMatrix3 m_kRotation;

    NiTransform m_kLastColliderTransform;
    NiTransform m_kLastPSysTransform;
};

NiSmartPointer(NiPSysPlanarCollider);

#include "NiPSysPlanarCollider.inl"

#endif  // #ifndef NIPSYSPLANARCOLLIDER_H
