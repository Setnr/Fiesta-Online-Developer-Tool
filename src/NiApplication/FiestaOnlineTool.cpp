#pragma once
#include "FiestaOnlineTool.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#define IMAPP_IMPL
#include "ImGui/ImApp.h"
#include "ImGui/ImGuizmo.h"

#include <NiD3DShaderFactory.h>
#include <NiDX9Renderer.h>
std::atomic<float> FiestaOnlineTool::DeltaTime = 0.f;

void FiestaOnlineTool::OnIdle()
{
    if (MeasureTime())
    {
        ResetFrameTimings();
        /*Update FrameRate*/
        if (m_pkFrameRate)
        {
            m_pkFrameRate->TakeSample();
            m_pkFrameRate->Update();
        }
        //MainWorldScene
        if (_Scene)
        {
            float Time = NiGetCurrentTimeInSec();
            DeltaTime = Time - m_fLastUpdateTime;
            m_fLastUpdateTime = Time;
            _Scene->ProcessInput();
            _Scene->Update(Time);
            _Scene->UpdateCamera(Time);
        
            /*Prepare Framerendering*/
            this->UpdateFrame();
            this->BeginFrame();

            m_spRenderer->BeginUsingDefaultRenderTargetGroup(NiRenderer::CLEAR_ALL);

            _Scene->Draw(this->m_spRenderer);

            NiDX9RendererPtr renderer = (NiDX9Renderer*)(NiRenderer*)this->m_spRenderer; //Reset some TextureStage Shit to fix some broken Rendering
            renderer->GetRenderState()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0);
            renderer->GetRenderState()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
            renderer->GetRenderState()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            _Scene->StartImGuiFrame();
            _Scene->DrawImGui();
            _Scene->EndImGuiFrame();

            /*Draw NiScreenElements Maybe needs Work so it Draws Player HP Hud and stuff*/
            this->RenderScreenItems();
        }

        /*Draws the Cursor*/
        DrawCursor();
        m_spRenderer->EndUsingRenderTargetGroup();
        this->EndFrame();

        this->DisplayFrame();

        ++this->m_iClicks;
    }

}
