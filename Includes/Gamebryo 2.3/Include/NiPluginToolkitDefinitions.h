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

#ifndef NIPLUGINTOOLKITDEFINITIONS_H
#define NIPLUGINTOOLKITDEFINITIONS_H

#include "NiPluginToolkitLibType.h"
#include <NiSystem.h>
#include <NiRTTI.h>

#define MAX_STRING_SIZE 5000
#define NIPT_INVALID_INDEX ((unsigned int) -1)

typedef HWND NiWindowRef;
typedef HINSTANCE  NiInstanceRef;
typedef HMODULE NiModuleRef;

enum ReturnCode
{
    UNKNOWN_ERROR = -1,
    SUCCESS = 1,
    FILE_NOT_FOUND,
    DIRECTORY_NOT_FOUND,
    FILE_PARSE_ERROR,
    NO_READER_FOR_FILE,
    NO_WRITER_FOR_FILE,
    IO_FAILURE,
    SCRIPT_OF_SAME_NAME_ALREADY_EXISTS,
    NULL_INPUT_SCRIPT,
    SCRIPT_IGNORED,
    CANCELLED,
    NO_SCRIPTS_FOUND
};

enum NiPluginToolkitRenderer
{
    RENDER_UNKNOWN = -1,
    RENDER_DX8 = 0,
    RENDER_PS2,
    RENDER_GAMECUBE,
    RENDER_XBOX,
    RENDER_HURRICANE,
    RENDER_DX9,
    RENDER_XENON,
    RENDER_PS3,
    RENDER_GENERIC,
    RENDER_D3D10,
    RENDER_MAX
};

#if defined(NIDEBUG)
    #if (_MSC_VER == 1310) //VC7.1
        #define NIPLUGINTOOLKIT_MODULE_NAME "NiPluginToolkit23VC71D"
    #elif (_MSC_VER == 1400) //VC8.0
        #define NIPLUGINTOOLKIT_MODULE_NAME "NiPluginToolkit23VC80D"
    #else
        #error Unsupported version of Visual Studio
    #endif
#elif defined(NIRELEASE)
    #if (_MSC_VER == 1310) //VC7.1
        #define NIPLUGINTOOLKIT_MODULE_NAME "NiPluginToolkit23VC71R"
    #elif (_MSC_VER == 1400) //VC8.0
        #define NIPLUGINTOOLKIT_MODULE_NAME "NiPluginToolkit23VC80R"
    #else
        #error Unsupported version of Visual Studio
    #endif
#else
    #error Invalid build configuration
#endif

extern HMODULE gs_hNiPluginToolkitHandle;

inline bool NiIsKindOfStringMatch(const char* pcClassName, 
    const NiRTTI* pkRTTI)
{
    if (pcClassName == NULL)
        return false;

    if (pkRTTI != NULL)
    {
        if (NiStricmp(pkRTTI->GetName(), pcClassName) == 0)
            return true;
        return NiIsKindOfStringMatch(pcClassName, pkRTTI->GetBaseRTTI());

    }
    return false;
};

#endif
