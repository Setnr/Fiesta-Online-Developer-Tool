#pragma once
#include "FiestaOnlineTool.h"
#include "FiestaOnlineTool_GeneralHeaders.h"

#include <NiDX9Select.h>
#include <NiD3D10Select.h>
#include "SHNManager.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "SetupScene.h"
class DLLLoader
{

};
NiDeclareSDM(DLLLoader, )
bool DLLLoaderSDM::ms_bInitialized = false;
DLLLoaderSDM::DLLLoaderSDM() 
{
    char Path[513];
    GetModuleFileNameA(NULL, Path, sizeof(Path));
    std::string s = std::filesystem::path(Path).parent_path().string();
#ifdef NIRELEASE
    AddDllDirectory(L"\\FiestaOnlineTool\\Dlls");
#endif
#ifdef NIDEBUG
    s += "\\DebugDLLS";
#endif

    SetDllDirectoryA(s.c_str());
    static int siCounter = 0; 
    if (siCounter++ == 0) {
        NiStaticDataManager::AddLibrary(DLLLoaderSDM::Init, DLLLoaderSDM::Shutdown);
    }
}

void DLLLoaderSDM::Init()
{
    char Path[513];
    GetModuleFileNameA(NULL, Path, sizeof(Path));
    std::string s = std::filesystem::path(Path).parent_path().string();
#ifdef NIRELEASE
    AddDllDirectory(L"\\FiestaOnlineTool\\Dlls");
#endif
#ifdef NIDEBUG
    s += "\\DebugDLLS";
#endif

    SetDllDirectoryA(s.c_str());
}
void DLLLoaderSDM::Shutdown()
{

}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
FiestaOnlineTool::FiestaOnlineTool() : NiApplication("DeveloperTools bei Set", 1600, 900)
{
    if (!_Tool)
        _Tool = this;
}

