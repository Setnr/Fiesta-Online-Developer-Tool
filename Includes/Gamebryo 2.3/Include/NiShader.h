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

#ifndef NISHADER_H
#define NISHADER_H

#include "NiRTTI.h"
#include "NiRefObject.h"
#include "NiStream.h"

#include "NiSkinPartition.h"
#include "NiGeometryData.h"
#include "NiGPUProgram.h"
#include "NiAlphaProperty.h"

class NiGeometry;
class NiPropertyState;
class NiDynamicEffectState;
class NiSkinInstance;
class NiShaderConstantMap;
class NiShaderDeclaration;

class NIMAIN_ENTRY NiShader : public NiRefObject
{
    NiDeclareRootRTTI(NiShader);
public:
    enum Platform
    {
        // Unused (legacy)  = 0x00000001,
        NISHADER_DX9        = 0x00000002,
        // Unused (legacy)  = 0x00000004,
        NISHADER_XENON      = 0x00000008, 
        NISHADER_PS3        = 0x00000010,
        NISHADER_D3D10      = 0x00000020,
        NISHADER_AGNOSTIC   = NISHADER_DX9 | NISHADER_D3D10 | 
                                NISHADER_XENON | NISHADER_PS3
    };

    enum
    {
        DEFAULT_IMPLEMENTATION = 0xffffffff
    };

    NiShader();
    virtual ~NiShader();

    const NiFixedString& GetName() const;
    unsigned int GetImplementation() const;

    // Overridable functions
    virtual bool IsInitialized();

    virtual bool Initialize();

    virtual bool SetupGeometry(NiGeometry* pkGeometry);

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

    virtual bool GetVertexInputSemantics(
        NiShaderDeclaration::ShaderRegisterEntry* pakSemantics);

    // Advance to the next pass.
    virtual unsigned int FirstPass();
    virtual unsigned int NextPass();

    // Determine whether or not this class implements the generic
    // NiShader configuration API
    virtual bool IsGenericallyConfigurable();

    // Geometry Packing
    virtual NiShaderDeclaration* CreateShaderDeclaration(
        unsigned int uiMaxStreamEntryCount, 
        unsigned int uiStreamCount = 1);

    // Render Pass
    virtual bool AppendRenderPass(unsigned int& uiPassId);

    // Render States
    virtual bool SetAlphaOverride(unsigned int uiPassId, bool bAlphaBlend,
        bool bUsePreviousSrcBlendMode, 
        NiAlphaProperty::AlphaFunction eSrcBlendMode, 
        bool bUsePreviousDestBlendMode, 
        NiAlphaProperty::AlphaFunction eDestBlendMode);
    virtual bool SetUsesNiRenderState(bool bRenderState);

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

    // *** begin Emergent internal use only ***
    void SetName(const NiFixedString& kName);
    void SetImplementation(unsigned int uiImplementation);

    class NIMAIN_ENTRY NiShaderInstanceDescriptor : public NiMemObject
    {
        NiDeclareRootRTTI(NiShaderInstanceDescriptor);
    public:
        virtual ~NiShaderInstanceDescriptor(){};
    };

    virtual const NiShaderInstanceDescriptor* 
        GetShaderInstanceDesc() const;
    
    // *** end Emergent internal use only ***

protected:
    NiFixedString m_kName;
    unsigned int m_uiImplementation;

    bool m_bInitialized;
};

typedef NiPointer<NiShader> NiShaderPtr;

#endif  //#ifndef NISHADER_H
