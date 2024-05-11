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

#ifndef NIRENDERER_H
#define NIRENDERER_H

#include "Ni2DBuffer.h"
#include "NiAccumulator.h"
#include "NiAVObject.h"
#include "NiBool.h"
#include "NiColor.h"
#include "NiDynamicEffectState.h"
#include "NiMaterial.h"
#include "NiPropertyState.h"
#include "NiRect.h"
#include "NiRTLib.h"
#include "NiShader.h"
#include "NiTexture.h"
#include "NiTList.h"

class NiBound;
class NiDynamicEffect;
class NiDynamicTexture;
class NiFragmentMaterial;
class NiPoint2;
class NiPoint3;
class NiPlane;
class NiCamera;
class NiDepthStencilBuffer;
class NiFrustum;
class NiGeometry;
class NiGeometryData;
class NiMaterialDescriptor;
class NiPalette;
class NiTransform;
class NiTriShape;
class NiTriShapeData;
class NiTriStrips;
class NiTriStripsData;
class NiParticles;
class NiParticlesData;
class NiLines;
class NiLinesData;
class NiShader;
class NiSkinInstance;
class NiSourceCubeMap;
class NiSourceTexture;
class NiScreenTexture;
class NiScreenElements;
class NiRenderedCubeMap;
class NiRenderedTexture;
class NiRenderTargetGroup;
class NiSkinPartition;
class NiStandardMaterial;

NiSmartPointer(NiStandardMaterial);

class NIMAIN_ENTRY NiRenderer : public NiObject 
{
    NiDeclareRTTI;
    NiDeclareViewerStrings;
public:
    // local constants -
    // return flags from GetFlags:
    enum
    {
        // indicates that renderer supports palettized textures
        CAPS_PALETTIZED                 = 0x00000001,
        // indicates renderer supports hardware skinning
        CAPS_HARDWARESKINNING           = 0x00000002,
        // indicates renderer has limited support for textures with 
        // non-power-of-2 dimensions
        CAPS_NONPOW2_CONDITIONAL_TEXT   = 0x00000004,
        // indicates renderer has full support for textures with 
        // non-power-of-2 dimensions
        CAPS_NONPOW2_TEXT               = 0x00000008,
        // indicates renderer can antialias rendered textures
        CAPS_AA_RENDERED_TEXTURES       = 0x00000010,

        CAPS_MAX                        = 0x00000010
    };
      
    enum ClearFlags
    {
        CLEAR_BACKBUFFER = 0x1,
        CLEAR_STENCIL = 0x2,
        CLEAR_ZBUFFER = 0x4,
        CLEAR_NONE = 0, 
        CLEAR_ALL = CLEAR_BACKBUFFER | CLEAR_STENCIL | CLEAR_ZBUFFER
    }; 

    // Note: Check_FrameState needs to be updated when these are updated
    enum EFrameState
    {
        FRAMESTATE_OUTSIDE_FRAME,
        FRAMESTATE_INSIDE_FRAME,
        FRAMESTATE_INSIDE_OFFSCREEN_FRAME,
        FRAMESTATE_WAITING_FOR_DISPLAY,
        FRAMESTATE_INTERNAL_FRAME
    };

    enum DisplayCorner
    {
        CORNER_TOP_LEFT,
        CORNER_TOP_RIGHT,
        CORNER_BOTTOM_LEFT,
        CORNER_BOTTOM_RIGHT
    };

    enum EScreenshotFormat
    {
        FORMAT_TARGA,
        FORMAT_PNG,
        FORMAT_JPEG
    };

    // construction and destruction
    NiRenderer();
    virtual ~NiRenderer();

    // String describing renderer
    virtual const char* GetDriverInfo() const = 0;

    // Renderer capabilities
    virtual unsigned int GetFlags() const = 0;

    // Renderer locking methods
    void LockRenderer();
    void UnlockRenderer();
    unsigned int LockCount() const;

