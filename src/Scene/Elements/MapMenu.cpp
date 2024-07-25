#include "MapMenu.h"

#include <future>
#include "../Data/SHNManager.h"
#include "../MapCreateScene.h"
//#include "../MapTextureScene.h"
#include "../EditScene.h"
#include "../../CustomNi/NiFileLoader.h"
void MapMenu::ShowMenuBar() {
	if (ImGui::MenuItem("Load Map"))
	{
		ShowLoadMenu = true;
	}
    if (ImGui::MenuItem("Reload MapInfo"))
    {
        SHNManager::ReloadSHN(SHNManager::MapInfoType);
    }
}

void MapMenu::RenderMenu()
{
    if (!ShowLoadMenu)
        return;
    static std::future<void> future;
    auto shn = SHNManager::Get(SHNManager::MapInfoType);
    if (ImGui::Begin("Load MapInfo", &ShowLoadMenu))
    {
        static char buffer[15];
        ImGui::InputText("Filter Maps", buffer, sizeof(buffer));
        ImGui::SameLine();
        if (ImGui::Button("Create New Map")) {
            MapCreateScenePtr ptr = NiNew MapCreateScene;
            
            FiestaScenePtr scene = (FiestaScene*)&*ptr;
            FiestaOnlineTool::UpdateScene(scene);
        }
       
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
                    if (ImGui::Button(std::string("Load Map##" + std::to_string(i)).c_str()))
                    {
                        ImGui::SetWindowFocus("");
                        ShowLoadMenu = false;
                        future = std::async(std::launch::async, [this, shn, i]
                            {
                                EditScenePtr ptr = NiNew EditScene;
                                ptr->HideShowLoadMenu();
                                MapInfo* info = shn->GetRow<MapInfo>(i);
                                ptr->LoadMap(info);
                                FiestaScenePtr scene = (FiestaScene*)&*ptr;
                                FiestaOnlineTool::UpdateScene(scene);

                            });

                    }

                }
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }

}