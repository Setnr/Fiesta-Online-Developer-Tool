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

#ifndef NISTRINGPALETTE_H
#define NISTRINGPALETTE_H

#include "NiAnimationLibType.h"
#include <NiObject.h>

class NIANIMATION_ENTRY NiStringPalette : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;

public:
    NiStringPalette(unsigned int uiAllocatedSize);
    ~NiStringPalette();

    char* GetBasePointer() const;
    unsigned int AddString(const char* pcString);

protected:
    // For streaming only.
    NiStringPalette();

    bool FindString(const char* pcString, unsigned int& uiOffset);
    void ReallocateBuffer();

    char* m_pcBuffer;
    unsigned int m_uiAllocatedSize;
    unsigned int m_uiEndOfBuffer;
};

NiSmartPointer(NiStringPalette);

#include "NiStringPalette.inl"

#endif  // #ifndef NISTRINGPALETTE_H
