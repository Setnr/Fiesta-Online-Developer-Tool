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

#ifndef NIDX9RENDERER_H
#define NIDX9RENDERER_H

#include "NiDX9Headers.h"
#include "NiDX9RendererLibType.h"

#include "NiGeometryBufferData.h"
#include "NiDX9Error.h"
#include "NiDX9LightManager.h"
#include "NiDX9PixelFormat.h"
#include "NiDX9RendererSDM.h"
#include "NiDX9RenderState.h"
#include "NiDX92DBufferData.h"
#include "NiDX9ShaderDeclaration.h"
#include "NiD3DDefaultShader.h"
#include "NiD3DShaderInterface.h"
#include "NiD3DShaderLibraryVersion.h"
#include "NiD3DUtils.h"
#include "NiDX9ShadowWriteShader.h"

#include <NiUniversalTypes.h>
#include <NiAccumulator.h>
#include <NiBool.h>
#include <NiColor.h>
#include <NiTexture.h>
#include <NiDynamicEffect.h>
#include <NiDynamicEffectState.h>
#include <NiPropertyState.h>
#include <NiRect.h>
#include <NiRenderer.h>
#include <NiRenderTargetGroup.h>
#include <NiSkinInstance.h>
#include <NiTPointerList.h>

class NiAVObject;
class NiBound;
class NiPoint3;
class NiPoint2;
class NiPlane;
class NiCamera;
class NiFrustum;
class NiGeometryData;
class NiTriShapeData;
class NiTriStripsData;
class NiParticlesData;
class NiLinesData;
class NiTexture;
NiSmartPointer(NiPixelData);
NiSmartPointer(NiDX92DBufferData);
NiSmartPointer(NiPersistentSrcTextureRendererData);

//  DX9 Specific classes
class NiDX9AdapterDesc;
class NiDX9DeviceDesc;
class NiDX9SystemDesc;
class NiDX9VertexBufferManager;
class NiDX9IndexBufferManager;
class NiDX9TextureManager;
class NiDX9TextureData;
class NiDX9SourceTextureData;
class NiDX9RenderedTextureData;
class NiDX9RenderedCubeMapData;
class NiDX9DynamicTextureData;
class NiDX9PersistentSrcTextureRendererData;
class NiDynamicGeometryGroup;
class NiUnsharedGeometryGroup;
class NiD3DGeometryGroupManager;
class NiVBBlock;

// DX9 ID Number (Must be unique to all renderers).
enum { NI_DX9_RENDER_ID = 8 };

typedef LPDIRECT3D9 (CALLBACK *NILPD3DCREATE) (unsigned int);

class NIDX9RENDERER_ENTRY NiDX9Renderer : public NiRenderer
{
    NiDeclareRTTI;

public:
    // Flags for creation options.
    enum FlagType
    {
        USE_NOFLAGS                      = 0x00000000,
        USE_MANUALDEPTHSTENCIL           = 0x00000001,
        USE_FULLSCREEN                   = 0x00000004,
        USE_STENCIL                      = 0x00000008,
        USE_16BITBUFFERS                 = 0x00000010,
        USE_MULTITHREADED                = 0x00000020,
        USE_FPU_PRESERVE                 = 0x00000040
    };

    // Device Description
    enum DeviceDesc
    {
        DEVDESC_PURE,
        DEVDESC_HAL_HWVERTEX,
        DEVDESC_HAL_MIXEDVERTEX,
        DEVDESC_HAL_SWVERTEX,
        DEVDESC_REF,
        DEVDESC_REF_HWVERTEX,
        DEVDESC_REF_MIXEDVERTEX,
        DEVDESC_NUM
    };

    //  DX9 supported frame buffer formats
    enum FrameBufferFormat
    {
        FBFMT_UNKNOWN = 0,
        FBFMT_R8G8B8,
        FBFMT_A8R8G8B8,
        FBFMT_X8R8G8B8,
        FBFMT_R5G6B5,
        FBFMT_X1R5G5B5,
        FBFMT_A1R5G5B5,
        FBFMT_A4R4G4B4,
        FBFMT_R3G3B2,
        FBFMT_A8,
        FBFMT_A8R3G3B2,
        FBFMT_X4R4G4B4,
        FBFMT_R16F,
        FBFMT_G16R16F,
        FBFMT_A16B16G16R16F,
        FBFMT_R32F,
        FBFMT_G32R32F,
        FBFMT_A32B32G32R32F,
        FBFMT_NUM
    };

    //  DX9 supported depth/stencil buffer formats
    enum DepthStencilFormat
    {
        DSFMT_UNKNOWN      =   0,
        DSFMT_D16_LOCKABLE =  70,
        DSFMT_D32          =  71,
        DSFMT_D15S1        =  73,
        DSFMT_D24S8        =  75,
        DSFMT_D16          =  80,
        DSFMT_D24X8        =  77,
        DSFMT_D24X4S4      =  79,
    };

    // Presentation Interval
    enum PresentationInterval
    {
        PRESENT_INTERVAL_IMMEDIATE = 0,
        PRESENT_INTERVAL_ONE = 1,
        PRESENT_INTERVAL_TWO = 2,
        PRESENT_INTERVAL_THREE = 3,
        PRESENT_INTERVAL_FOUR = 4,
        PRESENT_INTERVAL_NUM
    };

    // Swap Effect
    enum SwapEffect
    {
        SWAPEFFECT_DEFAULT,
        SWAPEFFECT_DISCARD,
        SWAPEFFECT_FLIP,
        SWAPEFFECT_COPY,
        SWAPEFFECT_NUM
    };

