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

#ifndef NID3D10RENDERER_H
#define NID3D10RENDERER_H

#include "NiD3D10RendererLibType.h"

#include "NiD3D10Headers.h"
#include "NiD3D102DBufferData.h"
#include "NiD3D10SystemDesc.h"
#include "NiD3D10ShaderLibraryVersion.h"
#include "NiD3D10ShadowWriteShader.h"

#include <NiRenderer.h>
#include <NiRenderTargetGroup.h>

NiSmartPointer(NiD3D10Renderer);
NiSmartPointer(NiScreenElements);

class NiD3D10DeviceState;
class NiD3D10GeometryData;
class NiD3D10GeometryPacker;
class NiD3D10RenderStateManager;
class NiD3D10ResourceManager;
class NiD3D10ShaderConstantManager;
class NiD3D10VertexDescription;

// D3D10 ID Number (Must be unique to all renderers).
enum { NI_D3D10_RENDER_ID = 10 };

typedef HRESULT (WINAPI *NILPD3D10CREATEBLOB)(SIZE_T, LPD3D10BLOB*);
typedef HRESULT (WINAPI *NILPD3D10CREATEDEVICE)(IDXGIAdapter*,
    D3D10_DRIVER_TYPE, HMODULE, UINT, UINT, ID3D10Device**);
typedef HRESULT (WINAPI *NILPD3D10CREATEDEVICEANDSWAPCHAIN)(IDXGIAdapter*,
    D3D10_DRIVER_TYPE, HMODULE, UINT, UINT, DXGI_SWAP_CHAIN_DESC*,
    IDXGISwapChain**, ID3D10Device**);
typedef HRESULT (WINAPI *NILPD3D10CREATESTATEBLOCK)(ID3D10Device*, 
    D3D10_STATE_BLOCK_MASK*, ID3D10StateBlock**);
typedef HRESULT (WINAPI *NILPD3D10GETINPUTANDOUTPUTSIGNATUREBLOB)(CONST void*,
    SIZE_T, ID3D10Blob**);
typedef HRESULT (WINAPI *NILPD3D10GETINPUTSIGNATUREBLOB)(CONST void*, SIZE_T,
    ID3D10Blob**);
typedef HRESULT (WINAPI *NILPD3D10GETOUTPUTSIGNATUREBLOB)(CONST void*, SIZE_T, 
    ID3D10Blob**);
typedef HRESULT (WINAPI *NILPD3D10STATEBLOCKMASKDIFFERENCE)
    (D3D10_STATE_BLOCK_MASK*, D3D10_STATE_BLOCK_MASK*, 
    D3D10_STATE_BLOCK_MASK*);
typedef HRESULT (WINAPI *NILPD3D10STATEBLOCKMASKDISABLEALL)
    (D3D10_STATE_BLOCK_MASK*);
typedef HRESULT (WINAPI *NILPD3D10STATEBLOCKMASKDISABLECAPTURE)
    (D3D10_STATE_BLOCK_MASK*, D3D10_DEVICE_STATE_TYPES, UINT, UINT);
typedef HRESULT (WINAPI *NILPD3D10STATEBLOCKMASKENABLEALL)
    (D3D10_STATE_BLOCK_MASK*);
typedef HRESULT (WINAPI *NILPD3D10STATEBLOCKMASKENABLECAPTURE)
    (D3D10_STATE_BLOCK_MASK*, D3D10_DEVICE_STATE_TYPES, UINT, UINT);
typedef BOOL (WINAPI *NILPD3D10STATEBLOCKMASKGETSETTING)
    (D3D10_STATE_BLOCK_MASK*, D3D10_DEVICE_STATE_TYPES, UINT);
typedef HRESULT (WINAPI *NILPD3D10STATEBLOCKMASKINTERSECT)
    (D3D10_STATE_BLOCK_MASK*, D3D10_STATE_BLOCK_MASK*, 
    D3D10_STATE_BLOCK_MASK*);
typedef HRESULT (WINAPI *NILPD3D10STATEBLOCKMASKUNION)(D3D10_STATE_BLOCK_MASK*,
    D3D10_STATE_BLOCK_MASK*, D3D10_STATE_BLOCK_MASK*);

