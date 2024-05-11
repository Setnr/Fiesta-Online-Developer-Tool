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

#ifndef NISCREENTEXTURE_H
#define NISCREENTEXTURE_H

#include "NiColor.h"
#include "NiGeometryData.h"
#include "NiObject.h"
#include "NiTSet.h"
#include "NiTexturingProperty.h"

NiSmartPointer(NiTexture);

class NiRenderer;

class NIMAIN_ENTRY NiScreenTexture : public NiObject
{
    NiDeclareRTTI;
    NiDeclareClone(NiScreenTexture);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:

    class NIMAIN_ENTRY ScreenRect : public NiMemObject
    {
    public:
        short m_sPixTop;
        short m_sPixLeft;
        unsigned short m_usPixWidth;
        unsigned short m_usPixHeight;

        unsigned short m_usTexTop;
        unsigned short m_usTexLeft;

        NiColorA m_kColor;
    };

    // construction and destruction
    NiScreenTexture(NiTexture* pkTexture);
    virtual ~NiScreenTexture();

    // Screen Rect handling
    unsigned int AddNewScreenRect(short sPixTop, short sPixLeft, 
        unsigned short usPixWidth, unsigned short usPixHeight, 
        unsigned short usTexTop, unsigned short usTexLeft, 
        NiColorA kColor = NiColorA::WHITE);

    const ScreenRect& GetScreenRect(unsigned int uiIndex) const;
    ScreenRect& GetScreenRect(unsigned int uiIndex);
    void RemoveScreenRect(unsigned int uiIndex);

    unsigned int GetNumScreenRects() const;
    void RemoveAllScreenRects();

    NiTexture* GetTexture() const;
    void SetTexture(NiTexture* pkTexture);

    NiTexturingProperty::ClampMode GetClampMode() const;
    void SetClampMode(NiTexturingProperty::ClampMode eClampMode);

    NiTexturingProperty::ApplyMode GetApplyMode() const;
    void SetApplyMode(NiTexturingProperty::ApplyMode eApplyMode);

    // MarkAsChanged should be called if vertices, texture coordinates,
    // heights, widths, or colors are changed, or if rectangles are added or
    // deleted. Values from the following enum should be OR'd together and
    // passed to MarkAsChanged to indicate what values have changed. 
    
    // (If heights or widths are changed, VERTEX_MASK | TEXTURE_MASK
    // should be passed.)

    enum
    {
        VERTEX_MASK = 1,
        TEXTURE_MASK = 2,
        COLOR_MASK = 4,
        RECTANGLE_COUNT_MASK = 8,

        EVERYTHING_MASK = 
            (VERTEX_MASK | TEXTURE_MASK | COLOR_MASK | RECTANGLE_COUNT_MASK)
    };
    void MarkAsChanged(unsigned short usFlags);

    // display
    void Draw(NiRenderer* pkRenderer);

    // *** begin Emergent internal use only ***

    // revision ID access
    unsigned short GetRevisionID() const;
    void ClearRevisionID();

    NiTexturingProperty* GetTexturingProperty() const;

    NiGeometryData::RendererData* GetRendererData() const;
    void SetRendererData(NiGeometryData::RendererData* pkRendererData);

    // *** end Emergent internal use only ***

protected:
    NiScreenTexture();

    NiTPrimitiveSet<ScreenRect> m_kScreenRects;
    NiTexturingPropertyPtr m_spTexProp;
    unsigned short m_usDirtyFlags;

    NiGeometryData::RendererData* m_pkBuffData;
};

NiSmartPointer(NiScreenTexture);
typedef NiTObjectArray<NiScreenTexturePtr> NiScreenTextureArray;

#include "NiScreenTexture.inl"

#endif // NISCREENTEXTURE_H