    // Begin / End Frame
    bool BeginFrame();
    unsigned int GetFrameID() const;
    bool EndFrame();
    bool DisplayFrame();
    bool BeginOffScreenFrame();
    bool EndOffScreenFrame();
    EFrameState GetFrameState() const;
    bool GetInsideFrameState() const;

    // Render target clearing operations
    void ClearBuffer(const NiRect<float>* pkR, unsigned int uiMode);

    virtual void SetDepthClear(const float fZClear) = 0;
    virtual float GetDepthClear() const = 0;
    virtual void SetBackgroundColor(const NiColor& kColor) = 0;
    virtual void SetBackgroundColor(const NiColorA& kColor) = 0;
    virtual void GetBackgroundColor(NiColorA& kColor) const = 0;
    virtual void SetStencilClear(unsigned int uiClear) = 0;
    virtual unsigned int GetStencilClear() const = 0;

    // Camera setup
    void SetCameraData(const NiCamera* pkCamera);
    void SetCameraData(
        const NiPoint3& kWorldLoc,
        const NiPoint3& kWorldDir,
        const NiPoint3& kWorldUp, 
        const NiPoint3& kWorldRight,
        const NiFrustum& kFrustum, 
        const NiRect<float>& kPort);
    void SetScreenSpaceCameraData(const NiRect<float>* pkPort = NULL);
    void GetCameraData(NiCamera& kCamera);
    void GetCameraData(NiPoint3& kWorldLoc, NiPoint3& kWorldDir,
        NiPoint3& kWorldUp, NiPoint3& kWorldRight, NiFrustum& kFrustum,
        NiRect<float>& kPort);

    // Render target access and management
    bool BeginUsingRenderTargetGroup(NiRenderTargetGroup* pkTarget, 
        unsigned int uiClearMode);
    bool BeginUsingDefaultRenderTargetGroup(unsigned int uiClearMode);
    bool IsRenderTargetGroupActive() const;
    bool EndUsingRenderTargetGroup();

    virtual bool ValidateRenderTargetGroup(NiRenderTargetGroup* pkTarget) = 0;
    virtual bool IsDepthBufferCompatible(Ni2DBuffer* pkBuffer, 
        NiDepthStencilBuffer* pkDSBuffer) = 0;

    virtual NiRenderTargetGroup* GetDefaultRenderTargetGroup() const = 0;
    virtual const NiRenderTargetGroup* GetCurrentRenderTargetGroup() const
        = 0;

    virtual NiDepthStencilBuffer* GetDefaultDepthStencilBuffer() const = 0;
    virtual Ni2DBuffer* GetDefaultBackBuffer() const = 0;

    // Pixel format support queries
    virtual const NiPixelFormat* 
        FindClosestPixelFormat(NiTexture::FormatPrefs& kFmtPrefs) const = 0;
    virtual const NiPixelFormat* FindClosestDepthStencilFormat(
        const NiPixelFormat* pkFrontBufferFormat) const;
    virtual const NiPixelFormat* FindClosestDepthStencilFormat(
        const NiPixelFormat* pkFrontBufferFormat, unsigned int uiDepthBPP,
        unsigned int uiStencilBPP) const = 0;

    // Multiple render target queries
    virtual unsigned int GetMaxBuffersPerRenderTargetGroup() const = 0;
    virtual bool GetIndependentBufferBitDepths() const = 0;

    // Sorting
    void SetSorter(NiAccumulator* pkAccum);
    NiAccumulator* GetSorter() const;

    // Default material
    void SetDefaultMaterial(NiMaterial* pkMaterial);
    NiMaterial* GetDefaultMaterial() const;
    virtual void UseLegacyPipelineAsDefaultMaterial() = 0;
    virtual void ApplyDefaultMaterial(NiGeometry* pkGeometry);

    NiMaterial* GetInitialDefaultMaterial() const;

    // Batch rendering
    void BeginBatch(NiPropertyState* pkPropertyState, 
        NiDynamicEffectState* pkEffectState);
    void EndBatch();
    void BatchRenderShape(NiTriShape* pkTriShape);
    void BatchRenderStrips(NiTriStrips* pkTriStrips);
    bool GetBatchRendering() const;

