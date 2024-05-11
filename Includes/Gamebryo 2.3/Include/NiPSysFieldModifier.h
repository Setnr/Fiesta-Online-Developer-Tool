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

#ifndef NIPSYSFIELDMODIFIER_H
#define NIPSYSFIELDMODIFIER_H

#include "NiPSysModifier.h"
#include "NiPoint3.h"

class NiAVObject;

class NIPARTICLE_ENTRY NiPSysFieldModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareAbstractClone(NiPSysFieldModifier);
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;

public:
    NiPSysFieldModifier(const char* pcName, NiAVObject* pkFieldObj, 
        float fMagnitude, float fAttenuation, bool bUseMaxDistance,
        float fMaxDistance);

    NiAVObject* GetFieldObj() const;
    void SetFieldObj(NiAVObject* pkFieldObj);

    float GetMagnitude() const;
    void SetMagnitude(float fMagnitude);
   
    float GetAttenuation() const;
    void SetAttenuation(float fAttenuation);
   
    bool GetUseMaxDistance() const;
    void SetUseMaxDistance(bool bUseMaxDistance);

    float GetMaxDistance() const;
    void SetMaxDistance(float fMaxDistance);
   

    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysFieldModifier();

    NiAVObject* m_pkFieldObj;
    float m_fMagnitude;
    float m_fAttenuation;
    bool m_bUseMaxDistance;
    float m_fMaxDistance;
    float m_fMaxDistanceSqr;
};


NiSmartPointer(NiPSysFieldModifier);

#include "NiPSysFieldModifier.inl"

#endif  // #ifndef NIPSYSFIELDMODIFIER_H
