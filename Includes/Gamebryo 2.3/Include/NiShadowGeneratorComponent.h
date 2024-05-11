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

#ifndef NISHADOWGENERATORCOMPONENT_H
#define NISHADOWGENERATORCOMPONENT_H

#include <NiRefObject.h>
#include <NiSmartPointer.h>
#include "NiEntityComponentInterface.h"
#include <NiFlags.h>
#include <NiShadowGenerator.h>
#include <NiTArray.h>
#include "NiEntityInterface.h"

NiSmartPointer(NiShadowGeneratorComponent);

class NIENTITY_ENTRY NiShadowGeneratorComponent : public NiRefObject,
    public NiEntityComponentInterface
{
    NiDeclareFlags(unsigned int);

public:
    // Error strings.
    static NiFixedString ERR_SCENE_ROOT_POINTER_NOT_FOUND;
    static NiFixedString ERR_SCENE_ROOT_POINTER_NOT_A_DYNAMIC_EFFECT;

    // Semantic types.
    static NiFixedString SEMANTIC_SIZE_HINT;
    static NiFixedString SEMANTIC_SHADOW_TECHNIQUE;
    static NiFixedString SEMANTIC_DEPTH_BIAS;

    NiShadowGeneratorComponent(unsigned int uiCasterArraySize = 0,
        unsigned int uiCasterArrayGrowBy = 8,
        unsigned int uiUnaffectedReceiverArraySize = 0,
        unsigned int uiUnaffectedReceiverArrayGrowBy = 8);
    NiShadowGeneratorComponent(bool bCastShadows, bool bStaticShadows,
        unsigned short usSizeHint, bool bStrictlyObserveSizeHint,
        const NiFixedString& kShadowTechnique, bool bRenderBackfaces,
        float fDepthBias, bool bUseDefaultDepthBias,
        unsigned int uiCasterArraySize = 0,
        unsigned int uiCasterArrayGrowBy = 8,
        unsigned int uiUnaffectedReceiverArraySize = 0,
        unsigned int uiUnaffectedReceiverArrayGrowBy = 8);

    virtual ~NiShadowGeneratorComponent();

    void SetCastShadows(bool bCastShadows);
    bool GetCastShadows() const;

    void SetStaticShadows(bool bStaticShadows);
    bool GetStaticShadows() const;

    void SetSizeHint(unsigned short usSizeHint);
    unsigned short GetSizeHint() const;

    void SetStrictlyObserveSizeHint(bool bStrictlyObserveSizeHint);
    bool GetStrictlyObserveSizeHint() const;

    unsigned int GetUnaffectedCasterCount() const;
    NiEntityInterface* GetUnaffectedCasterAt(unsigned int uiIndex) const;
    void AddUnaffectedCaster(NiEntityInterface* pkEntity);
    void RemoveUnaffectedCaster(NiEntityInterface* pkEntity);
    void RemoveUnaffectedCasterAt(unsigned int uiIndex);
    void RemoveAllUnaffectedCasters();

    unsigned int GetUnaffectedReceiverCount() const;
    NiEntityInterface* GetUnaffectedReceiverAt(unsigned int uiIndex) const;
    void AddUnaffectedReceiver(NiEntityInterface* pkEntity);
    void RemoveUnaffectedReceiver(NiEntityInterface* pkEntity);
    void RemoveUnaffectedReceiverAt(unsigned int uiIndex);
    void RemoveAllUnaffectedReceivers();

    void SetShadowTechnique(const NiFixedString& kShadowTechnique);
    const NiFixedString& GetShadowTechnique() const;

    void SetRenderBackfaces(bool bRenderBackfaces);
    bool GetRenderBackfaces() const;

    void SetDepthBias(float fDepthBias);
    float GetDepthBias() const;

    void SetUseDefaultDepthBias(bool bUseDefaultDepthBias);
    bool GetUseDefaultDepthBias() const;

    NiShadowGenerator* GetShadowGenerator() const;

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    NiShadowGeneratorComponent(unsigned int uiCasterArraySize,
        unsigned int uiCasterArrayGrowBy,
        unsigned int uiUnaffectedReceiverArraySize,
        unsigned int uiUnaffectedReceiverArrayGrowBy,
        NiShadowGeneratorComponent* pkMasterComponent);

    // Flags.
    enum
    {
        SHADOW_GENERATOR_PROPERTIES_CHANGED_MASK    = 0x0001,
        UNAFFECTED_CASTERS_CHANGED_MASK             = 0x0002,
        UNAFFECTED_RECEIVERS_CHANGED_MASK           = 0x0004,
        CAST_SHADOWS_UNIQUE_MASK                    = 0x0008,
        STATIC_SHADOWS_UNIQUE_MASK                  = 0x0010,
        SIZE_HINT_UNIQUE_MASK                       = 0x0020,
        STRICTLY_OBSERVE_SIZE_HINT_UNIQUE_MASK      = 0x0040,
        SHADOW_TECHNIQUE_UNIQUE_MASK                = 0x0080,
        RENDER_BACKFACES_UNIQUE_MASK                = 0x0100,
        DEPTH_BIAS_UNIQUE_MASK                      = 0x0200,
        USE_DEFAULT_DEPTH_BIAS_MASK                 = 0x0400
    };
    NiBool GetShadowGeneratorPropertiesChanged() const;
    void SetShadowGeneratorPropertiesChanged(
        bool bShadowGeneratorPropertiesChanged);
    NiBool GetUnaffectedCastersChanged() const;
    void SetUnaffectedCastersChanged(bool bCastersChanged);
    NiBool GetUnaffectedReceiversChanged() const;
    void SetUnaffectedReceiversChanged(bool bUnaffectedReceiversChanged);
    NiBool GetCastShadowsUnique() const;
    void SetCastShadowsUnique(bool bCastShadowsUnique);
    NiBool GetStaticShadowsUnique() const;
    void SetStaticShadowsUnique(bool bStaticShadowsUnique);
    NiBool GetSizeHintUnique() const;
    void SetSizeHintUnique(bool bSizeHintUnique);
    NiBool GetStrictlyObserveSizeHintUnique() const;
    void SetStrictlyObserveSizeHintUnique(bool bStrictlyObserveSizeHintUnique);
    NiBool GetShadowTechniqueUnique() const;
    void SetShadowTechniqueUnique(bool bShadowTechniqueUnique);
    NiBool GetRenderBackfacesUnique() const;
    void SetRenderBackfacesUnique(bool bRenderBackfacesUnique);
    NiBool GetDepthBiasUnique() const;
    void SetDepthBiasUnique(bool bDepthBiasUnique);
    NiBool GetUseDefaultDepthBiasUnique() const;
    void SetUseDefaultDepthBiasUnique(bool bUseDefaultDepthBiasUnique);

    void CreateShadowGenerator(NiDynamicEffect* pkDynamicEffect);
    void ClearShadowGenerator();

    bool ShouldUpdateShadowGeneratorProperties() const;
    void RefreshCachedShadowGeneratorProperties();

    NiShadowGeneratorPtr m_spShadowGenerator;
    NiDynamicEffectPtr m_spLastDynamicEffect;
    NiShadowGeneratorComponentPtr m_spMasterComponent;

    // Properties.
    NiTPrimitiveArray<NiEntityInterface*> m_kUnaffectedCasters;
    NiTPrimitiveArray<NiEntityInterface*> m_kUnaffectedReceivers;
    NiFixedString m_kShadowTechnique;
    float m_fDepthBias;
    unsigned short m_usSizeHint;
    bool m_bCastShadows;
    bool m_bStaticShadows;
    bool m_bStrictlyObserveSizeHint;
    bool m_bRenderBackfaces;
    bool m_bUseDefaultDepthBias;

    // Class name.
    static NiFixedString ms_kClassName;

    // Component name.
    static NiFixedString ms_kComponentName;

    // Property names.
    static NiFixedString ms_kCastShadowsName;
    static NiFixedString ms_kStaticShadowsName;
    static NiFixedString ms_kSizeHintName;
    static NiFixedString ms_kStrictlyObserveSizeHintName;
    static NiFixedString ms_kUnaffectedCastersName;
    static NiFixedString ms_kUnaffectedReceiversName;
    static NiFixedString ms_kShadowTechniqueName;
    static NiFixedString ms_kRenderBackfacesName;
    static NiFixedString ms_kDepthBiasName;
    static NiFixedString ms_kUseDefaultDepthBiasName;
    static NiFixedString ms_kShadowGeneratorPointerName;

    // Property descriptions.
    static NiFixedString ms_kCastShadowsDescription;
    static NiFixedString ms_kStaticShadowsDescription;
    static NiFixedString ms_kSizeHintDescription;
    static NiFixedString ms_kStrictlyObserveSizeHintDescription;
    static NiFixedString ms_kUnaffectedCastersDescription;
    static NiFixedString ms_kUnaffectedReceiversDescription;
    static NiFixedString ms_kShadowTechniqueDescription;
    static NiFixedString ms_kRenderBackfacesDescription;
    static NiFixedString ms_kDepthBiasDescription;
    static NiFixedString ms_kUseDefaultDepthBiasDescription;

    // Dependent property names.
    static NiFixedString ms_kSceneRootPointerName;

public:
    // NiEntityComponentInterface overrides.
    virtual NiEntityComponentInterface* Clone(bool bInheritProperties);
    virtual NiEntityComponentInterface* GetMasterComponent();
    virtual void SetMasterComponent(
        NiEntityComponentInterface* pkMasterComponent);
    virtual void GetDependentPropertyNames(
        NiTObjectSet<NiFixedString>& kDependentPropertyNames);

    // NiEntityPropertyInterface overrides.
    virtual NiBool SetTemplateID(const NiUniqueID& kID);
    virtual NiUniqueID GetTemplateID();
    virtual void AddReference();
    virtual void RemoveReference();
    virtual NiFixedString GetClassName() const;
    virtual NiFixedString GetName() const;
    virtual NiBool SetName(const NiFixedString& kName);
    virtual void Update(NiEntityPropertyInterface* pkParentEntity,
        float fTime, NiEntityErrorInterface* pkErrors,
        NiExternalAssetManager* pkAssetManager);
    virtual void BuildVisibleSet(NiEntityRenderingContext* pkRenderingContext,
        NiEntityErrorInterface* pkErrors);
    virtual void GetPropertyNames(
        NiTObjectSet<NiFixedString>& kPropertyNames) const;
    virtual NiBool CanResetProperty(const NiFixedString& kPropertyName,
        bool& bCanReset) const;
    virtual NiBool ResetProperty(const NiFixedString& kPropertyName);
    virtual NiBool MakePropertyUnique(const NiFixedString& kPropertyName,
        bool& bMadeUnique);
    virtual NiBool GetDisplayName(const NiFixedString& kPropertyName,
        NiFixedString& kDisplayName) const;
    virtual NiBool SetDisplayName(const NiFixedString& kPropertyName,
        const NiFixedString& kDisplayName);
    virtual NiBool GetPrimitiveType(const NiFixedString& kPropertyName,
        NiFixedString& kPrimitiveType) const;
    virtual NiBool SetPrimitiveType(const NiFixedString& kPropertyName,
        const NiFixedString& kPrimitiveType);
    virtual NiBool GetSemanticType(const NiFixedString& kPropertyName,
        NiFixedString& kSemanticType) const;
    virtual NiBool SetSemanticType(const NiFixedString& kPropertyName,
        const NiFixedString& kSemanticType);
    virtual NiBool GetDescription(const NiFixedString& kPropertyName,
        NiFixedString& kDescription) const;
    virtual NiBool SetDescription(const NiFixedString& kPropertyName,
        const NiFixedString& kDescription);
    virtual NiBool GetCategory(const NiFixedString& kPropertyName,
        NiFixedString& kCategory) const;
    virtual NiBool IsPropertyReadOnly(const NiFixedString& kPropertyName,
        bool& bIsReadOnly);
    virtual NiBool IsPropertyUnique(const NiFixedString& kPropertyName,
        bool& bIsUnique);
    virtual NiBool IsPropertySerializable(const NiFixedString& kPropertyName,
        bool& bIsSerializable);
    virtual NiBool IsPropertyInheritable(const NiFixedString& kPropertyName,
        bool& bIsInheritable);
    virtual NiBool IsExternalAssetPath(const NiFixedString& kPropertyName,
        unsigned int uiIndex, bool& bIsExternalAssetPath) const;
    virtual NiBool GetElementCount(const NiFixedString& kPropertyName,
        unsigned int& uiCount) const;
    virtual NiBool SetElementCount(const NiFixedString& kPropertyName,
        unsigned int uiCount, bool& bCountSet);
    virtual NiBool IsCollection(const NiFixedString& kPropertyName,
        bool& bIsCollection) const;
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        float& fData, unsigned int uiIndex = 0) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        float fData, unsigned int uiIndex = 0);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        bool& bData, unsigned int uiIndex = 0) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        bool bData, unsigned int uiIndex = 0);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        unsigned short& usData, unsigned int uiIndex = 0) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        unsigned short usData, unsigned int uiIndex = 0);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        NiFixedString& kData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        const NiFixedString& kData, unsigned int uiIndex);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        NiObject*& pkData, unsigned int uiIndex) const;
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        NiEntityInterface*& pkData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        NiEntityInterface* pkData, unsigned int uiIndex);
};

#include "NiShadowGeneratorComponent.inl"

#endif  // #ifndef NISHADOWGENERATORCOMPONENT_H
