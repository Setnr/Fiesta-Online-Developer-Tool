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

//---------------------------------------------------------------------------
#ifndef NISTREAMPROCESSOR_H
#define NISTREAMPROCESSOR_H
//---------------------------------------------------------------------------
#include <NiSystem.h>
#include <NiTPointerMap.h>
#include <NiProcessorAffinity.h>
#include <NiThread.h>
#include <NiSemaphore.h>
#include <NiTQueue.h>

#include "NiFloodgateSDM.h"
#include "NiSPWorkflow.h"
#include "NiSPTask.h"
#include "NiSPAnalyzer.h"
#include "NiSPThreadPool.h"

static NiFloodgateSDM NiFloodgateSDMObject;

//---------------------------------------------------------------------------
class NIFLOODGATE_ENTRY NiStreamProcessor : public NiMemObject
{
public:

    enum Priority
    {
        LOW = 0,
        MEDIUM,
        HIGH,
        IMMEDIATE,
        NUM_PRIORITIES
    };
    
    // Gets the single managed instance
    inline static NiStreamProcessor* Get();

    // Destructor
    ~NiStreamProcessor();

    // Sets processor affinity
    inline bool SetAffinity(const NiProcessorAffinity& kAffinity);

    // Sets thread priority
    inline bool SetPriority(NiThread::Priority ePriority);

    // Gets the total number of tasks
    inline NiUInt32 GetWorkflowCount(Priority ePriority) const;

    // Gets the total number of tasks
    inline NiUInt32 GetTotalWorkflowCount() const;

    // Gets a workflow from the pool
    NiSPWorkflow* GetFreeWorkflow();
    
    // Releases a workflow back to the queue
    void ReleaseWorkflow(NiSPWorkflow* pkWorkflow);
    
    // Clears the workflow queue
    void Clear();

    // Clears the workflow queue
    void Clear(NiSPWorkflow* pkWorkflow);

    // Determines if a workflow has completed
    bool Poll(const NiSPWorkflow* pkWorkflow);

    // Waits for a workflow to complete with optional timeout
    // Returns true if task completed; false if timeout expired
    bool Wait(const NiSPWorkflow* pkWorkflow, NiUInt64 uiTimeout = 0ULL);

    // Submits a Workflow to be executed by the manager.
    // Returns true if the workflow was added or false if the queue was full
    // and the workflow could not be added. The queue size should be increased
    // if this condition is detected.
    bool Submit(NiSPWorkflow* pkWorkflow, Priority ePriority);

    /*** begin Emergent internal use only ***/
    
    enum Status
    {
        STOPPED,
        STOPPING,
        RUNNING
    };

	// This value controls the priority of the scheduler thread
	// (and the receive thread on PS3)
	static const NiThread::Priority eThreadPriority = NiThread::ABOVE_NORMAL;
    
    // Starts the manager
    Status Start();

    // Stops the manager
    Status Stop();

    // Gets the status of the manager
    inline Status GetStatus() const;

    // Indicates if manager is in running state
    inline bool IsRunning();

    // Returns true if the manager exists and active
    inline static bool IsActive();

    // Gets the background thread
    inline const NiThread* GetThread() const;

    // Gets the thread pool
    inline NiSPThreadPool* GetThreadPool();

    // Initializes object pools
    static void InitializePools();

    // Shuts down object pools
    static void ShutdownPools();

    // Initializes the streaming task manager
    static bool Initialize(NiUInt32 uiMaxQueueSize);

    // Shuts down the streaming task manager
    static bool Shutdown();
    
    /*** end Emergent internal use only ***/

    
protected:

    class ManagerProc : public NiThreadProcedure
    {
    public:
        ManagerProc(NiStreamProcessor* pkManager);

        virtual unsigned int ThreadProcedure(void* pvArg);
        
    protected:
        NiThread* m_pkThread;
        NiStreamProcessor* m_pkManager;
    };

    class PollProc : public NiThreadProcedure
    {
    public:
        PollProc(NiStreamProcessor* pkManager);

        virtual unsigned int ThreadProcedure(void* pvArg);
        
    protected:
        NiThread* m_pkThread;
        NiStreamProcessor* m_pkManager;
    };

    // Hidden constructor
    NiStreamProcessor(NiUInt32 uiMaxQueueSize);    

    // Low level Initialization
    bool Initialize();

    // Low level Initialization
    NiStreamProcessor::Status StartInternal();

    // Low level Initialization
    NiStreamProcessor::Status StopInternal();

    // Low level receive
    void Receive();

    // Low level shutdown
    bool ShutdownInternal();
    
    // Executes a workflow
    bool ExecuteOne(Priority ePriority);

    // Marks a workflow and marks it as complete
    void FinishOne(bool bIsBlocking, NiUInt64 uiTimeout = 0);

    // Class Members
    static NiStreamProcessor* ms_pkInstance; // Singelton instance
    
    // NiSPWorkflow object pool
    static NiTObjectPool<NiSPWorkflow>* ms_pkWorkflowPool;       
    
    static const unsigned int NUM_WORKER_THREAD = 6;

    // Members
    typedef NiTPointerMap<NiUInt32, NiSPWorkflow*> NiSPWorkflowPtrMap;
    typedef NiTPrimitiveQueue<NiSPWorkflow*> NiSPWorkflowQueue;

    NiSPWorkflowQueue       m_akWorkflowQueues[NUM_PRIORITIES];
    NiSPWorkflowPtrMap      m_kWorkflows;       // Lookup table for workflows
    NiCriticalSection       m_kManagerLock;     // Serializes access to queues
    NiSPAnalyzer            m_kAnalyzer;        // Task dependency analyzer
    Status                  m_eStatus;          // Manage status
    NiUInt32                m_uiMaxQueueSize;   // Max # of workflows per queue
    NiSPThreadPool*         m_pkThreadPool;     // Thread pool
    
    NiSemaphore             m_kSubmitSemaphore;     // Manages execution thread
    NiThread*               m_pkSubmitThread;       // Submition thread
    ManagerProc*            m_pkSubmitThreadProc;   // Submition proc
    
    NiThread*               m_pkPollThread;         // Polling thread
    PollProc*               m_pkPollThreadProc;     // Polling proc
    
    bool                    m_bIsActive;

};

//---------------------------------------------------------------------------
#include "NiStreamProcessor.inl"

#endif
//---------------------------------------------------------------------------
