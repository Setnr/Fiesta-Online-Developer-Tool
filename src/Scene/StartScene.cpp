#include "StartScene.h"
#include "PgUtil.h"
#include <NiThread.h>
#include "EditorScene.h"
#include "SHNManager.h"
#include <future>
#include "FiestaOnlineTool.h"

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
    return;
}

void StartScene::DrawImGui() 
{
    FiestaScene::DrawImGui();
    static std::future<void> future;
    auto shn = SHNManager::Get(SHNManager::MapInfoType);

    if(ShowLoadMenu)
    {
        if (ImGui::Begin("Load MapInfo", &ShowLoadMenu));
        {
            static char buffer[15];
            ImGui::InputText("Filter Maps", buffer, sizeof(buffer));

            if (shn->DrawHeader())
            {
                for (unsigned int i = 0; i < shn->GetRows(); i++)
                {
                    MapInfo* info = shn->GetRow<MapInfo>(i);
                    if (std::string(info->MapName).find(buffer) == std::string::npos)
                        continue;
                    shn->DrawRow(i);
                    if (!future.valid() || future.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout)
                    {
                        if (ImGui::Button(std::string("Load##" + std::to_string(i)).c_str()))
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
                    }
                }
                ImGui::EndTable();
            }
            ImGui::End();
        }
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
        if (ImGui::MenuItem("Save Map",0,false,false))
        {
            
        }
        ImGui::EndMenu();
    }
}