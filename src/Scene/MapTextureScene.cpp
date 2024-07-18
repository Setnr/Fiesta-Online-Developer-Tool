#include "MapTextureScene.h"
#include "../Settings/Settings.h"
#include <future>

void MapTextureScene::Draw(NiRenderer* renderer) 
{
	if (!kWorld->GetTerrainScene())
		return;
	NiVisibleArray m_kVisible2;
	NiCullingProcess m_spCuller2(&m_kVisible2);
	NiDrawScene(kWorld->GetCamera(), kWorld->GetTerrainScene(), m_spCuller2);
    
    if(_LayerEdit.GetScreenTexture())
    {
        renderer->SetScreenSpaceCameraData();
        _LayerEdit.GetScreenTexture()->Draw(renderer);
    }
}

void MapTextureScene::DrawImGui() 
{
    FiestaScene::DrawImGui();
    _MapMenu.RenderMenu();
    auto& io = ImGui::GetIO();
    auto& Ini = kWorld->GetIniFile();
    
    ImGui::SetWindowPos("Texture Editor",ImVec2(io.DisplaySize.x - 470, io.DisplaySize.y - 350));
    ImGui::SetWindowSize(ImVec2(470.f, 350.f));
    if (ImGui::Begin("Texture Editor", 0, ImGuiWindowFlags_NoCollapse || ImGuiWindowFlags_NoMove || ImGuiWindowFlags_NoResize))
    {
        if (ImGui::Button("Reload World"))
        {
            kWorld->ReloadTerrain();
        }
        if (ImGui::BeginChild("Layer List",ImVec2(120.f, 350.f))) 
        {
            if(ImGui::Button("Create New Layer"))
            {
                SelectedLayer = _LayerEdit.NewLayer(Ini.VertexColorTexture, Ini.HeightMap_width - 1, Ini.HeightMap_height - 1);
                SelectedLayer->BlendFileName = _SubPath + "\\" + SelectedLayer->Name + ".bmp";

                //_LayerEdit.UpdateLayer(kWorld->GetHTD());
                Ini.LayerList.push_back(SelectedLayer);
            }
            for (auto layer : Ini.LayerList)
                if (ImGui::Selectable(layer->Name.c_str(), layer == SelectedLayer))
                {
                    SelectedLayer = layer;
                    _LayerEdit.ChangeLayer(SelectedLayer);
                }

            ImGui::EndChild();
        }
        ImGui::SameLine();
        if (ImGui::BeginChild("Layer Overview",ImVec2(350.f,350.f))) 
        {
            if (SelectedLayer)
            {
                if (ImGui::Button("Delete Layer")) 
                {
                    if (Ini.LayerList.size() <= 1)
                        LogWarning("Cant Delete the Last Layer");
                    else
                    {
                        for(auto layer = Ini.LayerList.begin(); layer != Ini.LayerList.end(); layer++)
                        {
                            if (*layer == SelectedLayer)
                            {
                                Ini.LayerList.erase(layer);
                                break;
                            }
                            SelectedLayer = Ini.LayerList.at(Ini.LayerList.size() - 1);
                        }
                        kWorld->ReloadTerrain();
                    }
                }
           
                char Buffer[128];
                memcpy(&Buffer, SelectedLayer->Name.c_str(), sizeof(Buffer));
                if (ImGui::InputText("Layerame", Buffer, sizeof(Buffer)))
                {
                    SelectedLayer->Name = Buffer;
                    SelectedLayer->BlendFileName = _SubPath + "\\" + SelectedLayer->Name + ".bmp";
                }
                if (ImGui::Selectable(("TextureFile: " + SelectedLayer->DiffuseFileName).c_str()))
                {
                    loader.Prepare();
                }
                if (loader.DrawImGui())
                {
                    SelectedLayer->DiffuseFileName = loader.Load();
                    SelectedLayer->LoadDiffuseFile();
                    auto start = std::chrono::steady_clock::now();
                    kWorld->CreateTerrainLayer(SelectedLayer);
                    LogTime("Terrain Created in: ", start);
                    _LayerEdit.UpdateTexturePreview();
                }

            }
            ImGui::EndChild();
        }
        ImGui::End();
    }
   
    if(_LayerEdit.Show())
    {
       
        kWorld->CreateTerrainLayer(SelectedLayer);
    }

    if (ImGui::IsKeyPressed((ImGuiKey)0x52)) //R
    {
        LookAndMoveAtWorldPoint(kWorld->GetSpawnPoint());
    }
}
void MapTextureScene::CreateMenuBar() 
{
    if (ImGui::BeginMenu("File"))
    {
        _MapMenu.ShowMenuBar();
        if (ImGui::MenuItem("Save Map"))
        {
            SaveMap();
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
void MapTextureScene::UpdateCamera(float fTime) { if (Camera)FiestaScene::UpdateCamera(fTime); }
//void MapTextureScene::Update(float fTime) {}

void MapTextureScene::SaveMap() 
{
    IniFile& ini = kWorld->GetIniFile();
    ini.Save();
}