typedef HRESULT (WINAPI *NILPD3D10COMPILESHADER)(LPCSTR, SIZE_T, LPCSTR, 
    CONST D3D10_SHADER_MACRO*, LPD3D10INCLUDE, LPCSTR, LPCSTR, UINT, 
    ID3D10Blob**, ID3D10Blob**);
typedef HRESULT (WINAPI *NILPD3D10DISASSEMBLESHADER)(CONST UINT*, SIZE_T, 
    BOOL, LPCSTR, ID3D10Blob**);
typedef LPCSTR (WINAPI *NILPD3D10GETPIXELSHADERPROFILE)(ID3D10Device*);
typedef LPCSTR (WINAPI *NILPD3D10GETVERTEXSHADERPROFILE)(ID3D10Device*);
typedef LPCSTR (WINAPI *NILPD3D10GETGEOMETRYSHADERPROFILE)(ID3D10Device*);
typedef HRESULT (WINAPI *NILPD3D10PREPROCESSSHADER)(LPCSTR, SIZE_T, LPCSTR, 
    CONST D3D10_SHADER_MACRO*, LPD3D10INCLUDE, ID3D10Blob**, ID3D10Blob**);
typedef HRESULT (WINAPI *NILPD3D10REFLECTSHADER)(CONST void*, SIZE_T, 
    ID3D10ShaderReflection**);

typedef HRESULT (WINAPI *NILPD3D10COMPILEEFFECTFROMMEMORY)(void*, SIZE_T, 
    LPCSTR, CONST D3D10_SHADER_MACRO*, ID3D10Include*, UINT, UINT, 
    ID3D10Blob**, ID3D10Blob**);
typedef HRESULT (WINAPI *NILPD3D10CREATEEFFECTFROMMEMORY)(void*, SIZE_T, 
    UINT, ID3D10Device*, ID3D10EffectPool*, ID3D10Effect**);
typedef HRESULT (WINAPI *NILPD3D10CREATEEFFECTPOOLFROMMEMORY)(void*, SIZE_T, 
    UINT, ID3D10Device*, ID3D10EffectPool**);
typedef HRESULT (WINAPI *NILPD3D10DISASSEMBLEEFFECT)(ID3D10Effect*, BOOL, 
    ID3D10Blob**);

class NID3D10RENDERER_ENTRY NiD3D10Renderer : public NiRenderer
{
    NiDeclareRTTI;

    virtual ~NiD3D10Renderer();

    enum DriverType
    {
        DRIVER_HARDWARE     = D3D10_DRIVER_TYPE_HARDWARE,
        DRIVER_REFERENCE    = D3D10_DRIVER_TYPE_REFERENCE,
        DRIVER_NULL         = D3D10_DRIVER_TYPE_NULL
    };

    enum CreateDeviceFlags
    {
        CREATE_DEVICE_SINGLETHREADED    = D3D10_CREATE_DEVICE_SINGLETHREADED,
        CREATE_DEVICE_DEBUG             = D3D10_CREATE_DEVICE_DEBUG,
        CREATE_DEVICE_SWITCH_TO_REF     = D3D10_CREATE_DEVICE_SWITCH_TO_REF,
        CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS  = 
            D3D10_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS
    };

    enum WindowAssociationFlags
    {
        WIND_ASN_NO_WINDOW_CHANGES  = DXGI_MWA_NO_WINDOW_CHANGES,
        WIND_ASN_NO_ALT_ENTER       = DXGI_MWA_NO_ALT_ENTER,
        WIND_ASN_NO_PRINT_SCREEN    =  DXGI_MWA_NO_PRINT_SCREEN 
    };

    struct NID3D10RENDERER_ENTRY CreationParameters
    {
        CreationParameters();
        CreationParameters(HWND hWnd);

        unsigned int m_uiAdapterIndex;
        unsigned int m_uiOutputIndex;
        DriverType m_eDriverType;
        unsigned int m_uiCreateFlags;
        bool m_bCreateSwapChain;
        bool m_bCreateDepthStencilBuffer;
        bool m_bAssociateWithWindow;
        unsigned int m_uiWindowAssociationFlags;

        DXGI_FORMAT m_eDepthStencilFormat;

        DXGI_SWAP_CHAIN_DESC m_kSwapChain;
    };

    // Renderer creation
    static bool Create(CreationParameters& kCreate, 
        NiD3D10RendererPtr& spRenderer);

