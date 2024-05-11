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

#ifndef NIDIRECTIONALSHADOWWRITEMATERIAL_H
#define NIDIRECTIONALSHADOWWRITEMATERIAL_H

#include "NiStandardMaterial.h"

class NiStandardVertexProgramDescriptor;

class NIMAIN_ENTRY NiDirectionalShadowWriteMaterial : public NiStandardMaterial
{
    NiDeclareRTTI;
public:

    NiDirectionalShadowWriteMaterial(NiMaterialNodeLibrary* pkLibrary,
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

    virtual bool HandlePositionFragment(
        Context& kContext, 
        TransformType eTransType, 
        NiMaterialResource*& pkVertWorldPos,
        NiMaterialResource*& pkWorldMatrix);

    virtual bool HandleViewProjectionFragment(Context& kContext, 
        bool bForceViewPos, NiMaterialResource* pkVertWorldPos,
        NiMaterialResource*& pkVertOutProjectedPos,
        NiMaterialResource*& pkVertOutViewPos);

    virtual bool HandleViewVectorFragment(Context& kContext,
        NiMaterialResource* pkWorldPos, NiMaterialResource* pkWorldNorm,
        NiMaterialResource* pkWorldBinorm, NiMaterialResource* pkWorldTangent,
        bool bComputeTangent, NiMaterialResource*& pkWorldViewVector, 
        NiMaterialResource*& pkTangentViewVector);

    virtual bool SetupTransformPipeline(Context& kContext,
        NiMaterialResource* pkVertOutProjPos,
        NiStandardVertexProgramDescriptor* pkVertDesc, bool bForceView,
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

NiSmartPointer(NiDirectionalShadowWriteMaterial);


#endif // NIDIRECTIONALSHADOWWRITEMATERIAL_H
