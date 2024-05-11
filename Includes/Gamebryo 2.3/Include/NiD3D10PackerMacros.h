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

#ifndef NID3D10PACKERMACROS_H
#define NID3D10PACKERMACROS_H

#include "NiD3D10Utility.h"

// Converters to float
//  SNORM32_TO_FLOAT
//  Assumes that the range will be -1.0 to 1.0
#define NIPACKER_SNORM32_TO_FLOAT32(lIn, fOut)          \
    {                                                   \
        float fTemp = ((float)(lIn) / 2147483647.0f);   \
        fOut = (fTemp < -1.0f ? -1.0f : fTemp);         \
    }

//  UNORM32_TO_FLOAT
//  Assumes that the range will be 0.0 to 1.0
#define NIPACKER_UNORM32_TO_FLOAT32(lIn, fOut)          \
    {                                                   \
        fOut = ((float)(lIn) / 4294967295.0f);          \
    }

//  SNORM16_TO_FLOAT
//  Assumes that the range will be -1.0 to 1.0
#define NIPACKER_SNORM16_TO_FLOAT32(sIn, fOut)          \
    {                                                   \
        float fTemp = ((float)(sIn) / 32767.0f);        \
        fOut = (fTemp < -1.0f ? -1.0f : fTemp);         \
    }

//  UNORM16_TO_FLOAT
//  Assumes that the range will be 0.0 to 1.0
#define NIPACKER_UNORM16_TO_FLOAT32(sIn, fOut)          \
    {                                                   \
        fOut = ((float)(sIn) / 65535.0f);               \
    }

//  SNORM8_TO_FLOAT
//  Assumes that the range will be -1.0 to 1.0
#define NIPACKER_SNORM8_TO_FLOAT32(cIn, fOut)          \
    {                                                   \
        float fTemp = ((float)(cIn) / 127.0f);         \
        fOut = (fTemp < -1.0f ? -1.0f : fTemp);         \
    }

//  UNORM8_TO_FLOAT
//  Assumes that the range will be 0.0 to 1.0
#define NIPACKER_UNORM8_TO_FLOAT32(cIn, fOut)          \
    {                                                   \
        fOut = (float)(cIn) / 255.0f;                  \
    }

//---------------------------------------------------------------------------
//  Converters to UBYTECOLOR
#define NIPACKER_FLOAT32_TO_UBYTECOLOR(fA, fR, fG, fB, uiOut)         \
    {                                                               \
        uiOut =                                                     \
            ((NiD3D10Utility::FastFloatToInt(fA * 255.0f)) << 24) | \
            ((NiD3D10Utility::FastFloatToInt(fB * 255.0f)) << 16) | \
            ((NiD3D10Utility::FastFloatToInt(fG * 255.0f)) << 8)  | \
            (NiD3D10Utility::FastFloatToInt(fR * 255.0f));          \
    }

#define NIPACKER_SINT32_TO_UBYTECOLOR(lIn, uiOut)             \
    {                                                       \
        uiOut = (unsigned int)lIn;                          \
    }

#define NIPACKER_UINT32_TO_UBYTECOLOR(lIn, uiOut)            \
    {                                                       \
        uiOut = (unsigned int)lIn;                          \
    }

#define NIPACKER_SINT16_TO_UBYTECOLOR(sInA, sInR, sInG, sInB, uiOut)  \
    {                                                       \
        uiOut =                                             \
            ((((sInA & 0xFF00) >> 8) << 24)     |           \
            (((sInB & 0xFF00) >> 8) << 16)      |           \
            (((sInG & 0xFF00) >> 8) <<  8)      |           \
            (((sInR & 0xFF00) >> 8) <<  0));                \
    }

#define NIPACKER_UINT16_TO_UBYTECOLOR(sInA, sInR, sInG, sInB, uiOut) \
    {                                                       \
        uiOut =                                             \
            ((((sInA & 0xFF00) >> 8) << 24)     |           \
            (((sInB & 0xFF00) >> 8) << 16)      |           \
            (((sInG & 0xFF00) >> 8) <<  8)      |           \
            (((sInR & 0xFF00) >> 8) <<  0));                \
    }

#define NIPACKER_SINT8_TO_UBYTECOLOR(cInA, cInR, cInG, cInB, uiOut)  \
    {                                                   \
        uiOut =                                         \
            (((cInA & 0xFF) << 24)     |               \
            ((cInB & 0xFF) << 16)      |               \
            ((cInG & 0xFF) <<  8)      |               \
            ((cInR & 0xFF) <<  0));                    \
    }

#define NIPACKER_UINT8_TO_UBYTECOLOR(cInA, cInR, cInG, cInB, uiOut)  \
    {                                                   \
        uiOut =                                         \
            (((cInA & 0xFF) << 24)     |               \
            ((cInB & 0xFF) << 16)      |               \
            ((cInG & 0xFF) <<  8)      |               \
            ((cInR & 0xFF) <<  0));                    \
    }


