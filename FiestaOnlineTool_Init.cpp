#pragma once
#include "FiestaOnlineTool.h"
#include "FiestaOnlineTool_GeneralHeaders.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "Detours/detours.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
typedef LRESULT(__stdcall* WindProcHookDef)(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
auto WinProc_Org = (WindProcHookDef)(&NiApplication::WinProc);
LRESULT CALLBACK WinProc_Hook(HWND hWnd, UINT uiMsg, WPARAM wParam,
    LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uiMsg, wParam, lParam))
        return true;
    return WinProc_Org(hWnd, uiMsg, wParam, lParam);
}

FiestaOnlineTool::FiestaOnlineTool() : NiSample("DeveloperTools bei Set", 1600, 900)
{
    if (!_Tool)
        _Tool = this;

}
bool FiestaOnlineTool::Initialize()
{
    /* 
    Gamebryo doesnt allow to overwrite its WinProc Function
    So i just hook it, aka fuck it.    
    */
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)WinProc_Org, &WinProc_Hook);
    DetourTransactionCommit();

    NiSample::Initialize();
    Sorter = NiNew NiAlphaAccumulator();
    m_spRenderer->SetSorter(Sorter);

    _Scene = NiNew StartScene;

    _Scene->SetupScene(m_spScene, m_spCamera);

    MoveCamera = false;

    this->m_pkFrameRate = NiNew NiFrameRate;
    this->m_pkFrameRate->Init(true);
    this->m_pkFrameRate->SetColor(NiColor::BLACK);
    
    EnableFrameRate(true);
    //this->SetMaxFrameRate(144.0f);

    NiRect<int> kRect;
    kRect.m_left = 0;
    kRect.m_top = 0;
    kRect.m_right = m_spRenderer->GetDefaultBackBuffer()->GetWidth();
    kRect.m_bottom = m_spRenderer->GetDefaultBackBuffer()->GetHeight();

    cursor = NiCursor::Create(this->m_spRenderer, kRect, NiCursor::IMMEDIATE, 8, 10, ".\\FiestaOnlineTool\\NorCursor.tga");
    cursor->SetPosition(0.0f, 320, 240);
    cursor->Show(true);
    ShowCursor(true);

    Pgg_kWinMgr = NiNew PgWinMgr;
    Pgg_kWinMgr->PgInit(m_spRenderer);

    m_spScene->Update(0.0);
    m_spScene->UpdateProperties();
    m_spScene->UpdateEffects();
    m_spCamera->Update(0.0f);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui_ImplWin32_Init(this->GetWindowReference());
    NiDX9Renderer* ptr = (NiDX9Renderer*)&*this->m_spRenderer;
    ImGui_ImplDX9_Init(ptr->GetD3DDevice());

    

    return true;
}
//No Fucking Clue what this it xD
static const GUID s_kInputDemoGUID = { 0xef33646e, 0x82d0, 0x4b8b,
    { 0xab, 0x49, 0x2F, 0xc8, 0xb2, 0x1a, 0x29, 0xff } };

