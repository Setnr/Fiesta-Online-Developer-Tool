#include "TerrainMode.h"
#include <NiPick.h>
#include <FiestaOnlineTool.h>
#include <thread>
#include <future>
#include <Data/IngameWorld/WorldChanges/FogChange.h>

NiImplementRTTI(TerrainMode, EditMode);

void TerrainMode::Draw()
{
	EditMode::Draw();

}
void TerrainMode::Update(float fTime)
{
	UpdateMouseIntersect();
	MouseOrb->SetTranslate(MouseIntersect);

	MouseOrb->Update(fTime);
}
void TerrainMode::ProcessInput()
{
	ImGuiIO& io = ImGui::GetIO();
	float DeltaTime = FiestaOnlineTool::GetDeltaTime();

	if (io.MouseWheel != 0.0f)
	{
		if (ImGui::IsKeyDown((ImGuiKey)VK_MENU))
		{

			if (io.MouseWheel > 0.f)
			{
				if (_BrushSize < 100)
					_BrushSize++;
			}
			else if (io.MouseWheel < 0.f)
			{
				if (_BrushSize > 0)
					_BrushSize--;
			}
			SetBrushSize(_BrushSize); //We need to set it here to resize the Visual Identifier
		}
	}
	if (ImGui::IsKeyPressed((ImGuiKey)VK_F1, false))
	{
		SetShowElements(!GetShowElements());
	}
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		_Update = true;
		MouseIntersectAtClick = MouseIntersect;
		LogInfo("Mouse Down at " + std::to_string(MouseIntersect.z));
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		_Update = false;
	}
}
void TerrainMode::SetShowElements(bool Show)
{
	_ShowElements = Show;
	kWorld->ShowSHMDElements(Show);
}
void TerrainMode::UpdateMouseIntersect()
{
	NiPoint3 kOrigin, kDir;
	auto Point = FiestaOnlineTool::CurrentMousePosition();
	if (Camera->WindowPointToRay(Point.x, Point.y, kOrigin, kDir))
	{
		NiPick _Pick;
		_Pick.SetPickType(NiPick::FIND_FIRST);
		_Pick.SetSortType(NiPick::SORT);
		_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
		_Pick.SetFrontOnly(true);
		_Pick.SetReturnNormal(true);
		_Pick.SetObserveAppCullFlag(true);
		_Pick.SetTarget(kWorld->GetTerrainNode());
		if (_Pick.PickObjects(kOrigin, kDir, true))
		{
			NiPick::Results& results = _Pick.GetResults();
			if (results.GetSize() > 0)
			{
				MouseIntersect = results.GetAt(0)->GetIntersection();
				return;
			}
		}
	}
}