//---------------------------------------------------------------------------
//  Converters to SINT16
#define NIPACKER_FLOAT32_TO_SINT16(fIn, sOut)              \
    {                                                   \
        sOut = (short)fIn;                              \
    }

#define NIPACKER_SINT32_TO_SINT16(lIn, sOut)               \
    {                                                   \
        sOut = (short)lIn;                              \
    }

#define NIPACKER_UINT32_TO_SINT16(lIn, sOut)              \
    {                                                   \
        sOut = (short)lIn;                              \
    }

#define NIPACKER_SINT8_TO_SINT16(cIn, sOut)              \
    {                                                   \
        sOut = (short)cIn;                             \
    }

#define NIPACKER_UINT8_TO_SINT16(cIn, sOut)             \
    {                                                   \
        sOut = (short)cIn;                             \
    }

//---------------------------------------------------------------------------
//  Converters to SNORM16
#define NIPACKER_FLOAT32_TO_SNORM16(fIn, sOut)          \
    {                                                   \
        sOut = (short)fIn;                              \
    }

#define NIPACKER_SINT32_TO_SNORM16(lIn, sOut)           \
    {                                                   \
        sOut = (short)lIn;                              \
    }

#define NIPACKER_UINT32_TO_SNORM16(lIn, sOut)          \
    {                                                   \
        sOut = (short)lIn;                              \
    }

#define NIPACKER_SINT8_TO_SNORM16(cIn, sOut)          \
    {                                                   \
        sOut = (short)cIn;                             \
    }

#define NIPACKER_UINT8_TO_SNORM16(cIn, sOut)         \
    {                                                   \
        sOut = (short)cIn;                             \
    }

//---------------------------------------------------------------------------
//  Converters to PACKEDNORM (float to unsigned int)
#define NIPACKER_FLOAT3_TO_NORMPACKED3(f0, f1, f2, uiOut)       \
    {                                                           \
        uiOut = ((DWORD)(((DWORD)(f0 *  511) & 0x3ff) << 22) |  \
            (DWORD)(((DWORD)(f1 * 1023) & 0x7ff) << 11) |       \
            (DWORD)(((DWORD)(f2 * 1023) & 0x7ff)));             \
    }

//---------------------------------------------------------------------------
//  Converters to PSINT8 (Packed SINT8)
#define NIPACKER_FLOAT_TO_SINT8(fIn, cOut)             \
    {                                                   \
        cOut = (char)fIn;                              \
    }

#define NIPACKER_SINT32_TO_SINT8(lIn, cOut)              \
    {                                                   \
        cOut = (char)lIn;                              \
    }

#define NIPACKER_UINT32_TO_SINT8(lIn, cOut)             \
    {                                                   \
        cOut = (char)lIn;                              \
    }

#define NIPACKER_SINT16_TO_SINT8(sIn, cOut)             \
    {                                                   \
        cOut = (char)sIn;                              \
    }

#define NIPACKER_UINT16_TO_SINT8(sIn, cOut)            \
    {                                                   \
        cOut = (char)sIn;                              \
    }

//---------------------------------------------------------------------------
//  Converters to UINT8 (Unsigned SINT8)
#define NIPACKER_FLOAT32_TO_UINT8(fIn, cOut)             \
    {                                                   \
        cOut = (char)fIn;                              \
    }

#define NIPACKER_SINT32_TO_UINT8(lIn, cOut)              \
    {                                                   \
        cOut = (char)lIn;                              \
    }

#define NIPACKER_UINT32_TO_UINT8(lIn, cOut)             \
    {                                                   \
        cOut = (char)lIn;                              \
    }

#define NIPACKER_SINT16_TO_UINT8(sIn, cOut)             \
    {                                                   \
        cOut = (char)sIn;                              \
    }

#define NIPACKER_UINT16_TO_UINT8(sIn, cOut)            \
    {                                                   \
        cOut = (char)sIn;                              \
    }

//---------------------------------------------------------------------------
//  Converters to UNORM8 (Normalized UINT8)
//  Assumes that the input range will be 0.0 to 1.0
#define NIPACKER_FLOAT32_TO_UNORM8(fIn, cOut)                       \
    {                                                               \
        cOut = (char)NiD3D10Utility::FastFloatToInt(fIn * 255.0f);  \
    }

//---------------------------------------------------------------------------
//  Converters to SINT8
#define NIPACKER_FLOAT32_TO_SINT8(fIn, ucOut)              \
    {                                                   \
        ucOut = (short)fIn;                              \
    }

//---------------------------------------------------------------------------

#endif  //#ifndef NID3D10PACKERMACROS_H
