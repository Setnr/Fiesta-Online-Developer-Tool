#include "HTDGMode.h"
#include <FiestaOnlineTool.h>
NiImplementRTTI(HTDGMode, TerrainMode);

void HTDGMode::Update(float fTime)
{
	TerrainMode::Update(fTime);
	
	if (_Update)
	{

	}
}
void HTDGMode::ProcessInput()
{
	TerrainMode::ProcessInput();

	ImGuiIO& io = ImGui::GetIO();
	float DeltaTime = FiestaOnlineTool::GetDeltaTime();
	if (io.MouseWheel != 0.0f)
	{
		
	}
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		_Update = true;
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		_Update = false;
	}
}