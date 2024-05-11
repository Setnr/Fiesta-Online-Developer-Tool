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

#ifndef NSBTEXTURE_H
#define NSBTEXTURE_H

#include "NiBinaryShaderLibLibType.h"
#include "NSBStageAndSamplerStates.h"
#include "NSBStateGroup.h"

#include <NiD3DDefines.h>
#include <NiD3DUtility.h>
#include <NiD3DTextureStage.h>

class NIBINARYSHADERLIB_ENTRY NSBTexture : public NiMemObject
{
public:
    NSBTexture();
    ~NSBTexture();

    // Name
    const char* GetName();
    void SetName(const char* pcName);

    unsigned int GetStage();
    void SetStage(unsigned int uiStage);

    // 
    NSBStateGroup* GetTextureStageGroup();
    NSBStateGroup* GetSamplerStageGroup();

    unsigned int GetTextureFlags();

    void SetSource(unsigned int uiNDLMap);
    void SetSourceDecal(unsigned int uiDecal);
    void SetSourceShader(unsigned int uiShaderMap);
    void SetSourceObject(NiShaderAttributeDesc::ObjectType eObjectType,
        unsigned int uiObjectIndex);

    bool SetupD3DTextureStage(NiD3DTextureStage& kD3DStage);

    // *** begin Emergent internal use only
    bool SaveBinary(NiBinaryStream& kStream);
    bool LoadBinary(NiBinaryStream& kStream);

#if defined(_DEBUG)
    void Dump(FILE* pf);
#endif  //#if defined(_DEBUG)
    // *** end Emergent internal use only

protected:
    bool SetupD3DTextureStage_DX9(NiD3DTextureStage& kD3DStage);
    bool SetupD3DTextureStage_Xenon(NiD3DTextureStage& kD3DStage);

    char* m_pcName;

    unsigned int m_uiStage;
    unsigned int m_uiTextureFlags;
    unsigned short m_usObjTextureFlags;
};

#include "NSBTexture.inl"

#endif  //NSBTEXTURE_H
