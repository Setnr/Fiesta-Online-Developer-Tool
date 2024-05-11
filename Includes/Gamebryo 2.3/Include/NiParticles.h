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

#ifndef NIPARTICLES_H
#define NIPARTICLES_H

#include "NiGeometry.h"
#include "NiParticlesData.h"


class NIMAIN_ENTRY NiParticles : public NiGeometry
{
    NiDeclareRTTI;
    NiDeclareClone(NiParticles);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    // The constructed object is given ownership of the input arrays and
    // has the responsibility for deleting them when finished with them.
    NiParticles(unsigned short usVertices, NiPoint3* pkVertex,
        NiPoint3* pkNormal, NiColorA* pkColor, NiQuaternion* pkRots = NULL,
        float* pfRotationAngles = NULL, NiPoint3* pkRotationAxes = NULL);

    // The constructed object shares the input data.
    NiParticles(NiParticlesData* pkModelData);

    // particle attributes
    float* GetRadii();
    float* GetSizes();
    NiQuaternion* GetRotations();

    // display
    virtual void RenderImmediate(NiRenderer* pkRenderer);

    // *** begin Emergent internal use only ***
    virtual void UpdateEffectsDownward(NiDynamicEffectState* pkParentState);

    virtual void CalculateConsistency(bool bTool = false);
    // *** end Emergent internal use only ***

protected:
    // streaming support
    NiParticles();
};

typedef NiPointer<NiParticles> NiParticlesPtr;

#include "NiParticles.inl"

#endif // NIPARTICLES_H
