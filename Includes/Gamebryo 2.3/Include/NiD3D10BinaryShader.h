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

#ifndef NID3D10BINARYSHADER_H
#define NID3D10BINARYSHADER_H

#include "NiD3D10BinaryShaderLibLibType.h"
#include "NSBD3D10UserDefinedDataSet.h"

#include <NiD3D10Shader.h>
#include <NiD3D10VertexDescription.h>

// For reporting errors...
#include <NiD3D10ShaderFactory.h>

class NID3D10BINARYSHADERLIB_ENTRY NiD3D10BinaryShader : public NiD3D10Shader
{
    NiDeclareRTTI;
public:
    NiD3D10BinaryShader();
    virtual ~NiD3D10BinaryShader();

    // The D3D10ShaderInterface functions
    virtual bool Initialize();

    virtual unsigned int UpdatePipeline(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects,
        const NiTransform& kWorld, const NiBound& kWorldBound);

    NSBD3D10UserDefinedDataSet* GetShaderUserDefinedDataSet();
    NSBD3D10UserDefinedDataSet* GetImplementationUserDefinedDataSet();
    NSBD3D10UserDefinedDataSet* GetPassUserDefinedDataSet(
        unsigned int uiPass);

    // *** begin Emergent internal use only
    // These functions are used for the creation of the shader
    void SetUserDefinedDataSet(NSBD3D10UserDefinedDataSet* pkUDDSet);
    void SetImplementationUserDefinedDataSet(
        NSBD3D10UserDefinedDataSet* pkUDDSet);
    void SetPassUserDefinedDataSet(unsigned int uiPass, 
        NSBD3D10UserDefinedDataSet* pkUDDSet);

    // Render State Group
    void SetRenderStateGroup(NiD3D10RenderStateGroup* pkRenderStateGroup);

    // Vertex shader mapping
    unsigned int GetVertexConstantMapCount() const;
    NiD3D10ShaderConstantMap* GetVertexConstantMap(unsigned int uiIndex)
        const;
    void SetVertexConstantMap(unsigned int uiIndex,
        NiD3D10ShaderConstantMap* pkSCMVertex);

    // Geometry shader mapping
    unsigned int GetGeometryConstantMapCount() const;
    NiD3D10ShaderConstantMap* GetGeometryConstantMap(unsigned int uiIndex)
        const;
    void SetGeometryConstantMap(unsigned int uiIndex,
        NiD3D10ShaderConstantMap* pkSCMGeometry);

    // Pixel shader mapping
    unsigned int GetPixelConstantMapCount() const;
    NiD3D10ShaderConstantMap* GetPixelConstantMap(unsigned int uiIndex)
        const;
    void SetPixelConstantMap(unsigned int uiIndex,
        NiD3D10ShaderConstantMap* pkSCMPixel);

    // Inserts the pass at the given position.
    bool InsertPass(unsigned int uiPass, NiD3D10Pass* pkPass);

    // For skinning shaders
    enum BoneMatrixCalcMethod
    {
        BONECALC_SKIN       = 0x00000000,
        BONECALC_NO_SKIN    = 0x00000001
    };

    unsigned int GetBonesPerPartition() const;
    void SetBonesPerPartition(unsigned int uiBones);
    unsigned int GetBoneMatrixRegisters() const;
    void SetBoneMatrixRegisters(unsigned int uiRegisters);
    BoneMatrixCalcMethod GetBoneCalcMethod() const;
    void SetBoneCalcMethod(BoneMatrixCalcMethod eMethod);

    NiGeometryData::DataFlags GetBinormalTangentMethod() const;
    void SetBinormalTangentMethod(NiGeometryData::DataFlags eNBTMethod);
    unsigned int GetBinormalTangentUVSource() const;
    void SetBinormalTangentUVSource(unsigned int uiSource);

    virtual bool SetupGeometry(NiGeometry* pkGeometry);
    virtual bool IsTimeBased(NiD3D10ShaderConstantMap* pkMap);

    // *** end Emergent internal use only

protected:
    bool LoadVertexShaderProgram(NiD3D10Pass& kPass);
    bool LoadGeometryShaderProgram(NiD3D10Pass& kPass);
    bool LoadPixelShaderProgram(NiD3D10Pass& kPass);
    bool ResolveVertexShaderFileName(const char* pcOriginalName,
        char* pcTrueName, unsigned int uiNameLength);
    bool ResolveGeometryShaderFileName(const char* pcOriginalName,
        char* pcTrueName, unsigned int uiNameLength);
    bool ResolvePixelShaderFileName(const char* pcOriginalName,
        char* pcTrueName, unsigned int uiNameLength);

    void PrepareTexture(NiGeometry* pkGeometry, NiD3D10Pass* pkPass, 
        unsigned int uiTexture,
        const NiTexturingProperty* pkTexProp,
        const NiDynamicEffectState* pkEffects, unsigned int uiPass);

    unsigned int m_uiBonesPerPartition;
    unsigned int m_uiBoneMatrixRegisters;
    BoneMatrixCalcMethod m_eBoneCalcMethod;
    NiGeometryData::DataFlags m_eBinormalTangentMethod;
    unsigned int m_uiBinormalTangentUVSource;

    // User defined data set
    NSBD3D10UserDefinedDataSetPtr m_spUDDSet_Shader;
    NSBD3D10UserDefinedDataSetPtr m_spUDDSet_Implementation;
    NiTObjectArray<NSBD3D10UserDefinedDataSetPtr> m_kUDDSet_PassArray;

    unsigned int m_uiPassCount;
};

#include "NiD3D10BinaryShader.inl"

#endif  //NID3D10BINARYSHADER_H