bool FiestaOnlineTool::OnDefault(NiEventRef pEventRecord)
{
    bool r = NiApplication::OnDefault(pEventRecord);
    ImGui_ImplWin32_WndProcHandler(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
    return r;
}
bool FiestaOnlineTool::Initialize()
{
    LoadSettings();
    bool LoadSetupScene = PgUtil::CreateFullFilePathFromBaseFolder("") != "";
    NiApplication::Initialize();
    Sorter = NiNew NiAlphaAccumulator();
    Sorter->SetObserveNoSortHint(true);
    Sorter->SetSortByClosestPoint(false);
    m_spRenderer->SetSorter(Sorter);

    if (LoadSetupScene)
    {
        _Scene = NiNew StartScene;
        this->RegisterShaderParsers();
        this->RegisterShaderLibraries();
    }
    else
        _Scene = NiNew StartUpScene;

    _Scene->SetupScene(m_spScene, m_spCamera);

    MoveCamera = false;

    this->m_pkFrameRate = NiNew NiFrameRate;
    this->m_pkFrameRate->Init(true);
    this->m_pkFrameRate->SetColor(NiColor::BLACK);
    
    //EnableFrameRate(true);
    this->SetMaxFrameRate(144.0f);

    NiRect<int> kRect;
    kRect.m_left = 0;
    kRect.m_top = 0;
    kRect.m_right = m_spRenderer->GetDefaultBackBuffer()->GetWidth();
    kRect.m_bottom = m_spRenderer->GetDefaultBackBuffer()->GetHeight();

    std::string FilePath = PgUtil::CreateFullFilePathFromApplicationFolder(".\\FiestaOnlineTool\\NorCursor.tga");
    cursor = NiCursor::Create(this->m_spRenderer, kRect, NiCursor::IMMEDIATE, 8, 10, FilePath.c_str());
    cursor->SetPosition(0.0f, 320, 240);
    cursor->Show(true);
    ShowCursor(false);

    Pgg_kWinMgr = NiNew PgWinMgr;
    Pgg_kWinMgr->PgInit(m_spRenderer);

    m_spScene->Update(0.0);
    m_spScene->UpdateProperties();
    m_spScene->UpdateEffects();
    m_spCamera->Update(0.0f);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    static std::string ImGuiIniFile = PgUtil::CreateFullFilePathFromApplicationFolder(".\\FiestaOnlineTool\\imgui.ini");
    io.IniFilename = ImGuiIniFile.c_str();
    ImGui::LoadIniSettingsFromDisk(io.IniFilename);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui_ImplWin32_Init(this->GetRenderWindowReference());
    NiDX9Renderer* ptr = (NiDX9Renderer*)&*this->m_spRenderer;
    ImGui_ImplDX9_Init(ptr->GetD3DDevice());
    if(LoadSetupScene)
    {
        std::string LoadingScreen = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\loading\\NowLoading.tga");
        PgUtil::LoadingScreen(this->m_spRenderer, LoadingScreen, 0.8f, false);
        SHNManager::Init();
        LoadingScreen = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\loading\\NowLoading.tga");
        PgUtil::LoadingScreen(this->m_spRenderer, LoadingScreen, 1.f, false);
    }
    return true;
}

bool FiestaOnlineTool::CreateRenderer()
{
    if (false) //MAYBE I WILL NEVER USE D3DX10 xD So just hardcode it for D3DX9 for now
    {
        m_spRenderer = NiD3D10Select::CreateRenderer(this->GetWindowReference(), false, this->GetAppWindow()->GetWidth(), this->GetAppWindow()->GetHeight(), this->m_bMultiThread, this->m_bRefRast, this->m_bFullscreen);
        return (m_spRenderer != NULL);
    }
    else
    {
        char Flag = NiDX9Renderer::USE_NOFLAGS;
        Flag = NiDX9Renderer::USE_MULTITHREADED  ; //NiDX9Renderer::USE_STENCIL| NiDX9Renderer::USE_16BITBUFFERS | NiDX9Renderer::USE_FULLSCREEN;
        //m_spRenderer = NiDX9Select::CreateRenderer(this->GetWindowReference(), this->GetWindowReference(), true, this->m_uiBitDepth, this->GetAppWindow()->GetWidth(), this->GetAppWindow()->GetHeight(), this->m_bStencil, this->m_bMultiThread, this->m_bRefRast, this->m_bSWVertex, this->m_bNVPerfHUD, this->m_bFullscreen);
        m_spRenderer = NiDX9Renderer::Create(this->m_pkAppWindow->GetWidth(), this->m_pkAppWindow->GetHeight(), Flag, this->m_pkAppWindow->GetWindowReference(), this->m_pkAppWindow->GetRenderWindowReference());
        if (m_spRenderer != NULL) 
        {
            std::string LoadingScreen = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\loading\\NowLoading.tga");
            PgUtil::LoadingScreen(this->m_spRenderer, LoadingScreen, .20f,false);
        }
        return (m_spRenderer != NULL);
    }
    return false;
}
void PgUtil::LoadingScreen(NiRenderer* Renderer, std::string LoadingScreen, float Percent, bool Map = true)
{

    NiColorA m_kBackGroundColor;
    Renderer->SetBackgroundColor(m_kBackGroundColor);

    NiTexture::FormatPrefs kPrefs;
    kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;

    NiTexture* pkBGTexture = NiSourceTexture::Create(LoadingScreen.c_str(), kPrefs);
    if (!pkBGTexture)
        return;

    NiScreenElementsPtr pkScreenPoly = NiNew NiScreenElements(
        NiNew NiScreenElementsData(false, false, 1));
    if (!pkScreenPoly)
        return;

    // We know that the polygon handle is zero and will use it directly in the
    // vertex and texture coordinate assignments.
    pkScreenPoly->Insert(4);

    // Account for overscan for consoles.
    pkScreenPoly->SetRectangle(0, 0.f, 0.f, 1.f, 1.f);

    pkScreenPoly->UpdateBound();
    pkScreenPoly->SetTextures(0, 0, 0.f, 0.f, 1.f, 1.f);

    NiTexturingPropertyPtr pkTextProp = NiNew NiTexturingProperty;
    if (!pkTextProp)
        return;

    pkTextProp->SetBaseTexture(pkBGTexture);
    pkTextProp->SetBaseFilterMode(NiTexturingProperty::FILTER_BILERP);
    pkTextProp->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
    pkTextProp->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
    pkScreenPoly->AttachProperty(pkTextProp);

    pkScreenPoly->UpdateProperties();
    pkScreenPoly->Update(0.0f);

    NiScreenElementsPtr pkScreenProgressBar = CreateProgressbar(Map,Percent);


    Renderer->BeginFrame();
    Renderer->BeginUsingDefaultRenderTargetGroup(7u);
    Renderer->SetScreenSpaceCameraData();

    pkScreenPoly->Draw(Renderer);
    if(pkScreenProgressBar)
        pkScreenProgressBar->Draw(Renderer);

    Renderer->EndUsingRenderTargetGroup();
    Renderer->EndFrame();
    Renderer->DisplayFrame();

}
NiScreenElements* PgUtil::CreateProgressbar(bool Map, float Percent)
{
    NiTexture::FormatPrefs kPrefs;
    kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;
    std::string LoadingScreen;
    if (Map)
        LoadingScreen = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\loading\\NowLoadingTextColor.tga");
    else
        LoadingScreen = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\loading\\ProgressBar.tga");

    NiTexture* pkBGTexture = NiSourceTexture::Create(LoadingScreen.c_str(), kPrefs);
    if (!pkBGTexture)
        return NULL;

    NiScreenElements* pkScreenPoly = NiNew NiScreenElements(
        NiNew NiScreenElementsData(false, false, 1));
    if (!pkScreenPoly)
        return NULL;

    // We know that the polygon handle is zero and will use it directly in the
    // vertex and texture coordinate assignments.
    pkScreenPoly->Insert(4);

    // Account for overscan for consoles.
    //pkScreenPoly->SetRectangle(0, 0.01f, 0.01f, 0.98f, 0.98f);
    if (Map)
    {
        pkScreenPoly->SetRectangle(0, 0.0, 0.87900001, Percent, 0.121f);

        pkScreenPoly->UpdateBound();
        pkScreenPoly->SetTextures(0, 0, 0.f, 0.f, 1.f, 1.f);
    }
    else 
    {
        pkScreenPoly->SetRectangle(0, 0.27734751, 0.75520003, Percent * 0.5f, 0.015f);

        pkScreenPoly->UpdateBound();
        pkScreenPoly->SetTextures(0, 0, 0.f, 0.f, 1.f, 1.f);
    }

    
    NiTexturingPropertyPtr pkTextProp = NiNew NiTexturingProperty;
    if (!pkTextProp)
        return NULL;

    pkTextProp->SetBaseTexture(pkBGTexture);
    pkTextProp->SetBaseFilterMode(NiTexturingProperty::FILTER_BILERP);
    pkTextProp->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
    pkTextProp->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
    pkScreenPoly->AttachProperty(pkTextProp);

    pkScreenPoly->UpdateProperties();
    pkScreenPoly->Update(0.0f);

    return pkScreenPoly;
}

void FiestaOnlineTool::LoadSettings() 
{
    std::ifstream Settings;
    std::string Path = PgUtil::CreateFullFilePathFromApplicationFolder(SettingsPath);
    if (!std::filesystem::exists(Path)) 
    {
        NiMessageBox::DisplayMessage("Settings-File does not exist", "Error");
        return;
    }
    Settings.open(Path, std::ios::in);

    if (!Settings.is_open())
    {
        NiMessageBox::DisplayMessage("Failed to open Settings-File", "Error");
        return;
    }
    std::string line;
    while (Settings >> line) 
    {
        if (line == "#ClientPath")
        {
            Settings >> PgUtil::FolderPath;
        }

    }
}