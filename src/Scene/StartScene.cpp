#include "StartScene.h"
#include "PgUtil.h"
#include <NiThread.h>
#include "EditorScene.h"
#include "SHNManager.h"
#include <future>
#include "FiestaOnlineTool.h"
#include "Settings.h"
#include "ImGui/imgui.h"
#include "ImGui/imfilebrowser.h"
#include "SHBDScene.h"
#include <algorithm>
StartScene::StartScene() 
{
    NiSortAdjustNodePtr sortNode = NiNew NiSortAdjustNode;
    sortNode->SetSortingMode(NiSortAdjustNode::SORTING_INHERIT);
    BaseNode = sortNode;
    BaseNode->SetName("Scene BaseNode");


    std::string Path = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\account\\LoginBackground.nif");
    NiNodePtr NiN = PgUtil::LoadNifFile(Path.c_str(), 0);
    BaseNode->AttachChild(NiN, 1);

    SetupScene(BaseNode, Camera);
    CanSwitch = true;
    ShowLoadMenu = true;
    ShowSettingsMenu = false;
    return;
}

void StartScene::DrawImGui() 
{
    FiestaScene::DrawImGui();

    if(ShowLoadMenu)
    {
        ShowMapInfo();
    }
    if (ShowSettingsMenu)
    {
        ShowSettings();
    }
}

void StartScene::ShowMapInfo()
{
    static std::future<void> future;
    auto shn = SHNManager::Get(SHNManager::MapInfoType);
    if (ImGui::Begin("Load MapInfo", &ShowLoadMenu));
    {
        static char buffer[15];
        ImGui::InputText("Filter Maps", buffer, sizeof(buffer));

        if (shn->DrawHeader())
        {
            for (unsigned int i = 0; i < shn->GetRows(); i++)
            {
                MapInfo* info = shn->GetRow<MapInfo>(i);
                std::string MapName = info->MapName;
                std::transform(MapName.begin(), MapName.end(), MapName.begin(), ::tolower);
                std::string BufferStr = buffer;
                std::transform(BufferStr.begin(), BufferStr.end(), BufferStr.begin(), ::tolower);

                if (MapName.find(BufferStr) == std::string::npos)
                    continue;
                shn->DrawRow(i);
                if (!future.valid() || future.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout)
                {
                    if (ImGui::Button(std::string("Load SHMD##" + std::to_string(i)).c_str()))
                    {
                        ImGui::SetWindowFocus("");
                        ShowLoadMenu = false;
                        future = std::async(std::launch::async, [this, shn, i]
                            {
                                MapInfo* info = shn->GetRow<MapInfo>(i);

                                auto LoadedScene = NiNew EditorScene(info);
                                FiestaOnlineTool::UpdateScene(LoadedScene);

                            });

                    }
                    ImGui::SameLine();
                    if (ImGui::Button(std::string("Load SHBD##" + std::to_string(i)).c_str()))
                    {
                        ImGui::SetWindowFocus("");
                        ShowLoadMenu = false;
                        future = std::async(std::launch::async, [this, shn, i]
                            {
                                MapInfo* info = shn->GetRow<MapInfo>(i);

                                auto LoadedScene = NiNew SHBDScene(info);
                                FiestaOnlineTool::UpdateScene(LoadedScene);

                            });

                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }
}

void StartScene::ShowSettings() 
{
    static ImGui::FileBrowser fileDialog(ImGuiFileBrowserFlags_SelectDirectory);
    fileDialog.Display();
    if (fileDialog.HasSelected()) 
    {
        std::string File = fileDialog.GetSelected().string();
        if (File.find(".exe") != std::string::npos || File.find(".bin") != std::string::npos)
        {
            NiMessageBox::DisplayMessage("Please Select the Folder!", "Info");
            fileDialog.ClearSelected();
            return;
        }
        Settings::SetClientPath(File);
        fileDialog.ClearSelected();
    }
    if (ImGui::Begin("Settings", &ShowSettingsMenu));
    {
        std::string ClientPath = "ClientPath: " + PgUtil::CreateFullFilePathFromBaseFolder("");
        ImGui::Text(ClientPath.c_str()); ImGui::SameLine(); 
        if (ImGui::Button("PlaceHolder"))
        {
            fileDialog.SetTitle("Select Game-Client-Folder");
            fileDialog.SetTypeFilters({ ".exe", ".bin" });
            fileDialog.SetPwd(PgUtil::CreateFullFilePathFromBaseFolder(""));
            fileDialog.Open();
        }
        static const char* items[]{"640 x 360","1024 x 576","1138 x 640", "1600 x 900","1920 x 1080", "2560 x 1440"};
        int Selecteditem = 0;

        for (int i = 0; i < IM_ARRAYSIZE(items); i++) 
        {
            if (std::string(items[i]) == Settings::GetResoultion())
                Selecteditem = i;
        }
        if (ImGui::Combo("ScreenResolution", &Selecteditem, items, IM_ARRAYSIZE(items))) 
        {
            Settings::SetResolution(std::string(items[Selecteditem]));
            LogInfo("To apply Resoultion Changes please restart the Tool");
        }
        float fps = FiestaOnlineTool::GetFPSCap();
        if (ImGui::DragFloat("FPS Cap", &fps, 1.f, 30.f, 144.f))
            FiestaOnlineTool::SetFPSCap(fps);
        /*bool FullScreen = Settings::FullScreen();
        if (ImGui::Checkbox("FullScreen", &FullScreen))
        {
            Settings::SetFullScreen(FullScreen);
        }*/
        if (ImGui::Button("Safe Settings"))
        {
            Settings::SaveSettings();
            //FiestaOnlineTool::RecreateRenderer = true;
        }
        ImGui::End();
    }
}

void StartScene::CreateMenuBar() 
{
    if (ImGui::BeginMenu("File")) 
    {
        if (ImGui::MenuItem("Load Map")) 
        {
            ShowLoadMenu = true;
        }
        if (ImGui::MenuItem("Settings"))
        {
            ShowSettingsMenu = true;
        }
        ImGui::EndMenu();
    }
    
}