    // Texture precaching
    virtual bool PrecacheTexture(NiTexture* pkTexture, 
        bool bForceLoad = false, bool bLocked = false) = 0;

    // Texture mipmap skip level
    virtual bool SetMipmapSkipLevel(unsigned int uiSkip) = 0;
    virtual unsigned int GetMipmapSkipLevel() const = 0;

    // Geometry precaching
    // Unskinned geometry will pass 0 for the last two parameters.
    virtual bool PrecacheGeometry(
        NiGeometry* pkGeometry, 
        unsigned int uiBonesPerPartition, 
        unsigned int uiBonesPerVertex);
    virtual NiShaderDeclaration* CreateShaderDeclaration(
        unsigned int uiEntriesPerStream, unsigned int uiNumStreams) = 0;

    // Purge cached renderer data
    virtual void PurgeGeometryData(NiGeometryData* pkData) = 0;
    virtual void PurgeMaterial(NiMaterialProperty* pkMaterial) = 0;
    virtual void PurgeEffect(NiDynamicEffect* pLight) = 0;
    virtual void PurgeScreenTextureRendererData(
        NiScreenTexture* pkScreenTexture) = 0;
    virtual void PurgeSkinPartitionRendererData(
        NiSkinPartition* pkSkinPartition) = 0;
    virtual void PurgeSkinInstance(NiSkinInstance* pkSkinInstance) = 0;
    virtual bool PurgeTexture(NiTexture* pkTexture) = 0;

    virtual bool PurgeAllTextures(bool bPurgeLocked) = 0;
    void PurgeAllRendererData(NiAVObject* pkScene); 

