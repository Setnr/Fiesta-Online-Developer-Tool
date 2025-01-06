#include "FiestaOnlineTool.h"

#include <PgUtil.h>
#include <NiDX9Renderer.h>
#include <NiHeaders.h>

#include <Settings.h>

#include <Logger.h>

#include <wininet.h>
#include "../Global/Version/Version.h"
#include <InitializeScene/InitializeScene.h>
#include <EditorScene/EditorScene.h>
#include "ScreenElements/LoadObject/AddSingleObject.h"
#include <NiSystemCursor.h>

#include <SHN/SHNManager.h>

#include <detours/detours.h>
#include "Data/NiCustom/NiBoundingBox.h"
NiCursorPtr FiestaOnlineTool::cursor;
POINT FiestaOnlineTool::MousePos;
NiNodePtr FiestaOnlineTool::BoundingBox;


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
#ifdef _WIN64 
static const char* ApplicaitonName = "DeveloperTools bei Set(x64)";
#else
static const char* ApplicaitonName = "DeveloperTools bei Set(x86)";
#endif
FiestaOnlineTool::FiestaOnlineTool() : NiApplication(ApplicaitonName, 1600, 900)
{
  
}
void FiestaOnlineTool::ProcessInput()
{
    ProcessVisualTrackerInput();
    GetCursorPos(&MousePos);
    ScreenToClient(GetRenderWindowReference(), &MousePos);
}
bool FiestaOnlineTool::OnDefault(NiEventRef pEventRecord)
{
    ImGui_ImplWin32_WndProcHandler(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
    bool r = NiApplication::OnDefault(pEventRecord);
    return r;
}
bool FiestaOnlineTool::Initialize()
{
    if(!Settings::Load())
        NiMessageBox("Failed to Load Settings!\nContinue with DefaultSettings", "Settings Error");

    AddSingleObject::SetInitPath(PgUtil::PathFromClientFolder(""));

    std::thread SHNThread(StartSHNLoadingThread);
    SHNThread.detach();

    std::thread VersionCheck(StartVersionCheckThread);
    VersionCheck.detach();

	NiApplication::Initialize();

    Sorter = NiNew NiAlphaAccumulator();
    Sorter->SetObserveNoSortHint(true);
    Sorter->SetSortByClosestPoint(false);
    m_spRenderer->SetSorter(Sorter);

    this->SetShowAllTrackers(true);

    this->m_pkFrameRate = NiNew NiFrameRate;
    this->m_pkFrameRate->Init(true);
    this->m_pkFrameRate->SetColor(NiColor::BLACK);
    this->SetMaxFrameRate(144.f);
    NiRect<int> kRect;
    kRect.m_left = 0;
    kRect.m_top = 0;
    kRect.m_right = m_spRenderer->GetDefaultBackBuffer()->GetWidth();
    kRect.m_bottom = m_spRenderer->GetDefaultBackBuffer()->GetHeight();

    std::string FilePath = PgUtil::PathFromApplicationFolder(".\\FiestaOnlineTool\\NorCursor.tga");

    cursor = cursor = NiCursor::Create(this->m_spRenderer, kRect, NiCursor::IMMEDIATE, 8, 10, FilePath.c_str());//NiSystemCursor::Create(this->m_spRenderer, kRect, NiCursor::IMMEDIATE, NiSystemCursor::SystemCursor::STANDARD, FilePath.c_str());

    cursor->SetPosition(0.0f, 320, 240);
    cursor->Show(true);
    ShowCursor(false);
      
    LoadInitialScene();
    if(_Scene)
        _Scene->SetupScene();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    static std::string ImGuiIniFile = PgUtil::PathFromApplicationFolder(".\\FiestaOnlineTool\\imgui.ini");
    io.IniFilename = ImGuiIniFile.c_str();
    ImGui::LoadIniSettingsFromDisk(io.IniFilename);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui_ImplWin32_Init(this->GetRenderWindowReference());
    NiDX9Renderer* ptr = (NiDX9Renderer*)(NiRenderer*)this->m_spRenderer;
    ImGui_ImplDX9_Init(ptr->GetD3DDevice());

    BoundingBox = NiBoundingBox::LoadBaseNode();

    return true;
}  

void FiestaOnlineTool::StartSHNLoadingThread()
{
    SHN::SHNManager::Load();
}

void FiestaOnlineTool::StartVersionCheckThread()
{
    Version Cur(Version::CurVersion);
    DWORD dwFlags;
    if (!InternetGetConnectedState(&dwFlags, 0))
    {
        LogInfo("Cant check Version!\n Please check your connection.");
        return;
    }

    Version latest(GetLatestVersion());
    if (Cur < latest)
        LogInfo("A New Version is available download at: \nhttps://github.com/Setnr/Fiesta-Online-Developer-Tool");
    else
        LogInfo("No New Version available");
}

bool FiestaOnlineTool::CreateRenderer()
{
    char Flag = NiDX9Renderer::USE_NOFLAGS;
    Flag = NiDX9Renderer::USE_MULTITHREADED | NiDX9Renderer::USE_STENCIL;// | NiDX9Renderer::USE_FULLSCREEN| NiDX9Renderer::USE_16BITBUFFERS;//  

    if (Settings::IsFullScreen())
    {

        auto WindowWitdh = GetSystemMetrics(0);
        auto WindowHeight = GetSystemMetrics(1);

        m_pkAppWindow->SetWidth(WindowWitdh);
        m_pkAppWindow->SetHeight(WindowHeight);
        _devicemodeA dm;
        dm.dmSize = 156;
        dm.dmFields = 1835008;
        dm.dmBitsPerPel = 32;
        dm.dmPelsWidth = m_pkAppWindow->GetWidth();
        dm.dmPelsHeight = m_pkAppWindow->GetHeight();
        auto v4 = GetWindowLongA(m_pkAppWindow->GetWindowReference(), -16);
        SetWindowLongA(m_pkAppWindow->GetWindowReference(), -16, v4 & 0xFF37FFFF | 0x80);
        if (!ChangeDisplaySettingsA(&dm, 2u))
        {
            bool _IsWindow = false;

            if (ChangeDisplaySettingsA(&dm, 4u))
                ChangeDisplaySettingsA(&dm, 0);
            ShowWindow(m_pkAppWindow->GetWindowReference(), 3);

            if (m_pkAppWindow->GetNumStatusPanes() && m_pkAppWindow->GetStatusWindowReference() != 0)
                _IsWindow = IsWindow(m_pkAppWindow->GetWindowReference());
            if (_IsWindow)
            {
                tagRECT statusRect;
                GetWindowRect(m_pkAppWindow->GetWindowReference(), &statusRect);
                WindowHeight = statusRect.bottom - statusRect.top;
                ShowWindow(m_pkAppWindow->GetWindowReference(), 0);
            }

            SetWindowPos(m_pkAppWindow->GetWindowReference(), 0, 0, 0, m_pkAppWindow->GetWidth(), m_pkAppWindow->GetHeight() + WindowHeight, 0);
        }
    }
    m_spRenderer = NiDX9Renderer::Create(m_pkAppWindow->GetWidth(), m_pkAppWindow->GetHeight(), Flag, m_pkAppWindow->GetWindowReference(), m_pkAppWindow->GetRenderWindowReference());

    if (m_spRenderer == NULL)
    {
        NiMessageBox::DisplayMessage("Failed to Create Renderer, \ntry to downscale your Resolution in Settings.ini", "Error");
        NiMessageBox::DisplayMessage(NiRenderer::GetLastErrorString(), "Msg");
    }
    return (m_spRenderer != NULL);
}

void FiestaOnlineTool::LoadInitialScene()
{
    if (!Settings::IsClientSetup())
    {
        _Scene = NiNew InitializeScene;
        return;
    }
    _Scene = NiNew EditorScene;
    NiShaderFactory::RegisterErrorCallback(ShaderErrorCallback);
    RunShaderParser();
    RegisterShaderLibraries();

    std::thread SHNThread(StartSHNLoadingThread);
    SHNThread.detach();
}


// the _iob Struct got changed back in the days now we need to do a scuffed workaround other wise parserload crashes because a borked write
#ifdef _COMPILENILIB
FILE _iob[3];
extern "C" FILE* __cdecl __iob_func(void)
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

    return 0;
}
bool FiestaOnlineTool::RunShaderParser()
{
#ifdef _COMPILENILIB
    std::string acProgramPath = PgUtil::PathFromClientFolder(".\\shader\\");

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
    std::string acProgramPath = PgUtil::PathFromClientFolder(".\\shader\\");

    NiShaderFactory::RegisterClassCreationCallback(LibraryClassCreate);
    char* pacShaderDir = (char*)acProgramPath.c_str();
    NiShaderFactory::AddShaderProgramFileDirectory(pacShaderDir);

    if (!NiShaderFactory::LoadAndRegisterShaderLibrary("NSBShaderLibDX9" NI_DLL_SUFFIX ".dll", 1, &pacShaderDir, true))
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