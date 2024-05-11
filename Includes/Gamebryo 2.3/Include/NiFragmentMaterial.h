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

#ifndef NIFRAGMENTMATERIAL_H
#define NIFRAGMENTMATERIAL_H

#include "NiMaterial.h"
#include "NiGeometry.h"
#include "NiTPtrSet.h"
#include "NiMaterialDescriptor.h"
#include "NiGPUProgramDescriptor.h"
#include "NiMaterialConfigurator.h"
#include "NiMaterialResourceProducerNode.h"
#include "NiMaterialResourceConsumerNode.h"
#include "NiMaterialFragmentNode.h"
#include "NiMaterialNodeLibrary.h"
#include "NiShaderConstantMap.h"
#include "NiFragmentShaderInstanceDescriptor.h"
#include "NiGPUProgramCache.h"

class NIMAIN_ENTRY NiFragmentMaterial : public NiMaterial
{
    NiDeclareRTTI;

public:
    enum TextureMapSamplerType
    {
        TEXTURE_SAMPLER_2D = 0,
        TEXTURE_SAMPLER_CUBE,
        TEXTURE_SAMPLER_MAX
    };

    // Static variables for all NiFragmentMaterials
    static bool GetDefaultLoadProgramCacheOnCreation();
    static bool GetDefaultAutoSaveProgramCache();
    static bool GetDefaultWriteDebugProgramData();
    static bool GetDefaultLockProgramCache();
    static bool GetDefaultAutoCreateProgramCache();

    static void SetDefaultLoadProgramCacheOnCreation(bool bSet);
    static void SetDefaultAutoSaveProgramCache(bool bSet);
    static void SetDefaultWriteDebugProgramData(bool bSet);
    static void SetDefaultLockProgramCache(bool bSet);
    static void SetDefaultAutoCreateProgramCache(bool bSet);

    // NiFragmentMaterial methods
    unsigned int GetProgramVersion(NiGPUProgram::ProgramType eType) const;

    virtual bool IsShaderCurrent(NiShader* pkShader, 
        const NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects,
        unsigned int uiMaterialExtraData);

    virtual NiShader* GetCurrentShader(const NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects, 
        unsigned int uiMaterialExtraData);

    virtual bool PrecacheGPUPrograms(const NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects, 
        unsigned int uiMaterialExtraData);

    virtual void UnloadShaders();
    virtual void SetProgramCache(NiGPUProgramCache* pkCache, 
        NiGPUProgram::ProgramType eType);
    virtual NiGPUProgramCache* GetProgramCache(
        NiGPUProgram::ProgramType eType) const;
    virtual void LoadAllCaches();

    virtual void SetWorkingDirectory(const char* pcWorkingDir);

    unsigned int GetMaterialNodeLibraryCount() const;
    NiMaterialNodeLibrary* GetMaterialNodeLibrary(unsigned int uiIndex) const;

    enum ReturnCode
    {
        RC_SUCCESS                  = 0x0000,
        RC_COMPILE_FAILURE_PIXEL    = 0x0001,
        RC_COMPILE_FAILURE_GEOMETRY = 0x0002,
        RC_COMPILE_FAILURE_VERTEX   = 0x0004,
        RC_INVALID_MATERIAL         = 0x0008,
        RC_INVALID_SHADER           = 0x0010,
        RC_INVALID_CONSTANTMAP      = 0x0020
    };

    class ObjectResourceOffset : public NiMemObject
    {
    public:
        ObjectResourceOffset();
        NiGPUProgram::ProgramType m_eProgramType;
        NiShaderAttributeDesc::ObjectType m_eObjectType;
        unsigned int m_uiOffset;
    };

    class NIMAIN_ENTRY RenderPassDescriptor : public NiMemObject
    {
    public:
        RenderPassDescriptor();
        RenderPassDescriptor(RenderPassDescriptor&);
        RenderPassDescriptor& operator=(RenderPassDescriptor& kOther);
        void Initialize();

        NiGPUProgramDescriptor m_kVertexDesc;
        NiGPUProgramDescriptor m_kGeometryDesc;
        NiGPUProgramDescriptor m_kPixelDesc;
        bool m_bUsesNiRenderState;
        bool m_bAlphaOverride;
        bool m_bAlphaBlend;
        bool m_bResetObjectOffsets;
        NiTObjectSet<ObjectResourceOffset> m_kObjectOffsets;
        bool m_bUsePreviousSrcBlendMode;
        bool m_bUsePreviousDestBlendMode;
        NiAlphaProperty::AlphaFunction m_eSrcBlendMode;
        NiAlphaProperty::AlphaFunction m_eDestBlendMode; 
    };

