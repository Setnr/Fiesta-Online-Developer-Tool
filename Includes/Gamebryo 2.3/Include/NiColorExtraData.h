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

#ifndef NICOLOREXTRADATA_H
#define NICOLOREXTRADATA_H

#include "NiColor.h"
#include "NiExtraData.h"

class NIMAIN_ENTRY NiColorExtraData : public NiExtraData
{
    NiDeclareRTTI;
    NiDeclareClone(NiColorExtraData);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiColorExtraData(const NiColorA kColorA);
    NiColorExtraData(const NiColor kColor);
    ~NiColorExtraData();

    void SetValue(const NiColorA kColorA);  // 4-tuple accessors
    const NiColorA GetValue() const;

    void SetValue(const NiColor kColor);    // 3-tuple accessors
    const NiColor GetValueColor3() const;

    void SetRed(const float fRed);  // Component accessors
    void SetGreen(const float fGreen);
    void SetBlue(const float fBlue);
    void SetAlpha(const float fAlpha);
    float GetRed() const;
    float GetGreen() const;
    float GetBlue() const;
    float GetAlpha() const;

protected:
    // support for streaming
    NiColorExtraData();

    NiColorA m_kColorA;
};

NiSmartPointer(NiColorExtraData);
//---------------------------------------------------------------------------
//  Inline include
#include "NiColorExtraData.inl"

//---------------------------------------------------------------------------

#endif

