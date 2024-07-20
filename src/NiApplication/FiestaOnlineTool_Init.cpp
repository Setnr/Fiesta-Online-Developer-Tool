#pragma once
#include "FiestaOnlineTool.h"
#include "FiestaOnlineTool_GeneralHeaders.h"

#include <NiDX9Select.h>
#include <NiD3D10Select.h>
#include "SHNManager.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "EditScene.h"
#include "SetupScene.h"
#include "Settings.h"
#include <NiDX9SystemDesc.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
FiestaOnlineTool::FiestaOnlineTool() : NiApplication("DeveloperTools bei Set", Settings::WindowWidth(), Settings::WindowHeight())
{
    if (!_Tool)
        _Tool = this;
}

bool FiestaOnlineTool::OnDefault(NiEventRef pEventRecord)
{
    ImGui_ImplWin32_WndProcHandler(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
    bool r = NiApplication::OnDefault(pEventRecord);
    return r;
}
bool FiestaOnlineTool::Initialize()
{
    auto Path = PgUtil::CreateFullFilePathFromBaseFolder("");
    bool LoadSetupScene = !Settings::FirstStartUp();
    NiApplication::Initialize();
    Sorter = NiNew NiAlphaAccumulator();
    Sorter->SetObserveNoSortHint(true);
    Sorter->SetSortByClosestPoint(false);
    m_spRenderer->SetSorter(Sorter);

    if (LoadSetupScene)
    {
        EditScene* Scene = NiNew EditScene;
        Scene->ShowLoadMenu();
        _Scene = Scene;
        NiShaderFactory::RegisterErrorCallback(ShaderErrorCallback);
        this->RunShaderParser();
        this->RegisterShaderLibraries();
    }
    else
        _Scene = NiNew StartUpScene;

    //_Scene->SetupScene(m_spScene, m_spCamera);

    MoveCamera = false;

    this->m_pkFrameRate = NiNew NiFrameRate;
    this->m_pkFrameRate->Init(true);
    this->m_pkFrameRate->SetColor(NiColor::BLACK);
    
    //EnableFrameRate(true);
    this->SetMaxFrameRate(Settings::FPSCap());

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
    NiDX9Renderer* ptr = (NiDX9Renderer*)(NiRenderer*)this->m_spRenderer;
    ImGui_ImplDX9_Init(ptr->GetD3DDevice());
    if(LoadSetupScene)
    {
        std::string LoadingScreen = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\loading\\NowLoading.tga");
        PgUtil::LoadingScreen(this->m_spRenderer, LoadingScreen, 0.8f, false);
        SHNManager::Init();
        LoadingScreen = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\loading\\NowLoading.tga");
        PgUtil::LoadingScreen(this->m_spRenderer, LoadingScreen, 1.f, false);
    }
    switch (PgUtil::CheckVersion()) 
    {
    case Version::Status::CantCheck:
        LogError("Cant Check for new Version");
        break;

    case Version::Status::New:
        LogWarning("There is a new Version on Github :)");
        break;
    }
    return true;
}

bool FiestaOnlineTool::CreateRenderer()
{
    char Flag = NiDX9Renderer::USE_NOFLAGS;
    Flag = NiDX9Renderer::USE_MULTITHREADED;//  | NiDX9Renderer::USE_STENCIL | NiDX9Renderer::USE_FULLSCREEN| NiDX9Renderer::USE_16BITBUFFERS;//  

    //m_spRenderer = NiDX9Select::CreateRenderer(this->GetWindowReference(), this->GetWindowReference(), true, this->m_uiBitDepth, this->GetAppWindow()->GetWidth(), this->GetAppWindow()->GetHeight(), this->m_bStencil, this->m_bMultiThread, this->m_bRefRast, this->m_bSWVertex, this->m_bNVPerfHUD, this->m_bFullscreen);

    m_spRenderer = NiDX9Renderer::Create(this->m_pkAppWindow->GetWidth(), this->m_pkAppWindow->GetHeight(), Flag, this->m_pkAppWindow->GetWindowReference(), this->m_pkAppWindow->GetRenderWindowReference());
    //NiDX9Renderer* rend = (NiDX9Renderer*)&*m_spRenderer;
    //rend->CreateSwapChainRenderTargetGroup(Flag,)
    if (m_spRenderer == NULL)
        NiMessageBox::DisplayMessage("Failed to Create Renderer, \ntry to downscale your Resolution in Settings.ini", "Error");
    if (Settings::FullScreen())
    {
        Flag = Flag | NiDX9Renderer::USE_FULLSCREEN;
        
    }
    if (m_spRenderer != NULL)
    {
        std::string LoadingScreen = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\loading\\NowLoading.tga");
        PgUtil::LoadingScreen(this->m_spRenderer, LoadingScreen, .20f, false);
    }
    return (m_spRenderer != NULL);
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



#ifdef _COMPILENILIB
FILE _iob[3];
extern "C" FILE * __cdecl __iob_func(void)
{
    _iob[0] = *stdin;

    _iob[0] = *stderr;

    _iob[0] = *stdout;

    return _iob;

}
#endif

unsigned int FiestaOnlineTool::RunParser(const char* pcLibFile, NiRenderer* pkRenderer, const char* pcDirectory, bool bRecurseSubFolders)
{
#ifdef _COMPILENILIB
    return NSFParserLib_RunShaderParser(pcDirectory, bRecurseSubFolders);
#else
    return true;
#endif
}
unsigned int FiestaOnlineTool::ShaderErrorCallback(const char* pcError,
    NiShaderError eError, bool bRecoverable)
{
    NiMessageBox(pcError, "Shader Error");
    LogError(pcError);

    return 0;
}
bool FiestaOnlineTool::RunShaderParser()
{
#ifdef _COMPILENILIB
    std::string acProgramPath = PgUtil::CreateFullFilePathFromBaseFolder(".\\shader\\");

    NiShaderFactory::RegisterRunParserCallback(RunParser);
    unsigned int uiCount = NiShaderFactory::LoadAndRunParserLibrary(0, acProgramPath.c_str(), true);
    return true;
#else
    std::string acProgramPath = PgUtil::CreateFullFilePathFromBaseFolder(".\\shader\\");

    if (!NiShaderFactory::LoadAndRunParserLibrary(".\\NSFParserLibDX9" NI_DLL_SUFFIX ".dll",
        acProgramPath.c_str(), true))
    {
        NiMessageBox("Failed to load shader library!", "ERROR");
        return false;
    }
    return true;
#endif

}

bool FiestaOnlineTool::RegisterShaderLibraries() 
{
#ifdef _COMPILENILIB
    std::string acProgramPath = PgUtil::CreateFullFilePathFromBaseFolder(".\\shader\\");

    NiShaderFactory::RegisterClassCreationCallback(LibraryClassCreate);
    char* pacShaderDir = (char*)acProgramPath.c_str();
    NiShaderFactory::AddShaderProgramFileDirectory(pacShaderDir);

    if (!NiShaderFactory::LoadAndRegisterShaderLibrary("NSBShaderLibDX9" NI_DLL_SUFFIX ".dll",1, &pacShaderDir, true))
    {
        NiMessageBox("Failed to load shader library!", "ERROR");
        return false;
    }

    auto inst = NiD3DShaderProgramFactory::GetInstance();
    inst->AddProgramDirectory(acProgramPath.c_str());

    NiShaderFactory::RegisterClassCreationCallback(EffectLibraryClassCreate);
    if (!NiShaderFactory::LoadAndRegisterShaderLibrary("NiD3DXEffectShaderLibDX9" NI_DLL_SUFFIX ".dll", //".\\NiD3DXEffectShaderLibDX9" NI_DLL_SUFFIX ".dll"
        1, &pacShaderDir, true))
    {
        NiMessageBox("Failed to load shader library!", "ERROR");
        return false;
    }
    return true;
#else
    int iDirectoryCount = 1;
    char* apcDirectories[1];

    apcDirectories[0] = (char*)PgUtil::FolderPath.c_str();
    std::string acProgramPath = PgUtil::CreateFullFilePathFromBaseFolder(".\\shader\\");

    NiShaderFactory::AddShaderProgramFileDirectory(acProgramPath.c_str());


    if (!NiShaderFactory::LoadAndRegisterShaderLibrary(".\\NSBShaderLibDX9" NI_DLL_SUFFIX ".dll",
        iDirectoryCount, apcDirectories, true))
    {
        NiMessageBox("Failed to load shader library!", "ERROR");
        return false;
    }
    if (!NiShaderFactory::LoadAndRegisterShaderLibrary(".\\NiD3DXEffectShaderLibDX9" NI_DLL_SUFFIX ".dll",
        iDirectoryCount, apcDirectories, true))
    {
        NiMessageBox("Failed to load shader library!", "ERROR");
        return false;
    }
    return true;
#endif
}

bool FiestaOnlineTool::LibraryClassCreate(const char* pcLibFile,
    NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[],
    bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary)
{
    *ppkLibrary = NULL;
#ifdef _COMPILENILIB
    return NSBShaderLib_LoadShaderLibrary(pkRenderer, iDirectoryCount,
        apcDirectories, bRecurseSubFolders, ppkLibrary);
#else
    return true;
#endif

}
bool FiestaOnlineTool::EffectLibraryClassCreate(const char* pcLibFile,
    NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[],
    bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary)
{
    *ppkLibrary = NULL;
#ifdef _COMPILENILIB
    return NiD3DXEffectShaderLib_LoadShaderLibrary(pkRenderer, iDirectoryCount,
        apcDirectories, bRecurseSubFolders, ppkLibrary);
#else
return true;
#endif
}

NiScreenElements* PgUtil::CreateScreenElement(float width, float height, NiSourceTexturePtr texture)
{
    NiScreenElements* pkScreenTexture = NiNew NiScreenElements(NiNew NiScreenElementsData(false, true, 1));
        pkScreenTexture->Insert(4);
        pkScreenTexture->SetRectangle(0, 0.4f, 0.4f, 0.8f, 0.8f);
        pkScreenTexture->UpdateBound();
        pkScreenTexture->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);
        pkScreenTexture->SetColors(0, NiColorA::WHITE);
        auto& io = ImGui::GetIO();

        float fLeft = 0.0f;
        float fBottom = 0.0f;
        float fRight = width / io.DisplaySize.x + fLeft;
        float fTop = height / io.DisplaySize.y + fBottom;
        pkScreenTexture->SetVertex(0, 0, NiPoint2(fLeft, 1.0f - fBottom));
        pkScreenTexture->SetVertex(0, 1, NiPoint2(fRight, 1.0f - fBottom));
        pkScreenTexture->SetVertex(0, 2, NiPoint2(fRight, 1.0f - fTop));
        pkScreenTexture->SetVertex(0, 3, NiPoint2(fLeft, 1.0f - fTop));
        pkScreenTexture->UpdateBound();

        NiTexturingPropertyPtr pTexture = NiNew NiTexturingProperty();
        pTexture->SetBaseTexture(texture);
        pTexture->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
        pTexture->SetBaseFilterMode(NiTexturingProperty::FILTER_BILERP);

        auto transform = NiNew NiTextureTransform();
        //transform->SetScale(NiPoint2(-1.f, 1.f));
        transform->SetRotate(-3.14159 / 2);
        //transform->SetTransformMethod(NiTextureTransform::MAYA_DEPRECATED_TRANSFORM);

        pTexture->SetBaseTextureTransform(transform);
        pkScreenTexture->AttachProperty(pTexture);

        NiVertexColorProperty* pVertex = NiNew NiVertexColorProperty;
        pVertex->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
        pVertex->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
        pkScreenTexture->AttachProperty(pVertex);

        NiAlphaProperty* pAlpha = NiNew NiAlphaProperty;
        pAlpha->SetAlphaBlending(true);
        pAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
        pkScreenTexture->AttachProperty(pAlpha);

        pkScreenTexture->UpdateProperties();
        pkScreenTexture->UpdateEffects();
        pkScreenTexture->Update(0.0f);
        return pkScreenTexture;
}
bool PgUtil::HoveringScreenElement(NiScreenElements* pElement, float& x, float& y)
{
    if (!pElement)
        return false;
    long xCoord, yCoord;
    if (FiestaOnlineTool::GetMousePosition(xCoord, yCoord))
    {
        unsigned int Width, Height;
        if (FiestaOnlineTool::GetResolution(Width, Height))
        {
            float XPos = static_cast<float>(xCoord) / static_cast<float>(Width);
            float YPos = static_cast<float>(yCoord) / static_cast<float>(Height);
            float fLeft, fTop, fWidth, fHeight;
            pElement->GetRectangle(0, fLeft, fTop, fWidth, fHeight);
            if (fLeft <= XPos && fLeft + fWidth >= XPos && fTop + fHeight <= YPos && fTop >= YPos)
            {
                x = (XPos - fLeft) / fWidth; 
                y =  1.f - ((fTop - YPos) / (fHeight * -1.f));
                return true;
            }
        }
    }
    return false;
}