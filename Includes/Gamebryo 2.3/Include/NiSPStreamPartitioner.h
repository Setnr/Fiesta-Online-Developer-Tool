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

#ifndef NISPSTREAMPARTITIONER_H
#define NISPSTREAMPARTITIONER_H
//---------------------------------------------------------------------------
#include <NiSystem.h>
#include <NiMath.h>
#include "NiSPWorkload.h"
//---------------------------------------------------------------------------

// Stream Partitioning Policy
class NiSPJobList;
class NiSPTask;
class NiSPStream;
class NIFLOODGATE_ENTRY NiSPStreamPartitioner : public NiMemObject
{
public:

    // Constructor / Destructor
    NiSPStreamPartitioner();
    virtual ~NiSPStreamPartitioner(){}
    
    // Gets the optimal number of blocks to send at one time
    inline NiUInt32 GetOptimalBlockCount();

    // Indicates if the set has more chunks to transfer
    inline bool HasMoreBlocks();

    // Resets the set back to the start of its streams
    inline void Reset();

    // Gets the current chunk index
    inline NiInt32 GetBlockIndex();

    // Gets the number of blocks to transfer
    inline NiInt32 GetTransferBlockCount();

    // Gets the size of a chunk across all streams in bytes
    inline NiUInt32 GetTransferSize();

    // Sets the task to partition
    inline void SetTask(NiSPTask* pkTask);

    // Increases the chunk index by transfer chunk count
    inline void AdvanceBlockIndex();

     // Initializes the stream partitioner
    virtual void Initialize(NiSPTask* pkTask);

    // Partition data from streams 
    virtual void Partition(NiSPJobList* pkJob, NiUInt32 uiJobIndex,
        NiUInt32& uiInputSize, NiUInt32& uiOutputSize);

    // Partition input stream
    virtual NiUInt32 PartitionInput(NiSPJobList* pkJob, NiUInt32 uiJobIndex,
        NiSPStream* pStream, NiSPWorkload::Header& kWorkloadHeader);

    // Partition output stream
    virtual NiUInt32 PartitionOutput(NiSPJobList* pkJob, NiUInt32 uiJobIndex,
        NiSPStream* pStream, NiSPWorkload::Header& kWorkloadHeader);


protected:

    // Creates the meta data to pass to SPU describing streams
    void InitializeWorkload(NiSPWorkload& kWorkload);


    // Members
    NiUInt32    m_uiOptimalBlockCount; // Number of blocks to xfer at a time
    NiUInt32    m_uiCurrentBlockIndex;  // Pointer to current block to xfer
    NiUInt32    m_uiTotalBlockCount;    // Number of blocks per stream
    NiUInt32    m_uiTransferSize;       // Size of one blocks in bytes
    NiSPTask*  m_pkTask;               // Task containing streams

};

#include "NiSPStreamPartitioner.inl"

#endif
