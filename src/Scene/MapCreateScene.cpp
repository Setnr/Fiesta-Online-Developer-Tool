#include "MapCreateScene.h"

#include "EditScene.h"
#include "../NiApplication/FiestaOnlineTool.h"
#include "MapTextureScene.h"



void MapCreateScene::DrawImGui()
{
    FiestaScene::DrawImGui();
    _MapMenu.RenderMenu();

    if (ImGui::Begin("Create Map", 0 , ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
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
            if (ImGui::DragInt("Octave", &Octave, 1.f, 1.f, 20.f))
                UpdateAlgo = true;
            if (ImGui::DragFloat("Min Height", &MinValue, 1.0f, -5000.0f, 1000.f))
                UpdateAlgo = true;
            if (ImGui::DragFloat("Max Height", &MaxValue, 1.0f, -1000.0f, 5000.f))
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
        if (ImGui::Button("Procced To Texturing")) 
        {
            ShowHTDSave = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("Save as .nif"))
            PgUtil::SaveNode(PgUtil::CreateFullFilePathFromBaseFolder(".\\Test.nif"), kWorld->GetTerrainScene());
        if (UpdateAlgo)
            UpdateAlgorithm(_Algo);
        ImGui::End();
    }

    if (ShowHTDSave) 
    {
        if (ImGui::Begin("Save HTD", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
        {
            static enum MapType {
                Field,
                IDField,
                KDField
            } SaveType = Field;
            if (ImGui::RadioButton("Field Map", SaveType == Field))
                SaveType = Field;
            ImGui::SameLine();
            if (ImGui::RadioButton("ID Map", SaveType == IDField))
                SaveType = IDField;
            ImGui::SameLine();
            if (ImGui::RadioButton("KD Map", SaveType == KDField))
                SaveType = KDField;
            static char MapName[128] = "SetTool";
            ImGui::InputText("MapName", MapName, sizeof(MapName));
            if (ImGui::Button("Save"))
            {
                std::string SubPath = ".\\resmap\\";
                switch (SaveType)
                {
                case Field: SubPath += "field";
                    break;
                case IDField: SubPath += "IDField";
                    break;
                case KDField: SubPath += "KDField";
                    break;
                }
                std::string _MapName = std::string(MapName);
                SubPath += "\\" + _MapName;
                std::string Direcotry = PgUtil::CreateFullFilePathFromBaseFolder(SubPath);
                if (!std::filesystem::exists(Direcotry))
                {
                    std::filesystem::create_directories(Direcotry);
                }

                _Fractal->SaveHTD(Direcotry + "\\" + _MapName + ".HTD");
                IniFile file(PgUtil::CreateFullFilePathFromApplicationFolder(".\\FiestaOnlineTool\\BaseIni.ini"));
                if (!file.Load()) 
                {
                    NiMessageBox::DisplayMessage("Cant Find BaseIni.ini", "ERROR");
                    return;
                }
                file.HeightMap_height = MapSize;
                file.HeightMap_width = MapSize;
                file.HeightFileName = SubPath + "\\" + _MapName + ".HTD";
                file.VertexColorTexture = SubPath + "\\Vertex" + _MapName + ".bmp";
                file.LayerList[0]->Height = MapSize;
                file.LayerList[0]->Width = MapSize;
                file.Save(Direcotry + "\\" + _MapName + ".ini");
                PgUtil::SaveTexture(Direcotry + "\\Vertex" + _MapName + ".bmp", _Fractal->GetSourceTexture());
                MapTextureScenePtr ptr = NiNew MapTextureScene(Direcotry + "\\" + _MapName + ".ini");
                FiestaScenePtr scene = (FiestaScene*)&*ptr;
                FiestaOnlineTool::UpdateScene(scene);
            }
            ImGui::End();
        }
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