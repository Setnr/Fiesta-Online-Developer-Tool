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
    static char buffer[15];
    ImGui::InputText("Filter Maps", buffer, sizeof(buffer)); ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted("Move with WASD");
        ImGui::TextUnformatted("Press Shift to Move faster");
        ImGui::TextUnformatted("Right Click to Rotate Cam");
        ImGui::TextUnformatted("Left Click to Select a World Object");
        ImGui::TextUnformatted("Middle Mouse to open Menu");
        ImGui::TextUnformatted("Copyright Gamebryo / Gamgio / IDK");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    if(shn->DrawHeader())
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

