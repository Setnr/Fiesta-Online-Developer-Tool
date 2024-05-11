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

#ifndef NISHADOWGENERATOR_H
#define NISHADOWGENERATOR_H

#include "NiShaderConstantMap.h"
#include "NiDynamicEffect.h"
#include "NiGeometry.h"
#include "NiShadowTechnique.h"

class NiShadowMap;
class NiShadowCubeMap;
class NiGeometry;

class NIMAIN_ENTRY NiShadowGenerator : public NiObject
{
    NiDeclareRTTI;
    NiDeclareClone(NiShadowGenerator);
    NiDeclareStream;
    NiDeclareFlags(unsigned short);

public:
    enum
    {
        AUTO_DETERMINE_SM_INDEX = 0xFF
    };

    // flags
    enum
    {
        DIRTY_SHADOWMAP  = 0x0001,
        DIRTY_RENDERVIEWS  = 0x0002,
        STATIC = 0x0004,
        ACTIVE = 0x0008,
        RENDER_BACKFACES = 0x0010,
        STRICTLY_OBSERVE_SIZE_HINT = 0x0020,

        AUTO_CALC_NEAR_DIST = 0x0040,
        AUTO_CALC_FAR_DIST = 0x0080,
        AUTO_CALC_DIR_FRUSTUM_WIDTH = 0x0100,
        AUTO_CALC_DIR_FRUSTUM_POSITION = 0x0200,

        AUTO_CALC_NEARFAR_MASK = 0x00C0,
        AUTO_CALC_FULL_MASK = 0x03C0
    };

    NiShadowGenerator();
    NiShadowGenerator(NiDynamicEffect* pkDynEffect);
    virtual ~NiShadowGenerator();

    NiDynamicEffect* GetAssignedDynamicEffect() const;

    void SetShadowTechnique(const NiFixedString& kTechniqueName, 
        bool bAssignDefaultDepthBias = true);
    void SetShadowTechnique(NiShadowTechnique* pkTechnique,
        bool bAssignDefaultDepthBias = true);
    NiShadowTechnique* GetShadowTechnique() const;

    bool AssignShadowMap(NiShadowMap* pkShadowMap, unsigned int uiSMIndex,
        NiTListIterator kSMIter = NULL, NiGeometry* pkGeometry = NULL);
    NiShadowMap* RetrieveShadowMap(unsigned int uiSMIndex, 
        NiGeometry* pkGeometry = NULL);

    NiShadowMap* GetOwnedShadowMap(unsigned int uiIndex);
    unsigned int GetShadowMapCount();

    bool SetShaderConstantData(NiGeometry* pkGeometry, unsigned int uiSMIndex, 
        NiShaderConstantMap::ObjectMappings eMapping, void* pkData);

    bool GetShaderConstantData(void* pvData, unsigned int uiDataSize, 
        NiGeometry* pkGeometry, unsigned int uiSMIndex, 
        NiShaderConstantMap::ObjectMappings eMapping, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld, const NiBound& kWorldBound, 
        unsigned int uiPass);

    // This function set the material needs update flag to true for all of the 
    // shadow receiving geometry.
    void ForceMaterialNeedsUpdate();

    // Flag management
    void MarkShadowMapsDirty();
    bool AreShadowMapsDirty();

    void SetRenderViewsDirty(bool bDirty);
    bool GetRenderViewsDirty();

    void SetStatic(bool bStatic);
    bool GetStatic();

    void SetRenderBackfaces(bool bBackfaces);
    bool GetRenderBackfaces();

    bool SetActive(bool bActive, bool bForceMaterialUpdate = false,
        bool bRetainShadowMaps = false);
    bool GetActive();

    // These functions manually builds the NiGeometryLists when called.
    void GetCasterGeometryList(NiGeometryList& kCasterList);
    void GetReceiverGeometryList(NiGeometryList& kReceiverList);

    // Unaffected shadow caster list management
    const NiNodeList& GetUnaffectedCasterNodeList();
    void AttachUnaffectedCasterNode(NiNode* pkUnaffectedCasterNode);
    void DetachUnaffectedCasterNode(NiNode* pkUnaffectedCasterNode);
    void DetachAllUnaffectedCasterNodes();