    enum FramebufferMode
    {
        FBMODE_DEFAULT,
        FBMODE_LOCKABLE,
        FBMODE_MULTISAMPLES_2           = 0x00010000,
        FBMODE_MULTISAMPLES_3           = 0x00020000,
        FBMODE_MULTISAMPLES_4           = 0x00030000,
        FBMODE_MULTISAMPLES_5           = 0x00040000,
        FBMODE_MULTISAMPLES_6           = 0x00050000,
        FBMODE_MULTISAMPLES_7           = 0x00060000,
        FBMODE_MULTISAMPLES_8           = 0x00070000,
        FBMODE_MULTISAMPLES_9           = 0x00080000,
        FBMODE_MULTISAMPLES_10          = 0x00090000,
        FBMODE_MULTISAMPLES_11          = 0x000a0000,
        FBMODE_MULTISAMPLES_12          = 0x000b0000,
        FBMODE_MULTISAMPLES_13          = 0x000c0000,
        FBMODE_MULTISAMPLES_14          = 0x000d0000,
        FBMODE_MULTISAMPLES_15          = 0x000e0000,
        FBMODE_MULTISAMPLES_16          = 0x000f0000,
        FBMODE_MULTISAMPLES_NONMASKABLE = 0x80000000,
        FBMODE_QUALITY_MASK             = 0x0000FFFF,
        FBMODE_NUM = 18
    };

    // Refresh Rate
    enum RefreshRate
    {
        REFRESHRATE_DEFAULT   = 0
    };

    enum TexFormat
    {
        TEX_RGB555,
        TEX_RGB565,
        TEX_RGB888,
        TEX_RGBA5551,
        TEX_RGBA4444,
        TEX_RGBA8888,
        TEX_PAL8,
        TEX_PALA8,
        TEX_DXT1,
        TEX_DXT3,
        TEX_DXT5,
        TEX_BUMP88,
        TEX_BUMPLUMA556,
        TEX_BUMPLUMA888,
        TEX_L8,
        TEX_A8,
        TEX_R16F,
        TEX_RG32F,
        TEX_RGBA64F,
        TEX_R32F,
        TEX_RG64F,
        TEX_RGBA128F,
        TEX_DEFAULT,
        TEX_NUM         = TEX_DEFAULT
    };

    enum TexUsage
    {
        TEXUSE_TEX,
        TEXUSE_RENDERED_TEX,
        TEXUSE_CUBE,
        TEXUSE_RENDERED_CUBE,
        TEXUSE_NUM
    };

    enum FramebufferStatus
    {
        FBSTATUS_OK,
        FBSTATUS_NOT_FOUND,
        FBSTATUS_NEEDS_RESET
    };

    enum RecreateStatus
    {
        RECREATESTATUS_FAILED,
        RECREATESTATUS_RESTORED,
        RECREATESTATUS_OK,
    };

    enum
    {
        HW_BONE_LIMIT = 4
    };

    virtual ~NiDX9Renderer();

    // Renderer creation
    static NiDX9Renderer* Create(unsigned int uiWidth, unsigned int uiHeight,
        unsigned int uiUseFlags, NiWindowRef kWndDevice, 
        NiWindowRef kWndFocus, unsigned int uiAdapter = D3DADAPTER_DEFAULT, 
        DeviceDesc eDesc = DEVDESC_PURE,
        FrameBufferFormat eFBFormat = FBFMT_UNKNOWN, 
        DepthStencilFormat eDSFormat = DSFMT_UNKNOWN, 
        PresentationInterval ePresentationInterval = PRESENT_INTERVAL_ONE,
        SwapEffect eSwapEffect = SWAPEFFECT_DEFAULT,
        unsigned int uiFBMode = FBMODE_DEFAULT,
        unsigned int uiBackBufferCount = 1, 
        unsigned int uiRefreshRate = REFRESHRATE_DEFAULT);

    void GetCreationParameters(unsigned int& uiWidth, unsigned int& uiHeight,
        unsigned int& uiUseFlags, NiWindowRef& kWndDevice, 
        NiWindowRef& kWndFocus, unsigned int& uiAdapter, DeviceDesc& eDesc, 
        FrameBufferFormat& eFBFormat, DepthStencilFormat& eDSFormat, 
        PresentationInterval& ePresentationInterval, SwapEffect& eSwapEffect,
        unsigned int& uiFBMode, unsigned int& uiBackBufferCount, 
        unsigned int& uiRefreshRate) const;

    // Renderer recreation
    RecreateStatus Recreate(unsigned int uiWidth, unsigned int uiHeight,
        unsigned int uiUseFlags, NiWindowRef kWndDevice = 0, 
        FrameBufferFormat eFBFormat = FBFMT_UNKNOWN, 
        DepthStencilFormat eDSFormat = DSFMT_UNKNOWN, 
        PresentationInterval ePresentationInterval = PRESENT_INTERVAL_ONE,
        SwapEffect eSwapEffect = SWAPEFFECT_DEFAULT,
        unsigned int uiFBMode = FBMODE_DEFAULT,
        unsigned int uiBackBufferCount = 1, 
        unsigned int uiRefreshRate = REFRESHRATE_DEFAULT);

    RecreateStatus Recreate(unsigned int uiWidth, unsigned int uiHeight);

    bool Recreate();

    // Respond to D3D device reset
    unsigned int GetResetCounter() const;
    bool IsDeviceLost() const;

    // Callbacks for improved response to D3D device reset
    typedef bool (*RESETNOTIFYFUNC)(bool bBeforeReset, void* pvData);

    unsigned int AddResetNotificationFunc(RESETNOTIFYFUNC pfnNotify,
        void* pvData);
    bool RemoveResetNotificationFunc(RESETNOTIFYFUNC pfnNotify);
    bool RemoveResetNotificationFunc(unsigned int uiIndex);
    void RemoveAllResetNotificationFuncs();

    bool ChangeResetNotificationFuncData(RESETNOTIFYFUNC pfnNotify,
        void* pvData);
    bool ChangeResetNotificationFuncData(unsigned int uiIndex,
        void* pvData);

