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

#ifndef NISTANDARDMATERIAL_H
#define NISTANDARDMATERIAL_H

#include "NiGeometry.h"
#include "NiShaderConstantMap.h"
#include "NiTSet.h"
#include "NiMaterialDescriptor.h"
#include "NiGPUProgramDescriptor.h"
#include "NiMaterialConfigurator.h"
#include "NiMaterialResourceProducerNode.h"
#include "NiMaterialResourceConsumerNode.h"
#include "NiMaterialFragmentNode.h"
#include "NiMaterialNodeLibrary.h"
#include "NiFragmentMaterial.h"

class NiStandardVertexProgramDescriptor;
class NiStandardPixelProgramDescriptor;
class NiStandardMaterialShader;

class NIMAIN_ENTRY NiStandardMaterial : public NiFragmentMaterial
{
    NiDeclareRTTI;
public:
    enum TransformType
    {
        TRANSFORM_DEFAULT = 0,
        TRANSFORM_SKINNED,
        TRANSFORM_SKINNED_NOPALETTE,
        TRANSFORM_MAX
    };

    enum NormalType
    {
        NORMAL_NONE = 0,
        NORMAL_ONLY,
        NORMAL_NBT,
        NORMAL_MAX
    };

    enum Fog
    {
        FOG_NONE = 0,
        FOG_LINEAR,
        FOG_SQUARED,
        FOG_MAX
    };

    enum TexGenInput
    {
        TEX_IN_TEXCOORD = 0,
        TEX_IN_WORLD_POS,
        TEX_IN_WORLD_NORM,
        TEX_IN_WORLD_REFLECT,
        TEX_IN_MAX
    };

    enum TexGenOutput
    {
        TEX_OUT_PASSTHRU = 0,
        TEX_OUT_TRANSFORM,
        TEX_OUT_PARALLEL_PROJECT,
        TEX_OUT_PERSPECTIVE_PROJECT,
        TEX_OUT_MAX
    };

    enum VertexColorType
    {
        VC_NONE = 0,
        VC_EXISTS,
        VC_MAX
    };

    enum AmbDiffEmissiveEnum
    {
        ADE_IGNORE = 0,
        ADE_EMISSIVE,
        ADE_AMB_DIFF,
        ADE_MAX
    };

    enum LightingModeEnum
    {
        LIGHTING_E = 0,
        LIGHTING_E_A_D,
        LIGHTING_MAX
    };

    enum ApplyMode
    {
        APPLY_REPLACE = 0,
        APPLY_MODULATE,
        APPLY_MAX
    };

    enum NormalMapType
    {
        NORMAL_MAP_STANDARD = 0,
        NORMAL_MAP_DXN,
        NORMAL_MAP_DXT5,
        NORMAL_MAP_MAX
    };

    enum LightType
    {
        LIGHT_DIR = 0,
        LIGHT_POINT,
        LIGHT_SPOT,
        LIGHT_MAX
    };

    enum TexEffectType
    {
        TEXEFFECT_NONE = 0,
        TEXEFFECT_WORLD_PARALLEL,
        TEXEFFECT_WORLD_PERSPECTIVE,
        TEXEFFECT_SPHERICAL,
        TEXEFFECT_DIFFUSE_CUBE,
        TEXEFFECT_SPECULAR_CUBE,
        TEXEFFECT_MAX
    };

