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

#ifndef NID3DSHADERPROGRAMCREATOROBJ_H
#define NID3DSHADERPROGRAMCREATOROBJ_H

#include "NiD3DShaderProgramCreatorAsm.h"

class NID3D_ENTRY NiD3DShaderProgramCreatorObj : 
    public NiD3DShaderProgramCreatorAsm
{
public:
    virtual ~NiD3DShaderProgramCreatorObj();

    // *** begin Emergent internal use only ***

    static void _SDMInit();
    static void _SDMShutdown();

    static NiD3DShaderProgramCreatorObj* GetInstance();
    static void Shutdown();

    // *** end Emergent internal use only ***

protected:
    NiD3DShaderProgramCreatorObj();

    virtual bool LoadShaderCodeFromFile(const char* pcFileName, 
        void*& pvCode, unsigned int& uiCodeSize, void*& pvConstantData, 
        unsigned int& uiConstantSize, bool bRecoverable);
    virtual bool LoadShaderCodeFromBuffer(const void* pvBuffer, 
        unsigned int uiBufferSize, void*& pvCode, unsigned int& uiCodeSize, 
        void*& pvConstantData, unsigned int& uiConstantSize, 
        bool bRecoverable);

    static NiD3DShaderProgramCreatorObj* ms_pkCreator;
};

#include "NiD3DShaderProgramCreatorObj.inl"

#endif  //NID3DSHADERPROGRAMCREATOROBJ_H
