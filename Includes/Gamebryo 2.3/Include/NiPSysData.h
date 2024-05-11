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

#ifndef NIPSYSDATA_H
#define NIPSYSDATA_H

#include "NiParticleLibType.h"
#include <NiParticlesData.h>
#include "NiParticleInfo.h"

class NIPARTICLE_ENTRY NiPSysData : public NiParticlesData
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysData);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    static const unsigned short INVALID_PARTICLE;

    // The constructed object is given ownership of the input arrays and
    // has the responsibility for deleting them when finished with them.
    NiPSysData(unsigned short usMaxNumParticles, bool bHasColors,
        bool bHasRotations);
    virtual ~NiPSysData();

    NiParticleInfo* GetParticleInfo();
    const NiParticleInfo* GetParticleInfo() const;
    void Replace(NiParticleInfo* pkParticleInfo);

    float* GetRotationSpeeds();
    const float* GetRotationSpeeds() const;
    void ReplaceRotationSpeeds(float* pfRotationSpeeds);

    virtual void RemoveParticle(unsigned short usParticle);

    // AddParticle returns the index to the new particle.
    // ResolveAddedParticles must be called by the NiParticleSystem object
    // to actually add the particles to the particle array. Particles are
    // only added to the active particle count after all modifiers have been
    // updated.
    virtual unsigned short AddParticle();
    virtual void ResolveAddedParticles();

    // Removes all particles and clears any new particles waiting to be added.
    void ClearAllParticles();

    unsigned short GetMaxNumParticles() const;
    unsigned short GetNumParticles() const;

    // *** begin Emergent internal use only ***

    // For temporarily using data only. This call is like Replace(), 
    // but does not delete any pointers. Be careful to call this only 
    // on objects created by the default (empty) constructor to prevent 
    // memory leaks. These pointers will be owned (and deleted upon 
    // destruction) by this object so use this call carefully.
    void SetData(NiParticleInfo* pkParticleInfo);
    void SetRotationSpeedsData(float* pfRotationSpeeds);

    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysData();

    NiParticleInfo* m_pkParticleInfo;
    float* m_pfRotationSpeeds;

    unsigned short m_usNumAddedParticles;
    unsigned short m_usAddedParticlesBase;
};

NiSmartPointer(NiPSysData);

#include "NiPSysData.inl"

#endif  // #ifndef NIPSYSDATA_H
