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
inline NiCommand* NiApplication::GetCommand()
{
    return ms_pkCommand;
}
//---------------------------------------------------------------------------
inline void NiApplication::SetInstanceReference(NiInstanceRef pInstance)
{
    ms_pInstance = pInstance;
}
//---------------------------------------------------------------------------
inline NiInstanceRef NiApplication::GetInstanceReference()
{
    return ms_pInstance;
}
//---------------------------------------------------------------------------
inline void NiApplication::SetAcceleratorReference(NiAcceleratorRef pAccel)
{
    ms_pAccel = pAccel;
}
//---------------------------------------------------------------------------
inline NiAcceleratorRef NiApplication::GetAcceleratorReference()
{
    return ms_pAccel;
}
//---------------------------------------------------------------------------
inline void NiApplication::SetFullscreen(bool bOn)
{
    m_bFullscreen = bOn;
}
//---------------------------------------------------------------------------
inline bool NiApplication::GetFullscreen() const
{
    return m_bFullscreen;
}
//---------------------------------------------------------------------------
inline void NiApplication::SetStencil(bool bOn)
{
    m_bStencil = bOn;
}
//---------------------------------------------------------------------------
inline bool NiApplication::GetStencil() const
{
    return m_bStencil;
}
//---------------------------------------------------------------------------
inline void NiApplication::SetRendererDialog(bool bOn)
{
    m_bRendererDialog = bOn;
}
//---------------------------------------------------------------------------
inline bool NiApplication::GetRendererDialog() const
{
    return m_bRendererDialog;
}
//---------------------------------------------------------------------------
inline float NiApplication::GetMaxFrameRate()
{
    return 1.0f / m_fMinFramePeriod;    
}
//---------------------------------------------------------------------------
inline NiAppWindow* NiApplication::GetAppWindow() const
{
    return m_pkAppWindow;
}
//---------------------------------------------------------------------------
inline unsigned int NiApplication::GetMenuID() const
{
    return m_uiMenuID;
}
//---------------------------------------------------------------------------
inline NiWindowRef NiApplication::CreateMainWindow(int iWinMode,
    NiWindowRef hWnd) const
{
    return m_pkAppWindow->CreateMainWindow(iWinMode, hWnd);
}
//---------------------------------------------------------------------------
inline NiWindowRef NiApplication::GetWindowReference() const
{
    return m_pkAppWindow->GetWindowReference();
}
//---------------------------------------------------------------------------
inline NiWindowRef NiApplication::GetRenderWindowReference() const
{
    return m_pkAppWindow->GetRenderWindowReference();
}
//---------------------------------------------------------------------------
inline float NiApplication::GetCurrentTime() 
{ 
    return m_fCurrentTime; 
};
//---------------------------------------------------------------------------
inline float NiApplication::GetLastTime() 
{ 
    return m_fLastTime; 
};
//---------------------------------------------------------------------------
inline float NiApplication::GetAccumTime() 
{ 
    return m_fAccumTime; 
};
//---------------------------------------------------------------------------
inline float NiApplication::GetFrameTime() 
{ 
    return m_fFrameTime;
};
//---------------------------------------------------------------------------
inline NiInputSystem* NiApplication::GetInputSystem()
{
    return m_spInputSystem;
}
//---------------------------------------------------------------------------
#if defined(WIN32)
inline bool NiApplication::IsExclusiveMouse() const
{
    return m_bExclusiveMouse;
}
#endif
//---------------------------------------------------------------------------
inline NiTPointerList<NiScreenElementsPtr>& NiApplication::GetScreenElements()
{
    return m_spScreenElementsRenderView->GetScreenElements();
}
//---------------------------------------------------------------------------
inline NiTPointerList<NiScreenTexturePtr>& NiApplication::GetScreenTextures()
{
    return m_spScreenTextureRenderClick->GetScreenTextures();
}
//---------------------------------------------------------------------------
inline NiTPointerList<NiVisualTrackerPtr>& NiApplication::GetVisualTrackers()
{
    return m_spVisualTrackerRenderClick->GetVisualTrackers();
}
//---------------------------------------------------------------------------
