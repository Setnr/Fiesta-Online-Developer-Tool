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

#ifndef NID3D10SHADER_H
#define NID3D10SHADER_H

#include "NiD3D10ShaderInterface.h"

#include "NiD3D10Pass.h"

NiSmartPointer(NiD3D10RenderStateGroup);
NiSmartPointer(NiD3D10ShaderConstantMap);

#include <NiFixedString.h>
#include <NiTArray.h>
#include <NiTextureEffect.h>

class NID3D10RENDERER_ENTRY NiD3D10Shader : public NiD3D10ShaderInterface
{
    NiDeclareRTTI;

public:
    static void _SDMInit();
    static void _SDMShutdown();

    NiD3D10Shader();
    virtual ~NiD3D10Shader();

    virtual bool IsInitialized();

    // Override these functions to implement a custom pipeline...
    // Initialize function
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

    virtual bool GetVertexInputSemantics(
        NiShaderDeclaration::ShaderRegisterEntry* pkSemantics);

    // Advance to the next pass.
    virtual unsigned int FirstPass();
    virtual unsigned int NextPass();

    // This function is called by NiMaterialInstance::GetCurrentShader, and
    // is used to verify the geometry is usable for the shader, as well
    // as to 'process' the geometry, adding what is needed.
    virtual bool SetupGeometry(NiGeometry* pkGeometry);

    // This function sets up an NiSCMExtraData object on the geometry that
    // caches extra data lookups for shaders. It is declared static so that
    // applications can call it if necessary when removing and adding extra
    // data.
    static void SetupSCMExtraData(NiD3D10Shader* pkShader, 
        NiGeometry* pkGeometry);
    void ResetSCMExtraData(NiGeometry* pkGeometry);

    // Allow for complete shader reconstruction
    virtual void DestroyRendererData();
    virtual void RecreateRendererData();

    // Allows the shader to read render state from NiProperties
    bool GetUsesNiRenderState() const;
    bool SetUsesNiRenderState(bool bUses);

    void GetBoneParameters(bool& bTransposeBones, 
        unsigned int& uiBoneMatrixRegisters, bool& bWorldSpaceBones);
    void SetBoneParameters(bool bTransposeBones, 
        unsigned int uiBoneMatrixRegisters, bool bWorldSpaceBones);

    // *** begin Emergent internal use only

    // Packing particles
    virtual NiGeometryData::RendererData* PrepareGeometryForRendering_Points(
        NiGeometry* pkGeometry, const NiSkinPartition::Partition* pkPartition,
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState);

    // Packing lines
    virtual NiGeometryData::RendererData* PrepareGeometryForRendering_Lines(
        NiGeometry* pkGeometry, const NiSkinPartition::Partition* pkPartition,
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState);

    static const NiFixedString& GetEmergentShaderMapName();

    static void UseWorldSpaceSphereMaps(bool bWorldSpace);

    // *** begin Emergent internal use only

protected:
    // Projected texture transform packing functions
    static void PackDynamicEffect(const NiTextureEffect* pkTextureEffect,
        NiD3D10Pass* pkPass, unsigned int uiSampler);

    typedef void (*DynamicEffectPacker)(
        const NiMatrix3& kWorldMat, const NiPoint3& kWorldTrans, 
        NiD3D10Pass* pkPass, unsigned int uiSampler);

    static void PackWorldParallelEffect(const NiMatrix3& kWorldMat, 
        const NiPoint3& kWorldTrans, NiD3D10Pass* pkPass, 
        unsigned int uiSampler);
    static void PackWorldPerspectiveEffect(const NiMatrix3& kWorldMat, 
        const NiPoint3& kWorldTrans, NiD3D10Pass* pkPass, 
        unsigned int uiSampler);
    static void PackWorldSphereEffect(const NiMatrix3& kWorldMat, 
        const NiPoint3& kWorldTrans, NiD3D10Pass* pkPass, 
        unsigned int uiSampler);
    static void PackCameraSphereEffect(const NiMatrix3& kWorldMat, 
        const NiPoint3& kWorldTrans, NiD3D10Pass* pkPass, 
        unsigned int uiSampler);
    static void PackSpecularCubeEffect(const NiMatrix3& kWorldMat, 
        const NiPoint3& kWorldTrans, NiD3D10Pass* pkPass, 
        unsigned int uiSampler);
    static void PackDiffuseCubeEffect(const NiMatrix3& kWorldMat, 
        const NiPoint3& kWorldTrans, NiD3D10Pass* pkPass, 
        unsigned int uiSampler);

    static DynamicEffectPacker ms_apfnDynEffectPackers[
        NiTextureEffect::NUM_COORD_GEN];

    NiD3D10RenderStateGroupPtr m_spRenderStateGroup;

    NiTObjectArray<NiD3D10ShaderConstantMapPtr> m_kVertexShaderConstantMaps;
    NiTObjectArray<NiD3D10ShaderConstantMapPtr> m_kGeometryShaderConstantMaps;
    NiTObjectArray<NiD3D10ShaderConstantMapPtr> m_kPixelShaderConstantMaps;

    unsigned int m_uiCurrentPass;
    NiTObjectArray<NiD3D10PassPtr> m_kPasses;

    NiD3D10Pass* m_pkCurrentPass;

    unsigned int m_uiBoneMatrixRegisters;
    bool m_bTransposeBones;
    bool m_bWorldSpaceBones;

    bool m_bUsesNiRenderState;

    static NiFixedString ms_kEmergentShaderMapName;
};

NiSmartPointer(NiD3D10Shader);

//#include "NiD3D10Shader.inl"

#endif  //#ifndef NID3D10SHADER_H
