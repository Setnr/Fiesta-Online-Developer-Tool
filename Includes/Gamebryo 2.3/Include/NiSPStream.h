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

#ifndef NISPSTREAM_H
#define NISPSTREAM_H
//---------------------------------------------------------------------------
#include <NiSystem.h>
#include <NiTPtrSet.h>

#include "NiFloodgateLibType.h"
//---------------------------------------------------------------------------
class NiSPTask;
typedef NiTPrimitivePtrSet<NiSPTask*> NiSPTaskPtrSet;

class NIFLOODGATE_ENTRY NiSPStream : public NiMemObject
{
    
public:

    // Constructor / Destructor
    NiSPStream(void* pData = NULL, NiUInt32 uiStride = 0, 
        NiUInt32 uiBlockCount = 0, bool bPartitioningEnabled = true);
    virtual ~NiSPStream();

    // Gets the a pointer to the data contained within the stream
    inline void* GetData();
    
    // Sets the a pointer to the data contained within the stream
    inline void SetData(void*);
    
    // Gets pointer to the data prealigned to the platform alignment
    inline void* GetAlignedData();
    
    // Sets pointer to the data prealigned to the platform alignment
    inline void SetAlignedData(void*);
    
    // Indicates if the data is already aligned
    inline bool IsDataAligned();

    // Gets the size of the data
    inline NiUInt32 GetDataSize();
    
    // Indicates if the stream contains data that should be partitioned
    // if true, or fixed data that should not be partitioned if false.
    inline bool IsPartitioningEnabled();

    // Gets the stride of the data
    inline unsigned short GetStride();
    
    // Sets the stride of the data
    inline void SetStride(unsigned short uiStride);
    
    // Gets the number elements in the stream
    inline NiUInt32 GetBlockCount();
    
    // Sets the number elements in the stream
    inline void SetBlockCount(NiUInt32 uiBlockCount);
    
    // Gets the number of segments (chunks) of data
    inline NiUInt32 GetSegmentBlockCount();
    
    // Gets the amount of padding required to align the data
    inline NiUInt32 GetPadding(NiUInt32 uiBlockIndex);

    // Adds an input task to the stream
    inline void AddInput(NiSPTask* pkTask);
    
    // Gets the input tasks from a stream
    inline NiSPTask* GetInputAt(NiUInt32 uiIndex) const;
    
    // Gets the number of inputs in the stream
    inline NiUInt32 GetInputSize() const;

    // Adds an output task to the stream
    inline void AddOutput(NiSPTask* pkTask);

    // Gets the output tasks from the stream
    inline NiSPTask* GetOutputAt(NiUInt32 uiIndex) const;

    // Gets the number of outputs in the stream
    inline NiUInt32 GetOutputSize() const;

    // Gets the pointer to the block at the index specified 
    void* GetBlockStartPointer(NiUInt32 uiBlockIndex);

    // Gets aligned pointer to the block at the index specified 
    void* GetBlockStartPointerAligned(NiUInt32 uiBlockIndex);
    
    // Prepares the stream for partitioning
    void Prepare();
    
protected:
    
    // Computes the least common multiple of the 2 inputs
    NiUInt32 ComputeLCM(NiUInt32 uiInput1, NiUInt32 uiInput2);  
    
    // Computes the greatest common denominator of the 2 inputs
    NiUInt32 ComputeGCD(NiUInt32 uiInput1, NiUInt32 uiInput2);

    // Resets the cached state of tasks dependent on this stream
    void NotifyDependentTasks();

    // Members
    NiSPTaskPtrSet m_kInputs;              // Collection of input tasks
    NiSPTaskPtrSet m_kOutputs;             // Collection of output tasks
    
    void*           m_pvData;               // Pointer to raw data buffer
    void*           m_pvAlignedData;        // Aligned pointer to data
    NiUInt32        m_uiDataSize;           // Size of raw data buffer   
    NiUInt32        m_uiBlockCount;         // Number of blocks in buffer
    unsigned short  m_uiStride;             // Stride of buffer
    NiUInt32        m_uiSegmentBlockCount;  // Number blocks in an aligned unit
    bool            m_bPartitioningEnabled; // Indicates if stream one element
 };

#include "NiSPStream.inl"

// Helper class for constructing streams from known data types
template <class T>
class NiTSPStream : public NiSPStream
{
public:
    NiTSPStream() : NiSPStream(NULL, sizeof(T), 0){}
    NiTSPStream(T* pData, NiUInt32 uiBlockCount) : 
      NiSPStream(pData, sizeof(T), uiBlockCount){}
    virtual ~NiTSPStream(){}
};

// Helper class for constructing fixed inputs
template <class T>
class NiTSPFixedInput: public NiSPStream
{
public:
    NiTSPFixedInput() : NiSPStream(NULL, sizeof(T), 0, false){}
    NiTSPFixedInput(T* pData, NiUInt32 uiBlockCount) : 
      NiSPStream(pData, sizeof(T), uiBlockCount, false){}
    virtual ~NiTSPFixedInput(){}
};


#endif
