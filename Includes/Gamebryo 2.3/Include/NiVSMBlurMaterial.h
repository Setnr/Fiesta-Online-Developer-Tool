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

#ifndef NIVSMBLURMATERIAL_H
#define NIVSMBLURMATERIAL_H

#include "NiFragmentMaterial.h"

class NiVSMBlurVertexProgramDescriptor;

class NIMAIN_ENTRY NiVSMBlurMaterial : public NiFragmentMaterial
{
    NiDeclareRTTI;
public:

    enum TransformType
    {
        TRANSFORM_DEFAULT = 0,
        TRANSFORM_SKINNED,
        TRANSFORM_SKINNED_NOPALETTE,
        TRANSFORM_MAX
    };

    enum TexGenOutput
    {
        TEX_OUT_PASSTHRU = 0,
        TEX_OUT_TRANSFORM,
        TEX_OUT_PARALLEL_PROJECT,
        TEX_OUT_PERSPECTIVE_PROJECT,
        TEX_OUT_MAX
    };

    NiVSMBlurMaterial(NiMaterialNodeLibrary* pkLibrary,
        bool bAutoCreateCaches);

    bool GetVertexInputSemantics(NiGeometry* pkGeometry, 
        unsigned int uiMaterialExtraData,
        NiShaderDeclaration::ShaderRegisterEntry* pakSemantics);

protected:
    virtual bool GenerateDescriptor(const NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects,
        NiMaterialDescriptor& kMaterialDesc);

    virtual NiGPUProgram* GenerateVertexShaderProgram(
        NiGPUProgramDescriptor* pkDesc, 
        NiTObjectPtrSet<NiMaterialResourcePtr>& kUniforms);

    virtual NiGPUProgram* GeneratePixelShaderProgram(
        NiGPUProgramDescriptor* pkDesc, 
        NiTObjectPtrSet<NiMaterialResourcePtr>& kUniforms);

    virtual unsigned int VerifyShaderPrograms(NiGPUProgram* pkVertexShader,
        NiGPUProgram* pkGeometryShader, NiGPUProgram* pkPixelShader);

    virtual bool InsertTexture(Context& kContext, NiMaterialResource* pkUV,
        NiMaterialResource*& pkOutputColor, NiMaterialNode*& pkNode,
        NiFixedString& kString, unsigned int uiBlurKernel);

    virtual bool HandleTexGen(Context& kContext, 
        NiMaterialResource* pkInputUVResource,
        NiMaterialResource*& pkOutputUVResource,
        const NiFixedString& kMapName, unsigned int uiExtra,
        NiVSMBlurMaterial::TexGenOutput eOutputType);

    virtual bool HandlePositionFragment(
        Context& kContext, 
        TransformType eTransType, 
        NiMaterialResource*& pkVertWorldPos,
        NiMaterialResource*& pkWorldMatrix);

    virtual bool HandleViewProjectionFragment(Context& kContext, 
        bool bForceViewPos, NiMaterialResource* pkVertWorldPos,
        NiMaterialResource*& pkVertOutProjectedPos,
        NiMaterialResource*& pkVertOutViewPos);

    virtual bool SetupTransformPipeline(Context& kContext,
        NiMaterialResource* pkVertOutProjPos,
        NiVSMBlurVertexProgramDescriptor* pkVertDesc, bool bForceView,
        bool bForceViewPos, NiMaterialResource*& pkWorldPos, 
        NiMaterialResource*& pkViewPos,  NiMaterialResource*& pkWorldNormal,
        NiMaterialResource*& pkWorldView);

    virtual ReturnCode GenerateShaderDescArray(
        NiMaterialDescriptor* pkMaterialDescriptor,
        RenderPassDescriptor* pkRenderPasses, unsigned int uiMaxCount,
        unsigned int& uiCountAdded);

    virtual NiShader* CreateShader(NiMaterialDescriptor* pkDesc);

    virtual bool SetupPackingRequirements(NiShader* pkShader,
        NiMaterialDescriptor* pkMaterialDescriptor,
        RenderPassDescriptor* pkRenderPasses, unsigned int uiCount);

    NiFixedString m_kDescriptorName;
};

NiSmartPointer(NiVSMBlurMaterial);


#endif // NIVSMBLURMATERIAL_H
