#include "MapCreateScene.h"

#include <future>
#include "../Data/SHNManager.h"
#include "EditScene.h"
#include "../NiApplication/FiestaOnlineTool.h"

void MapCreateScene::ShowMapInfo() 
{
    static std::future<void> future;
    auto shn = SHNManager::Get(SHNManager::MapInfoType);
    if (ImGui::Begin("Load MapInfo", &ShowLoadMenu));
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

void MapCreateScene::DrawImGui()
{
    FiestaScene::DrawImGui();
    if (ShowLoadMenu)
        ShowMapInfo();

    if (ImGui::Begin("Create Map"))
    {
        bool UpdateAlgo = false;
        if (ImGui::RadioButton("64x64", MapSize == 65))
        {
            UpdateAlgo = true;
            MapSize = 65;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("128x128",MapSize == 129))
        {
            UpdateAlgo = true;
            MapSize = 129;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("256x256", MapSize == 257))
        {
            UpdateAlgo = true;
            MapSize = 257;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("512x512", MapSize == 513))
        {
            UpdateAlgo = true;
            MapSize = 513;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("1024x1024", MapSize == 1025))
        {
            UpdateAlgo = true;
            MapSize = 1025;
        }
        if(ImGui::RadioButton("Perlin Algorithm", _Algo == CreateAlgorithm::Perlin))
            UpdateAlgorithm(Perlin);
        ImGui::SameLine();
        if (ImGui::RadioButton("Worley Algorithm", _Algo == CreateAlgorithm::Worley))
            UpdateAlgorithm(Worley);
        ImGui::SameLine();
        if (ImGui::RadioButton("Diamond Square Algorithm", _Algo == CreateAlgorithm::DiamondSquare))
            UpdateAlgorithm(DiamondSquare);
        if (ImGui::Button("Show Algorithm Preview"))
        {
            AlgoPreview = !AlgoPreview;
            _Fractal->Show(AlgoPreview);
        }

        if (ImGui::DragInt("Seed", &Seed))
            UpdateAlgo = true;;
        
        switch (_Algo) 
        {
        case Perlin:
        {
            if (ImGui::DragInt("Octave", &Octave, 1.f, 1.f, 20.f) ||
                ImGui::DragFloat("Min Height", &MinValue, 1.0f, -5000.0f, 1000.f) ||
                ImGui::DragFloat("Max Height", &MaxValue, 1.0f, -1000.0f, 5000.f)) {
                
                UpdateAlgo = true;
            }
            break;
        }
        case Worley:
        {
            if (ImGui::DragInt("Num Points", &NumPoints,1.0,0,25) ||
                ImGui::DragInt("Point Loc", &PointLoc, 1.0, 0, NumPoints - 1) ||
                ImGui::DragFloat("Min Height", &MinValue, 1.0f, -5000.0f, 1000.f) ||
                ImGui::DragFloat("Max Height", &MaxValue, 1.0f, -1000.0f, 5000.f)) {

                UpdateAlgo = true;
            }
            break;
        }
        case DiamondSquare: 
            {
                if(ImGui::DragFloat("Noise",&noise,0.01f) || 
                    ImGui::DragFloat("Min Height", &MinValue, 1.0f, -5000.0f, 1000.f) ||
                    ImGui::DragFloat("Max Height", &MaxValue, 1.0f, -1000.0f, 5000.f))
                    UpdateAlgo = true;
            }
            break;
        }

        if (ImGui::Button("Apply Thermal Erosion")) {
            _Fractal->applyThermalErosion();
            CreateTerrain();
        }
        if (ImGui::Button("Apply Hydraulic Erosion")) {
            _Fractal->applyHydraulicErosion(0.01f,0.05f);
            CreateTerrain();
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Grid"))
            UpdateAlgo = true;
        if (ImGui::Button("Save Test Nif")) 
        {
            NiStream s;
            s.InsertObject(kWorld->GetTerrainScene());
            s.Save(PgUtil::CreateFullFilePathFromApplicationFolder("./Test.nif").c_str());
        }

        if (UpdateAlgo)
            UpdateAlgorithm(_Algo);
        ImGui::End();
    }
}

void MapCreateScene::CreateMenuBar() 
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Load Map"))
        {
            ShowLoadMenu = true;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
        if (ImGui::MenuItem("General View"))
        {
            Settings::SwitchGeneralView();
        }

        ImGui::EndMenu();
    }
   
    
    ImGui::SameLine(ImGui::GetWindowWidth() - 30);
    ImGui::Text("(?)");
    if (ImGui::IsItemClicked())
    {
        ShowAbout = true;
    }
}