    unsigned int GetResetNotificationFuncCount() const;
    unsigned int GetResetNotificationFuncArrayCount() const;
    unsigned int FindResetNotificationFunc(RESETNOTIFYFUNC pfnNotify) const;
    RESETNOTIFYFUNC GetResetNotificationFunc(unsigned int uiIndex) const;

    typedef bool (*LOSTDEVICENOTIFYFUNC)(void* pvData);

    unsigned int AddLostDeviceNotificationFunc(LOSTDEVICENOTIFYFUNC pfnNotify,
        void* pvData);
    bool RemoveLostDeviceNotificationFunc(LOSTDEVICENOTIFYFUNC pfnNotify);
    bool RemoveLostDeviceNotificationFunc(unsigned int uiIndex);
    void RemoveAllLostDeviceNotificationFuncs();

    bool ChangeLostDeviceNotificationFuncData(LOSTDEVICENOTIFYFUNC pfnNotify,
        void* pvData);
    bool ChangeLostDeviceNotificationFuncData(unsigned int uiIndex,
        void* pvData);

    unsigned int GetLostDeviceNotificationFuncCount() const;
    unsigned int GetLostDeviceNotificationFuncArrayCount() const;
    unsigned int FindLostDeviceNotificationFunc(
        LOSTDEVICENOTIFYFUNC pfnNotify) const;
    LOSTDEVICENOTIFYFUNC GetLostDeviceNotificationFunc(unsigned int uiIndex)
        const;

    // Swap Chain RenderTarget management
    bool CreateSwapChainRenderTargetGroup(unsigned int uiUseFlags, 
        NiWindowRef kWndDevice, 
        FrameBufferFormat eFBFormat = FBFMT_UNKNOWN, 
        DepthStencilFormat eDSFormat = DSFMT_UNKNOWN, 
        PresentationInterval ePresentationInterval = PRESENT_INTERVAL_ONE,
        SwapEffect eSwapEffect = SWAPEFFECT_DEFAULT,
        unsigned int uiFBMode = FBMODE_DEFAULT,
        unsigned int uiBackBufferCount = 1, 
        unsigned int uiRefreshRate = REFRESHRATE_DEFAULT);
    void DestroySwapChainRenderTargetGroup(NiWindowRef kWndDevice);
    NiRenderTargetGroup* GetSwapChainRenderTargetGroup(NiWindowRef kWndDevice) 
        const;

    // Optimize decal maps in the default shader
    void UseQuickDecals(bool bQuickDecals);

    // Perform the geometry precaches that the renderer has cached
    void PerformPrecache();

    // Wrapping an NiTexture around an IDirect3DBaseTexture9
    NiTexturePtr CreateNiTextureFromD3DTexture(
        D3DBaseTexturePtr pkD3DTexture);
    bool RecreateNiTextureFromD3DTexture(NiTexture* pkTexture,
        D3DBaseTexturePtr pkD3DTexture);

    // Display formats to query upon renderer creation
    static void AddDisplayFormat(D3DFORMAT eFormat);
    static void RemoveDisplayFormat(D3DFORMAT eFormat);

    // DX9-specific pixel format query
    const NiPixelFormat* FindClosestPixelFormat(
        TexFormat eFormat, TexUsage eUsage) const;

    // Handle resources for vertex and pixel shader programs
    void PurgeVertexShader(NiD3DVertexShader* pkShader);
    void PurgePixelShader(NiD3DPixelShader* pkShader);

    // Handle resources for Gamebryo shaders
    void RegisterD3DShader(NiD3DShaderInterface* pkD3DShader);
    void ReleaseD3DShader(NiD3DShaderInterface* pkD3DShader);
    void PurgeD3DShader(NiD3DShaderInterface* pkD3DShader);

    void PurgeAllD3DShaders();

    // Access D3D interfaces
    LPDIRECT3DDEVICE9 GetD3DDevice() const;
    unsigned int GetAdapter() const;
    D3DDEVTYPE GetDevType() const;
    D3DPRESENT_PARAMETERS* GetPresentParams(
        const NiRenderTargetGroup* pkTarget = NULL) const;
    D3DFORMAT GetAdapterFormat() const;
    const D3DCAPS9* GetDeviceCaps() const;

    static LPDIRECT3D9 GetDirect3D();

    // Access Gamebryo representations of the device
    const NiDX9AdapterDesc* GetAdapterDesc() const;
    const NiDX9DeviceDesc* GetDeviceDesc() const;

    static const NiDX9SystemDesc* GetSystemDesc();

    // Access DX9 renderer manager classes
    NiDX9RenderState* GetRenderState() const;
    NiDX9VertexBufferManager* GetVBManager() const;
    NiDX9TextureManager* GetTextureManager() const;
    NiDX9LightManager* GetLightManager() const;

    NiD3DShader* GetLegacyDefaultShader() const;

    // Query D3D device capabilities
    unsigned int GetMaxStreams() const;
    unsigned int GetPixelShaderVersion() const;
    unsigned int GetVertexShaderVersion() const;
    bool GetMipmapCubeMaps() const;
    bool IsDynamicTexturesCapable() const;
    bool GetMRTPostPixelShaderBlendingCapability() const;
    bool GetFormatPostPixelShaderBlendingCapability(D3DFORMAT eFormat) const;

    bool GetSWVertexCapable() const;
    bool GetSWVertexSwitchable() const;

    // Remove renderer-specific data from a scene graph to allow it
    // to safely stream
    static void PrepareForStreaming(NiObjectNET* pkRoot);
    static void RestoreAfterStreaming();