    // Unaffected shadow receiver list management
    const NiNodeList& GetUnaffectedReceiverNodeList();
    void AttachUnaffectedReceiverNode(NiNode* pkUnaffectedNode);
    void DetachUnaffectedReceiverNode(NiNode* pkUnaffectedNode);
    void DetachAllUnaffectedReceiverNodes();
    bool IsUnaffectedReceiverNode(const NiNode* pkPossibleNode);
    bool IsUnaffectedReceiverNode(const NiGeometry* pkGeometry);
    
    // Callback bookkeeping

    // Typedefs to define the parameters for the NiShadowMap and shader
    // constants callbacks.
    typedef bool (*AssignSMTextureCallback) 
        (NiShadowGenerator* pkGenerator, NiGeometry* pkGeometry, 
        NiShadowMap* pkShadowMap, unsigned int uiSMIndex);

    typedef NiShadowMap* (*RetrieveSMTextureCallback)
        (NiShadowGenerator* pkGenerator, NiGeometry* pkGeometry, 
        unsigned int uiSMIndex);

    typedef NiShadowMap* (*RetrieveOwnedShadowMap)
        (NiShadowGenerator* pkGenerator, unsigned int uiIndex);

    typedef unsigned int (*ShadowMapCountCallback)
        (NiShadowGenerator* pkGenerator);

    typedef bool (*SetSMConstantCallback)
        (NiShadowGenerator* pkGenerator, NiGeometry* pkGeometry, 
        unsigned int uiSMIndex, NiShaderConstantMap::ObjectMappings eMapping, 
        void* pkData);

    typedef bool (*GetSMConstantCallback)
        (void* pvData, unsigned int uiDataSize, 
        NiShadowGenerator* pkGenerator,
        NiGeometry* pkGeometry, unsigned int uiSMIndex, 
        NiShaderConstantMap::ObjectMappings eMapping,
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld, const NiBound& kWorldBound, 
        unsigned int uiPass);

    typedef void (*CleanupExtraDataCallback)
        (NiShadowGenerator* pkGenerator);

    // ShadowMap Texture Assignment callback.
    void SetSMTextureAssignmentCallbackFunc(
        AssignSMTextureCallback pfnCallback);
    AssignSMTextureCallback GetSMTextureAssignmentCallbackFunc() const;

    // ShadowMap Texture Retrieval callback.
    void SetSMTextureRetrievalCallbackFunc(
        RetrieveSMTextureCallback pfnCallback);
    RetrieveSMTextureCallback GetSMTextureRetrievalCallbackFunc() const;

    // Owned Shadow Map callback.
    void SetOwnedSMCallbackFunc(RetrieveOwnedShadowMap pfnCallback);
    RetrieveOwnedShadowMap GetOwnedSMCallbackFunc() const;

    // ShadowMap Count Callback
    void SetSMCountCallbackFunc(ShadowMapCountCallback pfnCallback);
    ShadowMapCountCallback GetSMCountCallbackFunc() const;

    // ShadowMap Constant Retrieval callback.
    void SetSMConstantRetrievalCallbackFunc(
        GetSMConstantCallback pfnCallback);
    GetSMConstantCallback GetSMConstantRetrievalCallbackFunc() const;

    // ShadowMap Constant Assignment callback.
    void SetSMConstantAssignmentCallbackFunc(
        SetSMConstantCallback pfnCallback);
    SetSMConstantCallback GetSMConstantAssignmentCallbackFunc() const;

    // Extra data cleanup callback (used to free any allocations made during
    // other callbacks when swapping out the set of callback functions).
    void SetExtraDataCleanupCallbackFunc(
        CleanupExtraDataCallback pfnCallback);
    CleanupExtraDataCallback GetExtraDataCleanupCallbackFunc() const;

    void* GetExtraData() const;
    void SetExtraData(void* pvExtraData);

    void SetSizeHint(unsigned short usSizeHint);
    unsigned short GetSizeHint() const;

    void SetStrictlyObserveSizeHint(bool bStrictlyObserveSizeHint);
    bool GetStrictlyObserveSizeHint() const;

    void SetDepthBias(float fDepthBias);
    float GetDepthBias();
    bool IsDepthBiasDefault();
    void SetDepthBiasToDefault();

    float GetNearClippingDist();
    void SetNearClippingDist(float fNearClippingDist);
    bool GetAutoCalcNearClippingDist();
    void SetAutoCalcNearClippingDist(bool bAutoDetermine);

    float GetFarClippingDist();
    void SetFarClippingDist(float fFarClippingDist);
    bool GetAutoCalcFarClippingDist();
    void SetAutoCalcFarClippingDist(bool bAutoDetermine);

