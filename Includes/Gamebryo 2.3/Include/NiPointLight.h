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

#ifndef NIPOINTLIGHT_H
#define NIPOINTLIGHT_H

#include "NiBool.h"
#include "NiLight.h"
#include "NiPoint3.h"


class NIMAIN_ENTRY NiPointLight : public NiLight
{
    NiDeclareRTTI;
    NiDeclareClone(NiPointLight);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPointLight();

    // The model location of the light is (0,0,0).  The world location is
    // the world translation vector.
    const NiPoint3& GetWorldLocation() const;

    void SetConstantAttenuation(float fC);
    float GetConstantAttenuation() const;
    void SetLinearAttenuation(float fL);
    float GetLinearAttenuation() const;
    void SetQuadraticAttenuation(float fQ);
    float GetQuadraticAttenuation() const;

protected:
    void UpdateWorldData();

    // attenuation
    float m_fAtten0;
    float m_fAtten1;
    float m_fAtten2;
};


typedef NiPointer<NiPointLight> NiPointLightPtr;

#include "NiPointLight.inl"

#endif
