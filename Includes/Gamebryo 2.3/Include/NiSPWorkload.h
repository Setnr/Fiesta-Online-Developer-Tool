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

#ifndef NISPWORKLOAD_H
#define NISPWORKLOAD_H

#include <NiSystem.h>

class NIFLOODGATE_ENTRY NiSPWorkload : public NiMemObject
{
public:
    friend class NiSPStreamPartitioner;

    static const NiUInt32 MAX_INPUTS = 16;
    static const NiUInt32 MAX_OUTPUTS = 8;

    struct Header                       
    {
        void*       m_pvEffectiveAddress;  
        NiUInt32    m_uiBlockCount;        
        NiUInt32    m_uiStride;            
        NiUInt32    m_uiDataOffset;        
        NiUInt32    m_uiReserved;          

    };                                      

    // Gets an input stream by index
    template <class T> T* GetInput(NiUInt32 uiInputStreamIndex);

    // Gets an output stream by index
    template <class T> T* GetOutput(NiUInt32 uiOutputStreamIndex);

    // Gets the number of chunks to process
    inline NiUInt32 GetBlockCount();

    // Gets the number of inputs
    inline NiUInt32 GetInputCount();

    // Gets the number of output streams
    inline NiUInt32 GetOutputCount();

    // Gets/Sets the kernel
    inline NiSPKernel* GetKernel();
    inline void SetKernel(NiSPKernel* pKernel);

    // Clears the workload
    inline void Clear();

protected:
    Header          m_kInputs[MAX_INPUTS];
    Header          m_kOutputs[MAX_OUTPUTS];
    NiUInt32        m_uiInputCount;
    NiUInt32        m_uiOutputCount;
    NiUInt32        m_uiBlockCount;
    NiUInt32        m_uiRangeStart;
    NiSPKernel*     m_pkKernel;
};

#include "NiSPWorkload.inl"
#endif
