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

#ifndef NIOPTVERTEX_H
#define NIOPTVERTEX_H

#include "NiOptimizationLibType.h"
#include <NiColor.h>
#include <NiPoint2.h>
#include <NiPoint3.h>

class NIOPTIMIZATION_ENTRY NiOptVertex : public NiMemObject
{
public:
    // Gamebryo does not have an internal limit on the number of texture
    // sets per object, but most of the renderers do.  To support a renderer
    // that allows more than 20 textures per object, this enumerated value
    // must be increased accordingly.
    enum
    {
        MAX_UV = 100,
        MAX_VERTEX_FLOATS = 1
    };
    
    enum
    {
        INVALID,
        VERTEX_WEIGHT
    };

    NiOptVertex();
    int Compare(const NiOptVertex& kVert) const;

    // vertex attributes
    NiPoint3 m_kPosition;
    NiPoint3 m_kNormal;
    NiColorA m_kColor;
    unsigned int m_uiNumUVs;
    NiPoint2 m_akUV[MAX_UV];
    unsigned int m_uiNumVertexFloats;
    float m_afVertexFloat[MAX_VERTEX_FLOATS];
    unsigned int m_auiVertexFloatType[MAX_VERTEX_FLOATS];

    // bones that influence this vertex
    unsigned int m_uiNumBones;
    unsigned int* m_puiBoneIndex;
    float* m_pfWeight;
};

#endif  // #ifndef NIOPTVERTEX_H