    float GetDirLightFrusturmWidth();
    void SetDirLightFrusturmWidth(float fDirLightFrustumWidth);
    bool GetAutoCalcDirLightFrusturmWidth();
    void SetAutoCalcDirLightFrusturmWidth(bool bAutoDetermine);

    bool GetAutoCalcDirLightFrusturmPosition();
    void SetAutoCalcDirLightFrusturmPosition(bool bAutoDetermine);

    unsigned short GetFlags();
    void ReleaseAllShadowMaps();

    // Callback functions:
    static bool SingleShadowMapAssignmentCallback(
        NiShadowGenerator* pkGenerator, NiGeometry* pkGeometry, 
        NiShadowMap* pkShadowMap, unsigned int uiSMIndex);
    static NiShadowMap* SingleShadowMapRetrievalCallback(
        NiShadowGenerator* pkGenerator, NiGeometry* pkGeometry, 
        unsigned int uiSMIndex);
    static NiShadowMap* SingleShadowMapGetOwnedCallback(
        NiShadowGenerator* pkGenerator, unsigned int uiIndex);
    static unsigned int SingleShadowMapCountCallback(
        NiShadowGenerator* pkGenerator);
    static void SingleShadowMapCleanupCallback(NiShadowGenerator* pkGenerator);
    
    static bool DefaultGetShaderConstantCallback(void* pvData, 
        unsigned int uiDataSize, NiShadowGenerator* pkGenerator, 
        NiGeometry* pkGeometry, unsigned int uiSMIndex, 
        NiShaderConstantMap::ObjectMappings eMapping,
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld, const NiBound& kWorldBound, 
        unsigned int uiPass);

    // *** begin Emergent internal use only ***
    virtual bool StreamCanSkip();
    void SetDynamicEffect(NiDynamicEffect* pkEffect);
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:

    static void SetShadowMapGenerator(NiShadowMap* pkShadowMap,
        NiShadowGenerator* pkGenerator);
    static void SetShadowCubeMapGenerator(NiShadowCubeMap* pkShadowCubeMap,
        NiShadowGenerator* pkGenerator);
    
    bool IsUnaffectedReceiverNodeHelper(NiNode* pkCurrNode, 
        const NiNode* pkPossibleNode);
    bool IsUnaffectedReceiverNodeHelper(NiNode* pkCurrNode, 
        const NiGeometry* pkPossibleGeometry);

    void AdditiveListHelper(NiNode* pkNode, NiGeometryList* pkGeomList);
    void SubtractiveListHelper(NiNode* pkNode, NiGeometryList* pkGeomList);

    static NiStandardMaterial::LightType ConvertLightType(
        NiDynamicEffect::EffectType eEffectType);

    NiShadowTechnique* m_pkShadowTechnique;
   
    NiNodeList m_kUnaffectedCasterList;   
    NiNodeList m_kUnaffectedReceiverList;       
                           
    // Call back functions used to handle assignment and retrieval
    // of shadow maps.
    RetrieveSMTextureCallback m_pfnSMTextureRetrievalCallback;
    AssignSMTextureCallback m_pfnSMTextureAssignmentCallback;
    RetrieveOwnedShadowMap m_pfnOwnedSMCallback;
    ShadowMapCountCallback m_pfnSMCountCallback;

    // Call back functions used to handle assignment and retrieval
    // of shader constant maps.
    SetSMConstantCallback m_pfnSMConstantDataAssignmentCallback;
    GetSMConstantCallback m_pfnSMConstantDataRetrievalCallback;

    CleanupExtraDataCallback m_pfnExtraDataCleanupCallback;

    // Reference to the shadow creating source object. Currently only
    // lights are supported. A normal pointer is used to avoid cyclical 
    // dependencies.
    NiDynamicEffect* m_pkDynEffect;

    // This is intended to be used by custom callbacks.
    void* m_pvExtraData;

    // The depth bias applied by the DefaultGetShaderConstantCallback
    float m_fDepthBias;

    // Tweakable values to allow artist to explicitly override the
    // view frustum settings that are used.
    float m_fNearClippingDist;
    float m_fFarClippingDist;
    float m_fDirLightFrustumWidth;

    // The requested shadow map size for this shadow generator. Shadow click
    // generators may or may not observe this size hint.
    unsigned short m_usSizeHint;
};

#include "NiShadowGenerator.inl"

#endif // NISHADOWGENERATOR_H