    void GetCreationParameters(CreationParameters& kCreate) const;

    bool ResizeBuffers(unsigned int uiWidth, unsigned int uiHeight,
        HWND hOutputWnd = NULL);

    // Force loading of non-debug D3D10 library, even in debug builds.
    static void SetForceNonDebugD3D10(bool bForceNonDebugD3D10);
    static bool GetForceNonDebugD3D10();

    bool CreateSwapChainRenderTargetGroup(
        DXGI_SWAP_CHAIN_DESC& kSwapChainDesc, 
        unsigned int uiOutputIndex = 0, 
        bool bCreateDepthStencilBuffer = true, 
        DXGI_FORMAT eDepthStencilFormat = DXGI_FORMAT_UNKNOWN);
    void DestroySwapChainRenderTargetGroup(HWND hWnd);
    NiRenderTargetGroup* GetSwapChainRenderTargetGroup(HWND hWnd) const;

    bool SetDefaultSwapChainRenderTargetGroup(HWND hWnd);

    ID3D10Device* GetD3D10Device() const;

    // Occluded device notification functions
    typedef bool (*OCCLUDEDNOTIFYFUNC)(bool bBeginOccluded, void* pvData);

    unsigned int AddOccludedNotificationFunc(OCCLUDEDNOTIFYFUNC pfnNotify,
        void* pvData);
    bool RemoveOccludedNotificationFunc(OCCLUDEDNOTIFYFUNC pfnNotify);
    bool RemoveOccludedNotificationFunc(unsigned int uiIndex);
    void RemoveAllOccludedNotificationFuncs();

    bool ChangeOccludedNotificationFuncData(OCCLUDEDNOTIFYFUNC pfnNotify,
        void* pvData);
    bool ChangeOccludedNotificationFuncData(unsigned int uiIndex,
        void* pvData);

    unsigned int GetOccludedNotificationFuncCount() const;
    unsigned int GetOccludedNotificationFuncArrayCount() const;
    unsigned int FindOccludedNotificationFunc(OCCLUDEDNOTIFYFUNC pfnNotify) 
        const;
    OCCLUDEDNOTIFYFUNC GetOccludedNotificationFunc(unsigned int uiIndex) 
        const;

    // Removed device notification functions
    typedef bool (*DEVICEREMOVEDNOTIFYFUNC)(void* pvData);

    unsigned int AddDeviceRemovedNotificationFunc(
        DEVICEREMOVEDNOTIFYFUNC pfnNotify, void* pvData);
    bool RemoveDeviceRemovedNotificationFunc(
        DEVICEREMOVEDNOTIFYFUNC pfnNotify);
    bool RemoveDeviceRemovedNotificationFunc(unsigned int uiIndex);
    void RemoveAllDeviceRemovedNotificationFuncs();

    bool ChangeDeviceRemovedNotificationFuncData(
        DEVICEREMOVEDNOTIFYFUNC pfnNotify, void* pvData);
    bool ChangeDeviceRemovedNotificationFuncData(unsigned int uiIndex,
        void* pvData);

    unsigned int GetDeviceRemovedNotificationFuncCount() const;
    unsigned int GetDeviceRemovedNotificationFuncArrayCount() const;
    unsigned int FindDeviceRemovedNotificationFunc(
        DEVICEREMOVEDNOTIFYFUNC pfnNotify) const;
    DEVICEREMOVEDNOTIFYFUNC GetDeviceRemovedNotificationFunc(
        unsigned int uiIndex) const;

    bool DoesFormatSupportFlag(DXGI_FORMAT eFormat,
        unsigned int uiFlags) const;
    unsigned int GetFormatSupportFlags(DXGI_FORMAT eFormat) const;

    unsigned int GetSyncInterval() const;
    void SetSyncInterval(unsigned int uiSyncInterval);

    NiD3D10DeviceState* GetDeviceState() const;
    NiD3D10GeometryPacker* GetGeometryPacker() const;
    NiD3D10RenderStateManager* GetRenderStateManager() const;
    NiD3D10ResourceManager* GetResourceManager() const;
    NiD3D10ShaderConstantManager* GetShaderConstantManager() const;