    // Translate between D3D and Gamebryo values
    static unsigned char GetD3DFormatSize(D3DFORMAT eFormat);
    static D3DFORMAT GetD3DFormat(FrameBufferFormat eFBFormat);
    static D3DFORMAT GetCompatibleAdapterFormat(FrameBufferFormat eFBFormat);
    static FrameBufferFormat GetNiFBFormat(D3DFORMAT eFormat);
    static D3DFORMAT GetD3DFormat(DepthStencilFormat eDSFormat);
    static DepthStencilFormat GetNiDSFormat(D3DFORMAT eFormat);
    static D3DMULTISAMPLE_TYPE GetMultiSampleType(unsigned int uiMode);
    static unsigned int GetMultiSampleQuality(unsigned int uiMode);
    static D3DSWAPEFFECT GetSwapEffect(SwapEffect eMode, 
        unsigned int uiFBMode, unsigned int uiUseFlags);
    static unsigned int GetRefreshRate(unsigned int uiRefreshRateHz);
    static unsigned int GetPresentInterval(
        PresentationInterval ePresentationInterval);

    // NiRenderer virtual overrides

    // String describing renderer
    virtual const char* GetDriverInfo() const;

    // Renderer capabilities
    virtual unsigned int GetFlags() const;

    // Render target clearing operations
    virtual void SetDepthClear(const float fZClear);
    virtual float GetDepthClear() const;
    virtual void SetBackgroundColor(const NiColor& kColor);
    virtual void SetBackgroundColor(const NiColorA& kColor);
    virtual void GetBackgroundColor(NiColorA& kColor) const;
    virtual void SetStencilClear(unsigned int uiClear);
    virtual unsigned int GetStencilClear() const;

    // Render target access and management
    virtual bool ValidateRenderTargetGroup(NiRenderTargetGroup* pkTarget);
    virtual bool IsDepthBufferCompatible(Ni2DBuffer* pkBuffer, 
        NiDepthStencilBuffer* pkDSBuffer);

    virtual NiRenderTargetGroup* GetDefaultRenderTargetGroup() const;
    virtual const NiRenderTargetGroup* GetCurrentRenderTargetGroup() const;

    virtual NiDepthStencilBuffer* GetDefaultDepthStencilBuffer() const;
    virtual Ni2DBuffer* GetDefaultBackBuffer() const;

    // Pixel format support queries
    virtual const NiPixelFormat* 
        FindClosestPixelFormat(NiTexture::FormatPrefs& kFmtPrefs) const;
    virtual const NiPixelFormat* FindClosestDepthStencilFormat(
        const NiPixelFormat* pkFrontBufferFormat, unsigned int uiDepthBPP,
        unsigned int uiStencilBPP) const;

    // Multiple render target queries
    virtual unsigned int GetMaxBuffersPerRenderTargetGroup() const;
    virtual bool GetIndependentBufferBitDepths() const;

    // Texture precaching
    virtual bool PrecacheTexture(NiTexture* pkIm, bool bForceLoad = false, 
        bool bLocked = false);

    // Texture mipmap skip level
    virtual bool SetMipmapSkipLevel(unsigned int uiSkip);
    virtual unsigned int GetMipmapSkipLevel() const;

    // Geometry precaching
    // Unskinned geometry will pass 0 for the last two parameters.
    virtual bool PrecacheGeometry(NiGeometry* pkGeometry, 
        unsigned int uiBonesPerPartition, 
        unsigned int uiBonesPerVertex);

    virtual NiShaderDeclaration* CreateShaderDeclaration(
        unsigned int uiEntriesPerStream, unsigned int uiNumStreams);

    // Purge cached renderer data
    virtual void PurgeGeometryData(NiGeometryData* pkData);
    virtual void PurgeMaterial(NiMaterialProperty* pkMaterial);
    virtual void PurgeEffect(NiDynamicEffect* pLight);
    virtual void PurgeScreenTextureRendererData(
        NiScreenTexture* pkScreenTexture);
    virtual void PurgeSkinPartitionRendererData(
        NiSkinPartition* pkSkinPartition);
    virtual void PurgeSkinInstance(NiSkinInstance* pkSkinInstance);
    virtual bool PurgeTexture(NiTexture* pkTexture);

    virtual bool PurgeAllTextures(bool bPurgeLocked);

    // Screen shot
    virtual NiPixelData* TakeScreenShot(
        const NiRect<unsigned int>* pkScreenRect,
        const NiRenderTargetGroup* pkTarget = NULL);

    // Screenshot direct to file
    virtual bool SaveScreenShot(const char* pcFilename,
        EScreenshotFormat eFormat);

    // Ni2DBuffer interaction

    // Copy an arbitrary region from one 2D buffer to the specified 
    // coordinates of a different 2D buffer. Note that the NiPixelFormats
    // of both Ni2DBuffers must match for this method. This is the fastest path
    // possible for copying pixel data. Also note that a NULL value for the
    // NiRect will use the dimensions of the Ni2DBuffer.
    virtual bool FastCopy(const Ni2DBuffer* pkSrc, Ni2DBuffer* pkDest, 
        const NiRect<unsigned int>* pkSrcRect = NULL,
        unsigned int uiDestX = 0, unsigned int uiDestY = 0);
    // Copy an arbitrary region from one 2D buffer to another arbitrary region
    // in a different 2D buffer. Note that the NiPixelFormats
    // of both Ni2DBuffers may differ for this method. Also note that a NULL 
    // value for either NiRect will use the dimensions of the Ni2DBuffer.
    // This path is slower than the previous FastCopy method
    virtual bool Copy(const Ni2DBuffer* pkSrc, Ni2DBuffer* pkDest, 
        const NiRect<unsigned int>* pkSrcRect,
        const NiRect<unsigned int>* pkDestRect,
        Ni2DBuffer::CopyFilterPreference ePref);

    // Reversed left-right rendering
    virtual bool GetLeftRightSwap() const;
    virtual bool SetLeftRightSwap(bool bSwap);

    // Global fog settings
    virtual float GetMaxFogValue() const;
    virtual void SetMaxFogValue(float fFogVal);

    // Default material
    virtual void UseLegacyPipelineAsDefaultMaterial();

