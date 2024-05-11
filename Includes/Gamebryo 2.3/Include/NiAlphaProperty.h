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

#ifndef NIALPHAPROPERTY_H
#define NIALPHAPROPERTY_H

#include "NiProperty.h"
#include "NiFlags.h"

NiSmartPointer(NiAlphaProperty);

class NIMAIN_ENTRY NiAlphaProperty : public NiProperty
{
    NiDeclareRTTI;
    NiDeclareClone(NiAlphaProperty);
    NiDeclareStream;
    NiDeclareViewerStrings;
    NiDeclareFlags(unsigned short);

public:
    enum AlphaFunction
    {
        ALPHA_ONE,
        ALPHA_ZERO,
        ALPHA_SRCCOLOR,
        ALPHA_INVSRCCOLOR,
        ALPHA_DESTCOLOR,
        ALPHA_INVDESTCOLOR,
        ALPHA_SRCALPHA,
        ALPHA_INVSRCALPHA,
        ALPHA_DESTALPHA,
        ALPHA_INVDESTALPHA,
        ALPHA_SRCALPHASAT,
        ALPHA_MAX_MODES
    };

    enum TestFunction
    {
        TEST_ALWAYS,
        TEST_LESS,
        TEST_EQUAL,
        TEST_LESSEQUAL,
        TEST_GREATER,
        TEST_NOTEQUAL,
        TEST_GREATEREQUAL,
        TEST_NEVER,
        TEST_MAX_MODES
    };

    NiAlphaProperty();

    void SetAlphaBlending(bool bAlpha);
    bool GetAlphaBlending() const;

    void SetSrcBlendMode(AlphaFunction eSrcBlend);
    AlphaFunction GetSrcBlendMode() const;

    void SetDestBlendMode(AlphaFunction eDestBlend);
    AlphaFunction GetDestBlendMode() const;

    void SetAlphaTesting(bool bAlpha);
    bool GetAlphaTesting() const;

    void SetTestMode(TestFunction eTestFunc);
    TestFunction GetTestMode() const;
    
    void SetTestRef(unsigned char ucRef);
    unsigned char GetTestRef() const;

    void SetNoSorter(bool bNoSort);
    bool GetNoSorter() const;

    bool IsEqualFast(const NiAlphaProperty& kProp) const;

    virtual int Type() const;
    static int GetType();
    static NiAlphaProperty* GetDefault();

    // *** begin Emergent internal use only ***

    static char* GetViewerString(const char* pcPrefix, AlphaFunction eFunc);
    static char* GetViewerString(const char* pcPrefix, TestFunction eFunc);

    static void _SDMInit();
    static void _SDMShutdown();

    // *** end Emergent internal use only ***

protected:
    // --- Begin NIF conversion code
    // Deprecated flags - use new flag code
    enum
    {
        MAX_POS = 14
    };
    // --- End NIF conversion code

    // flags
    enum
    {
        ALPHA_BLEND_MASK          = 0x0001,
        SRC_BLEND_MASK      = 0x001e,
        SRC_BLEND_POS       = 1,
        DEST_BLEND_MASK     = 0x01e0,
        DEST_BLEND_POS      = 5,
        TEST_ENABLE_MASK    = 0x0200,
        TEST_FUNC_MASK      = 0x1c00,
        TEST_FUNC_POS       = 10,
        ALPHA_NOSORTER_MASK = 0x2000
    };

    unsigned char m_ucAlphaTestRef;

    static NiAlphaPropertyPtr ms_spDefault;
};

#include "NiAlphaProperty.inl"

#endif // NIALPHAPROPERTY_H
