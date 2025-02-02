#include "MapLoadElement.h"
#include <Data/SHN/SHNManager.h>

MapInfo EmptyData = { sizeof(MapInfo) - 2,12,"DevTool","DevTool", 0, 5000,5000, 0,"DevTool",0,5000 };

bool MapLoadElement::Draw() 
{
	std::shared_ptr<SHN::CDataReader> reader;
	bool ret = true;
	if (SHN::SHNManager::GetSHN(SHN::MapInfoType, reader))
	{
		if (ImGui::Begin("MapInfo"))
		{
			if (ImGui::CollapsingHeader("Create a New Map")) {
				ImGui::InputText("MapName", EmptyData.MapName, sizeof(EmptyData.MapName));
				ImGui::SameLine();
				if (ImGui::Button("New Map"))
				{
					std::thread LoadingThread(&EditorScene::CreateNewMap, _Scene, &EmptyData, MapSize);
					LoadingThread.detach();
					ret = false;
				}

				if (ImGui::BeginCombo("MapSize", std::to_string(MapSize).c_str()))
				{
					int Sizes[] = { 64,128,256,512,1024 };
					for (int i = 0; i < 5; i++)
						if (ImGui::Selectable(std::to_string(Sizes[i]).c_str(), MapSize == Sizes[i]))
							MapSize = Sizes[i];
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				const char* MapTypes[] = { "field","KDfield","MHfield", "IDfield" };
				if (ImGui::BeginCombo("MapType", MapTypes[EmptyData.KingdomMap]))
				{
					for (int i = 0; i < 4; i++)
						if (ImGui::Selectable(MapTypes[i], i == EmptyData.KingdomMap))
							EmptyData.KingdomMap = i;
					ImGui::EndCombo();
				}
			}

			static char buffer[15];
			ImGui::InputText("Filter Maps", buffer, sizeof(buffer));
			if (reader->DrawHeader(1))
			{
				for (int i = 0; i < reader->GetRows(); i++)
				{
					MapInfo* row = (MapInfo*)reader->GetRow(i);
					std::string MapName = row->MapName;
					std::string Buffer = buffer;

					if (std::search(MapName.begin(), MapName.end(),
						Buffer.begin(), Buffer.end(),
						[](char a, char b) {return tolower(a) == tolower(b); }) == MapName.end())
						continue;

					row->DrawTableRow();

					if (ImGui::Button(std::string("Load Map##" + std::to_string(i)).c_str()))
					{
						std::thread LoadingThread(&EditorScene::LoadNewMap, _Scene, row);
						LoadingThread.detach();
						ret = false;
						break;
					}
				}
				ImGui::EndTable();
			}
		}
		ImGui::End();
	}
	return ret;
}