    // Shaders
    void RegisterD3D10Shader(NiD3D10ShaderInterface* pkD3D10Shader);
    void ReleaseD3D10Shader(NiD3D10ShaderInterface* pkD3D10Shader);
    void PurgeD3D10Shader(NiD3D10ShaderInterface* pkD3D10Shader);
    void PurgeAllD3D10Shaders();

    // Wrapping an NiTexture around an ID3D10Resource
    NiTexturePtr CreateNiTextureFromD3D10Texture(
        ID3D10Resource* pkD3D10Texture, 
        ID3D10ShaderResourceView* pkResourceView);

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
    float GetMaxFogFactor() const;

    // Default material
    virtual void UseLegacyPipelineAsDefaultMaterial();

    // Return the renderer
    static NiD3D10Renderer* GetRenderer();

    // D3D10 Library functions
    static HRESULT D3D10CreateBlob(SIZE_T NumBytes, LPD3D10BLOB* ppBuffer);
    static HRESULT D3D10CreateDevice(IDXGIAdapter* pAdapter, 
        D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, 
        UINT SDKVersion, ID3D10Device** ppDevice);
    static HRESULT D3D10CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, 
        D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, 
        UINT SDKVersion, DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, 
        IDXGISwapChain** ppSwapChain, ID3D10Device** ppDevice);
    static HRESULT D3D10CreateStateBlock(ID3D10Device* pDevice, 
        D3D10_STATE_BLOCK_MASK* pStateBlockMask, 
        ID3D10StateBlock** ppStateBlock);
    static HRESULT D3D10GetInputAndOutputSignatureBlob(
        CONST void* pShaderBytecode, SIZE_T BytecodeLength, 
        ID3D10Blob** ppSignatureBlob);
    static HRESULT D3D10GetInputSignatureBlob(
        CONST void* pShaderBytecode, SIZE_T BytecodeLength, 
        ID3D10Blob** ppSignatureBlob);
    static HRESULT D3D10GetOutputSignatureBlob(CONST void* pShaderBytecode, 
        SIZE_T BytecodeLength, ID3D10Blob** ppSignatureBlob);
    static HRESULT D3D10StateBlockMaskDifference(D3D10_STATE_BLOCK_MASK* pA, 
        D3D10_STATE_BLOCK_MASK* pB, D3D10_STATE_BLOCK_MASK* pResult);
    static HRESULT D3D10StateBlockMaskDisableAll(
        D3D10_STATE_BLOCK_MASK* pMask);
    static HRESULT D3D10StateBlockMaskDisableCapture(
        D3D10_STATE_BLOCK_MASK* pMask, D3D10_DEVICE_STATE_TYPES StateType, 
        UINT RangeStart, UINT RangeLength);
    static HRESULT D3D10StateBlockMaskEnableAll(D3D10_STATE_BLOCK_MASK* pMask);
    static HRESULT D3D10StateBlockMaskEnableCapture(
        D3D10_STATE_BLOCK_MASK* pMask, D3D10_DEVICE_STATE_TYPES StateType, 
        UINT RangeStart, UINT RangeLength);
    static BOOL D3D10StateBlockMaskGetSetting(D3D10_STATE_BLOCK_MASK* pMask, 
        D3D10_DEVICE_STATE_TYPES StateType, UINT Entry);
    static HRESULT D3D10StateBlockMaskIntersect(D3D10_STATE_BLOCK_MASK* pA, 
        D3D10_STATE_BLOCK_MASK* pB, D3D10_STATE_BLOCK_MASK* pResult);
    static HRESULT D3D10StateBlockMaskUnion(D3D10_STATE_BLOCK_MASK* pA, 
        D3D10_STATE_BLOCK_MASK* pB, D3D10_STATE_BLOCK_MASK* pResult);
    static HRESULT D3D10CompileShader(LPCSTR pSrcData, SIZE_T SrcDataLen, 
        LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, 
        LPD3D10INCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, 
        UINT Flags, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs);
    static HRESULT D3D10DisassembleShader(CONST UINT* pShader, SIZE_T Size, 
        BOOL EnableColorCode, LPCSTR pComments, ID3D10Blob** ppDisassembly);
    static LPCSTR D3D10GetGeometryShaderProfile(ID3D10Device* pDevice);
    static LPCSTR D3D10GetPixelShaderProfile(ID3D10Device* pDevice);
    static LPCSTR D3D10GetVertexShaderProfile(ID3D10Device* pDevice);
    static HRESULT D3D10PreprocessShader(LPCSTR pSrcData, SIZE_T SrcDataSize, 
        LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, 
        LPD3D10INCLUDE pInclude, ID3D10Blob** ppShaderText, 
        ID3D10Blob** ppErrorMsgs);
    static HRESULT D3D10ReflectShader(CONST void* pShaderBytecode, 
        SIZE_T BytecodeLength, ID3D10ShaderReflection** ppReflector);
    static HRESULT D3D10CompileEffectFromMemory(void* pData, SIZE_T DataLength,
        LPCSTR pSrcFileName, CONST D3D10_SHADER_MACRO* pDefines, 
        ID3D10Include* pInclude, UINT HLSLFlags, UINT FXFlags, 
        ID3D10Blob** ppCompiledEffect, ID3D10Blob** ppErrors);
    static HRESULT D3D10CreateEffectFromMemory(void* pData, SIZE_T DataLength, 
        UINT FXFlags, ID3D10Device* pDevice, 
        ID3D10EffectPool* pEffectPool, ID3D10Effect** ppEffect);
    static HRESULT D3D10CreateEffectPoolFromMemory(void* pData, 
        SIZE_T DataLength, UINT FXFlags, ID3D10Device* pDevice, 
        ID3D10EffectPool** ppEffectPool);
    static HRESULT D3D10DisassembleEffect(ID3D10Effect* pEffect, 
        BOOL EnableColorCode, ID3D10Blob** ppDisassembly);

    // *** begin Emergent internal use only ***

    void SetModelTransform(const NiTransform& kWorld);
    void CalculateBoneMatrices(const NiSkinInstance* pkSkinInstance, 
        const NiTransform& kWorld, bool bTranspose = false, 
        unsigned int uiBoneMatrixRegisters = 4, bool bWorldSpaceBones = false);

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

    // Renderer specific data for geometry   
    NiD3D10GeometryData* CreateGeometryRendererData(NiGeometry* pkGeometry);

    // Geometry packing
    bool PackGeometry(NiD3D10GeometryData* pkD3D10GeomData, 
        NiGeometryData* pkData, NiSkinInstance* pkSkinInstance,
        NiD3D10VertexDescription* pkVertexDescription, bool bForce = false) 
        const; 

    bool PackParticlesSprites(NiD3D10GeometryData* pkD3D10GeomData, 
        NiGeometryData* pkData, NiD3D10VertexDescription* pkVertexDescription, 
        const NiPoint3& kModelCameraRight, const NiPoint3& kModelCameraUp, 
        bool bForce = false);

    bool PackLines(NiD3D10GeometryData* pkD3D10GeomData, 
        NiGeometryData* pkData, NiD3D10VertexDescription* pkVertexDescription, 
        bool bForce = false);

    virtual NiShader* GetFragmentShader(
        NiMaterialDescriptor* pkMaterialDescriptor);
    virtual NiShader* GetShadowWriteShader(
        NiMaterialDescriptor* pkMaterialDescriptor);
    virtual void SetRenderShadowCasterBackfaces(bool bRenderBackfaces);

    virtual void SetDefaultProgramCache(NiFragmentMaterial* pkMaterial, 
        bool bAutoWriteToDisk, bool bWriteDebugFile, bool bLoad,
        bool bNoNewProgramCreation, const char* pcWorkingDir);

    const D3DXMATRIXA16& GetProjectionMatrix() const;
    const D3DXMATRIXA16& GetViewMatrix() const;
    const D3DXMATRIXA16& GetInverseViewMatrix() const;
    const D3DXMATRIXA16& GetWorldMatrix() const;

    void GetCameraNearAndFar(float& fNear, float& fFar) const;

    static ID3D10Device* CreateTempDevice(IDXGIAdapter* pkAdapter, 
        D3D10_DRIVER_TYPE eType, unsigned int uiFlags);

    const NiD3D10ShaderLibraryVersion* GetShaderLibraryVersion() const;

    static void _SDMShutdown();

    // *** end Emergent internal use only ***

