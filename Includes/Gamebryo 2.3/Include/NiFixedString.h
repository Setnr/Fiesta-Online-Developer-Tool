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

#ifndef NIFIXEDSTRINGTABLE_H
#define NIFIXEDSTRINGTABLE_H

#include "NiRTLib.h"
#include "NiGlobalStringTable.h"
#include "NiPath.h"

class NIMAIN_ENTRY NiFixedString : public NiMemObject
{
public:
    NiFixedString();
    NiFixedString(const char* pcString);
    NiFixedString(const NiFixedString& kString);
    ~NiFixedString();

    operator const char*() const;

    bool Exists() const;

    NiFixedString& operator=(const NiFixedString& kString);
    NiFixedString& operator=(const char* pcString);

    size_t GetLength() const;
    unsigned int GetRefCount() const;

    bool Equals(const char* pcStr) const;
    bool EqualsNoCase(const char* pcStr) const;

    bool Contains(const char* pcStr) const;
    bool ContainsNoCase(const char* pcStr) const;

    friend bool operator==(const NiFixedString& s1, const NiFixedString& s2);
    friend bool operator!=(const NiFixedString& s1, const NiFixedString& s2);
   
    friend bool operator==(const NiFixedString& s1, const char* s2);
    friend bool operator!=(const NiFixedString& s1, const char* s2);
   
    friend bool operator==(const char* s1, const NiFixedString& s2);
    friend bool operator!=(const char* s1, const NiFixedString& s2);
   
    // Begin Emergent internal use only
    // End Emergent internal use only
protected:
    NiGlobalStringTable::GlobalStringHandle m_kHandle;
};

NIMAIN_ENTRY void NiStandardizeFilePath(NiFixedString& kString);

#include "NiFixedString.inl"

#endif