    // Return the renderer
    static NiDX9Renderer* GetRenderer();

    // *** begin Emergent internal use only ***

    // Model state
    void SetModelTransform(const NiTransform& kXform, 
        bool bPushToDevice = true);
    void SetSkinnedModelTransforms(const NiSkinInstance* pkSkin,
        const NiSkinPartition::Partition* pkPartition,
        const NiTransform& kWorld);
    void CalculateBoneMatrices(const NiSkinInstance* pkSkinInstance, 
        const NiTransform& kWorld, bool bTranspose = false, 
        unsigned int uiBoneMatrixRegisters = 4, 
        bool bPalettizedBones = false);

    void DrawPrimitive(NiGeometry* pkGeometry, NiGeometryData* pkData, 
        NiSkinInstance* pkSkinInstance, const NiTransform& kWorld, 
        const NiBound& kWorldBound, NiGeometryBufferData* pkBuffData);
    void DrawSkinnedPrimitive(NiGeometry* pkGeometry, NiGeometryData* pkData, 
        const NiSkinInstance* pkSkinInstance);

    // Vertex description
    enum
    {
        VERTEX_FLAG_DX_TEXTUREMASK              = 0xFF000000,
        VERTEX_FLAG_DX_TEXTURESHIFT             = 24,
        VERTEX_FLAG_DX_HASNORMALS               = 0x00800000,
        VERTEX_FLAG_DX_HASCOLORS                = 0x00400000,
        VERTEX_FLAG_DX_HASBINORMALSTANGENTS     = 0x00200000
    };

    static unsigned int CreateVertexFlags(
        bool bHasColors, 
        bool bHasNormals,
        bool bHasBinormalsTangents,
        unsigned int uiUVCount);

    static bool GetHasColorsFromFlags(unsigned int uiFlags);
    static bool GetHasNormalsFromFlags(unsigned int uiFlags);
    static bool GetHasBinormalsTangentsFromFlags(unsigned int uiFlags);
    static unsigned int GetNumUVsFromFlags(unsigned int uiFlags);

    // Free data once packed
    void FreeGeometricData(
        NiGeometryData* pkData, 
        NiSkinInstance* pkSkin, 
        NiSkinPartition::Partition* pkPartition);

    // Pointer release wrappers for cross-platform compatibility
    static void ReleaseResource(D3DResourcePtr pkResource);
    static void ReleaseVBResource(D3DVertexBufferPtr pkResource);
    static void ReleaseIBResource(D3DIndexBufferPtr pkResource);
    static void ReleaseTextureResource(D3DTexturePtr pkResource);
    static void ReleaseSwapChain(LPDIRECT3DSWAPCHAIN9 pkSwapChain);
    static void ReleaseDevice(D3DDevicePtr pkDevice);
    static void ReleaseVertexDecl(NiD3DVertexDeclaration pkVertDecl);

    // Renderer specific data for textures and palettes
    virtual bool CreateSourceTextureRendererData(NiSourceTexture* pkTexture);
    virtual bool CreateRenderedTextureRendererData(
        NiRenderedTexture* pkTexture, 
        Ni2DBuffer::MultiSamplePreference eMSAAPref = 
        Ni2DBuffer::MULTISAMPLE_NONE);
    virtual bool CreateSourceCubeMapRendererData(
        NiSourceCubeMap* pkCubeMap);
    virtual bool CreateRenderedCubeMapRendererData(
        NiRenderedCubeMap* pkCubeMap);
    virtual bool CreateDynamicTextureRendererData(
        NiDynamicTexture* pkTexture);
    virtual void CreatePaletteRendererData(NiPalette* pkPalette);
    virtual bool CreateDepthStencilRendererData(
        NiDepthStencilBuffer* pkDSBuffer, const NiPixelFormat* pkFormat,
        Ni2DBuffer::MultiSamplePreference eMSAAPref = 
        Ni2DBuffer::MULTISAMPLE_NONE);

    virtual void RemoveRenderedCubeMapData(NiRenderedCubeMap* pkCubeMap);
    virtual void RemoveRenderedTextureData(NiRenderedTexture* pkTexture);
    virtual void RemoveDynamicTextureData(NiDynamicTexture* pkTexture);

    virtual void* LockDynamicTexture(const NiTexture::RendererData* pkRData,
        int& iPitch);
    virtual bool UnLockDynamicTexture(const NiTexture::RendererData* pkRData);

    virtual NiShader* GetFragmentShader(
        NiMaterialDescriptor* pkMaterialDescriptor);
    virtual NiShader* GetShadowWriteShader(
        NiMaterialDescriptor* pkMaterialDescriptor);
    virtual void SetRenderShadowCasterBackfaces(bool bRenderBackfaces);

    virtual void SetDefaultProgramCache(NiFragmentMaterial* pkMaterial, 
        bool bAutoWriteToDisk, bool bWriteDebugFile, bool bLoad,
        bool bNoNewProgramCreation, const char* pcWorkingDir);

    NiPixelFormat** GetTextureFormatArray(TexUsage eUse);
    const NiPixelFormat* GetDefaultTextureFormat(TexUsage eUse) const;
    const NiPixelData* GetDefaultTextureData(TexUsage eUse) const;

    TexFormat GetReplacementDataFormat() const;

    NiTexture* GetClipperTexture() const;

    const D3DMATRIX& GetD3DMat() const;
    const D3DMATRIX& GetD3DView() const;
    const D3DMATRIX& GetD3DProj() const;
    const D3DMATRIX& GetInvView() const;

    // This is to be considered 'internal only'
    const D3DXALIGNEDMATRIX* GetD3DWorld() const;

