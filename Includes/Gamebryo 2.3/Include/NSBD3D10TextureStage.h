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

#ifndef NSBD3D10TEXTURESTAGE_H
#define NSBD3D10TEXTURESTAGE_H

#include "NiD3D10BinaryShaderLibLibType.h"
#include "NSBD3D10StageAndSamplerStates.h"
#include "NSBD3D10StateGroup.h"

class NID3D10BINARYSHADERLIB_ENTRY NSBD3D10TextureStage : public NiMemObject
{
public:
    NSBD3D10TextureStage();
    ~NSBD3D10TextureStage();

    // Name
    const char* GetName();
    void SetName(const char* pcName);

    unsigned int GetStage();
    void SetStage(unsigned int uiStage);

    // 
    NSBD3D10StateGroup* GetTextureStageGroup();
    NSBD3D10StateGroup* GetSamplerStageGroup();

    unsigned int GetTextureFlags();

    void SetNDLMap(unsigned int uiNDLMap);
    void SetDecalMap(unsigned int uiDecal);
    void SetShaderMap(unsigned int uiShaderMap);
    void SetUseIndexFromMap(bool bUse);
    void SetObjTextureSettings(NiShaderAttributeDesc::ObjectType eObjectType,
        unsigned int uiObjectIndex);
    unsigned short GetObjTextureSettings();

    bool GetUseTextureTransformation();
    D3DMATRIX& GetTextureTransformation();

    void SetUseTextureTransformation(bool bUse);
    void SetTextureTransformation(D3DMATRIX& kTrans);

    unsigned int GetTextureTransformFlags();
    void SetTextureTransformFlags(unsigned int uiFlags);

    const char* GetGlobalName();
    void SetGlobalName(const char* pcGlobalName);

    bool SetupD3D10TextureStage(NiD3D10Pass& kPass);

    // *** begin Emergent internal use only
    bool SaveBinary(NiBinaryStream& kStream);
    bool LoadBinary(NiBinaryStream& kStream);

#if defined(_DEBUG)
    void Dump(FILE* pf);
#endif  //#if defined(_DEBUG)

    static bool InterpretFlags(unsigned int uiNSBTextureFlags, 
        unsigned int& uiGBMap, unsigned int& uiGBMapInstance);
    // *** end Emergent internal use only

protected:
    bool SaveBinaryTextureTransform(NiBinaryStream& kStream);
    bool LoadBinaryTextureTransform(NiBinaryStream& kStream);

    // NiD3D10Pass doesn't define (or use) any enumeration comparable to
    // NiD3DPass::TSTF_MAP_USE_INDEX, so it must be defined here for 
    // streaming purposes.
    enum
    {
        MAP_USE_INDEX = 0x10000000
    };

    char* m_pcName;

    unsigned int m_uiStage;
    unsigned int m_uiTextureFlags;
    NSBD3D10StateGroup* m_pkTextureStageGroup;
    NSBD3D10StateGroup* m_pkSamplerStageGroup;
    unsigned short m_usObjTextureFlags;
    bool m_bTextureTransform;
    D3DMATRIX m_kTextureTransformation;
    unsigned int m_uiTextureTransformFlags;
    char* m_pcGlobalEntry;
};

#include "NSBD3D10TextureStage.inl"

#endif  //NSBD3D10TEXTURESTAGE_H
