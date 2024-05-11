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

#ifndef NISTATICDATAMANAGER_H
#define NISTATICDATAMANAGER_H

#include "NiSystemLibType.h"
#include "NiSDMMacros.h"
#include "NiInitOptions.h"

#define NI_NUM_LIBRARIES 16

//---------------------------------------------------------------------------
class NISYSTEM_ENTRY NiStaticDataManager
{
public:
    static void Init(const NiInitOptions* pkOptions = NULL);
    static void Shutdown();

    typedef void (*InitFunction)(void);
    typedef void (*ShutdownFunction)(void);

    static void SetRootLibrary(InitFunction pfnInit,
        ShutdownFunction pfnShutdown);
    static void AddLibrary(InitFunction pfnInit, 
        ShutdownFunction pfnShutdown);
    static void RemoveLibrary(InitFunction pfnInit,
        ShutdownFunction pfnShutdown);

    static const NiInitOptions* GetInitOptions();

    static void ProcessAccumulatedLibraries();

private:
    static InitFunction ms_pfnRootInitFunction;
    static ShutdownFunction ms_pfnRootShutdownFunction;
    static InitFunction ms_apfnInitFunctions[NI_NUM_LIBRARIES];
    static ShutdownFunction ms_apfnShutdownFunctions[NI_NUM_LIBRARIES];
    static unsigned int ms_uiNumLibraries;
    static unsigned int ms_uiNumInitializedLibraries;
    static bool ms_bAutoCreatedInitOptions;
    static bool ms_bInitialized;
    static const NiInitOptions* ms_pkInitOptions;
};


#define NiInit NiStaticDataManager::Init
#define NiShutdown NiStaticDataManager::Shutdown

#include "NiStaticDataManager.inl"

#endif // #ifndef NISTATICDATAMANAGER_H
