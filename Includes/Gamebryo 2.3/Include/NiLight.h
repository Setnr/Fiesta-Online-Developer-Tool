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

#ifndef NILIGHT_H
#define NILIGHT_H

#include "NiDynamicEffect.h"
#include "NiColor.h"

class NIMAIN_ENTRY NiLight : public NiDynamicEffect
{
    NiDeclareRTTI;
    NiDeclareAbstractClone(NiLight);
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;

public:

    // construction and destruction
    NiLight();
    virtual ~NiLight();

    // attributes
    void SetDimmer(float fDimmer);
    float GetDimmer() const;

    // Only global "color" may be set in base class
    void SetAmbientColor(const NiColor& kColor);
    const NiColor& GetAmbientColor() const;
    void SetDiffuseColor(const NiColor& kColor);
    const NiColor& GetDiffuseColor() const;
    void SetSpecularColor(const NiColor& kColor);
    const NiColor& GetSpecularColor() const;

    // *** begin Emergent internal use only ***

#if defined(WIN32) || defined(_XENON)
    void* GetRendererData() const;
    void SetRendererData(void* pvRendererData);
#endif // defined(WIN32) || defined(_XENON)

    // *** end Emergent internal use only ***

protected:
    float m_fDimmer;
    NiColor m_kAmb;
    NiColor m_kDiff;
    NiColor m_kSpec;

#if defined(WIN32) || defined(_XENON)
    void* m_pvRendererData;
#endif // defined(WIN32) || defined(_XENON)
};

typedef NiPointer<NiLight> NiLightPtr;

#include "NiLight.inl"

#endif

