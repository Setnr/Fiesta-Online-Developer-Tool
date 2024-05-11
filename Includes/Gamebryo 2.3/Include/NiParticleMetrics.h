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

#ifndef NIPARTICLEMETRICS_H
#define NIPARTICLEMETRICS_H

#include "NiParticleLibType.h"
#include <NiMetricsLayer.h>
#include <NiMetricsScopeTimer.h>

#define NIMETRICS_PARTICLE_PREFIX NIMETRICS_PREFIX "particle."

class NIPARTICLE_ENTRY NiParticleMetrics
{
public:
    enum NiMetric
    {
        // Number of particles updated in NiParticleSystem::NiUpdate
        UPDATED_PARTICLES,

        // Number of particles updated in NiPSysModifier::Update()
        UPDATED_MODIFIERS,

        // Number of particles spawned in NiParticleSystem::NiUpdate
        PARTICLES_SPAWNED,

        // Number of particles destroyed in NiParticleSystem::NiUpdate
        PARTICLES_DESTROYED,

        // Time for each particle update call (in seconds),
        UPDATE_PSYS_TIME,

        NUM_METRICS
    };

    const static char ms_acNames[NUM_METRICS][NIMETRICS_NAMELENGTH];
};

#if NIMETRICS
    #define NIMETRICS_PARTICLE_ADDVALUE(eMetric, fValue) \
        NIMETRICS_ADDVALUE(NiParticleMetrics::ms_acNames[ \
            NiParticleMetrics::eMetric], fValue);
    #define NIMETRICS_PARTICLE_SCOPETIMER(eMetric) \
        NiMetricsScopePCTimer kMetricsTimer(NiParticleMetrics::ms_acNames[ \
        NiParticleMetrics::eMetric]);
#else //#if NIMETRICS
    #define NIMETRICS_PARTICLE_ADDVALUE(eMetric, fValue)
    #define NIMETRICS_PARTICLE_SCOPETIMER(eMetric)
#endif //#if NIMETRICS

#endif //#ifndef NIPARTICLEMETRICS_H
