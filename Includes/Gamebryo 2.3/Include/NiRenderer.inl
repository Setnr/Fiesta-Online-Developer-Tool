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

//---------------------------------------------------------------------------
inline void NiRenderer::LockRenderer()
{
    m_kRendererLock.Lock();

    if (LockCount() == 1)
        Do_LockRenderer();
}
//---------------------------------------------------------------------------
inline void NiRenderer::UnlockRenderer()
{
    if (LockCount() == 1)
        Do_UnlockRenderer();

    m_kRendererLock.Unlock();
}
//---------------------------------------------------------------------------
inline unsigned int NiRenderer::LockCount() const
{
    return m_kRendererLock.GetCurrentLockCount();
}
//---------------------------------------------------------------------------
inline bool NiRenderer::BeginFrame()
{
    if (!Check_FrameState("BeginFrame", FRAMESTATE_OUTSIDE_FRAME))
        return false;

    if (!Do_BeginFrame())
        return false;

    m_eFrameState = FRAMESTATE_INSIDE_FRAME;
    return true;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::EndFrame()
{
    if (!Check_FrameState("EndFrame", FRAMESTATE_INSIDE_FRAME))
        return false;

    if (!Do_EndFrame())
    {
        m_eFrameState = FRAMESTATE_OUTSIDE_FRAME;
#ifdef _DEBUG
        Warning("NiRenderer::EndFrame> Failed - " 
            "Returning to frame to 'not set' state.\n");
#endif
        return false;
    }

    m_eFrameState = FRAMESTATE_WAITING_FOR_DISPLAY;
    return true;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::DisplayFrame()
{
    if (!Check_FrameState("DisplayFrame", FRAMESTATE_WAITING_FOR_DISPLAY))
        return false;

    if (!Do_DisplayFrame())
    {
#ifdef _DEBUG
        Warning("NiRenderer::DisplayFrame> Failed - " 
            "Returning to frame to 'not set' state.\n");
#endif
        m_eFrameState = FRAMESTATE_OUTSIDE_FRAME;
        return false;
    }

    m_eFrameState = FRAMESTATE_OUTSIDE_FRAME;
    m_uiFrameID++;
    return true;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::BeginOffScreenFrame()
{
    if (!Check_FrameState("BeginOffScreenFrame", FRAMESTATE_OUTSIDE_FRAME))
        return false;

    if (!Do_BeginFrame())
        return false;

    m_eFrameState = FRAMESTATE_INSIDE_OFFSCREEN_FRAME;
    return true;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::EndOffScreenFrame()
{
    if (!Check_FrameState("EndOffScreenFrame", 
        FRAMESTATE_INSIDE_OFFSCREEN_FRAME))
        return false;

    if (!Do_EndFrame())
    {
#ifdef _DEBUG
        Warning("NiRenderer::EndOffScreenFrame> Failed - " 
            "Returning to frame to 'not set' state.\n");
#endif
        m_eFrameState = FRAMESTATE_OUTSIDE_FRAME;
        return false;
    }

    m_eFrameState = FRAMESTATE_OUTSIDE_FRAME;
    return true;
}
//---------------------------------------------------------------------------
inline void NiRenderer::ClearBuffer(
    const NiRect<float>* pkR, unsigned int uiMode)
{
    if (!Check_RenderTargetGroupActive("ClearBuffer", true))
        return;

    Do_ClearBuffer(pkR, uiMode);
}
//---------------------------------------------------------------------------
inline void NiRenderer::SetCameraData(const NiPoint3& kWorldLoc,
        const NiPoint3& kWorldDir, const NiPoint3& kWorldUp, 
        const NiPoint3& kWorldRight, const NiFrustum& kFrustum, 
        const NiRect<float>& kPort)
{
    if (!Check_RenderTargetGroupActive("SetCameraData", true))
        return;

    Do_SetCameraData(
        kWorldLoc, kWorldDir, kWorldUp, kWorldRight, kFrustum, kPort);
}
//---------------------------------------------------------------------------
inline void NiRenderer::SetScreenSpaceCameraData(
    const NiRect<float>* pkPort)
{
    if (!Check_RenderTargetGroupActive("SetScreenSpaceCameraData", true))
        return;

    Do_SetScreenSpaceCameraData(pkPort);
}
//---------------------------------------------------------------------------
inline void NiRenderer::GetCameraData(NiPoint3& kWorldLoc, NiPoint3& kWorldDir,
    NiPoint3& kWorldUp, NiPoint3& kWorldRight, NiFrustum& kFrustum,
    NiRect<float>& kPort)
{
    if (!Check_RenderTargetGroupActive("GetCameraData", true))
        return;

    Do_GetCameraData(kWorldLoc, kWorldDir, kWorldUp, kWorldRight, kFrustum,
        kPort);
}
//---------------------------------------------------------------------------
inline bool NiRenderer::BeginUsingRenderTargetGroup(
    NiRenderTargetGroup* pkTarget, 
    unsigned int uiClearMode)
{
    if (!Check_RenderTargetGroupActive("BeginUsingRenderTargetGroup", false))
        return false;

    if (!pkTarget)
    {
        Warning("NiRenderer::BeginUsingRenderTargetGroup> "
            "Failed - NULL pointer for render target. "
            "Call BeginUsingDefaultRenderTargetGroup if you intended to use "
            "the backbuffer instead.");
        return false;
    }

    LockRenderer(); // Start lock for duration of render target

    bool bSuccess = Do_BeginUsingRenderTargetGroup(pkTarget, uiClearMode);
    
    if (bSuccess)
        m_bRenderTargetGroupActive = true;
    else
        UnlockRenderer();

    return bSuccess;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::BeginUsingDefaultRenderTargetGroup(
    unsigned int uiClearMode)
{
    if (!Check_RenderTargetGroupActive(
        "BeginUsingDefaultRenderTargetGroup", false))
        return false;

    return BeginUsingRenderTargetGroup(
        GetDefaultRenderTargetGroup(), uiClearMode);
}
//---------------------------------------------------------------------------
inline bool NiRenderer::IsRenderTargetGroupActive() const
{
    return m_bRenderTargetGroupActive;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::EndUsingRenderTargetGroup()
{
    if (!Check_RenderTargetGroupActive("EndUsingRenderTargetGroup", true))
        return false;

    if (!Do_EndUsingRenderTargetGroup())
        return false;

    UnlockRenderer(); // Finish lock for duration of render target

    m_bRenderTargetGroupActive = false;
    return true;
}
//---------------------------------------------------------------------------
inline NiMaterial* NiRenderer::GetDefaultMaterial() const
{
    return m_spCurrentDefaultMaterial;
}
//---------------------------------------------------------------------------
inline NiMaterial* NiRenderer::GetInitialDefaultMaterial() const
{
    return m_spInitialDefaultMaterial;
}
//---------------------------------------------------------------------------
inline void NiRenderer::BeginBatch(NiPropertyState* pkPropertyState, 
    NiDynamicEffectState* pkEffectState)
{
    if (!Check_BatchRendering("BeginBatch", false))
        return;

    Do_BeginBatch(pkPropertyState, pkEffectState);

    m_bBatchRendering = true;
}
//---------------------------------------------------------------------------
inline void NiRenderer::EndBatch()
{
    if (!Check_BatchRendering("EndBatch", true))
        return;

    Do_EndBatch();

    m_bBatchRendering = false;
}
//---------------------------------------------------------------------------
inline void NiRenderer::BatchRenderShape(NiTriShape* pkTriShape)
{
    NIASSERT(Check_BatchRendering("BatchRenderShape", true));

    Do_BatchRenderShape(pkTriShape);
}
//---------------------------------------------------------------------------
inline void NiRenderer::BatchRenderStrips(NiTriStrips* pkTriStrips)
{
    NIASSERT(Check_BatchRendering("BatchRenderStrips", true));

    Do_BatchRenderStrips(pkTriStrips);
}
//---------------------------------------------------------------------------
inline bool NiRenderer::GetBatchRendering() const
{
    return m_bBatchRendering;
}
//---------------------------------------------------------------------------
inline void NiRenderer::RenderShape(NiTriShape* pkTriShape)
{
    NIASSERT(Check_RenderTargetGroupActive("RenderShape", true));

    Do_RenderShape(pkTriShape);
}
//---------------------------------------------------------------------------
inline void NiRenderer::RenderTristrips(NiTriStrips* pkTriStrips)
{
    NIASSERT(Check_RenderTargetGroupActive("RenderTristrips", true));

    Do_RenderTristrips(pkTriStrips);
}
//---------------------------------------------------------------------------
inline void NiRenderer::RenderPoints(NiParticles* pkParticles)
{
    NIASSERT(Check_RenderTargetGroupActive("RenderPoints", true));

    Do_RenderPoints(pkParticles);
}
//---------------------------------------------------------------------------
inline void NiRenderer::RenderLines(NiLines* pkLines)
{
    NIASSERT(Check_RenderTargetGroupActive("RenderLines", true));

    Do_RenderLines(pkLines);
}
//---------------------------------------------------------------------------
inline void NiRenderer::RenderScreenTexture(NiScreenTexture* pkScreenTexture)
{
    NIASSERT(Check_RenderTargetGroupActive("RenderScreenTexture", true));

    Do_RenderScreenTexture(pkScreenTexture);
}
//---------------------------------------------------------------------------
inline float NiRenderer::GetMaxFogValue() const 
{ 
    return 1.0f; 
}
//---------------------------------------------------------------------------
inline bool NiRenderer::PrecacheGeometry(NiGeometry* pkGeometry, 
    unsigned int uiBonesPerPartition, 
    unsigned int uiBonesPerVertex)
{
    return false;
}
//---------------------------------------------------------------------------
inline const char* NiRenderer::GetLastErrorString() 
{ 
    return ms_acErrorString; 
}
//---------------------------------------------------------------------------
inline void NiRenderer::ClearLastErrorString() 
{ 
    SetLastErrorString(0); 
}
//---------------------------------------------------------------------------
inline void NiRenderer::SetLastErrorString(const char* pcStr)
{
    if (pcStr)
        NiStrncpy(ms_acErrorString, 256, pcStr, 255);
    else
        ms_acErrorString[0] = '\0';
}
//---------------------------------------------------------------------------
inline void NiRenderer::SetSorter(NiAccumulator* pkAccum)
{
    m_spAccum = pkAccum;
}
//---------------------------------------------------------------------------
inline NiAccumulator* NiRenderer::GetSorter() const
{
    return m_spAccum;
}
//---------------------------------------------------------------------------
inline NiDynamicEffectState* NiRenderer::GetEffectState() const
{
    return m_pkCurrEffects;
}
//---------------------------------------------------------------------------
inline void NiRenderer::SetEffectState(NiDynamicEffectState* pkState)
{
    m_pkCurrEffects = pkState;
}
//---------------------------------------------------------------------------
inline NiPropertyState* NiRenderer::GetPropertyState() const
{
    return m_pkCurrProp;
}
//---------------------------------------------------------------------------
inline void NiRenderer::SetPropertyState(NiPropertyState* pkState)
{
    m_pkCurrProp = pkState;
}
//---------------------------------------------------------------------------
inline NiRenderer* NiRenderer::GetRenderer()
{
    return ms_pkRenderer;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::PreloadTexture(NiTexture* pkTexture) 
{ 
    return true; 
}
//---------------------------------------------------------------------------
inline void NiRenderer::LockSourceDataCriticalSection()
{
    m_kSourceDataCriticalSection.Lock();
}
//---------------------------------------------------------------------------
inline void NiRenderer::UnlockSourceDataCriticalSection()
{
    m_kSourceDataCriticalSection.Unlock();
}
//---------------------------------------------------------------------------
inline NiRenderer::EFrameState NiRenderer::GetFrameState() const
{
    return m_eFrameState;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::GetInsideFrameState() const
{
    return (m_eFrameState == FRAMESTATE_INSIDE_FRAME) || 
        (m_eFrameState == FRAMESTATE_INSIDE_OFFSCREEN_FRAME) ||
        (m_eFrameState == FRAMESTATE_INTERNAL_FRAME);
}
//---------------------------------------------------------------------------
inline unsigned int NiRenderer::GetFrameID() const
{
    return m_uiFrameID;
}
//---------------------------------------------------------------------------
inline NiShader* NiRenderer::GetErrorShader() const
{
    return m_spErrorShader;
}
//---------------------------------------------------------------------------
inline void NiRenderer::SetErrorShader(NiShader* pkErrorShader)
{
    if (pkErrorShader)
        m_spErrorShader = pkErrorShader;
}
//---------------------------------------------------------------------------
inline void NiRenderer::LockPrecacheCriticalSection()
{
    LockRenderer();
    m_kPrecacheCriticalSection.Lock();
}
//---------------------------------------------------------------------------
inline void NiRenderer::UnlockPrecacheCriticalSection()
{
    m_kPrecacheCriticalSection.Unlock();
    UnlockRenderer();
}
//---------------------------------------------------------------------------
inline bool NiRenderer::Check_FrameState(
    const char* pcCallingFunction,
    EFrameState eExpectedValue) const
{
    if (m_eFrameState == eExpectedValue)
        return true;

#ifdef _DEBUG
    const char* apcCurrentValueStrings[5] = {
        "'not set'",
        "'set'",
        "'set for offscreen'",
        "'waiting for display'",
        "'set for internal'"
    };
    const char* apcNextStepStrings[5] = {
        "'BeginFrame()'",
        "'EndFrame()'",
        "'EndOffscreenFrame()'",
        "'DisplayFrame()'",
        "'EndInternalFrame()"
    };
    NIASSERT((unsigned int)m_eFrameState < 5);

    Warning("NiRenderer::%s> Failed - " 
        "called while frame is %s, Must call %s first.\n",
        pcCallingFunction,
        apcCurrentValueStrings[m_eFrameState],
        apcNextStepStrings[m_eFrameState]);
#endif
    return false;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::Check_RenderTargetGroupActive(
    const char* pcCallingFunction, 
    bool bExpectedValue) const
{
    // Check that we are in a frame of rendering
    if (!GetInsideFrameState())
    {
#ifdef _DEBUG
        Warning("NiRenderer::%s> Failed - " 
            "called while frame is not set for rendering, Must call "
            "'BeginFrame() or BeginOffScreenFrame()' first.\n");
#endif
        return false;
    }

    // Check that render target group is active (or we're in bypass mode)
    if ((m_bRenderTargetGroupActive == bExpectedValue) ||
        (m_eFrameState == FRAMESTATE_INTERNAL_FRAME))
        return true;

#ifdef _DEBUG
    if (m_bRenderTargetGroupActive)
    {
        Warning("NiRenderer::%s> Failed - " 
            "called while render target is 'set', Must call "
            "'EndUsingRenderTargetGroup()' first.\n",
            pcCallingFunction);
    }
    else
    {
        Warning("NiRenderer::%s> Failed - " 
            "called while render target is 'not set', Must call "
            "'BeginUsingRenderTargetGroup()' first.\n",
            pcCallingFunction);
    }
#endif
    return false;
}
//---------------------------------------------------------------------------
inline bool NiRenderer::Check_BatchRendering(
    const char* pcCallingFunction,
    bool bExpectedValue) const
{
    if (!Check_RenderTargetGroupActive(pcCallingFunction, true))
        return false;

    if (m_bBatchRendering == bExpectedValue)
        return true;

#ifdef _DEBUG
    if (m_bBatchRendering)
    {
        Warning("NiRenderer::%s> Failed - " 
            "called while batch rendering is 'set', Must call "
            "'EndBatch()' first.\n",
            pcCallingFunction);
    }
    else
    {
        Warning("NiRenderer::%s> Failed - " 
            "called while batch rendering is 'not set', Must call "
            "'BeginBatch()' first.\n",
            pcCallingFunction);
    }
#endif
    return false;
}
//---------------------------------------------------------------------------
inline void NiRenderer::SetSafeZone(const NiRect<float>& kRect)
{
    m_kDisplaySafeZone = kRect;
}
//---------------------------------------------------------------------------
inline NiRect<float> NiRenderer::GetSafeZone()
{
    return m_kDisplaySafeZone;
}
//---------------------------------------------------------------------------
inline void NiRenderer::ConvertFromPixelsToNDC(const unsigned int uiInX, 
    const unsigned int uiInY, float& fOutX, float& fOutY)
{
    fOutX = (float)uiInX / (float)GetDefaultBackBuffer()->GetWidth();
    fOutY = (float)uiInY / (float)GetDefaultBackBuffer()->GetHeight();
}
//---------------------------------------------------------------------------
inline void NiRenderer::ConvertFromNDCToPixels(const float fInX, 
    const float fInY, unsigned int& uiOutX, unsigned int& uiOutY)
{
    uiOutX = (unsigned int)
        (fInX * (float)GetDefaultBackBuffer()->GetWidth());
    uiOutY = (unsigned int)
        (fInY * (float)GetDefaultBackBuffer()->GetHeight());
}
//---------------------------------------------------------------------------
inline void NiRenderer::ForceInSafeZone(
    unsigned int& uiX, unsigned int& uiY)
{
    NiRect<int> kDisplaySafeZonePixels;
    float fWidth = (float)GetDefaultBackBuffer()->GetWidth();
    float fHeight = (float)GetDefaultBackBuffer()->GetHeight();

    // Create a pixel relative representation of the safe zone bounds
    kDisplaySafeZonePixels.m_left = 
        (unsigned int)(m_kDisplaySafeZone.m_left * fWidth);
    kDisplaySafeZonePixels.m_right = 
        (unsigned int)(m_kDisplaySafeZone.m_right * fWidth);
    kDisplaySafeZonePixels.m_top = 
        (unsigned int)(m_kDisplaySafeZone.m_top * fHeight);
    kDisplaySafeZonePixels.m_bottom = 
        (unsigned int)(m_kDisplaySafeZone.m_bottom * fHeight);

    // Clamp uiX inside of the safe zone
    uiX = NiMax(uiX, kDisplaySafeZonePixels.m_left);
    uiX = NiMin(uiX, kDisplaySafeZonePixels.m_right);

    // Clamp fY inside of the safe zone
    uiY = NiMax(uiY, kDisplaySafeZonePixels.m_top);
    uiY = NiMin(uiY, kDisplaySafeZonePixels.m_bottom);
}
//---------------------------------------------------------------------------
inline void NiRenderer::ForceInSafeZoneNDC(float& fX, float& fY)
{
    // Clamp fX inside of the safe zone
    fX = NiMax(fX, m_kDisplaySafeZone.m_left);
    fX = NiMin(fX, m_kDisplaySafeZone.m_right);

    // Clamp fY inside of the safe zone
    fY = NiMax(fY, m_kDisplaySafeZone.m_top);
    fY = NiMin(fY, m_kDisplaySafeZone.m_bottom);
}
//---------------------------------------------------------------------------
