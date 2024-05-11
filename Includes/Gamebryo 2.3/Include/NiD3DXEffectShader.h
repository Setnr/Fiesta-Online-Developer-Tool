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

#ifndef NID3DXEFFECTSHADER_H
#define NID3DXEFFECTSHADER_H

#include "NiD3DXEffectShaderLibType.h"
#include <NiD3DShader.h>

class NiD3DXEffectTechnique;

class NID3DXEFFECTSHADER_ENTRY NiD3DXEffectShader : public NiD3DShader
{
    NiDeclareRTTI;
public:
    NiD3DXEffectShader();
    virtual ~NiD3DXEffectShader();

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

    virtual unsigned int FirstPass();
    virtual unsigned int NextPass();

    virtual void HandleLostDevice();
    virtual void HandleResetDevice();

    virtual void DestroyRendererData();
    virtual void RecreateRendererData();

    // *** begin Emergent internal use only

    enum BoneMatrixCalcMethod
    {
        BONECALC_SKIN       = 0x00000000,
        BONECALC_NO_SKIN    = 0x00000001
    };

    unsigned int GetBoneMatrixRegisters() const;
    void SetBoneMatrixRegisters(unsigned int uiRegisters);
    BoneMatrixCalcMethod GetBoneCalcMethod() const;
    void SetBoneCalcMethod(BoneMatrixCalcMethod eMethod);

    // Shader Declaration (Packing definition)
    NiD3DShaderDeclaration* GetShaderDecl() const;
    void SetShaderDecl(NiD3DShaderDeclaration* pkShaderDecl);

    void SetEffectTechnique(NiD3DXEffectTechnique* pkD3DXEffectTechnique);
    void SetTechniquePtr(D3DXTechniquePtr pkTechnique);

    virtual bool SetupGeometry(NiGeometry* pkGeometry);

    void SetSoftwareVP(bool bSoftwareVP);

    // Query shader for presence of shader programs
    virtual bool GetVSPresentAllPasses() const;
    virtual bool GetVSPresentAnyPass() const;
    virtual bool GetPSPresentAllPasses() const;
    virtual bool GetPSPresentAnyPass() const;

    void SetActualImplementation(unsigned int uiActualImplementation);
    unsigned int GetActualImplementation() const;

    // *** end Emergent internal use only

protected:
    void CreateShaderDeclaration();

    NiD3DXEffectTechnique* m_pkD3DXEffectTechnique;
    D3DXTechniquePtr m_pkTechnique;

    LPD3DXBASEEFFECT m_pkD3DXEffect;
    LPD3DXEFFECT m_pkD3DXRenderableEffect;

    // Actual implementation number in case this shader was created
    // with the "default implementation" value.
    unsigned int m_uiActualImplementation;

    unsigned int m_uiBoneMatrixRegisters;
    BoneMatrixCalcMethod m_eBoneCalcMethod;
    bool m_bSoftwareVP;
};

NiSmartPointer(NiD3DXEffectShader);

#include "NiD3DXEffectShader.inl"

#endif  //NID3DXEFFECTSHADER_H
