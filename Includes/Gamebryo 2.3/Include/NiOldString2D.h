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

#ifndef NIOLDSTRING2D_H
#define NIOLDSTRING2D_H

#include "NiFontString.h"

#include <NiRect.h>
#include <NiScreenTexture.h>

// NiOldString2D
class NIFONT_ENTRY NiOldString2D : public NiFontString
{
public:
    NiOldString2D(NiFont* pkFont, unsigned int uiFlags,
        unsigned int uiMaxStringLen, const char* pcString,
        const NiColorA& kColor, unsigned int uiX, unsigned int uiY);
    virtual ~NiOldString2D();

    //
    void sprintf(const char* pcFormatString, ...);

    //
    void SetPosition(unsigned int uiX, unsigned int uiY);
    
    //
    unsigned int GetLimitWidth();
    unsigned int SetLimitWidth(unsigned int uiWidth);

    // Rendering Functions
    void Draw(NiRenderer* pkRenderer);

    // Get Size, etc.
    void GetTextExtent(float& fWidth, float& fHeight);

    //*** begin Emergent internal use only
    void CreateScreenTexture();
    void UpdateScreenTexture();
    //*** end Emergent internal use only

protected:
    unsigned int m_uiX;     // Position in screen coordinates
    unsigned int m_uiY;
    NiRect<float> m_rfPos;

    unsigned int m_uiLimitWidth;
    unsigned int m_uiLimitStringLen;
    float m_fLimitStringLen;

    NiScreenTexturePtr m_spScreenTexture;
};

typedef NiPointer<NiOldString2D> NiOldString2DPtr;

#include "NiOldString2D.inl"

#endif // NIOLDSTRING2D_H