    // Screen shot
    virtual NiPixelData* TakeScreenShot(
        const NiRect<unsigned int>* pkScreenRect,
        const NiRenderTargetGroup* pkTarget = NULL) = 0;

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
        unsigned int uiDestX = 0, unsigned int uiDestY = 0) = 0;
    // Copy an arbitrary region from one 2D buffer to another arbitrary region
    // in a different 2D buffer. Note that the NiPixelFormats
    // of both Ni2DBuffers may differ for this method. Also note that a NULL 
    // value for either NiRect will use the dimensions of the Ni2DBuffer.
    // This path is slower than the previous FastCopy method
    virtual bool Copy(const Ni2DBuffer* pkSrc, Ni2DBuffer* pkDest, 
        const NiRect<unsigned int>* pkSrcRect,
        const NiRect<unsigned int>* pkDestRect,
        Ni2DBuffer::CopyFilterPreference ePref) = 0;

    // Reversed left-right rendering
    virtual bool GetLeftRightSwap() const = 0;
    virtual bool SetLeftRightSwap(bool bSwap) = 0;

    // Global fog settings
    virtual float GetMaxFogValue() const = 0;
    virtual void SetMaxFogValue(float fFogVal) = 0;

    // Error shader support
    virtual NiShader* GetErrorShader() const;
    virtual void SetErrorShader(NiShader* pkErrorShader);

    // Error status
    static const char* GetLastErrorString();
    static void ClearLastErrorString();

    // safe zone handling
    void SetSafeZone(const NiRect<float>& kRect);
    NiRect<float> GetSafeZone();
    void ForceInSafeZone(unsigned int& uiX, unsigned int& uiY);
    void ForceInSafeZoneNDC(float& fX, float& fY);
    void ConvertFromPixelsToNDC(const unsigned int uiInX, 
        const unsigned int uiInY, float& fOutX, float& fOutY);
    void ConvertFromNDCToPixels(const float fInX, const float fInY,
        unsigned int& uiOutX, unsigned int& uiOutY);

    // Helper methods to create object screen coordinates
    void GetOnScreenCoord(
        const float fXOffset, 
        const float fYOffset,
        const unsigned int uiWidth, 
        const unsigned int uiHeight,
        unsigned int& uiOutX, 
        unsigned int& uiOutY,
        const DisplayCorner eCorner, 
        const bool bForceSafeZone = true);
    NiScreenElements* CreateScreenElement(
        const float fXOffset, 
        const float fYOffset,
        const unsigned int uiWidth, 
        const unsigned int uiHeight,
        const DisplayCorner eCorner, 
        const bool bForceSafeZone = true);

    // Return the renderer
    static NiRenderer* GetRenderer();

    // *** begin Emergent internal use only ***

    // Property state
    NiPropertyState* GetPropertyState() const;
    void SetPropertyState (NiPropertyState* pkState);

    // Effect state
    NiDynamicEffectState* GetEffectState() const;
    void SetEffectState(NiDynamicEffectState* pkState);

    // Primitive rendering
    void RenderShape(NiTriShape* pkTriShape);
    void RenderTristrips(NiTriStrips* pkTriStrips);
    void RenderPoints(NiParticles* pkParticles);
    void RenderLines(NiLines* pkLines);
    void RenderScreenTexture(NiScreenTexture* pkScreenTexture);

    bool MapWindowPointToBufferPoint(unsigned int uiX, unsigned int uiY, 
        float& fX, float& fY, const NiRenderTargetGroup* pkTarget = NULL);
    
    virtual bool PreloadTexture(NiTexture* pkTexture);

    static void RemoveEffect(NiDynamicEffect* pkEffect);
    static void RemoveGeometryData(NiGeometryData* pkData);
    static void RemoveMaterial(NiMaterialProperty* pkMaterial);
    static void RemoveScreenTextureRendererData(
        NiScreenTexture* pkScreenTexture);
    static void RemoveSkinPartitionRendererData(
        NiSkinPartition* pkSkinPartition);
    static void RemoveSkinInstance(NiSkinInstance* pkSkinInstance);

    static void SetLastErrorString(const char* pcStr);

    // Source renderer data primitive protection methods.
    void LockSourceDataCriticalSection();
    void UnlockSourceDataCriticalSection();

    // Renderer specific data for textures and palettes
    virtual bool CreateSourceTextureRendererData(
        NiSourceTexture* pkTexture) = 0;
    virtual bool CreateRenderedTextureRendererData(
        NiRenderedTexture* pkTexture, 
        Ni2DBuffer::MultiSamplePreference eMSAAPref = 
        Ni2DBuffer::MULTISAMPLE_NONE) = 0;
    virtual bool CreateSourceCubeMapRendererData(
        NiSourceCubeMap* pkCubeMap) = 0;
    virtual bool CreateRenderedCubeMapRendererData(
        NiRenderedCubeMap* pkCubeMap) = 0;
    virtual bool CreateDynamicTextureRendererData(
        NiDynamicTexture* pkTexture) = 0;
    virtual void CreatePaletteRendererData(
        NiPalette* pkPalette) = 0;
    virtual bool CreateDepthStencilRendererData(
        NiDepthStencilBuffer* pkDSBuffer, const NiPixelFormat* pkFormat,
        Ni2DBuffer::MultiSamplePreference eMSAAPref = 
        Ni2DBuffer::MULTISAMPLE_NONE) = 0;

    // Dynamic texture support
    virtual void* LockDynamicTexture(const NiTexture::RendererData* pkRData,
        int& iPitch) = 0;
    virtual bool UnLockDynamicTexture(const NiTexture::RendererData* pkRData)
        = 0;

    // Fragment shader support
    virtual NiShader* GetFragmentShader(
        NiMaterialDescriptor* pkMaterialDescriptor) = 0;
    virtual void SetDefaultProgramCache(NiFragmentMaterial* pkMaterial);
    virtual void SetDefaultProgramCache(NiFragmentMaterial* pkMaterial, 
        bool bAutoWriteToDisk, bool bWriteDebugFile, bool bLoad, 
        bool bNoNewProgramCreation, const char* pcWorkingDir) = 0;

    // ShadowMap shader support
    virtual NiShader* GetShadowWriteShader(
        NiMaterialDescriptor* pkMaterialDescriptor) = 0;
    virtual void SetRenderShadowCasterBackfaces(bool bRenderBackfaces) = 0;

    // Message/Warning/Error reporting functions
    static void Message(const char* pcFormat, ...);
    static void Warning(const char* pcFormat, ...);
    static void Error(const char* pcFormat, ...);

    static void _SDMShutdown();

    // *** end Emergent internal use only ***

