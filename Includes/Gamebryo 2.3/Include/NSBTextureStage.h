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

#ifndef NSBTEXTURESTAGE_H
#define NSBTEXTURESTAGE_H

#include "NiBinaryShaderLibLibType.h"
#include "NSBStageAndSamplerStates.h"
#include "NSBStateGroup.h"

#include <NiD3DDefines.h>
#include <NiD3DUtility.h>
#include <NiD3DTextureStage.h>

class NiD3DTextureStage;

class NIBINARYSHADERLIB_ENTRY NSBTextureStage : public NiMemObject
{
public:
    NSBTextureStage();
    ~NSBTextureStage();

    // Name
    const char* GetName();
    void SetName(const char* pcName);

    unsigned int GetStage();
    void SetStage(unsigned int uiStage);

    // 
    NSBStateGroup* GetTextureStageGroup();
    NSBStateGroup* GetSamplerStageGroup();

    unsigned int GetTextureFlags();

    void SetNDLMap(unsigned int uiNDLMap);
    void SetDecalMap(unsigned int uiDecal);
    void SetShaderMap(unsigned int uiShaderMap);
    void SetUseIndexFromMap(bool bUse);
    void SetObjTextureSettings(NiShaderAttributeDesc::ObjectType eObjectType,
        unsigned int uiObjectIndex);

    bool GetUseTextureTransformation();
    D3DMATRIX& GetTextureTransformation();

    void SetUseTextureTransformation(bool bUse);
    void SetTextureTransformation(D3DMATRIX& kTrans);

    unsigned int GetTextureTransformFlags();
    void SetTextureTransformFlags(unsigned int uiFlags);

    const char* GetGlobalName();
    void SetGlobalName(const char* pcGlobalName);

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
    bool SetupD3DTextureStage_PS3(NiD3DTextureStage& kD3DStage);

    bool SaveBinaryTextureTransform(NiBinaryStream& kStream);
    bool LoadBinaryTextureTransform(NiBinaryStream& kStream);

    char* m_pcName;

    unsigned int m_uiStage;
    unsigned int m_uiTextureFlags;
    NSBStateGroup* m_pkTextureStageGroup;
    NSBStateGroup* m_pkSamplerStageGroup;
    unsigned short m_usObjTextureFlags;
    bool m_bTextureTransform;
    D3DMATRIX m_kTextureTransformation;
    unsigned int m_uiTextureTransformFlags;
    char* m_pcGlobalEntry;
};

#include "NSBTextureStage.inl"

#endif  //NSBTEXTURESTAGE_H