    // Callbacks for handling shader creation failures
    typedef bool (*SHADERFALLBACKFUNC)(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    unsigned int AddShaderFallbackFunc(SHADERFALLBACKFUNC pfnFallback);
    bool RemoveShaderFallbackFunc(SHADERFALLBACKFUNC pfnFallback);
    bool RemoveShaderFallbackFunc(unsigned int uiIndex);
    void RemoveAllShaderFallbackFuncs();

    unsigned int GetShaderFallbackFuncCount() const;
    unsigned int GetShaderFallbackFuncArrayCount() const;
    unsigned int FindShaderFallbackFunc(SHADERFALLBACKFUNC pfnFallback) const;
    SHADERFALLBACKFUNC GetShaderFallbackFunc(unsigned int uiIndex) const;

    virtual void AddDefaultFallbacks();
    
    void SetMaxPassCount(unsigned int uiMaxPassCount);
    unsigned int GetMaxPassCount() const;

protected:
    NiFragmentMaterial(NiMaterialNodeLibrary* pkLibrary,
        const NiFixedString& kName, unsigned short usVertexVersion, 
        unsigned short usGeometryVersion, unsigned short usPixelVersion, 
        bool bAutoCreateCaches);

    enum
    {
        MATERIAL_HASH_BUCKET_COUNT = 37
    };

    virtual unsigned int GetHashCode(const NiMaterialDescriptor* pkDescriptor);
    virtual unsigned int GetHashCode(
        const NiGPUProgramDescriptor* pkDescriptor);
    NiShader* RetrieveFromHash(const NiMaterialDescriptor* pkDescriptor);
    void InsertIntoHash(NiShader* pkShader);
    void RemoveFromHash(NiShader* pkShader);

    bool CheckFailedShader(const NiMaterialDescriptor* pkDescriptor);
    void SetFailedShader(const NiMaterialDescriptor* pkDescriptor);

    void SetFailedGPUProgram(NiGPUProgram::ProgramType eProgramType,
        const NiGPUProgramDescriptor* pkDescriptor);

    virtual NiShaderAttributeDesc::AttributeType 
        GetAttributeTypeFromString(const char* pcValue);
    virtual NiFixedString GetStringFromAttributeType(
        NiShaderAttributeDesc::AttributeType eType);

    virtual bool GenerateDescriptor(const NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects,
        NiMaterialDescriptor& kMaterialDescriptor) = 0;

    virtual NiShader* GenerateShader(
        NiMaterialDescriptor& kMaterialDescriptor);
    virtual bool PrecacheGPUPrograms(
        NiMaterialDescriptor& kMaterialDescriptor);
    
    class Context : public NiMemObject
    {
    public:
        NiMaterialConfiguratorPtr m_spConfigurator;
        NiMaterialResourceProducerNodePtr m_spStatics;
        NiMaterialResourceProducerNodePtr m_spUniforms;
        NiMaterialResourceProducerNodePtr m_spInputs;
        NiMaterialResourceConsumerNodePtr m_spOutputs;
    };

    virtual ReturnCode GenerateShaderPrograms(
        NiGPUProgramDescriptor* pkVertexDesc, 
        NiGPUProgramDescriptor* pkGeometryDesc,
        NiGPUProgramDescriptor* pkPixelDesc,
        const NiTObjectSet<ObjectResourceOffset>& kObjectOffsets, 
        NiShader* pkShader, unsigned int uiRenderPass);
    virtual NiGPUProgram* GenerateVertexShaderProgram(
        NiGPUProgramDescriptor* pkDesc, 
        NiTObjectPtrSet<NiMaterialResourcePtr>& kUniforms);
    virtual NiGPUProgram* GenerateGeometryShaderProgram(
        NiGPUProgramDescriptor* pkDesc, 
        NiTObjectPtrSet<NiMaterialResourcePtr>& kUniforms);
    virtual NiGPUProgram* GeneratePixelShaderProgram(
        NiGPUProgramDescriptor* pkDesc, 
        NiTObjectPtrSet<NiMaterialResourcePtr>& kUniforms);
    virtual ReturnCode GenerateShaderDescArray(
        NiMaterialDescriptor* pkMaterialDescriptor,
        RenderPassDescriptor* pkRenderPasses, unsigned int uiMaxCount,
        unsigned int& uiCountAdded) = 0;

    virtual unsigned int VerifyShaderPrograms(NiGPUProgram* pkVertexShader,
        NiGPUProgram* pkGeometryShader, NiGPUProgram* pkPixelShader) = 0;

    virtual bool AddResourceToShaderConstantMap(NiShader* pkShader,
        NiShaderConstantMap* pkMap, NiMaterialResource* pkRes, 
        NiGPUProgramDescriptor* pkVertexDesc, 
        NiGPUProgramDescriptor* pkGeometryDesc, 
        NiGPUProgramDescriptor* pkPixelDesc, 
        const NiTObjectSet<ObjectResourceOffset>& kObjectOffsets, 
        unsigned int uiPass, NiGPUProgram::ProgramType eCurrentType);

    virtual NiShader* CreateShader(NiMaterialDescriptor* pkDesc) = 0;
    virtual bool SetupPackingRequirements(NiShader* pkShader,
        NiMaterialDescriptor* pkMaterialDescriptor,
        RenderPassDescriptor* pkRenderPasses, unsigned int uiMaxCount) = 0;

    virtual NiFixedString GenerateUVSetName(unsigned int uiIndex);
    virtual NiMaterialResource* InsertTextureSampler(
        Context& kContext, 
        const NiFixedString& kMapName,
        TextureMapSamplerType eSamplerType,
        unsigned int uiOccurance,
        NiShaderAttributeDesc::ObjectType eObjectType = 
            NiShaderAttributeDesc::OT_UNDEFINED);
   
    virtual NiMaterialResource* AddOutputPredefined(NiMaterialNode* pkNode,
        NiShaderConstantMap::DefinedMappings eMapping, 
        unsigned int uiNumRegisters = 1, unsigned int uiCount = 1,
        unsigned int uiExtraData = 0);
    virtual NiMaterialResource* AddOutputObject(NiMaterialNode* pkNode,
        NiShaderConstantMap::ObjectMappings eMapping, 
        NiShaderAttributeDesc::ObjectType eObjectType,
        unsigned int uiObjectCount, const char* pcVariableModifier = NULL,
        unsigned int uiCount = 1);
    virtual NiMaterialResource* AddOutputAttribute(NiMaterialNode* pkNode,
        const NiFixedString& kVariableName,
        NiShaderAttributeDesc::AttributeType eType, 
        unsigned int uiCount = 1);
    virtual NiMaterialResource* AddOutputGlobal(NiMaterialNode* pkNode,
        const NiFixedString& kVariableName,
        NiShaderAttributeDesc::AttributeType eType,
        unsigned int uiCount = 1);

    static unsigned int GetHighestObjectOffset(
        NiShaderAttributeDesc::ObjectType eObjectType, 
        RenderPassDescriptor* pkRenderPasses, unsigned int uiCount);

    virtual NiMaterialNode* GetAttachableNodeFromLibrary(const NiFixedString&
        kNodeName);

    NiTObjectPtrSet<NiMaterialNodeLibraryPtr> m_kLibraries;
    NiGPUProgramCachePtr m_aspProgramCaches[NiGPUProgram::PROGRAM_MAX];

    NiTObjectPtrSet<NiShaderPtr> 
        m_akShaderHashTable[MATERIAL_HASH_BUCKET_COUNT];
    NiTObjectSet<NiMaterialDescriptor> 
        m_akFailedShaderDescriptorHashTable[MATERIAL_HASH_BUCKET_COUNT];

    NiTPrimitiveArray<SHADERFALLBACKFUNC> m_kShaderFallbackFuncs;

    unsigned int m_uiMaxPassCount;

    unsigned short m_usVertexVersion;
    unsigned short m_usGeometryVersion;
    unsigned short m_usPixelVersion;

#if defined(_DEBUG)
    NiFixedString m_kDebugIdentifier;
#endif

    static bool ms_bLoadProgramCacheOnCreation;
    static bool ms_bAutoWriteProgramCache;
    static bool ms_bWriteDebugProgram;
    static bool ms_bLockProgramCache;
    static bool ms_bAutoCreateCache;
};

typedef NiPointer<NiFragmentMaterial> NiFragmentMaterialPtr;

#endif  //#ifndef NIFRAGMENTMATERIAL_H
