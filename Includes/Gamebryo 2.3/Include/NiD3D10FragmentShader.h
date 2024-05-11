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

#ifndef NID3D10FRAGMENTSHADER_H
#define NID3D10FRAGMENTSHADER_H

#include "NiD3D10RendererLibType.h"

#include "NiD3D10Shader.h"

#include <NiShaderDesc.h>
#include <NiMaterialDescriptor.h>
#include <NiFragmentShaderInstanceDescriptor.h>
#include <NiTexturingProperty.h>

class NiD3D10Pass;
class NiMaterialDescriptor;
class NiShaderDesc;

class NID3D10RENDERER_ENTRY NiD3D10FragmentShader : public NiD3D10Shader
{
    NiDeclareRTTI;
public:
    NiD3D10FragmentShader(NiMaterialDescriptor* pkDesc);
    virtual ~NiD3D10FragmentShader();

    // Initialize function
    virtual bool Initialize();

    virtual unsigned int UpdatePipeline(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects,
        const NiTransform& kWorld, const NiBound& kWorldBound);

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

    void PrepareTexture(NiGeometry* pkGeometry, NiD3D10Pass* pkPass, 
        unsigned int uiTexture,
        const NiTexturingProperty* pkTexProp,
        const NiDynamicEffectState* pkEffects, unsigned int uiPass);

    NiFragmentShaderInstanceDescriptor m_kDescriptor;
};

typedef NiPointer<NiD3D10FragmentShader> NiD3D10FragmentShaderPtr;

#endif  //#ifndef NID3D10FRAGMENTSHADER_H
