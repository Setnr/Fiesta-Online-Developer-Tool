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

#ifndef NIDEVIMAGEQUANTIZER_H
#define NIDEVIMAGEQUANTIZER_H

#include <NiDevImageConverter.h>
#include "NiDevImageQuantizerLibType.h"

class QuantizeUtil;
struct IDirectDraw7;

//---------------------------------------------------------------------------
class NIDEVIMAGEQUANTIZER_ENTRY NiDevImageQuantizer : 
    public NiDevImageConverter
{
public:
    static NiDevImageQuantizer* Create();

    virtual ~NiDevImageQuantizer();
    
    static NiPixelDataPtr ProcessPixelData(NiPixelDataPtr spPixelData, 
        NiTexture::FormatPrefs kPrefs, bool bMipMap, bool bUseAlphaIfAvailable,
        NiPixelFormat kDestFormat, unsigned int uiSizeDivideBy, 
        NiDevImageConverter::Platform ePlatform, const char* pcFilename, 
        char* pcErrorBuffer, unsigned int uiErrorBufferLength);

    static char* GetImageFormatString(NiPixelFormat kFormat);
    static unsigned int CalcMaskSize(unsigned int uiMask);

    virtual bool CanConvertPixelData(const NiPixelFormat& kSrcFmt, 
        const NiPixelFormat& kDestFmt);

    virtual NiPixelData* ConvertPixelData(const NiPixelData& kSrc, 
        const NiPixelFormat& kDestFmt, NiPixelData* pkOptDest, bool bMipmap);

    virtual bool ConvertPixelDataFormat(NiPixelData& kDest, 
        const NiPixelData& kSrc, int iMipmapLevel = 0);

    virtual bool IsAnAlphaSupportedMipmapGenerationFormat(const 
        NiPixelFormat& kSrcFmt);

    virtual bool IsANonAlphaSupportedMipmapGenerationFormat(const 
        NiPixelFormat& kSrcFmt);

    virtual NiPixelDataPtr GenerateMipmapLevels(NiPixelDataPtr spSrc, 
        NiPixelDataPtr spOptDest);

    // *** begin Emergent internal use only ***
    void SetNumberOfColorBits( unsigned int uiColorBits );

    // *** end Emergent internal use only ***

protected:
    NiDevImageQuantizer();
    
    // Incoming format of kSrc _must_ be m_kRGBAFormat
    bool Quantize(NiPixelData& kDest, const NiPixelData& kSrc, 
        int iMipmapLevel);

    static bool FormatHasAlpha(NiPixelFormat eFormat);
    static bool ConvertIfNecessaryForAlphaSwitch(NiPixelFormat& eFormat);

    const NiPixelFormat m_kRGBAFormat;
    QuantizeUtil *m_pQuantUtil;

    // Incoming format of kSrc _must_ be m_kRGBAFormat
    bool Compress(NiPixelData& kDest, const NiPixelData& kSrc, 
        int iMipmapLevel);

    bool RGBA128toRGBA64(NiPixelData& kDest, const NiPixelData& kSrc);
    bool RGBA64toRGBA128(NiPixelData& kDest, const NiPixelData& kSrc);

    NiPixelDataPtr GenerateMipmapLevelsRGBA64(NiPixelDataPtr spSrc, 
        NiPixelDataPtr spOptDest);
    NiPixelDataPtr GenerateMipmapLevelsRGBA128(NiPixelDataPtr spSrc, 
        NiPixelDataPtr spOptDest);

    IDirectDraw7* m_pkDD7;
    void* m_pkDDrawDLL;
};

#endif
