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

#ifndef NID3D10DATASTREAM_H
#define NID3D10DATASTREAM_H

#include "NiD3D10RendererLibType.h"

#include "NiD3D10Headers.h"

#include <NiRefObject.h>
#include <NiSmartPointer.h>

NiSmartPointer(NiD3D10DataStream);

class NID3D10RENDERER_ENTRY NiD3D10DataStream : public NiRefObject
{
public:
    // Create a D3D10-specific data stream object
    static bool Create(size_t uiBufferSize, unsigned int uiAccessFlags,
        unsigned int uiUsageFlags, NiD3D10DataStreamPtr& spDataStream);

    static bool Create(size_t uiBufferSize, D3D10_USAGE eUsage, 
        unsigned int uiBindFlags, unsigned int uiCPUAccessFlags, 
        unsigned int uiMiscFlags, D3D10_SUBRESOURCE_DATA* pkInitialData,
        NiD3D10DataStreamPtr& spDataStream);

    // Create from an ID3D10Buffer object
    static bool Create(ID3D10Buffer* pkBuffer, 
        NiD3D10DataStreamPtr& spDataStream);

    enum Access
    {
        ACCESS_CPU_READ     = 1 << 0,
        ACCESS_CPU_WRITE    = 1 << 1,
        ACCESS_GPU_READ     = 1 << 2,
        ACCESS_GPU_WRITE    = 1 << 3,
        ACCESS_DYNAMIC      = 1 << 4
    };

    enum Lock
    {
        LOCK_READ     = 1 << 0,
        LOCK_WRITE    = 1 << 1
    };

    enum Usage
    {
        USAGE_INDEX             = 1 << 0,
        USAGE_VERTEX            = 1 << 1,
        USAGE_SHADER_CONSTANT   = 1 << 2
    };

    size_t GetSize() const;
    unsigned int GetAccessFlags() const;
    unsigned int GetUsageFlags() const;
    unsigned int GetMiscFlags() const;
    D3D10_USAGE GetD3D10Usage() const;

    virtual void* Lock(unsigned char ucLock);
    virtual void Unlock();
    bool GetLocked() const;

    ID3D10Buffer* GetBuffer() const;

    bool Reallocate(size_t uiBufferSize, 
        unsigned int uiAccessFlags, unsigned int uiUsageFlags);
    bool Reallocate(size_t uiBufferSize, D3D10_USAGE eUsage, 
        unsigned int uiBindFlags, unsigned int uiCPUAccessFlags, 
        unsigned int uiMiscFlags, D3D10_SUBRESOURCE_DATA* pkInitialData);
    bool Reallocate(size_t uiBufferSize);

    // D3D10-specific interpretation of flags
    static unsigned int InterpretAccessFlags(unsigned int uiAccessFlags, 
        D3D10_USAGE& eUsage);
    static unsigned int InterpretUsageFlags(unsigned int uiUsageFlags);

protected:
    NiD3D10DataStream(size_t uiBufferSize, D3D10_USAGE eUsage, 
        unsigned int uiBindFlags, unsigned int uiCPUAccessFlags, 
        unsigned int uiMiscFlags, D3D10_SUBRESOURCE_DATA* pkInitialData);
    NiD3D10DataStream(ID3D10Buffer* pkBuffer);
    virtual ~NiD3D10DataStream();

    void AllocateBuffer(size_t uiBufferSize, D3D10_USAGE eUsage, 
        unsigned int uiBindFlags, unsigned int uiCPUAccessFlags, 
        unsigned int uiMiscFlags, D3D10_SUBRESOURCE_DATA* pkInitialData);
    void ReleaseBuffer();

    void InterpretD3D10Parameters(D3D10_USAGE eD3D10Usage, 
        unsigned int uiD3D10BindFlags, unsigned int uiD3D10CPUAccessFlags, 
        unsigned int& uiUsageFlags, unsigned int& uiAccessFlags);

    ID3D10Buffer* m_pkBuffer;
    size_t m_uiBufferSize;
    unsigned int m_uiAccessFlags;

    unsigned int m_uiUsageFlags;
    unsigned int m_uiMiscFlags;
    D3D10_USAGE m_eUsage;
    bool m_bLocked;

    union
    {
        // Flag to simplify creation of IMMUTABLE resources.
        unsigned int m_uiImmutableUsageFlags; 
        // Pointer to temp memory for updating DEFAULT resources
        NiD3D10DataStream* m_pkTempStream;
        // Current write offset for DYNAMIC resources
        size_t m_uiCurrentOffset;
    } m_kLockHelpers;
};

#endif // NID3D10DATASTREAM_H
