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

#ifndef NIGEOMETRY_H
#define NIGEOMETRY_H

#include "NiAVObject.h"
#include "NiDynamicEffectState.h"
#include "NiGeometryData.h"
#include "NiMaterialInstance.h"
#include "NiProcessor.h"
#include "NiPropertyState.h"
#include "NiRenderer.h"
#include "NiShader.h"
#include "NiSkinInstance.h"
#include "NiTPtrSet.h"
#include "NiShaderDeclaration.h"

class NiRenderer;
class NiMaterial;

class NIMAIN_ENTRY NiGeometry : public NiAVObject
{
    NiDeclareRTTI;
    NiDeclareAbstractClone(NiGeometry);
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;

public:
    virtual ~NiGeometry();

    // vertices
    unsigned short GetVertexCount() const;
    NiPoint3* GetVertices() const;
    void SetActiveVertexCount(unsigned short usActive);
    unsigned short GetActiveVertexCount() const;

    // normals
    void CreateNormals(bool bInitialize);
    NiPoint3* GetNormals() const;
    NiGeometryData::DataFlags GetNormalBinormalTangentMethod() const;
    void StoreNormalBinormalTangent(NiGeometryData::DataFlags eNBTMethod);

    // bounds
    void SetModelBound(const NiBound& kBound);
    NiBound& GetModelBound() const;

    // color access
    void CreateColors(bool bInitialize);
    NiColorA* GetColors() const;

    // texture coordinate access
    void CreateTextures(bool bInitialize, unsigned short usSets);
    unsigned short GetTextureSets() const;
    NiPoint2* GetTextures() const;
    NiPoint2* GetTextureSet(unsigned short usSet) const;
    void AppendTextureSet(NiPoint2* pkSet);

    // skinning
    NiSkinInstance* GetSkinInstance() const;
    void SetSkinInstance(NiSkinInstance* pkSkinInstance);

    // State access
    NiPropertyStatePtr GetPropertyState() const;
    void SetPropertyState(NiPropertyState* pkState);
    NiDynamicEffectStatePtr GetEffectState() const;
    void SetEffectState(NiDynamicEffectState* pkState);

    // additional geometry data
    NiAdditionalGeometryData* GetAdditionalGeometryData();
    void SetAdditionalGeometryData(NiAdditionalGeometryData* pkData);

    inline void SetSortObject(bool bSort);
    inline bool GetSortObject() const;

    // display
    virtual void RenderImmediate(NiRenderer* pkRenderer);

    // NiShader system
    NiShader* GetShader() const;
    NiShader* GetShaderFromMaterial();
    void SetShader(NiShader* pkShader);

    // NiMaterial system
    bool ApplyMaterial(const NiFixedString& kName,
        unsigned int uiMaterialExtraData = 
        NiMaterialInstance::DEFAULT_EXTRA_DATA);
    bool ApplyMaterial(NiMaterial* pkMaterial,
        unsigned int uiMaterialExtraData = 
        NiMaterialInstance::DEFAULT_EXTRA_DATA);
    bool ApplyAndSetActiveMaterial(const NiFixedString& kName,
        unsigned int uiMaterialExtraData = 
        NiMaterialInstance::DEFAULT_EXTRA_DATA);
    bool ApplyAndSetActiveMaterial(NiMaterial* pkMaterial,
        unsigned int uiMaterialExtraData = 
        NiMaterialInstance::DEFAULT_EXTRA_DATA);
    bool RemoveMaterial(const NiFixedString& kName);
    bool RemoveMaterial(NiMaterial* pkMaterial);
    bool IsMaterialApplied(const NiFixedString& kName) const;
    bool IsMaterialApplied(const NiMaterial* pkMaterial) const;

    bool SetActiveMaterial(const NiFixedString& kName);
    bool SetActiveMaterial(const NiMaterial* pkMaterial);
    const NiMaterial* GetActiveMaterial() const;

    const NiMaterialInstance* GetActiveMaterialInstance() const;

    static bool GetStreamingAppliesDefaultMaterial();
    static void SetStreamingAppliesDefaultMaterial(bool bApplyDefault = true);

    // material updates
    virtual void SetMaterialNeedsUpdate(bool bNeedsUpdate);

    virtual void SetDefaultMaterialNeedsUpdateFlag(bool bFlag);
    bool GetDefaultMaterialNeedsUpdateFlag() const;

    // shared data access
    virtual void SetModelData(NiGeometryData* pkModelData);
    NiGeometryData* GetModelData() const;

    bool ContainsVertexData(NiShaderDeclaration::ShaderParameter 
        eParameter) const;

    // *** begin Emergent internal use only ***
    unsigned int GetMaterialCount();
    const NiMaterialInstance* GetMaterialInstance(unsigned int uiInstance);
    bool ComputeCachedShader(unsigned int uiMaterial);
    void ClearCachedShader(unsigned int uiMaterial);

    NiShaderDeclaration* GetShaderDecl();
    void SetShaderDecl(NiShaderDeclaration* pkDecl);

    // culling (separate from display)
    virtual void OnVisible(NiCullingProcess& kCuller);

    // transforms
    virtual void ApplyTransform(const NiMatrix3& kMat, const NiPoint3& kTrn,
        bool bOnLeft);

    // updates
    virtual void CalculateNormals();
    virtual void UpdatePropertiesDownward(NiPropertyState* pkParentState);
    virtual void UpdateEffectsDownward(NiDynamicEffectState* pkParentState);

    // consistency
    virtual void CalculateConsistency(bool bTool = false);
    NiGeometryData::Consistency GetConsistency() const;
    void SetConsistency(NiGeometryData::Consistency eConsistency);

    // cloning
    virtual void ProcessClone(
        NiCloningProcess& kCloning);

    // post-link
    virtual void PostLinkObject(NiStream& kStream);

    // set selective update flags
    virtual void SetSelectiveUpdateFlags(bool& bSelectiveUpdate, 
        bool bSelectiveUpdateTransforms, bool& bRigid);

    // renderer data
    virtual void PurgeRendererData(NiRenderer* pkRenderer);
    // *** end Emergent internal use only ***

protected:
    // construction (always uses a shared data object)
    NiGeometry(NiGeometryData* pkModelData);

    // streaming support
    NiGeometry();

    // updates
    virtual void UpdateWorldBound();

    // property and effect state caching
    NiPropertyStatePtr m_spPropertyState;
    NiDynamicEffectStatePtr m_spEffectState;

    // model data
    NiGeometryDataPtr m_spModelData;
    NiSkinInstancePtr m_spSkinInstance;

    // Materials
    NiTObjectSet<NiMaterialInstance> m_kMaterials;
    unsigned int m_uiActiveMaterial;
    NiShaderDeclarationPtr m_spShaderDecl;
    bool m_bDefaultMaterialDirtyFlag;

    static bool ms_bStreamingAppliesDefaultMaterial;

    enum 
    {
        NO_MATERIAL = UINT_MAX
    };
};

typedef NiPointer<NiGeometry> NiGeometryPtr;

// note these are stars, not smart pointers
typedef NiTPointerList<NiGeometry*> NiGeometryList;

#include "NiGeometry.inl"

#endif

