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

#ifndef NIOPTTRISHAPE_H
#define NIOPTTRISHAPE_H

#include <NiTriShape.h>
#include "NiOptGeometry.h"

class NIOPTIMIZATION_ENTRY NiOptTriShape : public NiOptGeometry
{
public:
    enum ConvertError
    {
        OPT_OK = 0,
        OPT_ALLDEGENERATES = 1,
        OPT_DUPCALLERROR = 2,
        OPT_VERTLIMITERROR = 3,
        OPT_TRISHAPEFAILED = 4
    };

    NiOptTriShape(unsigned int uiVertices, NiOptVertex* pkVertex,
        unsigned int uiNumBones, void** ppvBones,
        void *pvRootParent, NiTransform* pkInitXforms,
        NiTransform &kRootParentToSkin, unsigned int uiTriListLength,
        unsigned int* puiTriList, bool bAllowLargeTriList, bool bMakeTwoSided,
        bool bKeepDegenerates = false);

    ~NiOptTriShape();

    ConvertError ConvertTo(NiTriShape*& pkShape);

protected:
    // vertex reduction
    virtual void RemoveDegenerateVertices();

    // triangle reduction
    void RemoveDegenerateTriangles();

    // Gamebryo object construction
    void CreateNiTriShape(NiTriShape*& pkShape);
    void IdentifySharedNormals(NiTriShape* pkShape);

    // for two sided materials
    void MakeTwoSided();

    unsigned int m_uiTriListLength;
    unsigned int* m_puiTriList;
    bool m_bAllowLargeTriList;
    bool m_bKeepDegenerates;
};

#endif  // #ifndef NIOPTTRISHAPE_H
