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

#ifndef NISPJOB_H
#define NISPJOB_H

//---------------------------------------------------------------------------
#include <NiSystem.h>
#include <NiTPtrSet.h>
#include "NiSPWorkload.h"
#include "NiSPJobList.h"
//---------------------------------------------------------------------------
class NiSPTask;
class NIFLOODGATE_ENTRY NiSPJob : public NiTPrimitivePtrSet<NiSPJobList*>
{
public:

    // Initialize pools
    static void InitializePools(NiUInt32 uiJobListPoolSize);
    
    // Shutdown pool
    static void ShutdownPools();

    // Constructor / Destructor
    NiSPJob();
    virtual ~NiSPJob();

    // Initializes the job
    virtual void Initialize(NiSPTask* pkTask);
    
    // Clears a job and releases job lists back to pool
    void Clear();

protected:

    // Makes an educated guess about the amount of LS available
    inline NiUInt32 EstimateAvailableJobMemory();

    typedef NiTObjectPool<NiSPJobList> JobListPool;
    static JobListPool* ms_pkJobListPool;
    NiSPTask* m_pkTask;
};

#include "NiSPJob.inl"

#endif
