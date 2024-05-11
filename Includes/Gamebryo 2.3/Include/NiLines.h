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

#ifndef NILINES_H
#define NILINES_H

#include "NiGeometry.h"
#include "NiLinesData.h"

class NIMAIN_ENTRY NiLines : public NiGeometry
{
public:
    NiDeclareRTTI;
    NiDeclareClone(NiLines);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    // The constructed object is given ownership of the input arrays and
    // has the responsibility for deleting them when finished with them.
    NiLines(unsigned short usVertices, NiPoint3* pkVertex, NiColorA* pkColor,
        NiPoint2* pkTexture, unsigned short usNumTextureSets, 
        NiGeometryData::DataFlags eNBTMethod, NiBool* pbFlags);

    // The constructed object shares the input data.
    NiLines(NiLinesData* pkModelData);

    // connection flags
    NiBool* GetFlags();
    const NiBool* GetFlags() const;

    // display
    virtual void RenderImmediate(NiRenderer* pkRenderer);

    // *** begin Emergent internal use only ***
    virtual void UpdateEffectsDownward(NiDynamicEffectState* pkParentState);
    // *** end Emergent internal use only ***

protected:
    // streaming support
    NiLines();
};

typedef NiPointer<NiLines> NiLinesPtr;

//---------------------------------------------------------------------------
//  Inline include
#include "NiLines.inl"

//---------------------------------------------------------------------------

#endif

