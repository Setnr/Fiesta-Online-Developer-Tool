// NUMERICAL DESIGN LIMITED PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Numerical Design Limited and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2007 Numerical Design Limited.
//      All Rights Reserved.
//
// Numerical Design Limited, Chapel Hill, North Carolina 27514
// http://www.ndl.com

#ifndef NI2DSTRING_H
#define NI2DSTRING_H

#include "NiFontString.h"

#include <NiRect.h>
#include <NiScreenElements.h>
#include "NiFont.h"

#define NiWChar unsigned short

// Ni2DString
class NIFONT_ENTRY Ni2DString : public NiFontString
{
public:

    enum {
        DEFAULT_POINT_SIZE = 0
    };

    // Unicode Constructor
    Ni2DString(NiFont* pkFont, unsigned int uiFlags,
        unsigned int uiMaxStringLen, const NiWChar* pkString,
        const NiColorA& kColor, short sX, short sY,
        unsigned char ucDirection = NIFONTSTRING_LEFT_TO_RIGHT,
        unsigned short usPointSize = DEFAULT_POINT_SIZE, 
        const NiRenderTargetGroup* pkTarget = NULL);

    // ASCII Constructor
    Ni2DString(NiFont* pkFont, unsigned int uiFlags,
        unsigned int uiMaxStringLen, const char* pcString,
        const NiColorA& kColor, short sX, short sY,
        unsigned char ucDirection = NIFONTSTRING_LEFT_TO_RIGHT,
        unsigned short usPointSize = DEFAULT_POINT_SIZE, 
        const NiRenderTargetGroup* pkTarget = NULL);

    virtual ~Ni2DString();

    void sprintf(const char* pcFormatString, ...);
    void sprintf(const NiWChar* pkFormatString, ...);

    // Get/Set the position of this string on the renderer target in pixels.
    // The most likely renderer target is the screen.
    void GetPosition(short& sX, short& sY);
    void SetPosition(short sX, short sY);


    // Rendering Functions
    void Draw(NiRenderer* pkRenderer);

    // Get Size, etc.
    void GetTextExtent(float& fWidth, float& fHeight);

    // Get and set the final point size
    unsigned short GetPointSize();
    void SetPointSize(unsigned short usPointSize);

    // Get and Set the NiRenderTargetGroup so we can have the target size
    // for converting between pixels and normalized display coordinates.
    // The target is NOT used for rendering. The string will use the active
    // render target when drawing.
    const NiRenderTargetGroup* GetRenderTargetGroup();
    void SetRenderTargetGroup(NiRenderTargetGroup* pkTarget);

    // Get the NiScreenElements associated with this string. The 
    // NiScreenElements may be NULL if a string has not been set yet.
    NiScreenElements* GetScreenElements(unsigned char ucIndex);

    //*** begin NDL internal use only
    void CreateScreenElements();
    void UpdateScreenElements();
    //*** end NDL internal use only

protected:

    void CreateScreenPolygon(NiWChar ch, short& sCurrX, short& sCurrY, 
         short sW, short sH, float& fScreenWidth, float& fScreenHeight, 
         NiColorA& kColor, float* pfUVSets);

    unsigned short m_usPointSize;

    short m_sX;     // Position in pixel coordinates
    short m_sY;

    NiTObjectArray<NiScreenElementsPtr> m_spScreenElements;
    const NiRenderTargetGroup* m_pkTarget;
};

typedef NiPointer<Ni2DString> Ni2DStringPtr;

#include "Ni2DString.inl"

#endif // NI2DSTRING_H
