#include "FiestaOnlineTool.h"
#include <NiAnimation.h>
#include <NiParticle.h>
#include <NiSystem.h>
#include <NiCollision.h>
#include <NiMain.h>
#include <string>
#include <NiDI8InputSystem.h>
#include <NiDX9Renderer.h>
#pragma comment(lib, "NiDX9Renderer.lib")
#pragma comment(lib, "dxguid.lib")

#include <NiD3D10Renderer.h>
#pragma comment(lib, "NiD3D10Renderer.lib")
#include "PgUtil.h" 

#define MY_PRINTF(...) {char cad[512]; sprintf(cad, __VA_ARGS__);  OutputDebugStringA(cad);}

/*
int PgWin::ms_iScreenRightPos = 1023;
int PgWin::ms_iScreenBottomPos = 767;
unsigned int PgWin::ms_uiScreenWidth = 1024u;
unsigned int PgWin::ms_uiScreenHeight = 768u;
*/

FiestaOnlineTool* FiestaOnlineTool::_Tool = NULL;
NiApplication* NiApplication::Create() {
    return NiNew FiestaOnlineTool;
}

FiestaOnlineTool::FiestaOnlineTool() : NiApplication("DeveloperTools bei Set", 1600, 900)
{
    if (!_Tool)
        _Tool = this;
}
bool FiestaOnlineTool::Initialize() 
{
    NiApplication::Initialize();
    Sorter = NiNew NiAlphaAccumulator();
    m_spRenderer->SetSorter(Sorter);

    NiSortAdjustNode* BaseNode = NiNew NiSortAdjustNode;
    BaseNode->SetSortingMode(NiSortAdjustNode::SORTING_INHERIT);

     NiNodePtr NiN = PgUitl::LoadNifFile("E:/Coding/Core/Core Fiesta/resmenu/account/LoginBackground.nif", 0);

    BaseNode->AttachChild(NiN, 1);
    m_spScene = BaseNode;
    NIASSERT(m_spScene != NULL);
    if (!PgUitl::CatchCamera(m_spScene, &m_spCamera))
    {
        NiMessageBox::DisplayMessage("Failed to Catch Camera", "Error");
        return 0;
    }
    EnableFrameRate(true);
    
    NiRect<int> kRect;
    kRect.m_left = 0;
    kRect.m_top = 0;
    kRect.m_right = m_spRenderer->GetDefaultBackBuffer()->GetWidth();
    kRect.m_bottom = m_spRenderer->GetDefaultBackBuffer()->GetHeight();

    cursor = NiCursor::Create(this->m_spRenderer, kRect, NiCursor::IMMEDIATE, 8, 10, "E:\\Coding\\Core\\Core Fiesta\\resmenu\\cursor\\NorCursor.tga");
    cursor->SetPosition(0.0f, 320, 240);
    cursor->Show(true);
    ShowCursor(false);
    Pgg_kWinMgr = NiNew PgWinMgr;
    Pgg_kWinMgr->PgInit(m_spRenderer);


    LoginInputPanel = PgUitl::LoadNifFile("E:\\Coding\\Core\\Core Fiesta\\resmenu\\account\\LoginServerList.nif", NULL);
    Pgg_kWinMgr->ShowWin(LoginInputPanel);

    m_spScene->Update(0.0);
    m_spScene->UpdateProperties();
    m_spScene->UpdateEffects();
    m_spCamera->Update(0.0f);

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
void FiestaOnlineTool::DrawCursor() 
{
    tagPOINT kPoint;
    GetCursorPos(&kPoint);
    ScreenToClient(this->GetRenderWindowReference(), &kPoint);
    _Tool->cursor->SetPosition(0.0, kPoint.x + 5, kPoint.y + 9);
    cursor->Draw();
}
void FiestaOnlineTool::OnIdle()
{

    m_spScene->Update(NiGetCurrentTimeInSec());
    if(MeasureTime())
    {
        /*Prepare Framerendering*/
        this->UpdateFrame();
        this->BeginFrame();
        m_spRenderer->BeginUsingDefaultRenderTargetGroup(7u);
        /*Set BackgroundColor of Renderer*/
        NiColorA m_kBackGroundColor;
        this->m_spRenderer->SetBackgroundColor(m_kBackGroundColor);

        /*Draw MainScene (GameWorld)*/
        NiDrawScene(m_spCamera, m_spScene, *m_spCuller);
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
//No Fucking Clue what this it xD
static const GUID s_kInputDemoGUID = { 0xef33646e, 0x82d0, 0x4b8b,
    { 0xab, 0x49, 0x2F, 0xc8, 0xb2, 0x1a, 0x29, 0xff } }; 

NiActionMap* FiestaOnlineTool::CreateNewActionMap(const char* pcName) {
    return NiNew NiActionMap(pcName, (void*)&s_kInputDemoGUID);
}
NiActionMap* FiestaOnlineTool::CreateInitActionMap() 
{
    NiActionMap* ActionMap = CreateNewActionMap("StartScreenActionMap");
    /*
    * Mouse Movement is Currently not needed Click and Stuff could be interessting in the Future
    * But updateing Mouse Position is not Needed currently
    * Could be Usefull to move Ingame Camera at one Point
    if (!ActionMap->AddAction("MouseMoveActionSetX", MouseMovementActionCode::XAxis,
        NiAction::MOUSE_AXIS_X, 0, 0, 0, 0,
        (void*)FiestaOnlineTool::HandleMouseMovement)) 
    {
        NiMessageBox::DisplayMessage("Failed to AddAction MouseMoveActionSetX", "Error");
    }
    if (!ActionMap->AddAction("MouseMoveActionSetY", MouseMovementActionCode::YAxis,
        NiAction::MOUSE_AXIS_Y, 0, 0, 0, 0,
        (void*)FiestaOnlineTool::HandleMouseMovement))
        {
            NiMessageBox::DisplayMessage("Failed to AddAction MouseMoveActionSetX", "Error");
    }
    if (!ActionMap->AddAction("MouseMoveActionSetZ", MouseMovementActionCode::ZAxis,
        NiAction::MOUSE_AXIS_Z, 0, 0, 0, 0,
        (void*)FiestaOnlineTool::HandleMouseMovement))
        {
            NiMessageBox::DisplayMessage("Failed to AddAction MouseMoveActionSetX", "Error");
    }*/
    return ActionMap;
}
bool FiestaOnlineTool::HandleMouseMovement(NiActionData* pkActionData) 
{
    switch (pkActionData->GetAppData()) 
    {
    case MouseMovementActionCode::XAxis:
    case MouseMovementActionCode::YAxis:
    case MouseMovementActionCode::ZAxis:
        break;

    default:
        return false;
    }
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
    StartActionMap = CreateInitActionMap();
    EmptyActionMap = CreateNewActionMap("DeadMap");
    pkParams->SetActionMap(StartActionMap);

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

    char acTemp[256];
    NiOutputDebugString("Devices found:\n");

    NiInputDevice::Description* pkNiDesc =
        m_spInputSystem->GetFirstDeviceDesc();
    while (pkNiDesc)
    {
        NiSprintf(acTemp, 256, "    Port %d - %s\n", pkNiDesc->GetPort(),
            pkNiDesc->GetName());
        NiOutputDebugString(acTemp);

        pkNiDesc = m_spInputSystem->GetNextDeviceDesc();
    }

    // Open the mouse, keyboard, and and game pads
    m_spKeyboard = m_spInputSystem->OpenKeyboard();
    m_spMouse = m_spInputSystem->OpenMouse();
    return true;
}