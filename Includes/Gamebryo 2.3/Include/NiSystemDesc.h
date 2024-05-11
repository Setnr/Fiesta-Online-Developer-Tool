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

#ifndef NISYSTEMDESC_H
#define NISYSTEMDESC_H

#include "NiMemObject.h"
#include "NiSystemLibType.h"
#include "NiRTLib.h"

class NISYSTEM_ENTRY NiSystemDesc : public NiMemObject
{
public:
    static const NiSystemDesc& GetSystemDesc();

    typedef enum
    {
        NI_WIN32 = 0,
        NI_XENON,
        NI_PS3,
        NI_NUM_PLATFORM_IDS
    } PlatformID;

    bool IsLittleEndian() const;
    unsigned int GetLogicalProcessorCount() const;
    PlatformID GetPlatformID() const;
    float GetPerformanceCounterHz() const;

    static void InitSystemDesc();
    static void ShutdownSystemDesc();

protected:
    NiSystemDesc();
    NiSystemDesc(const NiSystemDesc&);

    static NiSystemDesc* ms_pkSystemDesc;
    unsigned int m_uiNumLogicalProcessors;
    float m_fPCCyclesPerSecond;
};

#include "NiSystemDesc.inl"

#endif // NISYSTEMDESC_H

