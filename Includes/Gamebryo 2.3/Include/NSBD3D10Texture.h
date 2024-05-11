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

#ifndef NSBD3D10TEXTURE_H
#define NSBD3D10TEXTURE_H

#include "NiD3D10BinaryShaderLibLibType.h"
#include "NSBD3D10StageAndSamplerStates.h"
#include "NSBD3D10StateGroup.h"

class NSBD3D10TextureStage;

class NID3D10BINARYSHADERLIB_ENTRY NSBD3D10Texture : public NiMemObject
{
public:
    NSBD3D10Texture();
    ~NSBD3D10Texture();

    // Name
    const char* GetName();
    void SetName(const char* pcName);

    unsigned int GetStage();
    void SetStage(unsigned int uiStage);

    // 
    NSBD3D10StateGroup* GetTextureStageGroup();
    NSBD3D10StateGroup* GetSamplerStageGroup();

    unsigned int GetTextureFlags();

    void SetSource(unsigned int uiNDLMap);
    void SetSourceDecal(unsigned int uiDecal);
    void SetSourceShader(unsigned int uiShaderMap);
    void SetSourceObject(NiShaderAttributeDesc::ObjectType eObjectType,
        unsigned int uiObjectIndex);

    bool SetupD3D10TextureStage(NiD3D10Pass& kPass);

    // *** begin Emergent internal use only
    bool SaveBinary(NiBinaryStream& kStream);
    bool LoadBinary(NiBinaryStream& kStream);

    bool CreateFromTextureStage(NSBD3D10TextureStage* pkTextureStage);

#if defined(_DEBUG)
    void Dump(FILE* pf);
#endif  //#if defined(_DEBUG)
    // *** end Emergent internal use only

protected:
    char* m_pcName;

    unsigned int m_uiStage;
    unsigned int m_uiTextureFlags;
    unsigned short m_usObjTextureFlags;
};

#include "NSBD3D10Texture.inl"

#endif  //NSBD3D10TEXTURE_H