protected:
    // Renderer specific implementations
    virtual void Do_LockRenderer() {};
    virtual void Do_UnlockRenderer() {};
    virtual bool Do_BeginFrame() = 0;
    virtual bool Do_EndFrame() = 0;
    virtual bool Do_DisplayFrame() = 0;
    virtual void Do_ClearBuffer(const NiRect<float>* pkR,
        unsigned int uiMode) = 0;
    virtual void Do_SetCameraData(const NiPoint3& kWorldLoc,
        const NiPoint3& kWorldDir, const NiPoint3& kWorldUp, 
        const NiPoint3& kWorldRight, const NiFrustum& kFrustum, 
        const NiRect<float>& kPort) = 0;
    virtual void Do_SetScreenSpaceCameraData(
        const NiRect<float>* pkPort = NULL) = 0;
    virtual void Do_GetCameraData(NiPoint3& kWorldLoc, NiPoint3& kWorldDir,
        NiPoint3& kWorldUp, NiPoint3& kWorldRight, NiFrustum& kFrustum,
        NiRect<float>& kPort) = 0;
    virtual bool Do_BeginUsingRenderTargetGroup(NiRenderTargetGroup* pkTarget, 
        unsigned int uiClearMode) = 0;
    virtual bool Do_EndUsingRenderTargetGroup() = 0;
    virtual void Do_BeginBatch(NiPropertyState* pkPropertyState,
        NiDynamicEffectState* pkEffectState);
    virtual void Do_EndBatch();
    virtual void Do_BatchRenderShape(NiTriShape* pkTriShape);
    virtual void Do_BatchRenderStrips(NiTriStrips* pkTriStrips);
    virtual void Do_RenderShape(NiTriShape* pkTriShape) = 0;
    virtual void Do_RenderTristrips(NiTriStrips* pkTriStrips) = 0;
    virtual void Do_RenderPoints(NiParticles* pkParticles) = 0;
    virtual void Do_RenderLines(NiLines* pkLines) = 0;
    virtual void Do_RenderScreenTexture(NiScreenTexture* pkScreenTexture) = 0;


    // Thread synchronization 
    void LockPrecacheCriticalSection();
    void UnlockPrecacheCriticalSection();

    NiAccumulatorPtr m_spAccum;
    NiPropertyState* m_pkCurrProp;
    NiDynamicEffectState* m_pkCurrEffects;

    // Default material
    NiMaterialPtr m_spInitialDefaultMaterial;
    NiMaterialPtr m_spCurrentDefaultMaterial;

    // Error shader
    NiShaderPtr m_spErrorShader;

    static char ms_acErrorString[256];
    static NiRenderer* ms_pkRenderer;

    // Safe zone rectangle
    NiRect<float> m_kDisplaySafeZone;

    bool BeginInternalFrame();
    bool EndInternalFrame();
private:
    // Checking code for API call validity 
    bool Check_FrameState(
        const char* pcCallingFunction,
        EFrameState eExpectedValue) const;

    bool Check_RenderTargetGroupActive(
        const char* pcCallingFunction,
        bool bExpectedValue) const;

    bool Check_BatchRendering(
        const char* pcCallingFunction,
        bool bExpectedValue) const;

    // Thread synchronization primitives
    NiCriticalSection m_kRendererLock;
    NiCriticalSection m_kPrecacheCriticalSection; // requires m_kRendererLock

    NiCriticalSection m_kSourceDataCriticalSection;

    // Render target group bracketing
    bool m_bRenderTargetGroupActive;

    // Frame and batch rendering bracketing
    EFrameState m_eFrameState;
    EFrameState m_eSavedFrameState;
    unsigned int m_uiFrameID;
    bool m_bBatchRendering;
};

typedef NiPointer<NiRenderer> NiRendererPtr;

#include "NiRenderer.inl"

#endif // NIRENDERER_H
