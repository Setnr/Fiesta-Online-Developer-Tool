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
#ifndef NISPKERNEL_H
#define NISPKERNEL_H
//---------------------------------------------------------------------------
#include <NiSystem.h>
#include <NiString.h>

#include "NiFloodgateLibType.h"
//---------------------------------------------------------------------------
class NiSPTask;
class NiSPWorkload;
class NIFLOODGATE_ENTRY NiSPKernel : public NiMemObject
{
public:
    NiSPKernel(){}
    NiSPKernel(const char* pucName) : m_strName(pucName) {}
    virtual ~NiSPKernel(){}
    const NiString& GetName() const { return m_strName; }
    NiSPKernel& operator=(const NiSPKernel& kKernel)
    {
        m_strName = kKernel.m_strName;
        return *this;
    }
    virtual void Execute(NiSPWorkload& pkWorkload){};
    virtual char* GetBinStart()
    {
        NIASSERT(0 && "Invalid call for this platform.");
        return NULL;
    }
    virtual char* GetBinSize()
    {
        NIASSERT(0 && "Invalid call for this platform.");
        return NULL;
    }
    protected:    
    NiString m_strName;
};
//---------------------------------------------------------------------------
#endif