    enum TextureMap
    {
        MAP_PARALLAX = 0,
        MAP_BASE,
        MAP_NORMAL,
        MAP_DARK,
        MAP_DETAIL,
        MAP_BUMP,
        MAP_GLOSS,
        MAP_GLOW,
        MAP_CUSTOM00,
        MAP_CUSTOM01,
        MAP_CUSTOM02,
        MAP_CUSTOM03,
        MAP_CUSTOM04,
        MAP_DECAL00,
        MAP_DECAL01,
        MAP_DECAL02,
        MAP_ENV,
        MAP_PROJECTED_LIGHT00,
        MAP_PROJECTED_LIGHT01,
        MAP_PROJECTED_LIGHT02,
        MAP_PROJECTED_SHADOW00,
        MAP_PROJECTED_SHADOW01,
        MAP_PROJECTED_SHADOW02,
        MAP_DIRSHADOW00,
        MAP_DIRSHADOW01,
        MAP_DIRSHADOW02,
        MAP_DIRSHADOW03,
        MAP_DIRSHADOW04,
        MAP_DIRSHADOW05,
        MAP_DIRSHADOW06,
        MAP_DIRSHADOW07,
        MAP_POINTSHADOW00,
        MAP_POINTSHADOW01,
        MAP_POINTSHADOW02,
        MAP_POINTSHADOW03,
        MAP_POINTSHADOW04,
        MAP_POINTSHADOW05,
        MAP_POINTSHADOW06,
        MAP_POINTSHADOW07,
        MAP_SPOTSHADOW00,
        MAP_SPOTSHADOW01,
        MAP_SPOTSHADOW02,
        MAP_SPOTSHADOW03,
        MAP_SPOTSHADOW04,
        MAP_SPOTSHADOW05,
        MAP_SPOTSHADOW06,
        MAP_SPOTSHADOW07,
        MAP_MAX
    };

    enum TextureMapSampleType
    {
        TEXTURE_SAMPLE_RGBA = 0,
        TEXTURE_SAMPLE_RGB,
        TEXTURE_SAMPLE_PROJ_RGB,
        TEXTURE_SAMPLE_CUBE_RGB,
        TEXTURE_SAMPLE_A,
        TEXTURE_SAMPLE_MAX
    };

    enum TextureMapApplyType
    {
        TEXTURE_RGB_APPLY_MULTIPLY = 0,
        TEXTURE_RGB_APPLY_ADD,
        TEXTURE_RGB_APPLY_LERP,
        TEXTURE_RGB_APPLY_NOOP,
        TEXTURE_MAX
    };

    enum 
    {
        STANDARD_PIPE_MAX_UVS_FOR_TEXTURES = 12,
        STANDARD_PIPE_MAX_TEXTURE_MAPS  = 16,
        STANDARD_PIPE_MAX_DECAL_MAPS = 3,
        STANDARD_PIPE_MAX_SHADER_MAPS = 5,
        STANDARD_PIPE_MAX_LIGHTS = 8,
        STANDARD_PIPE_CUSTOM_MAPS = 5,
        MATERIAL_HASH_BUCKET_COUNT = 37,
        VERTEX_VERSION = 16,
        GEOMETRY_VERSION = 0,
        PIXEL_VERSION = 26
    };

    NiStandardMaterial(NiMaterialNodeLibrary* pkLibrary, 
        bool bAutoCreateCaches = true);

    void SetForcePerPixelLighting(bool bForce);
    bool GetForcePerPixelLighting() const;

    void SetSaturateShading(bool bSaturate);
    bool GetSaturateShading() const;

    void SetSaturateTextures(bool bSaturate);
    bool GetSaturateTextures() const;

    virtual void AddDefaultFallbacks();