    // Shader version information
    void SetMinPixelShaderVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetPixelShaderVersionRequest(unsigned int uiMaj, 
        unsigned int uiMin);
    void SetMinVertexShaderVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetVertexShaderVersionRequest(unsigned int uiMaj, 
        unsigned int uiMin);
    void SetSystemUserVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetMinUserVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetUserVersionRequest(unsigned int uiMaj, unsigned int uiMin);
    unsigned int GetMinPixelShaderVersion() const;
    unsigned int GetPixelShaderVersionRequest() const;
    unsigned int GetMinVertexShaderVersion() const;
    unsigned int GetVertexShaderVersionRequest() const;
    unsigned int GetMinUserVersion() const;
    unsigned int GetUserVersionRequest() const;

    // Texture support information
    unsigned int GetMaxTextureWidth() const;
    unsigned int GetMaxTextureHeight() const;
    unsigned int GetMaxTextureAspectRatio() const;

    const NiD3DShaderLibraryVersion* GetShaderLibraryVersion() const;

    static void _SDMShutdown();

    // Packing geometry buffers
    bool PackGeometryBuffer(NiGeometryBufferData* pkBuffData,
        NiGeometryData* pkData, NiSkinInstance* pkSkinInstance = 0,
        NiD3DShaderDeclaration* pkShaderDecl = 0,
        bool bForce = false);
    bool PackSkinnedGeometryBuffer(NiGeometryBufferData* pkBuffData, 
        NiGeometryData* pkData, const NiSkinInstance* pkSkin, 
        NiSkinPartition::Partition* pkPartition, 
        NiD3DShaderDeclaration* pkShaderDecl = 0, 
        bool bForce = false);

    class  AddressSettings : public NiMemObject
    {
    public:
        D3DTEXTUREADDRESS m_eU;
        D3DTEXTUREADDRESS m_eV;
    };

    class FilterSettings : public NiMemObject
    {
    public:
        D3DTEXTUREFILTERTYPE m_eMin;
        D3DTEXTUREFILTERTYPE m_eMag;
        D3DTEXTUREFILTERTYPE m_eMip;
    };

    static AddressSettings GetD3DClampMode(
        NiTexturingProperty::ClampMode eClamp);
    static FilterSettings GetD3DFilterMode(
        NiTexturingProperty::FilterMode eFilter);

    static unsigned int GetProjectedTextureFlags();

    // *** end Emergent internal use only ***

protected:
    enum { SimpleLineVertexFVF = D3DFVF_XYZ };
    class  SimpleLineVertex : public NiMemObject
    {
    public:
        float fX;
        float fY;
        float fZ;
    };

    enum { TexturedLineVertexFVF = D3DFVF_XYZ | D3DFVF_TEXCOORDSIZE2(0) | 
        D3DFVF_TEX1 };
    class  TexturedLineVertex : public NiMemObject
    {
    public:
        float fX;
        float fY;
        float fZ;
        float fTU;
        float fTV;
    } ;

    enum { ColoredLineVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
    class  ColoredLineVertex : public NiMemObject
    {
    public:
        float fX;
        float fY;
        float fZ;
        D3DCOLOR kColor;
    } ;

    enum { SourceVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | 
        D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEX1 };
    class  SourceVertex : public NiMemObject
    {
    public:
        float fX;
        float fY;
        float fZ;
        D3DCOLOR kColor;
        float fTU;
        float fTV;
    } ;

    enum { SourceNormalVertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL | 
        D3DFVF_DIFFUSE | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEX1 };
    class  SourceNormalVertex : public NiMemObject
    {
    public:
        float fX;
        float fY;
        float fZ;
        float fNX;
        float fNY;
        float fNZ;
        D3DCOLOR kColor;
        float fTU;
        float fTV;
    } ;

    //  Start-up and shutdown related functions...
    NiDX9Renderer();
    bool Initialize(unsigned int uiWidth, unsigned int uiHeight,
        unsigned int uiUseFlags, NiWindowRef kWndDevice, NiWindowRef kWndFocus,
        unsigned int uiAdapter, DeviceDesc eDesc,
        FrameBufferFormat eFBFormat, DepthStencilFormat eDSFormat, 
        PresentationInterval ePresentationInterval, SwapEffect eSwapEffect,
        unsigned int uiFBMode, unsigned int uiBackBufferCount,
        unsigned int uiRefreshRate);

    void InitToDefault();
    static bool IsD3D9Create();
    static int CreateDirect3D9();

    // LostDevice handler
    bool LostDeviceRestore();

    // 'Check' functions
    bool InitializePresentParams(NiWindowRef kWndDevice, unsigned int uiWidth, 
        unsigned int uiHeight, unsigned int uiUseFlags, unsigned int uiFBMode, 
        FrameBufferFormat eFBFormat, DepthStencilFormat eDSFormat,
        unsigned int uiBackBufferCount, SwapEffect eSwapEffect, 
        unsigned int uiRefreshRate, 
        PresentationInterval ePresentationInterval,
        D3DPRESENT_PARAMETERS& kD3DPresentParams) const;

    bool InitializeTextureDefaults();
    bool InitializeDeviceCaps(const D3DPRESENT_PARAMETERS& kPresentParams);


    // Renderer specific implementations
    virtual bool Do_BeginFrame();
    virtual bool Do_EndFrame();
    virtual bool Do_DisplayFrame();
    virtual void Do_ClearBuffer(const NiRect<float>* pkR,
        unsigned int uiMode);
    virtual void Do_SetCameraData(const NiPoint3& kWorldLoc,
        const NiPoint3& kWorldDir, const NiPoint3& kWorldUp, 
        const NiPoint3& kWorldRight, const NiFrustum& kFrustum, 
        const NiRect<float>& kPort);
    virtual void Do_SetScreenSpaceCameraData(
        const NiRect<float>* pkPort = NULL);
    virtual void Do_GetCameraData(NiPoint3& kWorldLoc, NiPoint3& kWorldDir,
        NiPoint3& kWorldUp, NiPoint3& kWorldRight, NiFrustum& kFrustum,
        NiRect<float>& kPort);
    virtual bool Do_BeginUsingRenderTargetGroup(NiRenderTargetGroup* pkTarget, 
        unsigned int uiClearMode);
    virtual bool Do_EndUsingRenderTargetGroup();
    virtual void Do_BeginBatch(NiPropertyState* pkPropertyState,
        NiDynamicEffectState* pkEffectState);
    virtual void Do_EndBatch();
    virtual void Do_BatchRenderShape(NiTriShape* pkTriShape);
    virtual void Do_BatchRenderStrips(NiTriStrips* pkTriStrips);
    virtual void Do_RenderShape(NiTriShape* pkTriShape);
    virtual void Do_RenderTristrips(NiTriStrips* pkTriStrips);
    virtual void Do_RenderPoints(NiParticles* pkParticles);
    virtual void Do_RenderLines(NiLines* pkLines);
    virtual void Do_RenderScreenTexture(NiScreenTexture* pkScreenTexture);


