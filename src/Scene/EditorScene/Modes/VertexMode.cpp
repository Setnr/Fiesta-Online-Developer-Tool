#include "VertexMode.h"
#include <FiestaOnlineTool.h>
#include <filesystem>
#include "Logger.h"
#include "Brush/LuaBrush.h"
#include <Data/IngameWorld/WorldChanges/FogChange.h>
NiImplementRTTI(VertexMode, TerrainBrushMode);

void VertexMode::ProcessInput()
{
	TerrainBrushMode::ProcessInput();
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && kWorld->HasHTD())
	{
		
		_Data = NiNew NiPixelData(*kWorld->GetShineIni()->GetVertexImage());
		_Update = true;
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && kWorld->HasHTD())
	{
		NiPixelDataPtr NewData = NiNew NiPixelData(*kWorld->GetShineIni()->GetVertexImage());

		VertexColorChangePtr Change = NiNew VertexColorChange(kWorld, _Data, NewData);
		kWorld->AttachStack(NiSmartPointerCast(WorldChange, Change));
		_Update = false;
	}

	if (ImGui::IsKeyDown((ImGuiKey)VK_CONTROL) && ImGui::IsKeyPressed((ImGuiKey)0x43)) 
	{
		auto Ini = kWorld->GetShineIni();
		int w = MouseIntersect.x / Ini->GetOneBlockWidht();
		int h = MouseIntersect.y / Ini->GetOneBlockHeight();
		if(NiIsKindOf(LuaBrush, _CurrentBrush))
		{
			LuaBrushPtr brush = NiSmartPointerCast(LuaBrush, _CurrentBrush);
			brush->SetColor(Ini->GetColor(w, h));
		}
	}
}