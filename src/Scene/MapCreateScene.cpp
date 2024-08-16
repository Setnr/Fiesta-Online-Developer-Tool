#include "MapCreateScene.h"

#include "EditScene.h"
#include "../NiApplication/FiestaOnlineTool.h"
//#include "MapTextureScene.h"

void MapCreateScene::DrawImGui()
{
    FiestaScene::DrawImGui();
    _MapMenu.RenderMenu();

    if (ImGui::Begin("Create Map", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
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
            UpdateAlgo = true;
        ImGui::SameLine();
        if (ImGui::Button("Random Seed")) 
        {
            Seed = rand();
            UpdateAlgo = true;
        }
        
        switch (_Algo) 
        {
        case Perlin:
        {
            if (ImGui::DragInt("Octave", &Octave, 1.f, 1.f, 20.f))
                UpdateAlgo = true;
            if (ImGui::DragFloat("Min Height", &MinValue, 1.0f, -5000.0f, MaxValue - 1))
                UpdateAlgo = true;
            if (ImGui::DragFloat("Max Height", &MaxValue, 1.0f, MinValue + 1, 5000.f))
                UpdateAlgo = true;
            if (ImGui::DragFloat("Persistance", &Persistance, 0.001f, 0.01f, 1.f))
                UpdateAlgo = true;
            break;
        }
        case Worley:
        {
            if (ImGui::DragInt("Num Points", &NumPoints,1.0,0,25))
                UpdateAlgo = true;
            if (ImGui::DragInt("Point Loc", &PointLoc, 1.0, 0, NumPoints - 1))
                UpdateAlgo = true;
            if (ImGui::DragFloat("Min Height", &MinValue, 1.0f, -5000.0f, 1000.f))
                UpdateAlgo = true;
            if (ImGui::DragFloat("Max Height", &MaxValue, 1.0f, -1000.0f, 5000.f))
                UpdateAlgo = true;
            break;
        }
        case DiamondSquare: 
        {
            if (ImGui::DragFloat("Noise", &noise, 0.01f))
                UpdateAlgo = true;
            if (ImGui::DragFloat("Min Height", &MinValue, 1.0f, -5000.0f, 1000.f))
                UpdateAlgo = true;
            if (ImGui::DragFloat("Max Height", &MaxValue, 1.0f, -1000.0f, 5000.f))
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
        if (ImGui::Button("Procced To Shadowing")) 
        {
            ShadowMap = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("Save as .nif"))
            PgUtil::SaveNode(PgUtil::CreateFullFilePathFromBaseFolder(".\\Test.nif"), kWorld->GetTerrainScene());
        if (UpdateAlgo)
            UpdateAlgorithm(_Algo);
        ImGui::End();
    }

    if (ShadowMap && ImGui::Begin("Create Shadows", &ShadowMap, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
    {
        auto io = ImGui::GetIO();
        ImGui::SetWindowSize("Create Shadows", ImVec2(200, 500));
        ImGui::SetWindowPos("Create Shadows", ImVec2(io.DisplaySize.x - 200,io.DisplaySize.y - 800));
        if(ImGui::Button("Create Shadows"))
            CreateTerrain();
        if (_Fractal->ShowColorPickers())
            CreateTerrain();
        if (ImGui::Checkbox("Render Shadows Live", &RenderShadowsLive) || RenderShadowsLive)
            CreateTerrain();
        if(ImGui::Button("Save Map"))
            ShowHTDSave = true;
        ImGui::End();
    }

    if (ShowHTDSave && ImGui::Begin("Save HTD", &ShowHTDSave, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
    {

        if (ImGui::RadioButton("Field Map", SaveType == Field))
            SaveType = Field;
        ImGui::SameLine();
        if (ImGui::RadioButton("ID Map", SaveType == IDField))
            SaveType = IDField;
        ImGui::SameLine();
        if (ImGui::RadioButton("KD Map", SaveType == KDField))
            SaveType = KDField;
     
        ImGui::InputText("MapName", MapName, sizeof(MapName));
        if (ImGui::Button("Save"))
        {
            Save();
        }
        ImGui::End();
    }
    if (ImGui::IsKeyPressed((ImGuiKey)0x52)) //R
    {
        LookAndMoveAtWorldPoint(kWorld->GetSpawnPoint());
    }
}

void MapCreateScene::CreateMenuBar() 
{
    if (ImGui::BeginMenu("File"))
    {
        _MapMenu.ShowMenuBar();
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