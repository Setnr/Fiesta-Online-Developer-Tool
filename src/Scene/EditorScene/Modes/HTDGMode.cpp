#include "HTDGMode.h"
#include <FiestaOnlineTool.h>
#include <filesystem>
#include "Logger.h"
#include "Brush/LuaBrush.h"
#include <Data/IngameWorld/WorldChanges/FogChange.h>
NiImplementRTTI(HTDGMode, TerrainBrushMode);

void HTDGMode::ProcessInput() 
{
	TerrainBrushMode::ProcessInput();
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && kWorld->HasHTD())
	{
		_Data = *kWorld->GetHTD();
		_Update = true;
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && kWorld->HasHTD())
	{
		auto NewData = *kWorld->GetHTD();

		HTDGChangePtr Change = NiNew HTDGChange(kWorld, _Data, NewData);
		kWorld->AttachStack(NiSmartPointerCast(WorldChange, Change));
		_Update = false;
	}

	if (ImGui::IsKeyDown((ImGuiKey)VK_CONTROL) && ImGui::IsKeyPressed((ImGuiKey)0x53))
	{
		kWorld->SaveHTD();
	}
}