NiActionMapPtr FiestaOnlineTool::CreateNewActionMap(const char* pcName) {
    return NiNew NiActionMap(pcName, (void*)&s_kInputDemoGUID);
}
NiActionMapPtr FiestaOnlineTool::CreateInitActionMap()
{
    NiActionMapPtr ActionMap = CreateNewActionMap("StartScreenActionMap");

    if (!ActionMap->AddAction("MouseMoveActionX", NiAction::MOUSE_AXIS_X,
        NiAction::MOUSE_AXIS_X, 0, 0, 0, 0,
        (void*)FiestaOnlineTool::HandleMouseMovement))
    {
        NiMessageBox::DisplayMessage("Failed to AddAction MouseMoveActionX", "Error");
    }
    if (!ActionMap->AddAction("MouseMoveActionY", NiAction::MOUSE_AXIS_Y,
        NiAction::MOUSE_AXIS_Y, 0, 0, 0, 0,
        (void*)FiestaOnlineTool::HandleMouseMovement))
    {
        NiMessageBox::DisplayMessage("Failed to AddAction MouseMoveActionY", "Error");
    }
    if (!ActionMap->AddAction("MouseMoveActionZ", NiAction::MOUSE_AXIS_Z,
        NiAction::MOUSE_AXIS_Z, 0, 0, 0, 0,
        (void*)FiestaOnlineTool::HandleMouseMovement))
    {
        NiMessageBox::DisplayMessage("Failed to AddAction MouseMoveActionZ", "Error");
    }

    if (!ActionMap->AddAction("MouseClickActionLeft", NiAction::MOUSE_BUTTON_LEFT,
        NiAction::MOUSE_BUTTON_LEFT, NiAction::RETURN_BOOLEAN, 0, 0, 0,
        (void*)FiestaOnlineTool::HandleMouseMovement))
    {
        NiMessageBox::DisplayMessage("Failed to AddAction MouseClickActionLeft", "Error");
    }
    if (!ActionMap->AddAction("MouseClickActionRight", NiAction::MOUSE_BUTTON_RIGHT,
        NiAction::MOUSE_BUTTON_RIGHT, NiAction::RETURN_BOOLEAN, 0, 0, 0,
        (void*)FiestaOnlineTool::HandleMouseMovement))
    {
        NiMessageBox::DisplayMessage("Failed to AddAction MouseClickActionLeft", "Error");
    }

    return ActionMap;
}
bool FiestaOnlineTool::HandleMouseMovement(NiActionData* pkActionData)
{
    switch (pkActionData->GetAppData())
    {
    case NiAction::MOUSE_AXIS_X:
    case NiAction::MOUSE_AXIS_Y:
    case NiAction::MOUSE_AXIS_Z:
        //UtilDebugString("Mouse Moved Check For Hit")
        /*Check For Hits If Something is Hitted Update Frame*/
        FiestaOnlineTool::_Tool->CheckInterfaceForHit();
        break;
    case NiAction::MOUSE_BUTTON_LEFT:

        if (pkActionData->GetDataValue())
            FiestaOnlineTool::_Tool->CheckInterfaceForClick();
        else
            FiestaOnlineTool::_Tool->UpdateInterface();
        break;
    case NiAction::MOUSE_BUTTON_RIGHT:

        if (pkActionData->GetDataValue())
            FiestaOnlineTool::_Tool->EnableCameraMove();
        else
            FiestaOnlineTool::_Tool->DisableCameraMove();
        break;

    default:
        return false;
    }
    return true;
}
typedef bool(*InputFunc)(NiActionData*);
void FiestaOnlineTool::ProcessInput()
{
    NiActionData* pkActionData = m_spInputSystem->PopActionData();

    while (pkActionData)
    {
        InputFunc pfnCallback = (InputFunc)(pkActionData->GetContext());

        assert(pfnCallback);

        if (!pfnCallback(pkActionData))
        {
            NiMessageBox::DisplayMessage("Failed to Execute Input Function", "Error");
        }
        m_spInputSystem->ReleaseActionData(pkActionData);
        pkActionData = m_spInputSystem->PopActionData();

    }
}
NiInputSystem::CreateParams* FiestaOnlineTool::GetInputCreationParameters()
{
    NiDI8InputSystem::DI8CreateParams* pkParams =
        NiNew NiDI8InputSystem::DI8CreateParams;
    NIASSERT(pkParams);

    pkParams->SetOwnerInstance(GetInstanceReference());
    pkParams->SetOwnerWindow(GetWindowReference());
    return pkParams;
}
bool FiestaOnlineTool::CreateInputSystem()
{
    /*
    *Standard Function to Create the Input System
    */
    NiInputSystem::CreateParams* pkParams = GetInputCreationParameters();
    NIASSERT(pkParams);

    pkParams->SetRenderer(m_spRenderer);
    pkParams->SetKeyboardUsage(
        NiInputSystem::FOREGROUND | NiInputSystem::EXCLUSIVE);
    pkParams->SetMouseUsage(
        NiInputSystem::FOREGROUND | NiInputSystem::NONEXCLUSIVE);
    m_spActionMap = CreateInitActionMap();
    pkParams->SetActionMap(m_spActionMap);

    m_spInputSystem = NiInputSystem::Create(pkParams);
    NiDelete pkParams;
    if (!m_spInputSystem)
        return false;
    NiSleep(750);

    // We are just going to stall here...
    bool bGotDevices = false;
    while (!bGotDevices)
    {
        NiInputErr eErr = m_spInputSystem->CheckEnumerationStatus();
        switch (eErr)
        {
        case NIIERR_ENUM_NOTRUNNING:
            NIASSERT(!"EnumerateDevices failed");
            return false;
        case NIIERR_ENUM_FAILED:
            NIASSERT(!"CheckEnumerationStatus> FAILED");
            return false;
        case NIIERR_ENUM_COMPLETE:
            bGotDevices = true;
            break;
        case NIIERR_ENUM_NOTCOMPLETE:
        default:
            break;
        }
    }


    NiInputDevice::Description* pkNiDesc = m_spInputSystem->GetFirstDeviceDesc();
    while (pkNiDesc)
    {
        pkNiDesc = m_spInputSystem->GetNextDeviceDesc();
    }

    // Open the mouse, keyboard, and and game pads
    m_spKeyboard = m_spInputSystem->OpenKeyboard();
    m_spMouse = m_spInputSystem->OpenMouse();
    return true;
}

bool FiestaOnlineTool::CreateRenderer()
{
    NiWindowRef test = this->GetAppWindow()->GetWindowReference();
    return CreateRenderer(test);
}
bool FiestaOnlineTool::CreateRenderer(HWND hWnd)
{
    if (false) //MAYBE I WILL NEVER USE D3DX10 xD So just hardcode it for D3DX9 for now
    {
        NiD3D10Renderer::CreationParameters kParameters(hWnd);

        NiD3D10RendererPtr spD3D10Renderer;

        bool bResult = NiD3D10Renderer::Create(kParameters, spD3D10Renderer);

        if (bResult)
            m_spRenderer = spD3D10Renderer;

        return bResult;
    }
    else
    {
        char Flag = NiDX9Renderer::USE_NOFLAGS;
        //Flag = NiDX9Renderer::USE_STENCIL | NiDX9Renderer::USE_MULTITHREADED | NiDX9Renderer::USE_16BITBUFFERS | NiDX9Renderer::USE_FULLSCREEN;

        m_spRenderer = NiDX9Renderer::Create(1600, 900, Flag, hWnd, NULL, NULL);
        return (m_spRenderer != NULL);
    }
    return false;
}


