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

#ifndef NIMATERIALPROPERTY_H
#define NIMATERIALPROPERTY_H

#include "NiColor.h"
#include "NiProperty.h"

NiSmartPointer(NiMaterialProperty);

class NIMAIN_ENTRY NiMaterialProperty : public NiProperty
{
    NiDeclareRTTI;
    NiDeclareClone(NiMaterialProperty);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiMaterialProperty ();
    virtual ~NiMaterialProperty ();

    void SetAmbientColor (const NiColor& c);
    const NiColor& GetAmbientColor () const;

    void SetDiffuseColor (const NiColor& c);
    const NiColor& GetDiffuseColor () const;

    void SetSpecularColor (const NiColor& c);
    const NiColor& GetSpecularColor () const;

    void SetEmittance (const NiColor& c);
    const NiColor& GetEmittance () const;

    void SetShineness (float c);
    float GetShineness () const;

    void SetAlpha (float c);
    float GetAlpha () const;

    bool IsEqualFast (const NiMaterialProperty& prop) const;

    virtual int Type () const;
    static int GetType ();
    static NiMaterialProperty* GetDefault ();
    
    // *** begin Emergent internal use only ***

    void SetRevisionID (unsigned int uiRev);
    unsigned int GetRevisionID () const;

    void SetChanged (bool bChanged);

    int GetIndex () const;

#if defined(WIN32) || defined(_XENON)
    void* GetRendererData() const;
    void SetRendererData(void* pvRendererData);
#endif // defined(WIN32) || defined(_XENON)

    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    static int ms_iNextIndex;

    int m_iIndex;
    NiColor m_amb;
    NiColor m_diff;
    NiColor m_spec;
    NiColor m_emit;
    float m_fShine;
    float m_fAlpha;
    unsigned int m_uiRevID;

#if defined(WIN32) || defined(_XENON)
    void* m_pvRendererData;
#endif // defined(WIN32) || defined(_XENON)

    static NiMaterialPropertyPtr ms_spDefault;

#if !defined(WIN32) && !defined(_XENON)
    static NiCriticalSection ms_kCritSec;
#endif
};

//---------------------------------------------------------------------------
//  Inline include
#include "NiMaterialProperty.inl"

//---------------------------------------------------------------------------

#endif

