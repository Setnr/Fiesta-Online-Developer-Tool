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

#ifndef NIBINARYLOADSAVE_H
#define NIBINARYLOADSAVE_H

#include "NiRTLib.h"
#include "NiBinaryStream.h"
#include "NiSystemLibType.h"

//---------------------------------------------------------------------------
// Inline load/save routines for endian-aware streams.
//---------------------------------------------------------------------------
template <class T>
void NiBinaryStreamLoad (NiBinaryStream& is, T* pValue, 
    unsigned int uiNumEls = 1)
{
    NIASSERT(uiNumEls > 0);
    unsigned int uiSize = sizeof(T);
    is.BinaryRead(pValue, uiSize * uiNumEls, &uiSize, 1);
}

//---------------------------------------------------------------------------
inline void NiBinaryStreamLoad (NiBinaryStream& is, void* pvValue, 
    unsigned int uiNumEls, unsigned int* puiComponentSizes,
    unsigned int uiNumComponents = 1)
{
    NIASSERT(uiNumEls > 0);
    NIASSERT(puiComponentSizes != NULL);
    NIASSERT(uiNumComponents > 0);

    unsigned int uiSize = 0;
    {
        for (unsigned int uiComp = 0; uiComp < uiNumComponents; uiComp++)
        {
            unsigned int uiElSize = puiComponentSizes[uiComp];
            uiSize += uiElSize;
            NIASSERT(uiElSize == 1 || uiElSize == 2 || 
                uiElSize == 4 || uiElSize == 8);
        }
    }
    NIASSERT(uiSize != 0);

    is.BinaryRead(pvValue, uiSize * uiNumEls, puiComponentSizes, 
        uiNumComponents);
}

//---------------------------------------------------------------------------
template <class T>
void NiBinaryStreamLoadEnum (NiBinaryStream& is, T* pValue)
{
    int iDummy;
    unsigned int uiSize = sizeof(int);
    is.BinaryRead(&iDummy, uiSize, &uiSize, 1);
    *pValue = (T)iDummy;
}

//---------------------------------------------------------------------------
template <class T>
void NiBinaryStreamSave (NiBinaryStream& os, const T* pValue,
    unsigned int uiNumEls = 1)
{
    NIASSERT(uiNumEls > 0);
    unsigned int uiSize = sizeof(T);
    os.BinaryWrite(pValue, uiSize * uiNumEls, &uiSize, 1);
}

//---------------------------------------------------------------------------
inline void NiBinaryStreamSave (NiBinaryStream& os, const void* pvValue, 
    unsigned int uiNumEls, unsigned int* puiComponentSizes,
    unsigned int uiNumComponents)
{
    NIASSERT(uiNumEls != 0);
    NIASSERT(puiComponentSizes != NULL);
    NIASSERT(uiNumComponents > 0);

    unsigned int uiSize = 0;
    for (unsigned int uiComp = 0; uiComp < uiNumComponents; uiComp++)
    {
        unsigned int uiElSize = puiComponentSizes[uiComp];
        uiSize += uiElSize;
        NIASSERT(uiElSize == 1 || uiElSize == 2 || 
            uiElSize == 4 || uiElSize == 8);
    }
    os.BinaryWrite(pvValue, uiSize * uiNumEls, puiComponentSizes, 
        uiNumComponents);
}

//---------------------------------------------------------------------------
template <class T>
void NiBinaryStreamSaveEnum (NiBinaryStream& os, const T& value)
{
    unsigned int uiSize = sizeof(int);
    os.BinaryWrite(&value, uiSize, &uiSize, 1);
}

#endif // NIBINARYLOADSAVE_H