    // Conversion routines
    char* GetDevTypeString() const;
    char* GetBehaviorString() const;

    static bool ExpandDeviceType(DeviceDesc eDesc, D3DDEVTYPE& eDevType,
        unsigned int& uiBehaviorFlags);

    void PackScreenTextureGeomBuffer(NiScreenTexture* pkScreenTexture);

    void PackScreenTextureBuffers(NiScreenTexture* pkScreenTexture, 
        unsigned short usDirtyFlags);

    // Prepacking...
    class PrePackObject : public NiMemObject
    {
    public:
        NiGeometryData* m_pkData;
        NiSkinInstance* m_pkSkin;
        NiSkinPartition::Partition* m_pkPartition;
        NiD3DShaderDeclaration* m_pkShaderDecl;
        unsigned int m_uiBonesPerPartition;
        unsigned int m_uiBonesPerVertex;

        NiGeometryBufferData* m_pkBuffData;
        unsigned int m_uiStream;
        
        PrePackObject* m_pkNext;
    };

    bool PrePackGeometryBuffer(
        NiGeometry* pkGeom, 
        NiGeometryData* pkData, 
        unsigned short usTriCount, 
        unsigned short usMaxTriCount, 
        const unsigned short* pusIndexArray, 
        const unsigned short* pusArrayLengths, 
        unsigned short usArrayCount,
        NiD3DShaderDeclaration* pkShaderDecl = 0);
    bool PrePackSkinnedGeometryBuffer(
        NiGeometry* pkGeom, 
        NiGeometryData* pkData, 
        NiSkinInstance* pkSkin, 
        NiD3DShaderDeclaration* pkShaderDecl = 0, 
        unsigned int uiBonesPerPartition = 0, 
        unsigned int uiBonesPerVertex = 0);
    bool ReservePrepackedBufferSpace(
        NiGeometryBufferData* pkBuffData,
        NiGeometryData* pkData, 
        NiSkinPartition::Partition* pkPartition, 
        NiD3DShaderDeclaration* pkShaderDecl);

    // Batch handling
    void EmptyBatch();
    void RenderBatch();
    void AddToBatch(NiGeometry* pkGeometry);

    bool GenerateDefaultTextureData();

    void CreatePixelFormatArray(NiPixelFormat** ppkFormatArray,
        unsigned int uiUsage, D3DRESOURCETYPE eType);
    bool IsHardwareSkinned(const NiGeometry* pkGeometry, 
        const NiD3DShaderInterface* pkShader = NULL);

    static LPDIRECT3D9 ms_pkD3D9;
    LPDIRECT3DDEVICE9 m_pkD3DDevice9;
    D3DCAPS9 m_kD3DCaps9;

    NiWindowRef m_kWndDevice;
    NiWindowRef m_kWndFocus;
    mutable char m_acDriverDesc[512];
    unsigned int m_uiAdapter;
    D3DDEVTYPE m_eDevType;
    unsigned int m_uiBehaviorFlags;
    D3DFORMAT m_eAdapterFormat;
    bool m_bSWVertexCapable;
    bool m_bSWVertexSwitchable;
    
    const static NiDX9SystemDesc* ms_pkSystemDesc;
    const NiDX9AdapterDesc* m_pkAdapterDesc;
    const NiDX9DeviceDesc* m_pkDeviceDesc;

    unsigned int m_uiBackground;
    float m_fZClear;
    unsigned int m_uiStencilClear;
    unsigned int m_uiRendFlags;
    mutable char m_acBehavior[32];

    // Prepacking
    NiTPointerMap<NiVBBlock*, PrePackObject*> m_kPrePackObjects;

    // Batches
    class BatchedObject : public NiMemObject
    {
    public:
        static BatchedObject* GetNextAvailable();
        void Release();
        static void Purge();

        NiGeometry* m_pkGeometry;
        NiSkinPartition::Partition* m_pkCurrPartition;

        NiGeometryBufferData* m_pkBuffData;
        
        BatchedObject* m_pkNext;
    private:
        static BatchedObject* ms_pkFreeList;
    };

    BatchedObject* m_pkBatchHead;
    BatchedObject* m_pkBatchTail;
    NiPropertyState* m_pkBatchedPropertyState;
    NiDynamicEffectState* m_pkBatchedEffectState;
    NiD3DShaderInterfacePtr m_spBatchedShader;

    // Local copies of camera vectors for points and texture transforms
    NiPoint3 m_kCamRight;
    NiPoint3 m_kCamUp;
    NiPoint3 m_kModelCamRight;
    NiPoint3 m_kModelCamUp;

    float m_fNearDepth, m_fDepthRange;
    
    D3DMATRIX m_kD3DIdentity;

    D3DVIEWPORT9 m_kD3DPort; // full framebuffer viewport

    unsigned int m_uiHWBones;
    unsigned int m_uiMaxStreams;
    unsigned int m_uiMaxPixelShaderVersion;
    unsigned int m_uiMaxVertexShaderVersion;
    bool m_bMipmapCubeMaps;
    bool m_bDynamicTexturesCapable;

