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

#ifndef NIPHYSXMEMSTREAM_H
#define NIPHYSXMEMSTREAM_H

#include "NiPhysXLibType.h"

#include <NiMemStream.h>

#include <NxPhysics.h>
#include <NxStream.h>

#if defined(WIN32)
#pragma warning(push)
#pragma warning(disable: 4275) // DLL export warning due to PhysX
#endif

class NIPHYSX_ENTRY NiPhysXMemStream : public NxStream, public NiMemObject
{
public:
    NiPhysXMemStream();
    NiPhysXMemStream(const void* pBuffer, unsigned int uiSize);
    virtual ~NiPhysXMemStream();
    
    virtual unsigned int GetSize() const;
    virtual void* GetBuffer();
    virtual void Reset();

    // Loading API
    virtual NxU8 readByte() const;
    virtual NxU16 readWord() const;
    virtual NxU32 readDword() const;
    virtual NxF32 readFloat() const;
    virtual NxF64 readDouble() const;
    virtual void readBuffer(void* buffer, NxU32 size) const;

    // Saving API
    virtual NxStream& storeByte(NxU8 b);
    virtual NxStream& storeWord(NxU16 w);
    virtual NxStream& storeDword(NxU32 d);
    virtual NxStream& storeFloat(NxF32 f);
    virtual NxStream& storeDouble(NxF64 f);
    virtual NxStream& storeBuffer(const void* buffer, NxU32 size);

protected:
    NiMemStream* m_pkStream;
};

#if defined (WIN32)
#pragma warning(pop)
#endif

#endif // #ifndef NIPHYSXMEMSTREAM_H
