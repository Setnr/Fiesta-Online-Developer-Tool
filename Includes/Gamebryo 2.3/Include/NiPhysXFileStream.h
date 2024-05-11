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

#ifndef NIPHYSXFILESTREAM_H
#define NIPHYSXFILESTREAM_H

#include "NiPhysXLibType.h"

#include <NiSystem.h>
#include <NxStream.h>

#if defined(WIN32)
#pragma warning(push)
#pragma warning(disable: 4275) // DLL export warning due to PhysX
#endif

class NIPHYSX_ENTRY NiPhysXFileStream : public NxStream, public NiMemObject
{
public:
    NiPhysXFileStream(const char *pcName, NiFile::OpenMode eMode,
        unsigned int uiBufferSize = 32768);
    virtual ~NiPhysXFileStream();
    
    virtual bool Ready() const;

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
    NiFile* m_pkFile;
};

#if defined (WIN32)
#pragma warning(pop)
#endif


#endif // #ifndef NIPHYSXFILESTREAM_H
