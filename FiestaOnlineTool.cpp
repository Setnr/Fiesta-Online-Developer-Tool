#pragma once
#include "FiestaOnlineTool.h"
#include "FiestaOnlineTool_GeneralHeaders.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#define IMAPP_IMPL
#include "ImGui/ImApp.h"
#include "ImGui/ImGuizmo.h"

void FiestaOnlineTool::OnIdle()
{
    if(MeasureTime())
    {
        /*Update FrameRate*/
        if (m_pkFrameRate)
        {
            m_pkFrameRate->TakeSample();
            m_pkFrameRate->Update();
        }

        //MainWorldScene
        if (_Scene->UpdateScene()) 
        {
            _Scene = _Scene->GetNewScene();
            _Scene->SetupScene(m_spScene, m_spCamera);
        }
        _Scene->Update(NiGetCurrentTimeInSec());

        //Interface Scene
        Pgg_kWinMgr->Update();

        /*Prepare Framerendering*/
        this->UpdateFrame();
        this->BeginFrame();
        m_spRenderer->BeginUsingDefaultRenderTargetGroup(7u);
        /*Set BackgroundColor of Renderer*/
        NiColorA m_kBackGroundColor;
        this->m_spRenderer->SetBackgroundColor(m_kBackGroundColor);

        /*Draw MainScene (GameWorld)*/
        _Scene->Draw(this->m_spRenderer);
        
        /*Draw Interface Windows*/
        Pgg_kWinMgr->Draw(m_spRenderer);
        /*Draw NiScreenElements Maybe needs Work so it Draws Player HP Hud and stuff*/
        this->RenderScreenItems();
        /*Draws the Cursor*/
        DrawCursor();

        m_spRenderer->EndUsingRenderTargetGroup();
        this->EndFrame();
        this->DisplayFrame();
        ++this->m_iClicks;
    }
    
}

void FiestaOnlineTool::DrawCursor()
{
    long X, Y;
    FiestaOnlineTool::GetMousePosition(X, Y);
    _Tool->cursor->SetPosition(0.0, X + 5, Y + 9);
    cursor->Draw();
}