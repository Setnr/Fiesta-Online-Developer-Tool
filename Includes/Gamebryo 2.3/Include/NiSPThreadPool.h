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
#ifndef NISPTHREADPOOL_H
#define NISPTHREADPOOL_H

//---------------------------------------------------------------------------
#include <NiSystem.h>
#include <NiLoopedThread.h>
#include <NiTArray.h>
#include <NiTQueue.h>
#include "NiFloodgateLibType.h"

//---------------------------------------------------------------------------
// NiSPTaskThreadPool maintains a pool of NiLoopedThreads used 
// for executing Tasks, each on a separate thread.
class NiSPTask;
class NiSPWorkflow;
class NiThread;
class NiSPJobList;
class NIFLOODGATE_ENTRY NiSPThreadPool : public NiMemObject
{
public:
    
    // Constructor / Destructor
    NiSPThreadPool(unsigned int uiMaxThreads);
    virtual ~NiSPThreadPool();

    // Initializes object pools
    void Initialize(); 
    
    // Shuts down object pools
    void Shutdown();

    // Clears the thread. 
    void Clear();

    // Executes a workflow
    void ExecuteWorkflow(NiSPWorkflow* pkWorkflow);

    // Enqueues a job list
    void Enqueue(NiSPJobList* pkJobList);
    
    // Dequeues a job list
    NiSPJobList* Dequeue();

    /*** end Emergent internal use only ***/
protected:

    class WorkerProc : public NiThreadProcedure
    {
    public:
        virtual unsigned int ThreadProcedure(void* pvArg);
        void SetPool(NiSPThreadPool *pkPool)
        {
            m_pkPool = pkPool;
        }
        void SignalShutdown() {m_bShutdown = true;}

    protected:
        NiSPThreadPool *  m_pkPool;
        volatile bool     m_bShutdown;
    };

    typedef NiTPrimitiveArray<NiThread*> ThreadArray;
    
    NiCriticalSection       m_kLock;
    WorkerProc*             m_akWorkerProcs;
    ThreadArray*            m_apkWorkerThreads;
    unsigned int            m_uiMaxWorkerThreads;

    NiTPrimitiveQueue<NiSPJobList*> m_kWorkQueue;
    NiSemaphore                     m_kWorkQueueSemaphore;

};
    
//---------------------------------------------------------------------------
#include "NiSPThreadPool.inl"
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
