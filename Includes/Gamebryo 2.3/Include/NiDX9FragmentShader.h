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

#ifndef NIDX9FRAGMENTSHADER_H
#define NIDX9FRAGMENTSHADER_H

#include "NiD3DShader.h"
#include <NiShaderDesc.h>
#include <NiMaterialDescriptor.h>
#include <NiFragmentShaderInstanceDescriptor.h>

class NIDX9RENDERER_ENTRY NiDX9FragmentShader : public NiD3DShader
{
    NiDeclareRTTI;
public:
    NiDX9FragmentShader(NiMaterialDescriptor* pkDesc);
    virtual ~NiDX9FragmentShader();

    // Initialize function
    virtual bool Initialize(NiD3DRenderer* pkRenderer);

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
    virtual unsigned int PostRender(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        const NiSkinPartition::Partition* pkPartition, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
            const NiTransform& kWorld, const NiBound& kWorldBound);
    virtual unsigned int PostProcessPipeline(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects,
        const NiTransform& kWorld, const NiBound& kWorldBound);

    // Advance to the next pass.
    virtual unsigned int FirstPass();
    virtual unsigned int NextPass();

    virtual void DestroyRendererData();
    virtual void RecreateRendererData();

    virtual bool IsGenericallyConfigurable();

    // Render Pass
    virtual bool AppendRenderPass(unsigned int& uiPassId);
    virtual NiShaderDeclaration* CreateShaderDeclaration(
        unsigned int uiMaxStreamEntryCount, 
        unsigned int uiStreamCount = 1);

    // Render States
    virtual bool SetAlphaOverride(unsigned int uiPassId, bool bAlphaBlend,
        bool bUsePreviousSrcBlendMode, 
        NiAlphaProperty::AlphaFunction eSrcBlendMode, 
        bool bUsePreviousDestBlendMode, 
        NiAlphaProperty::AlphaFunction eDestBlendMode);

    // GPU Programs
    virtual bool SetGPUProgram(unsigned int uiPassId, 
        NiGPUProgram* pkProgram, NiGPUProgram::ProgramType& eProgramType);
    virtual NiShaderConstantMap* CreateShaderConstantMap(
        unsigned int uiPassId, NiGPUProgram::ProgramType eProgramType,
        unsigned int uiMapIndex = 0);

    // Texture samplers
    virtual bool AppendTextureSampler(unsigned int uiPassId, 
        unsigned int& uiSamplerId, const NiFixedString& kSemantic,
        const NiFixedString& kVariableName, unsigned int uiInstance = 0);

    virtual const NiShader::NiShaderInstanceDescriptor* 
        GetShaderInstanceDesc() const;

protected:

    void PrepareTextureStage(NiGeometry* pkGeometry,
        NiD3DTextureStage* pkStage,
        const NiTexturingProperty* pkTexProp,
        const NiDynamicEffectState* pkEffects, 
        unsigned int uiPass);

    static unsigned int GetSamplerValue(NiD3DRenderer* pkD3DRenderer,
        unsigned int uiState, unsigned int uiValue,  
        NiTexturingProperty::ClampMode eClampMode,
        NiTexturingProperty::FilterMode eFilterMode);

    NiFragmentShaderInstanceDescriptor m_kDescriptor;
};

typedef NiPointer<NiDX9FragmentShader> NiDX9FragmentShaderPtr;

#endif  //#ifndef NIDX9FRAGMENTSHADER_H
