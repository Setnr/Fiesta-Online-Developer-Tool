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

#ifndef NIBINARYSHADER_H
#define NIBINARYSHADER_H

#include "NiBinaryShaderLibLibType.h"
#include "NSBUserDefinedDataSet.h"

#include <NiD3DShader.h>
// For reporting errors...
#include <NiD3DShaderFactory.h>

class NIBINARYSHADERLIB_ENTRY NiBinaryShader : public NiD3DShader
{
    NiDeclareRTTI;
public:
    NiBinaryShader();
    virtual ~NiBinaryShader();

    // The D3DShaderInterface functions
    virtual bool Initialize();

    virtual unsigned int PreProcessPipeline(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects,
        const NiTransform& kWorld, const NiBound& kWorldBound);
    virtual unsigned int UpdatePipeline(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects,
        const NiTransform& kWorld, const NiBound& kWorldBound);
    virtual unsigned int SetupRenderingPass(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects,
        const NiTransform& kWorld, const NiBound& kWorldBound);
    virtual unsigned int SetupTransformations(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        const NiSkinPartition::Partition* pkPartition, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld, const NiBound& kWorldBound);
    virtual unsigned int SetupShaderPrograms(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        const NiSkinPartition::Partition* pkPartition, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld, const NiBound& kWorldBound);
    virtual NiGeometryData::RendererData* PrepareGeometryForRendering(
        NiGeometry* pkGeometry, const NiSkinPartition::Partition* pkPartition, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState);
    virtual unsigned int PostProcessPipeline(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects,
        const NiTransform& kWorld, const NiBound& kWorldBound);
    virtual unsigned int NextPass();

    NSBUserDefinedDataSet* GetShaderUserDefinedDataSet();
    NSBUserDefinedDataSet* GetImplementationUserDefinedDataSet();
    NSBUserDefinedDataSet* GetPassUserDefinedDataSet(
        unsigned int uiPass);

    // *** begin Emergent internal use only
    // These functions are used for the creation of the shader
    void SetUserDefinedDataSet(NSBUserDefinedDataSet* pkUDDSet);
    void SetImplementationUserDefinedDataSet(
        NSBUserDefinedDataSet* pkUDDSet);
    void SetPassUserDefinedDataSet(unsigned int uiPass, 
        NSBUserDefinedDataSet* pkUDDSet);

    // Shader Declaration (Packing definition)
    NiD3DShaderDeclaration* GetShaderDecl();
    void SetShaderDecl(NiD3DShaderDeclaration* pkShaderDecl);

    // Render State Group
    NiD3DRenderStateGroup* GetRenderStateGroup(void);
    void SetRenderStateGroup(NiD3DRenderStateGroup* pkRenderStateGroup);

    // Vertex shader mapping
    NiD3DShaderConstantMap* GetVertexConstantMap() const;
    void SetVertexConstantMap(NiD3DShaderConstantMap* pkSCMVertex);

    // Pixel shader mapping
    NiD3DShaderConstantMap* GetPixelConstantMap() const;
    void SetPixelConstantMap(NiD3DShaderConstantMap* pkSCMPixel);

    D3DDevicePtr GetD3DDevice();
    NiD3DRenderer* GetD3DRenderer();
    NiD3DRenderState* GetD3DRenderState();

    // Inserts the pass at the given position.
    bool InsertPass(unsigned int uiPass, NiD3DPass* pkPass);

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

    // *** end Emergent internal use only

protected:
    bool LoadVertexShaderProgram(NiD3DPass& kPass);
    bool LoadGeometryShaderProgram(NiD3DPass& kPass);
    bool LoadPixelShaderProgram(NiD3DPass& kPass);
    bool ResolveVertexShaderFileName(const char* pcOriginalName,
        char* pcTrueName, unsigned int uiNameLength);
    bool ResolveGeometryShaderFileName(const char* pcOriginalName,
        char* pcTrueName, unsigned int uiNameLength);
    bool ResolvePixelShaderFileName(const char* pcOriginalName,
        char* pcTrueName, unsigned int uiNameLength);

    void PrepareTextureStage(NiGeometry* pkGeometry, 
        NiD3DTextureStage* pkStage,
        const NiTexturingProperty* pkTexProp,
        const NiDynamicEffectState* pkEffects,
        unsigned int uiPass);
    
    unsigned int m_uiBonesPerPartition;
    unsigned int m_uiBoneMatrixRegisters;
    BoneMatrixCalcMethod m_eBoneCalcMethod;
    NiGeometryData::DataFlags m_eBinormalTangentMethod;
    unsigned int m_uiBinormalTangentUVSource;

    // User defined data set
    NSBUserDefinedDataSetPtr m_spUDDSet_Shader;
    NSBUserDefinedDataSetPtr m_spUDDSet_Implementation;
    NiTObjectArray<NSBUserDefinedDataSetPtr> m_kUDDSet_PassArray;
};

#include "NiBinaryShader.inl"

#endif  //NIBINARYSHADER_H
