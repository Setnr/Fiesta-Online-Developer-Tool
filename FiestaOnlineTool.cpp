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
#include "PgWin.h"

#include <Windows.h>
#include "StartScene.h"

#include <NiD3DShaderProgramFactory.h>

#define _LIB
#include <NSBShaderLib.h>
#include <NSFParserLib.h>
#include <NiD3DXEffectShaderLib.h>
#undef _LIB

#if defined(WIN32)
#pragma comment(lib, "NiBinaryShaderLibDX9.lib")
#pragma comment(lib, "NiD3D10BinaryShaderLibD3D10.lib")
#pragma comment(lib, "NSBShaderLibDX9.lib")
#pragma comment(lib, "NSFParserLibDX9.lib")
#pragma comment(lib, "NiD3DXEffectShaderLibDX9.lib")
#endif

int PgWinMgr::iScreenLeftPos;
int PgWinMgr::iScreenRightPos;

int PgWinMgr::iScreenWidth;
int PgWinMgr::iScreenHeight;

int PgWinMgr::iScreenTopPos;
int PgWinMgr::iScreenBottomPos;

char PgUtil::FolderPath[512];

FiestaOnlineTool* FiestaOnlineTool::_Tool = NULL;
NiApplication* NiApplication::Create() 
{
    GetCurrentDirectoryA(sizeof(PgUtil::FolderPath), PgUtil::FolderPath);
    NiSourceTexture::SetDestroyAppDataFlag(false);
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

    _Scene = NiNew StartScene;

    _Scene->SetupScene(m_spScene, m_spCamera);

    MoveCamera = false;

    this->m_pkFrameRate = NiNew NiFrameRate;
    this->m_pkFrameRate->Init(true);
    this->m_pkFrameRate->SetColor(NiColor::BLACK);
    EnableFrameRate(true);
    this->SetMaxFrameRate(144.0f);

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

    Engine3D::Init(NULL);

    return true;
}

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
        
        if(pkActionData->GetDataValue())
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

    NiOutputDebugString("Devices found:\n");

    NiInputDevice::Description* pkNiDesc = m_spInputSystem->GetFirstDeviceDesc();
    while (pkNiDesc)
    {
        UtilDebugString("    Port %d - %s\n", pkNiDesc->GetPort(), pkNiDesc->GetName());
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
void FiestaOnlineTool::DrawCursor()
{
    tagPOINT kPoint;
    GetCursorPos(&kPoint);
    ScreenToClient(this->GetRenderWindowReference(), &kPoint);
    _Tool->cursor->SetPosition(0.0, kPoint.x + 5, kPoint.y + 9);
    cursor->Draw();
}

void Engine3D::ShaderRunParserCallback(const char* pcLibFile, NiRenderer* pkRenderer, const char* pcDirectory, bool bRecurseSubFolders) 
{
    NSFParserLib_RunShaderParser(pcDirectory, bRecurseSubFolders);
}
BOOL Engine3D::ShaderClassCreateCallback(const char* pcLibFile, NiRenderer* pkRenderer, int iDirectoryCount, char** apcDirectories, bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary) 
{
    *ppkLibrary = 0;
    return NSBShaderLib_LoadShaderLibrary(pkRenderer, iDirectoryCount, apcDirectories, bRecurseSubFolders, ppkLibrary);
}
bool Engine3D::FXLibraryClassCreate(const char* pcLibFile, NiRenderer* pkRenderer, int iDirectoryCount, char** apcDirectories, bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary)
{
    *ppkLibrary = 0;
    return NiD3DXEffectShaderLib_LoadShaderLibrary(
        pkRenderer,
        iDirectoryCount,
        apcDirectories,
        bRecurseSubFolders,
        ppkLibrary);
}
bool Engine3D::GetEnableCarToon() 
{
    bool result = 0; // al

    //if (Engine3D::ms_spToonExtraData.m_pObject)
    //    result = Engine3D::ms_spToonExtraData.m_pObject->m_bOn;
    return result;
}
void Engine3D::EnableCarToon(bool bEnable)
{
    //if (Engine3D::ms_spToonExtraData.m_pObject)
    //    Engine3D::ms_spToonExtraData.m_pObject->m_bOn = bEnable;
}
bool Engine3D::Terminate() { return true; }
char Engine3D::Init(NiCamera* pkCamera) 
{
    NiShaderFactory::RegisterRunParserCallback((unsigned int(__cdecl*)(const char*, NiRenderer*, const char*, bool))Engine3D::ShaderRunParserCallback);
    NiShaderFactory::RegisterClassCreationCallback((bool(__cdecl*)(const char*, NiRenderer*, int, char**, bool, NiShaderLibrary**))Engine3D::ShaderClassCreateCallback);
    NiShaderFactory::RegisterErrorCallback(ShaderErrorCallback);
    NiD3DShaderProgramFactory* factory = NiD3DShaderProgramFactory::GetInstance();
    char acFolder[] = ".\\shader\\";
    char* pacShaderDir[2];
    pacShaderDir[0] = acFolder;
    pacShaderDir[1] = 0;
    factory->AddProgramDirectory(pacShaderDir[0]);
    if (!NiShaderFactory::LoadAndRunParserLibrary(0, acFolder, 1))
        MessageBoxA(0, "LoadAndRunParserLibrary 1", "Error", 0);
    if (!NiShaderFactory::LoadAndRegisterShaderLibrary(0, 1, pacShaderDir, 1))
        MessageBoxA(0, "LoadAndRunParserLibrary 2", "Error", 0);
    NiShaderFactory::RegisterClassCreationCallback(Engine3D::FXLibraryClassCreate);
    if (!NiShaderFactory::LoadAndRegisterShaderLibrary(0, 1, pacShaderDir, 1))
        MessageBoxA(0, "LoadAndRunParserLibrary 3", "Error", 0);
    return 1;
}