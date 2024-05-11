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

#ifndef NSBUTILITY_H
#define NSBUTILITY_H

#include "NiBinaryShaderLibLibType.h"
#include "NSBRenderStates.h"
#include <NiBinaryStream.h>

class NSBStateGroup;
class NSBConstantMap;

#ifndef D3DGS_VERSION
#define D3DGS_VERSION(_Major,_Minor) (0xFFFD0000|((_Major)<<8)|(_Minor))
#endif  //#ifndef D3DGS_VERSION

#if defined(_DEBUG)
#define STATE_CASE_STRING(x)      case x: return #x;
#endif  //#if defined(_DEBUG)

class NIBINARYSHADERLIB_ENTRY NSBUtility : public NiMemObject
{
public:
    static bool SaveBinaryStateGroup(NiBinaryStream& kStream,
        NSBStateGroup* pkStateGroup);
    static bool LoadBinaryStateGroup(NiBinaryStream& kStream,
        NSBStateGroup*& pkStateGroup);

    static bool SaveBinaryConstantMap(NiBinaryStream& kStream,
        NSBConstantMap* pkMap);
    static bool LoadBinaryConstantMap(NiBinaryStream& kStream,
        NSBConstantMap*& pkMap);

#if defined(_DEBUG)
    static unsigned int ms_uiIndent;

    static void IndentInsert();
    static void IndentRemove();
    static void Dump(FILE* pf, bool bIndent, char* pcFmt, ...);
#endif  //#if defined(_DEBUG)

};

#endif  //NSBUTILITY_H
