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

#ifndef NISCREENTEXTURERENDERCLICK_H
#define NISCREENTEXTURERENDERCLICK_H

#include "NiRenderClick.h"
#include "NiScreenTexture.h"

class NIMAIN_ENTRY NiScreenTextureRenderClick : public NiRenderClick
{
    NiDeclareRTTI;

public:
    NiScreenTextureRenderClick();

    // Functions for reporting statistics about the most recent frame.
    virtual unsigned int GetNumObjectsDrawn() const;
    virtual float GetCullTime() const;
    virtual float GetRenderTime() const;

    // Functions for accessing screen textures.
    void AppendScreenTexture(NiScreenTexture* pkScreenTexture);
    void PrependScreenTexture(NiScreenTexture* pkScreenTexture);
    void RemoveScreenTexture(NiScreenTexture* pkScreenTexture);
    void RemoveAllScreenTextures();
    NiTPointerList<NiScreenTexturePtr>& GetScreenTextures();
    const NiTPointerList<NiScreenTexturePtr>& GetScreenTextures() const;

protected:
    // Function for drawing screen textures.
    virtual void PerformRendering(unsigned int uiFrameID);

    // List of screen textures.
    NiTPointerList<NiScreenTexturePtr> m_kScreenTextures;

    // Rendering statistics.
    unsigned int m_uiNumObjectsDrawn;
    float m_fRenderTime;
};

NiSmartPointer(NiScreenTextureRenderClick);

#include "NiScreenTextureRenderClick.inl"

#endif  // #ifndef NISCREENTEXTURERENDERCLICK_H
