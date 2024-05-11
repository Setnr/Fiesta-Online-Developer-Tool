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

#ifndef NIPSYSSPAWNMODIFIER_H
#define NIPSYSSPAWNMODIFIER_H

#include "NiPSysModifier.h"

class NIPARTICLE_ENTRY NiPSysSpawnModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysSpawnModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysSpawnModifier(const char* pcName,
        unsigned short usNumSpawnGenerations = 1,
        float fPercentageSpawned = 1.0f, unsigned short usMinNumToSpawn = 1,
        unsigned short usMaxNumToSpawn = 1, float fSpawnSpeedChaos = 0.0f,
        float fSpawnDirChaos = 0.0f, float fLifeSpan = 0.0f,
        float fLifeSpanVar = 0.0f);

    virtual void Update(float fTime, NiPSysData* pkData);

    // This function uses the spawning variables to determine how many
    // particles to spawn.
    virtual void SpawnParticles(float fCurrentTime, float fSpawnTime,
        unsigned short usOldIndex, NiParticleSystem* pkOldPSystem);

    // This function just spawns a single particle.
    virtual void SpawnParticle(float fCurrentTime, float fSpawnTime,
        unsigned short usOldIndex, NiParticleSystem* pkOldPSystem);

    unsigned short GetNumSpawnGenerations() const;
    void SetNumSpawnGenerations(unsigned short usNumSpawnGenerations);

    float GetPercentageSpawned() const;
    void SetPercentageSpawned(float fPercentageSpawned);

    unsigned short GetMinNumToSpawn() const;
    void SetMinNumToSpawn(unsigned short usMinNumToSpawn);

    unsigned short GetMaxNumToSpawn() const;
    void SetMaxNumToSpawn(unsigned short usMaxNumToSpawn);

    float GetSpawnSpeedChaos() const;
    void SetSpawnSpeedChaos(float fSpawnSpeedChaos);

    float GetSpawnDirChaos() const;
    void SetSpawnDirChaos(float fSpawnDirChaos);

    float GetLifeSpan() const;
    void SetLifeSpan(float fLifeSpan);

    float GetLifeSpanVar() const;
    void SetLifeSpanVar(float fLifeSpanVar);

protected:
    // For cloning and streaming only.
    NiPSysSpawnModifier();

    unsigned short m_usNumSpawnGenerations;
    float m_fPercentageSpawned;
    unsigned short m_usMinNumToSpawn;
    unsigned short m_usMaxNumToSpawn;
    float m_fSpawnSpeedChaos;
    float m_fSpawnDirChaos;
    float m_fLifeSpan;
    float m_fLifeSpanVar;
};

#include "NiPSysSpawnModifier.inl"

#endif  // #ifndef NIPSYSSPAWNMODIFIER_H
