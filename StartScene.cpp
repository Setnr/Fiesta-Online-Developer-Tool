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
    return;
}




void StartScene::DrawImGui() 
{
    FiestaScene::DrawImGui();
    static std::future<void> future;
    auto shn = SHNManager::Get(SHNManager::MapInfo);
    ImGui::Begin("Load MapInfo");
    if(shn->DrawHeader())
    {
        for (unsigned int i = 0; i < shn->GetRows(); i++)
        {
            shn->DrawRow(i);
            if (!future.valid() || future.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout)
            {
                if (ImGui::Button(std::string("Load##" + std::to_string(i)).c_str()))
                {

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

