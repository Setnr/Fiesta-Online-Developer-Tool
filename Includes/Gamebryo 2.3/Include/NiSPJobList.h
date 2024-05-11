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

#ifndef NISPJOBLIST_H
#define NISPJOBLIST_H
//---------------------------------------------------------------------------
#include <NiSystem.h>
#include <NiTPool.h>
#include "NiSPJob.h"
#include "NiSPTask.h"
//---------------------------------------------------------------------------
class NIFLOODGATE_ENTRY NiSPJobList : public NiMemObject
{
public:

    // Constructor / Destructor
    NiSPJobList();
    virtual ~NiSPJobList();

    // Initializes the job list from data in the task.
    // Returns true if all data was processed,
    // false if an additional job list is needed
    bool Initialize(NiSPTask* pkTask);

    // Returns the number of jobs to be sent to SPU
    inline NiUInt32 GetJobCount();

    // Get the workload at the index specified
    inline NiSPWorkload& GetWorkload(NiUInt32 uiIndex);
        
    // Determines if the job list is finished
    inline void SetIsFinished(bool bIsFinished);
    inline bool IsFinished() const;

protected:

    // Max number of jobs in a job list
    static const NiUInt32 MAX_JOBS = 32;

    // Builds a single job
    void Build(NiUInt32 uiJobIndex, NiSPTask* pkTasks);

    NiSPWorkload    m_akWorkload[MAX_JOBS]; // Workloads
    NiUInt32        m_uiJobCount;           // Number of jobs segments
    bool            m_bIsFinished;          // Indicates if all work is done
};

#include "NiSPJobList.inl"

#endif
