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

#ifndef NIPIXELDATA_H
#define NIPIXELDATA_H

#include "NiObject.h"
#include "NiPalette.h"
#include "NiPixelFormat.h"

class NIMAIN_ENTRY NiPixelData : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPixelData(unsigned int uiWidth, unsigned int uiHeight,
        const NiPixelFormat& kDesc, unsigned int uiMipmapLevels = 1,
        unsigned int uiFaces = 1);
    NiPixelData(const NiPixelData& kSrc);
    ~NiPixelData();
    
    // The base image is counted as a mip-map level
    unsigned int GetNumMipmapLevels() const;

    unsigned int GetNumFaces() const;

    unsigned int GetWidth(unsigned int uiMipmapLevel = 0,
        unsigned int uiFace = 0) const;
    unsigned int GetHeight(unsigned int uiMipmapLevel = 0,
        unsigned int uiFace = 0) const;

    unsigned int GetPixelStride() const;

    size_t GetSizeInBytes(unsigned int uiMipmapLevel = 0,
        unsigned int uiFace = 0) const;
    ptrdiff_t GetOffsetInBytes(unsigned int uiMipmapLevel = 0,
        unsigned int uiFace = 0) const;

    size_t GetTotalSizeInBytes() const;
    size_t GetFaceSizeInBytes() const;

    const NiPixelFormat& GetPixelFormat() const;

    void SetPalette(NiPalette* pkPal);
    const NiPalette* GetPalette() const;
    NiPalette* GetPalette();

    const unsigned char* GetPixels(unsigned int uiMipmapLevel = 0,
        unsigned int uiFace = 0) const;
    unsigned char* GetPixels(unsigned int uiMipmapLevel = 0,
        unsigned int uiFace = 0);

    void MarkAsChanged();

    unsigned int GetRevisionID() const;

    const unsigned char* operator()(unsigned int uiX, unsigned int uiY, 
        unsigned int uiMipmapLevel = 0, unsigned int uiFace = 0) const;
    unsigned char* operator()(unsigned int uiX, unsigned int uiY, 
        unsigned int uiMipmapLevel = 0, unsigned int uiFace = 0);

    static unsigned int ComputeMipmapLevels (unsigned int uiWidth, 
        unsigned int uiHeight);

    /*** begin Emergent internal use only ***/
    bool ConvertToOppositeEndian(void);
    /*** end Emergent internal use only ***/
protected:
    NiPixelData(); 

    void AllocateData(unsigned int uiLevels, unsigned int uiFaces,
        unsigned int uiBytes);
    void FreeData();

    NiPixelFormat m_kPixelFormat;

    NiPalettePtr m_spPalette;

    unsigned char* m_pucPixels; // [pixels for all miplevels]

    unsigned int* m_puiWidth; // [miplevels] 
    unsigned int* m_puiHeight; // [miplevels] 
    unsigned int* m_puiOffsetInBytes; // [miplevels+1] 
    // Note that m_puiOffsetInBytes[m_uiMipmapLevels] is the same as the
    // total size of a given face's entire mipmap pyramid
    
    unsigned int m_uiMipmapLevels;
    unsigned int m_uiPixelStride;
    unsigned int m_uiRevID;
    unsigned int m_uiFaces;
};

typedef NiPointer<NiPixelData> NiPixelDataPtr;

#include "NiPixelData.inl"

#endif