    // Fallback techniques
    static bool SplitPerPixelLights(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    static bool DropShadowMaps(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    static bool DropShadowMapsThenSplitPerPixelLights(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    static bool SplitPerVertexLights(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    static bool SplitTextureMaps(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    static bool DropShadowMapsThenSplitTextureMaps(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    static bool DropParallaxMap(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    static bool DropParallaxMapThenSplitLights(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    static bool DropShadowMapsThenDropParallaxMapThenSplitLights(
        NiMaterialDescriptor* pkMaterialDescriptor, ReturnCode eFailedRC,
        unsigned int uiFailedPass, RenderPassDescriptor* pkRenderPasses, 
        unsigned int uiMaxCount, unsigned int& uiCount, 
        unsigned int& uiFunctionData);

    virtual bool GetVertexInputSemantics(NiGeometry* pkGeometry, 
        unsigned int uiMaterialExtraData,
        NiShaderDeclaration::ShaderRegisterEntry* pakSemantics);

protected:
    // Derived classes should call this constructor to register their material
    // name
    NiStandardMaterial(const NiFixedString& kName, 
        NiMaterialNodeLibrary* pkLibrary, unsigned int uiVertexVersion,
        unsigned int uiGeometryVersion, unsigned int uiPixelVersion, 
        bool bAutoCreateCaches);
    NiStandardMaterial(const NiFixedString& kName, 
        NiMaterialNodeLibrary* pkLibrary, bool bAutoCreateCaches);
        
    virtual NiShader* CreateShader(NiMaterialDescriptor* pkDesc);

    virtual bool SetupPackingRequirements(NiShader* pkShader,
        NiMaterialDescriptor* pkMaterialDescriptor,
        RenderPassDescriptor* pkRenderPasses, unsigned int uiCount);

    virtual bool GenerateDescriptor(const NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects,
        NiMaterialDescriptor& kMaterialDesc);
    virtual bool AddResourceToShaderConstantMap(NiShader* pkShader,
        NiShaderConstantMap* pkMap, NiMaterialResource* pkRes, 
        NiGPUProgramDescriptor* pkVertexDesc, 
        NiGPUProgramDescriptor* pkGeometryDesc, 
        NiGPUProgramDescriptor* pkPixelDesc, 
        const NiTObjectSet<ObjectResourceOffset>& kObjectOffsets, 
        unsigned int uiPass, NiGPUProgram::ProgramType eCurrentType);

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
        unsigned int& uiCountAdded);

    virtual unsigned int VerifyShaderPrograms(NiGPUProgram* pkVertexShader,
        NiGPUProgram* pkGeometryShader, NiGPUProgram* pkPixelShader);

    virtual bool HandlePositionFragment(Context& kContext,
        TransformType eTransType, 
        NiMaterialResource*& pkVertWorldPos,
        NiMaterialResource*& pkWorldMatrix);
    virtual bool HandleNormalFragment(Context& kContext,
        NormalType eNormType,
        NiMaterialResource* pkWorldMatrix,
        NiMaterialResource*& pkVertWorldNorm,
        NiMaterialResource*& pkVertWorldBiNorm,
        NiMaterialResource*& pkVertWorldTangent);
    virtual bool HandlePostWorldTransform(Context& kContext,
        NiGPUProgramDescriptor* pkVertexProgram,
        NiMaterialResource*& pkVertWorldPos,
        NiMaterialResource*& pkVertWorldNorm);
    virtual bool HandleViewProjectionFragment(Context& kContext, 
        bool bForceViewPos, NiMaterialResource* pkVertWorldPos,
        NiMaterialResource*& pkVertOutProjectedPos,
        NiMaterialResource*& pkVertOutViewPos);
    virtual bool HandleViewVectorFragment(Context& kContext,
        NiMaterialResource* pkWorldPos, NiMaterialResource* pkWorldNorm,
        NiMaterialResource* pkWorldBinorm, NiMaterialResource* pkWorldTangent,
        bool bComputeTangent, NiMaterialResource*& pkWorldViewVector, 
        NiMaterialResource*& pkTangentViewVector);
    virtual bool HandleReflectionVectorFragment(Context& kContext,
        NiMaterialResource* pkWorldNorm, bool bNormalizeNormal, 
        NiMaterialResource* pkWorldViewVector, 
        NiMaterialResource*& pkWorldReflectionVector);
    virtual bool SetupTransformPipeline(Context& kContext,
        NiMaterialResource* pkVertOutProjPos,
        NiStandardVertexProgramDescriptor* pkVertDesc, bool bForceView,
        bool bForceViewPos, NiMaterialResource*& pkWorldPos, 
        NiMaterialResource*& pkViewPos,  NiMaterialResource*& pkWorldNormal,
        NiMaterialResource*& pkWorldView);
    virtual bool HandleInitialSpecAmbDiffEmissiveColor(Context& kContext,
        bool bSpecularOn, AmbDiffEmissiveEnum eADF, 
        LightingModeEnum eLightingMode,
        NiMaterialResource*& pkDiffuseColorRes,
        NiMaterialResource*& pkSpecularColorRes,
        NiMaterialResource*& pkSpecularPowerRes,
        NiMaterialResource*& pkAmbientColorRes,
        NiMaterialResource*& pkEmissiveColorRes,
        NiMaterialResource*& pkOpacityRes);
    virtual bool HandleCalculateFog(Context& kContext,
        NiMaterialResource* pkViewPos, Fog eFogType);
    virtual bool HandleTextureUVSets(Context& kContext,
        NiStandardVertexProgramDescriptor* pkVertexDesc,
        NiMaterialResource** apkOutputUVResources, size_t stOutputUVMax,
        unsigned int& uiNextUVIndex);
    virtual bool HandleTextureEffectUVSets(Context& kContext,
        NiMaterialResource** apkOutputUVResources, size_t stOutputUVMax,
        NiMaterialResource* pkWorldPos, NiMaterialResource* pkWorldNorm, 
        NiMaterialResource* pkWorldReflection, 
        TexEffectType eEnvironmentMapType, unsigned int uiProjectedLightCount, 
        unsigned int uiProjectedLightTypes, 
        unsigned int uiProjectedShadowCount, 
        unsigned int uiProjectedShadowTypes,
        unsigned int& uiNextUVIndex);

    virtual bool HandleTextureEffectUVSet(Context& kContext,
        NiMaterialResource* pkWorldPos,
        NiMaterialResource* pkWorldNorm, 
        NiMaterialResource* pkWorldReflection,
        NiShaderAttributeDesc::ObjectType eObjType,
        TexEffectType eEffectType, unsigned int uiOccurance,
        NiMaterialResource*& pkUVResource);
    virtual bool HandlePreLightTextureApplication(Context& kContext,
        NiStandardPixelProgramDescriptor* pkPixelDesc, 
        NiMaterialResource*& pkWorldPos,
        NiMaterialResource*& pkWorldNormal,
        NiMaterialResource*& pkWorldBinormal,
        NiMaterialResource*& pkWorldTangent,
        NiMaterialResource*& pkWorldViewVector,
        NiMaterialResource*& pkTangentViewVector,
        NiMaterialResource*& pkMatDiffuseColor,
        NiMaterialResource*& pkMatSpecularColor,
        NiMaterialResource*& pkMatSpecularPower,
        NiMaterialResource*& pkMatGlossiness,
        NiMaterialResource*& pkMatAmbientColor,
        NiMaterialResource*& pkMatEmissiveColor,
        NiMaterialResource*& pkOpacityAccum,
        NiMaterialResource*& pkDiffuseTexAccum,
        NiMaterialResource*& pkSpecularTexAccum,
        unsigned int& uiTexturesApplied,
        NiMaterialResource** apkUVSets,
        unsigned int uiNumStandardUVs,
        unsigned int uiNumTexEffectUVs);
    virtual bool HandleGouraudCoefficients(Context& kContext,
        NiMaterialResource* pkMatEmissive, 
        NiMaterialResource* pkMatDiffuse, NiMaterialResource* pkMatAmbient, 
        NiMaterialResource* pkMatSpecular, 
        NiMaterialResource* pkLightSpecularAccum,
        NiMaterialResource* pkLightDiffuseAccum, 
        NiMaterialResource* pkLightAmbientAccum,
        NiMaterialResource*& pkDiffuseCoeff, 
        NiMaterialResource*& pkSpecularCoeff);
    virtual bool HandlePostLightTextureApplication(Context& kContext,
        NiStandardPixelProgramDescriptor* pkPixelDesc,
        NiMaterialResource*& pkWorldNormal, 
        NiMaterialResource* pkViewVector, 
        NiMaterialResource*& pkMatOpacity,
        NiMaterialResource*& pkDiffuseAccum,
        NiMaterialResource*& pkSpecularAccum,
        NiMaterialResource* pkGlossiness,
        unsigned int& uiTexturesApplied,
        NiMaterialResource** apkUVSets,
        unsigned int uiNumStandardUVs,
        unsigned int uiNumTexEffectUVs);
    virtual bool HandleTexGen(Context& kContext, 
        NiMaterialResource* pkInputUVResource,
        NiMaterialResource*& pkOutputUVResource,
        const NiFixedString& kMapName, unsigned int uiExtra,
        NiStandardMaterial::TexGenOutput eOutputType);
    virtual bool HandleParallaxMap(Context& kContext, 
        NiMaterialResource*& pkUVSet, NiMaterialResource* pkTangentViewVector);
    virtual bool HandleBaseMap(Context& kContext, NiMaterialResource* pkUVSet,
        NiMaterialResource*& pkDiffuseColorAccum, 
        NiMaterialResource*& pkOpacity, bool bOpacityOnly);
    virtual bool HandleNormalMap(Context& kContext, 
        NiMaterialResource* pkUVSet, NormalMapType eType, 
        NiMaterialResource*& pkWorldNormal, 
        NiMaterialResource* pkWorldBinormal, 
        NiMaterialResource* pkWorldTangent);
    virtual bool HandleDarkMap(Context& kContext, NiMaterialResource* pkUVSet,
        NiMaterialResource*& pkDiffuseColorRes, 
        NiMaterialResource*& pkSpecularTexAccum);
    virtual bool HandleDetailMap(Context& kContext, 
        NiMaterialResource* pkUVSet, NiMaterialResource*& pkDiffuseColorRes,
        NiMaterialResource*& pkSpecularTexAccum);
    virtual bool HandleGlossMap(Context& kContext, NiMaterialResource* pkUVSet,
        NiMaterialResource*& pkGlossiness);
    virtual bool HandleBumpMap(Context& kContext, NiMaterialResource* pkUVSet,
        NiMaterialResource*& pkBumpOffset);
    virtual bool HandleGlowMap(Context& kContext, NiMaterialResource* pkUVSet,
        NiMaterialResource*& pkEmissiveColorRes,
        NiMaterialResource*& pkSpecularTexAccum);
    virtual bool HandleCustomMaps(Context& kContext, 
        NiStandardPixelProgramDescriptor* pkPixDesc, 
        unsigned int& uiWhichTexture, NiMaterialResource** apkUVSets,
        unsigned int uiNumStandardUVs, NiMaterialResource*& pkDiffuseColorRes,
        NiMaterialResource*& pkSpecularColorRes,
        NiMaterialResource*& pkSpecularPowerRes,
        NiMaterialResource*& pkAmbientColorRes,
        NiMaterialResource*& pkEmissiveColorRes,
        NiMaterialResource*& pkDiffuseAccum, 
        NiMaterialResource*& pkSpecularAccum);
    virtual bool HandleEnvMap(Context& kContext, 
        NiStandardMaterial::TexEffectType eTexEffect,
        NiMaterialResource* pkUVResource,
        NiMaterialResource*& pkDiffuseAccum, 
        NiMaterialResource*& pkSpecularAccum, 
        NiMaterialResource* pkBumpOffset,
        NiMaterialResource* pkGlossiness);
    virtual bool HandleDecalMap(Context& kContext, NiMaterialResource* pkUVSet,
        unsigned int uiIdx, NiMaterialResource*& pkDiffuseColorRes,
        NiMaterialResource*& pkSpecularTexAccum);
    virtual bool HandleProjLightMap(Context& kContext, 
        NiMaterialResource* pkUVSet, unsigned int uiIdx, bool bPerspective,
        bool bClipped, NiMaterialResource* pkWorldPos,
        NiMaterialResource*& pkDiffuseAccum,
        NiMaterialResource*& pkSpecularTexAccum);
    virtual bool HandleProjShadowMap(Context& kContext,
        NiMaterialResource* pkUVSet,  unsigned int uiIdx, bool bPerspective, 
        bool bClipped, NiMaterialResource* pkWorldPos,
        NiMaterialResource*& pkDiffuseColorRes,
        NiMaterialResource*& pkSpecularTexAccum);

    virtual bool HandleLighting(Context& kContext, 
        bool bSpecular, unsigned int uiNumPoint, unsigned int uiNumDirectional,
        unsigned int uiNumSpot, unsigned int uiShadowBitfield,
        unsigned int uiShadowTechnique,
        NiMaterialResource* pkWorldPos, NiMaterialResource* pkWorldNorm,
        NiMaterialResource* pkViewVector, 
        NiMaterialResource* pkSpecularPower,
        NiMaterialResource*& pkAmbientAccum,
        NiMaterialResource*& pkDiffuseAccum,
        NiMaterialResource*& pkSpecularAccum);

    virtual bool HandleShadow(Context& kContext, unsigned int uiLight, 
        LightType eType, unsigned int uiLightByType, 
        NiMaterialResource* pkWorldPos, NiMaterialResource* pkWorldNorm,
        unsigned int uiShadowTechnique, NiMaterialResource*& pkShadow);

    virtual bool HandleLight(Context& kContext, unsigned int uiLight,
        LightType eType, unsigned int uiLightByType, 
        NiMaterialResource* pkWorldPos, 
        NiMaterialResource* pkWorldNorm, NiMaterialResource* pkViewVector, 
        NiMaterialResource* pkSpecularPower,
        NiMaterialResource*& pkAmbientAccum,
        NiMaterialResource*& pkDiffuseAccum,
        NiMaterialResource*& pkSpecularAccum,
        bool bSpecular, NiMaterialResource* pkShadow);

    virtual bool HandleApplyFog(Context& kContext, 
        NiStandardPixelProgramDescriptor* pkPixDesc,
        NiMaterialResource* pkUnfoggedColor, 
        NiMaterialResource*& pkFogOutput);

    virtual bool HandleAlphaTest(Context& kContext,
        NiStandardPixelProgramDescriptor* pkPixDesc,
        NiMaterialResource* pkAlphaTestInput);

    virtual NiFixedString GenerateUVSetName(unsigned int uiIndex);

    virtual NiMaterialResource* AddOutputPredefined(NiMaterialNode* pkNode,
        NiShaderConstantMap::DefinedMappings eMapping, 
        unsigned int uiNumRegisters = 1, unsigned int uiCount = 1,
        unsigned int uiExtraData = 0);
    virtual NiMaterialResource* AddOutputObject(NiMaterialNode* pkNode,
        NiShaderConstantMap::ObjectMappings eMapping, 
        NiShaderAttributeDesc::ObjectType eObjectType,
        unsigned int uiObjectCount, const char* pcVariableModifier = NULL,
        unsigned int uiCount = 1);

    virtual bool NormalizeVector(Context& kContext, 
        NiMaterialResource*& pkVector);
    virtual bool AddVector(Context& kContext, 
        NiMaterialResource* pkVector1, NiMaterialResource* pkVector2,
        NiMaterialResource*& pkValue);
    virtual bool MultiplyVector(Context& kContext, 
        NiMaterialResource* pkVector1, NiMaterialResource* pkVector2,
        NiMaterialResource*& pkValue);
    virtual bool MultiplyAddVector(Context& kContext, 
        NiMaterialResource* pkVector1, NiMaterialResource* pkVector2,
        NiMaterialResource* pkVector3, NiMaterialResource*& pkValue);
    virtual bool LerpVector(Context& kContext, 
        NiMaterialResource* pkVector1, NiMaterialResource* pkVector2,
        NiMaterialResource* pkLerpAmount, NiMaterialResource*& pkValue);
    virtual bool ScaleVector(Context& kContext, 
        NiMaterialResource* pkVector, NiMaterialResource* pkScale,
        NiMaterialResource*& pkValue);
    virtual bool SaturateVector(Context& kContext, 
        NiMaterialResource* pkVector, NiMaterialResource*& pkValue);
    virtual bool MultiplyScalarSatAddVector(Context& kContext, 
        NiMaterialResource* pkVector1, NiMaterialResource* pkVector2,
        NiMaterialResource* pkScale, NiMaterialResource*& pkValue);
    virtual bool InsertTexture(Context& kContext, TextureMap eMap, 
        unsigned int uiOccurance, TextureMapApplyType eApplyType, 
        TextureMapSampleType eSample, NiMaterialResource* pkUV,
        NiMaterialResource* pkInputColor, 
        NiMaterialResource* pkSampledRGBScalar,
        NiMaterialResource*& pkOutputColor,
        NiMaterialResource*& pkOutputOpacity);
    virtual bool InsertTexture(Context& kContext, 
        const NiFixedString& kSamplerName, unsigned int uiOccurance, 
        TextureMapApplyType eApplyType, TextureMapSampleType eSamplerType, 
        NiMaterialResource* pkUV, NiMaterialResource* pkInputColor, 
        NiMaterialResource* pkSampledRGBScalar,
        NiMaterialResource*& pkOutputColor, 
        NiMaterialResource*& pkOutputOpacity);
    virtual bool SplitColorAndOpacity(Context& kContext, 
        NiMaterialResource* pkColorAndOpacity, NiMaterialResource*& pkColor,
        NiMaterialResource*& pkOpacity);
    virtual bool SplitRGBA(Context& kContext, 
        NiMaterialResource* pkColorAndOpacity, NiMaterialResource*& pkRed,
        NiMaterialResource*& pkGreen, NiMaterialResource*& pkBlue,
        NiMaterialResource*& pkAlpha);
    virtual bool TypeCastBind(Context& kContext, 
        NiMaterialResource* pkSrc, NiMaterialResource* pkDest);

    // Utility functions 
    static unsigned int FindUVSetIndexForTextureIndex(
        unsigned int uiTexture, NiStandardPixelProgramDescriptor* pkPixDesc);
    static NiMaterialResource* FindUVSetIndexForTextureEnum( 
        TextureMap eMap, NiStandardPixelProgramDescriptor* pkPixDesc,
        NiMaterialResource** apkUVSets);
    static unsigned int FindUVSetIndexForTextureEnum(TextureMap eMap, 
        NiStandardPixelProgramDescriptor* pkPixDesc);
    static int MapIndexFromTextureEnum(TextureMap eMap,
        NiStandardPixelProgramDescriptor* pkPixDesc);
    static bool GetTextureNameFromTextureEnum(TextureMap eMap,
        NiFixedString& kString, unsigned int& uiOccurance);
    static bool GetTextureTransformMappingFromEnum(TextureMap eMap, 
        NiShaderConstantMap::DefinedMappings& eMapping, 
        unsigned int& uiOccurance);
    static bool RemoveTextureMap(TextureMap eMap, 
        NiStandardVertexProgramDescriptor* pkVertDesc, 
        NiStandardPixelProgramDescriptor* pkPixDesc);
    static bool GenerateVertexOutputUVArrays(
        unsigned int* puiInputUVSets, TexGenOutput* peTexGenOutputs,
        unsigned int* puiOutputUVSetsByTexIndex,
        unsigned int& uiTextureCount, 
        unsigned int& uiOutputUVCount,
        NiStandardVertexProgramDescriptor* pkVertexDesc, 
        NiStandardPixelProgramDescriptor* pkPixelDesc, 
        unsigned int uiMaxTextureCount);
    static bool IsMapEnabled(TextureMap eMap, 
        NiStandardPixelProgramDescriptor* pkPixelDesc);
    static bool SetMapEnabled(TextureMap eMap, bool bEnabled,
        NiStandardPixelProgramDescriptor* pkPixelDesc);

    static bool AssignTextureCoordinates(const unsigned int* puiUVSets,
        const TexGenOutput* peTexGenOutputs, const unsigned int uiTextureCount,
        NiStandardVertexProgramDescriptor* pkVertexDesc, 
        NiStandardPixelProgramDescriptor* pkPixelDesc);

    NiFixedString m_kDescriptorName;

    bool m_bForcePerPixelLighting;
    bool m_bSaturateShading;
    bool m_bSaturateTextures;
};

typedef NiPointer<NiStandardMaterial> NiStandardMaterialPtr;

#endif  //#ifndef NISTANDARDMATERIAL_H
