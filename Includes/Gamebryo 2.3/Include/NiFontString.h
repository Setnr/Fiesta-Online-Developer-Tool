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

#ifndef NIFONTSTRING_H
#define NIFONTSTRING_H

#include "NiFontLibType.h"
#include <NiRefObject.h>
#include <NiRTTI.h>
#include <NiColor.h>
#include "NiFont.h"

class NIFONT_ENTRY NiFontString : public NiRefObject
{
    NiDeclareRootRTTI(NiFontString);

public:
    enum Flags
    {
        COLORED     = 0x00000001,
        CENTERED    = 0x00000002
    };

    enum {
        NIFONTSTRING_LEFT_TO_RIGHT = 0,
        NIFONTSTRING_RIGHT_TO_LEFT,
        NIFONTSTRING_TOP_TO_BOTTOM,
        NIFONTSTRING_BOTTOM_TO_TOP
    };


protected:
    // ASCII Constructor
    NiFontString(NiFont* pkFont, unsigned int uiFlags,
        unsigned int uiMaxStringLen, const char* pcString,
        const NiColorA& kColor,
        unsigned char ucDirection = NIFONTSTRING_LEFT_TO_RIGHT);

    // Unicode Constructor
    NiFontString(NiFont* pkFont, unsigned int uiFlags,
        unsigned int uiMaxStringLen, const NiWChar* pkString,
        const NiColorA& kColor,
        unsigned char ucDirection = NIFONTSTRING_LEFT_TO_RIGHT);

public:
    virtual ~NiFontString();

    // Text
    void SetText(const char* pcText, unsigned int uiFlags = 0xffffffff);
    const char* GetText();

    void SetText(const NiWChar* pkText, unsigned int uiFlags = 0xffffffff);
    const NiWChar* GetUnicodeText();

    // Color
    void SetColor(const NiColorA& kColor);
    const NiColorA& GetColor() const;

    // Get and set the direction the string will be drawn
    unsigned char GetDirection() const;
    void SetDirection(unsigned char ucDirection);

    //*** begin Emergent internal use only
    void SplitString();
    void ShutdownString();
    //*** end Emergent internal use only

protected:
    NiFontPtr m_spFont;
    unsigned int m_uiFlags;
    unsigned int m_uiMaxStringLen;
    unsigned int m_uiStringLen;

    NiWChar* m_pkString;
    char* m_pcString;
    NiColorA m_kColor;
    unsigned char m_ucDirection;

    // *** begin Emergent internal use only *** //
    unsigned int m_uiStringCount;   // Used to handle '\n'
    float* m_pfStringWidth;         // contains width of each line
    float m_fStringWidth;           // contains max width
    float m_fStringHeight;

    bool m_bModified;

    // Be careful with threading when using the working string!
    static NiWChar ms_akWorkingString[NI_MAX_PATH];

    // *** end Emergent internal use only *** //
};

typedef NiPointer<NiFontString> NiFontStringPtr;

#include "NiFontString.inl"

#endif //#ifndef NIFONTSTRING_H