    unsigned int m_uiResetCounter;
    bool m_bDeviceLost;

    // Pixel Format mappings
    NiPixelFormat* m_aapkTextureFormats[TEXUSE_NUM][TEX_NUM];
    NiPixelFormat* m_apkDefaultTextureFormat[TEXUSE_NUM];
    // Default texture data
    NiPixelDataPtr m_aspDefaultTextureData[TEXUSE_NUM];

    TexFormat m_eReplacementDataFormat;

    NiRenderTargetGroupPtr m_spDefaultRenderTargetGroup;
    NiRenderTargetGroup* m_pkCurrRenderTargetGroup;

    NiRenderTargetGroup* m_pkCurrOnscreenRenderTargetGroup;
    NiTPointerMap<NiWindowRef, NiRenderTargetGroupPtr> 
        m_kOnscreenRenderTargetGroups;
    unsigned int m_uiMaxNumRenderTargets;
    bool m_bIndependentBitDepths;
    bool m_bMRTPostPixelShaderBlending;

    NiPropertyStatePtr m_spScreenTexturePropertyState;

    // Geometry Groups
    NiD3DGeometryGroupManager* m_pkGeometryGroupManager;
    NiUnsharedGeometryGroup* m_pkDefaultGroup;
    NiDynamicGeometryGroup* m_pkDynamicGroup;

    // Managers
    NiDX9RenderState* m_pkRenderState;
    NiDX9VertexBufferManager* m_pkVBManager;
    NiDX9IndexBufferManager* m_pkIBManager;
    NiDX9TextureManager* m_pkTextureManager;
    NiDX9LightManager* m_pkLightManager;

    // Rendered textures and cube maps
    NiTPointerMap<NiRenderedTexture*, NiDX9RenderedTextureData*> 
        m_kRenderedTextures;
    NiTPointerMap<NiRenderedCubeMap*, NiDX9RenderedCubeMapData*> 
        m_kRenderedCubeMaps;

    // Dynamic textures
    NiTPointerMap<NiDynamicTexture*, NiDX9DynamicTextureData*> 
        m_kDynamicTextures;

    // Clipper texture
    NiTexturePtr m_spClipperImage;

    NiTPointerList<NiDX92DBufferDataPtr> m_kBuffersToUseAtDisplayFrame;

    // Shaders
    NiTPointerList<NiD3DShaderInterface*> m_kD3DShaders;
    NiD3DShaderLibraryVersion m_kShaderLibraryVersion;

    // Temporary members - used to speed up code
    D3DXALIGNEDMATRIX m_kD3DMat;
    D3DMATRIX m_kD3DView;
    D3DMATRIX m_kD3DProj;
    D3DMATRIX m_kInvView;
    // Screen texture members
    NiPoint2* m_pkScreenTextureVerts;
    NiColorA* m_pkScreenTextureColors;
    NiPoint2* m_pkScreenTextureTexCoords;
    unsigned short m_usNumScreenTextureVerts;
    unsigned short* m_pusScreenTextureIndices;
    unsigned int m_uiNumScreenTextureIndices;

    // Creation parameters
    unsigned int m_uiCreationWidth;
    unsigned int m_uiCreationHeight;
    unsigned int m_uiCreationUseFlags;
    NiWindowRef m_kCreationWndDevice;
    NiWindowRef m_kCreationWndFocus;
    unsigned int m_uiCreationAdapter;
    DeviceDesc m_eCreationDesc;
    FrameBufferFormat m_eCreationFBFormat;
    DepthStencilFormat m_eCreationDSFormat;
    PresentationInterval m_eCreationPresentationInterval;
    SwapEffect m_eCreationSwapEffect;
    unsigned int m_uiCreationFBMode;
    unsigned int m_uiCreationBackBufferCount;
    unsigned int m_uiCreationRefreshRate;

    // Default shader
    NiD3DDefaultShaderPtr m_spLegacyDefaultShader;
    NiMaterialPtr m_spLegacyDefaultShaderMaterial;

    // Reset notification functions
    NiTPrimitiveArray<RESETNOTIFYFUNC> m_kResetNotifyFuncs;
    NiTPrimitiveArray<void*> m_kResetNotifyFuncData;

    // LostDevice notification functions
    NiTPrimitiveArray<LOSTDEVICENOTIFYFUNC> m_kLostDeviceNotifyFuncs;
    NiTPrimitiveArray<void*> m_kLostDeviceNotifyFuncData;

    mutable NiTMap<D3DFORMAT, NiPixelFormat*> m_kDepthStencilFormats;

    static NiTPointerList<unsigned int> ms_kDisplayFormats;

    static HINSTANCE ms_hDXLib;
    static NILPD3DCREATE ms_pfnD3DCreate9;
    static NiCriticalSection ms_kD3DCriticalSection;

    // Static flags/values/conversion arrays

    // Map NiTexturingProperty::ClampModes to D3D clamping
    static AddressSettings 
        ms_akD3DAddressMapping[NiTexturingProperty::CLAMP_MAX_MODES];
    // Map NiTexturingProperty::FilterModes to D3D filtering
    static FilterSettings 
        ms_akD3DFilterMapping[NiTexturingProperty::FILTER_MAX_MODES];

    // Map D3DFORMAT to sizes
    enum 
    {
        D3DFORMATSIZECOUNT = 118
    };

    static unsigned char ms_aucD3DFormatSizes[D3DFORMATSIZECOUNT];

    static unsigned int ms_uiProjectedTextureFlags; //  Projected flags

    // Cached camera data.
    NiFrustum m_kCachedFrustum;
    NiRect<float> m_kCachedPort;
};

typedef NiPointer<NiDX9Renderer> NiDX9RendererPtr;

#include "NiDX9Renderer.inl"

#endif  //#ifndef NIDX9RENDERER_H