protected:

    static bool EnsureD3D10Loaded();
    static bool LoadD3D10();
    static bool ReleaseD3D10();

    //  Start-up and shutdown related functions...
    NiD3D10Renderer();
    bool Initialize(CreationParameters& kCreate);
    bool CreateDevice(CreationParameters& kCreate);
    bool ReleaseDevice();

    void CreateManagers();
    void ReleaseManagers();

    void StoreDeviceSettings();

    IDXGISwapChain* CreateSwapChain(DXGI_SWAP_CHAIN_DESC& kSwapChainDesc, 
        unsigned int uiOutputIndex);
    NiRenderTargetGroup* CreateRenderTargetGroupFromSwapChain(
        IDXGISwapChain* pkSwapChain, bool bCreateDepthStencilBuffer,
        DXGI_FORMAT eDepthStencilFormat);

    bool HandleDisplayFrameResult(HRESULT hr);

    bool ReleaseResources();

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
    virtual void Do_GetCameraData(NiPoint3& kWorldLoc, NiPoint3& kWorldDir,
        NiPoint3& kWorldUp, NiPoint3& kWorldRight, NiFrustum& kFrustum,
        NiRect<float>& kPort);
    virtual void Do_SetScreenSpaceCameraData(
        const NiRect<float>* pkPort = NULL);
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

    D3D10_DRIVER_TYPE GetD3D10DriverType(DriverType eType) const;
    unsigned int GetD3D10CreateDeviceFlags(unsigned int uiCreateDeviceFlags) 
        const;
    unsigned int GetD3D10WindowAssociationFlags(
        unsigned int uiWindowAssociationFlags) const;

    enum RenderPath
    {
        RENDER_SHAPE,
        RENDER_STRIPS,
        RENDER_POINTS,
        RENDER_LINES
    };

    void Render(NiGeometry* pkGeometry, RenderPath ePath);

    void ReallocateTempArrays(unsigned int uiArraySize);

    // Emulate screen textures using screen elements
    void TransferScreenTextureToScreenElements(
        NiScreenTexture* pkScreenTexture);

    CreationParameters m_kInitialCreateParameters;
    NiD3D10SystemDescPtr m_spSystemDesc;

    ID3D10Device* m_pkD3D10Device;
    enum
    {
        NULL_ADAPTER = UINT_MAX
    };
    unsigned int m_uiAdapterIndex;

    bool m_bInitialized;

    enum 
    {
        DXGI_FORMAT_COUNT = 89
    };

    unsigned int m_auiFormatSupport[DXGI_FORMAT_COUNT];

    unsigned int m_uiMaxVertexShaderVersion;
    unsigned int m_uiMaxGeometryShaderVersion;
    unsigned int m_uiMaxPixelShaderVersion;

    NiRenderTargetGroupPtr m_spDefaultRenderTargetGroup;
    NiRenderTargetGroup* m_pkCurrentRenderTargetGroup;

    NiTPointerList<NiD3D102DBufferDataPtr> m_kBuffersToUseAtDisplayFrame;

    float m_afBackgroundColor[4];
    float m_fDepthClear;
    unsigned char m_ucStencilClear;

    // Camera-related data
    bool m_bLeftRightSwap;
    NiFrustum m_kCurrentFrustum;
    NiRect<float> m_kCurrentViewPort;

    // AdditionalSwapChains
    NiTPointerMap<HWND, NiRenderTargetGroupPtr> m_kSwapChainRenderTargetGroups;

    // Fog parameters
    float m_fMaxFogFactor;
    float m_fMaxFogValue;

    // Shaders
    NiTPointerList<NiD3D10ShaderInterface*> m_kD3D10Shaders;
    NiD3D10ShaderLibraryVersion m_kShaderLibraryVersion;

    D3DXMATRIXA16 m_kD3DView;
    D3DXMATRIXA16 m_kInvView;
    D3DXMATRIXA16 m_kD3DProj;
    D3DXMATRIXA16 m_kD3DModel;

    unsigned int m_uiSyncInterval;

    NiD3D10DeviceState* m_pkDeviceState;
    NiD3D10GeometryPacker* m_pkGeometryPacker;
    NiD3D10RenderStateManager* m_pkRenderStateManager;
    NiD3D10ResourceManager* m_pkResourceManager;
    NiD3D10ShaderConstantManager* m_pkShaderConstantManager;

    // Temporary data
    NiPoint3* m_pkTempVertices;
    NiColorA* m_pkTempColors;
    NiPoint2* m_pkTempTexCoords;
    unsigned int m_uiTempArraySize;

    // Screen elements object to assist with rendering screen textures
    NiScreenElementsPtr m_spSTScreenElements;
    NiScreenTexture* m_pkLastScreenTexture;

    // Batch rendering
    NiMaterialPtr m_spBatchMaterial;

    // Device state
    bool m_bDeviceOccluded;
    bool m_bDeviceRemoved;

    // Reset notification functions
    NiTPrimitiveArray<OCCLUDEDNOTIFYFUNC> m_kOccludedNotifyFuncs;
    NiTPrimitiveArray<void*> m_kOccludedNotifyFuncData;

    // Removed Device notification functions
    NiTPrimitiveArray<DEVICEREMOVEDNOTIFYFUNC> m_kDeviceRemovedNotifyFuncs;
    NiTPrimitiveArray<void*> m_kDeviceRemovedNotifyFuncData;


    static HINSTANCE ms_hD3D10;

    static NILPD3D10CREATEBLOB ms_pfnD3D10CreateBlob;
    static NILPD3D10CREATEDEVICE ms_pfnD3D10CreateDevice;
    static NILPD3D10CREATEDEVICEANDSWAPCHAIN 
        ms_pfnD3D10CreateDeviceAndSwapChain;
    static NILPD3D10CREATESTATEBLOCK ms_pfnD3D10CreateStateBlock;
    static NILPD3D10GETINPUTANDOUTPUTSIGNATUREBLOB 
        ms_pfnD3D10GetInputAndOutputSignatureBlob;
    static NILPD3D10GETINPUTSIGNATUREBLOB ms_pfnD3D10GetInputSignatureBlob;
    static NILPD3D10GETOUTPUTSIGNATUREBLOB ms_pfnD3D10GetOutputSignatureBlob;
    static NILPD3D10STATEBLOCKMASKDIFFERENCE 
        ms_pfnD3D10StateBlockMaskDifference;
    static NILPD3D10STATEBLOCKMASKDISABLEALL 
        ms_pfnD3D10StateBlockMaskDisableAll;
    static NILPD3D10STATEBLOCKMASKDISABLECAPTURE 
        ms_pfnD3D10StateBlockMaskDisableCapture;
    static NILPD3D10STATEBLOCKMASKENABLEALL ms_pfnD3D10StateBlockMaskEnableAll;
    static NILPD3D10STATEBLOCKMASKENABLECAPTURE 
        ms_pfnD3D10StateBlockMaskEnableCapture;
    static NILPD3D10STATEBLOCKMASKGETSETTING 
        ms_pfnD3D10StateBlockMaskGetSetting;
    static NILPD3D10STATEBLOCKMASKINTERSECT ms_pfnD3D10StateBlockMaskIntersect;
    static NILPD3D10STATEBLOCKMASKUNION ms_pfnD3D10StateBlockMaskUnion;

    static NILPD3D10COMPILESHADER ms_pfnD3D10CompileShader;
    static NILPD3D10DISASSEMBLESHADER ms_pfnD3D10DisassembleShader;
    static NILPD3D10GETGEOMETRYSHADERPROFILE 
        ms_pfnD3D10GetGeometryShaderProfile;
    static NILPD3D10GETPIXELSHADERPROFILE ms_pfnD3D10GetPixelShaderProfile;
    static NILPD3D10GETVERTEXSHADERPROFILE ms_pfnD3D10GetVertexShaderProfile;
    static NILPD3D10PREPROCESSSHADER ms_pfnD3D10PreprocessShader;
    static NILPD3D10REFLECTSHADER ms_pfnD3D10ReflectShader;

    static NILPD3D10COMPILEEFFECTFROMMEMORY ms_pfnD3D10CompileEffectFromMemory;
    static NILPD3D10CREATEEFFECTFROMMEMORY ms_pfnD3D10CreateEffectFromMemory;
    static NILPD3D10CREATEEFFECTPOOLFROMMEMORY 
        ms_pfnD3D10CreateEffectPoolFromMemory;
    static NILPD3D10DISASSEMBLEEFFECT ms_pfnD3D10DisassembleEffect;

    static NiCriticalSection ms_kD3D10LibraryCriticalSection;
    static bool ms_bForceNonDebugD3D10;
};

#include "NiD3D10Renderer.inl"

#endif  //#ifndef NID3D10RENDERER_H
