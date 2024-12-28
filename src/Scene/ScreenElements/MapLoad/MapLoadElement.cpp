#include "MapLoadElement.h"
#include <Data/SHN/SHNManager.h>

MapInfo EmptyData = { sizeof(MapInfo) - 2,12,"DevTool","DevTool", 0, 5000,5000, -1,"DevTool",0,5000 };

bool MapLoadElement::Draw() 
{
	std::shared_ptr<SHN::CDataReader> reader;
	bool ret = true;
	if (SHN::SHNManager::GetSHN(SHN::MapInfoType, reader))
	{
		if (ImGui::Begin("MapInfo"))
		{
			static char buffer[15];
			ImGui::InputText("Filter Maps", buffer, sizeof(buffer));
			ImGui::SameLine();
			if (ImGui::Button("New Map"))
			{
				std::thread LoadingThread(&EditorScene::LoadNewMap, _Scene, &EmptyData);
				LoadingThread.detach();
				ret = false;
			}
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
