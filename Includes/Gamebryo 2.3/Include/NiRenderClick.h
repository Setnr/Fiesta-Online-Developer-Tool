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

#ifndef NIRENDERCLICK_H
#define NIRENDERCLICK_H

#include "NiRenderClickValidator.h"
#include "NiRenderTargetGroup.h"
#include "NiRect.h"
#include "NiFixedString.h"
#include "NiColor.h"

class NIMAIN_ENTRY NiRenderClick : public NiRefObject
{
    NiDeclareRootRTTI(NiRenderClick);
    NiDeclareFlags(unsigned short);

public:
    NiRenderClick();

    // Functions for accessing the name of the render click.
    void SetName(const NiFixedString& kName);
    const NiFixedString& GetName() const;

    // Function that invokes callback functions and calls PerformRendering.
    void Render(unsigned int uiFrameID);

    // Uses render click validator to determine whether or not this render
    // click should be executed for the specified frame.
    bool ShouldPerformRendering(unsigned int uiFrameID);

    // Functions for specifying the render click validator.
    void SetValidator(NiRenderClickValidator* pkValidator);
    NiRenderClickValidator* GetValidator() const;

    // Functions for specifying the render target group.
    void SetRenderTargetGroup(NiRenderTargetGroup* pkRenderTargetGroup);
    NiRenderTargetGroup* GetRenderTargetGroup() const;

    // Functions for specifying buffers to be cleared.
    void SetClearAllBuffers(bool bClearAllBuffers);
    void SetClearColorBuffers(bool bClearBuffers);
    bool GetClearColorBuffers() const;
    void SetClearDepthBuffer(bool bClearBuffer);
    bool GetClearDepthBuffer() const;
    void SetClearStencilBuffer(bool bClearBuffer);
    bool GetClearStencilBuffer() const;

    // Functions for specifying the color to clear render target with.
    void SetBackgroundColor(const NiColorA& kColor);
    void GetBackgroundColor(NiColorA& kColor);
    void SetUseRendererBackgroundColor(bool bUseRendererBG);
    bool GetUseRendererBackgroundColor();
    void SetPersistBackgroundColorToRenderer(bool bPersist);
    bool GetPersistBackgroundColorToRenderer();

    // Functions for specifying the viewport.
    void SetViewport(const NiRect<float>& kViewport);
    const NiRect<float>& GetViewport() const;

    // Functions for accessing whether or not the render click is active.
    void SetActive(bool bActive);
    bool GetActive() const;

    // Functions for reporting statistics about the most recent frame.
    virtual unsigned int GetNumObjectsDrawn() const = 0;
    virtual float GetCullTime() const = 0;
    virtual float GetRenderTime() const = 0;

    // Callback definition.
    typedef bool (*Callback)(NiRenderClick* pkCurrentRenderClick,
        void* pvCallbackData);

    // Pre-processing callback.
    void SetPreProcessingCallbackFunc(Callback pfnCallback,
        void* pvCallbackData = NULL);
    Callback GetPreProcessingCallbackFunc() const;
    void* GetPreProcessingCallbackFuncData() const;

    // Post-processing callback.
    void SetPostProcessingCallbackFunc(Callback pfnCallback,
        void* pvCallbackData = NULL);
    Callback GetPostProcessingCallbackFunc() const;
    void* GetPostProcessingCallbackFuncData() const;

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    // flags
    enum
    {
        ACTIVE  = 0x0001,
        USE_RENDERER_BG_COLOR = 0x0002,
        PERSIST_BG_COLOR = 0x0004
    };

    // Function that actually performs the rendering for the render click.
    virtual void PerformRendering(unsigned int uiFrameID) = 0;

    // The validator used by the ShouldPerformRendering function.
    NiRenderClickValidatorPtr m_spValidator;

    // The render target group to use when rendering.
    NiRenderTargetGroupPtr m_spRenderTargetGroup;

    // The viewport to use when rendering.
    NiRect<float> m_kViewport;

    // Callback functions.
    Callback m_pfnPreProcessingCallback;
    Callback m_pfnPostProcessingCallback;

    // Callback data.
    void* m_pvPreProcessingCallbackData;
    void* m_pvPostProcessingCallbackData;

    // Bitfield that holds buffer clear mode.
    unsigned int m_uiClearMode;
    NiColorA m_kBackgroudColor;

    // The name of the render click.
    NiFixedString m_kName;

    // Default name for all render clicks.
    static NiFixedString ms_kDefaultName;
};

NiSmartPointer(NiRenderClick);

#include "NiRenderClick.inl"

#endif  // #ifndef NIRENDERCLICK_H
