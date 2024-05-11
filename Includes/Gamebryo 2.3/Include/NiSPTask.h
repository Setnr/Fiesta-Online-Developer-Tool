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
#ifndef NISPTASK_H
#define NISPTASK_H
//---------------------------------------------------------------------------
#include <NiSystem.h>
#include <NiTPtrSet.h>
#include <NiTPool.h>
#include <NiString.h>

#include "NiSPStream.h"
#include "NiStreamProcessor.h"
#include "NiSPTaskImpl.h"
//---------------------------------------------------------------------------
// NiSPTask is a proxy to NiSPStreamTask.
class NiSPKernel;
class NiSPStreamPartitioner;
class NIFLOODGATE_ENTRY NiSPTask : public NiMemObject
{
    friend class NiSPWorkflow;
public:

    // Maximum number of input streams
    static const NiUInt32 MAX_INPUTS = 16;
    
    // Maximum number of output streams
    static const NiUInt32 MAX_OUTPUTS = 8;
    
    // Constructor / Destructor
    NiSPTask();
    virtual ~NiSPTask();

    // Specifies the kernel to be used for this task
    inline void SetKernel(NiSPKernel* pkKernel);

    // Gets the kernel to be used for this task
    inline NiSPKernel* GetKernel() const;

    // Gets the number of input streams
    inline NiUInt32 GetInputCount() const;

    // Gets the number of output streams
    inline NiUInt32 GetOutputCount() const;

    // Gets the total number of streams
    inline NiUInt32 GetTotalCount() const;

    // Gets an input stream by index
    inline NiSPStream* GetInputAt(NiUInt32 uiIndex) const;

    // Gets an output stream by index
    inline NiSPStream* GetOutputAt(NiUInt32 uiIndex) const;

    // Adds an input to the task
    inline void AddInput(NiSPStream* pkInputStream);

    // Adds an output to the task
    inline void AddOutput(NiSPStream* pkOutputStream);

    // Sets the synchronization data
    inline void SetSyncData(NiUInt64 uiExtraData);

    // Gets the synchronization data
    inline NiUInt64 GetSyncData() const;

    // Indicates if this task can be cached by a workflow
    inline bool IsCacheable() const;

    // Specifies if this task can be cached by a workflow
    inline void SetIsCacheable(bool bIsCacheable);

    // Specifies the optimal block count for this task
    inline void SetOptimalBlockCount(NiUInt32 uiBlockCount);
    inline NiUInt32 GetOptimalBlockCount();

    /*** begin Emergent internal use only ***/
    
    enum Status
    {
        IDLE = 0,
        PENDING,
        RUNNING,
        COMPLETED,
        ABORTED
    };
    
    // Gets the id for this task
    inline NiUInt32 GetId() const;

    // Gets the parent id for this task
    inline NiUInt32 GetWorkflowId() const;

    // sets the parent id for this task
    inline void SetWorkflowId(NiUInt32);

    // Determines the current status of a task
    inline Status GetStatus() const;

    // Gets the stage number for this task
    inline unsigned short GetStage() const;

    // Sets the stage number for this task
    inline void SetStage(unsigned short usStage);
   
    // Gets the state of the marked flag
    inline bool IsMarked() const;

    // Sets the state of the marked flag
    inline void SetIsMarked(bool bIsMarked);

    // Indicates if this task is a root
    inline bool IsRoot() const;

    // Indicates if this task is a leaf
    inline bool IsLeaf() const;

    // Indicates if this is a synchronization task
    inline bool IsSync() const;

    // Indicates if the task's streams contained aligned data
    inline bool IsAligned() const;
    
    // Indicates if the task is currently cached
    inline bool IsCached() const;

    // Sets task as cached
    inline void SetIsCached(bool bCached);

    // Gets the implementation
    inline NiSPTaskImpl& GetImpl();

    // Prepares the task for scheduling and execution
    void Prepare();

    // Clears a task. 
    void Clear(bool bIgnoreCachine = false);

    /*** end Emergent internal use only ***/
protected:

    typedef NiTPrimitivePtrSet<NiSPStream*> StreamPtrSet;

    // Class Members
    static NiUInt32  ms_uiNextTaskId;
    
    // Methods
    // Sets the current status of a task
    inline void SetStatus(Status eStatus);

    // Resets the task for another run
    inline void Reset();

    // Members
    StreamPtrSet            m_kInputStreams;        // Input streams
    StreamPtrSet            m_kOutputStreams;       // Output streams
    NiUInt32                m_uiTaskId;             // id for task
    NiUInt32                m_uiWorkflowId;         // id for workflow
    NiSPTaskImpl            m_kImpl;                // Private implementation
    NiSPKernel*             m_pkKernel;             // Work function proxy
    NiUInt32                m_uiSliceSize;          // Size of slice...
    Status                  m_eStatus;              // Task status
    NiUInt64                m_uiSyncData;           // Synchronization data
    NiUInt32                m_uiOptimalBlockCount;  // Optimal xfer size 
    unsigned short          m_usStage;              // Task stage
    bool                    m_bIsMarked;            // Marker for graph trav.
    bool                    m_bIsRoot;              // Is this a root node
    bool                    m_bIsLeaf;              // Is this a leaf node
    bool                    m_bIsSync;              // Is this a sync task
    bool                    m_bIsAligned;           // Is all data aligned
    bool                    m_bIsCached;            // Is it cached
};

inline bool operator<(const NiSPTask& pkLhs, const NiSPTask& pkRhs);
    
//---------------------------------------------------------------------------
#include "NiSPTask.inl"
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
