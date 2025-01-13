#include "TextureMode.h"
#include <FiestaOnlineTool.h>
#include <filesystem>
#include "Logger.h"
#include "Brush/LuaBrush.h"
#include <Data/IngameWorld/WorldChanges/FogChange.h>
NiImplementRTTI(TextureMode, TerrainBrushMode);

void TextureMode::ProcessInput()
{
	TerrainBrushMode::ProcessInput();
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		_Data = NiNew NiPixelData(*_CurrentLayer->BlendTexture->GetSourcePixelData());
		_Update = true;
		
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		NiPixelDataPtr NewData = NiNew NiPixelData(*_CurrentLayer->BlendTexture->GetSourcePixelData());
		_Update = false;
		TextureChangePtr Change = NiNew TextureChange(_CurrentLayer, _Data, NewData);
		kWorld->AttachStack(NiSmartPointerCast(WorldChange, Change));
	}
}

void TextureMode::DrawLayers() 
{
	ImGui::BeginChild("BrushChildLayers", ImVec2(100, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	auto _Ini = kWorld->GetShineIni();
	for (auto layer : _Ini->GetLayers())
	{
		if (ImGui::Selectable(layer->Name.c_str(), layer == _CurrentLayer))
		{
			_CurrentLayer = layer;
			if (NiIsKindOf(LuaBrush, _CurrentBrush) && _CurrentLayer)
			{
				LuaBrushPtr ptr = NiSmartPointerCast(LuaBrush, _CurrentBrush);
				ptr->SetPointer((long long)_CurrentLayer.get(), "SetLayer");
			}
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	bool s;
	ImGui::BeginChild("BrushChildL", ImVec2(100, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (auto brush : _Brushes)
	{
		if (ImGui::Selectable(brush->GetBrushName().c_str(), &s))
		{
			_CurrentBrush = brush;
			if (NiIsKindOf(LuaBrush, _CurrentBrush) && _CurrentLayer)
			{
				LuaBrushPtr ptr = NiSmartPointerCast(LuaBrush, _CurrentBrush);
				ptr->SetPointer((long long)_CurrentLayer.get(), "SetLayer");
			}
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("BrushChildR", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (_CurrentBrush)
		_CurrentBrush->Draw();
	ImGui::EndChild();
}

void TextureMode::SaveSelectedLayer() 
{
	if (!_CurrentLayer)
		return;
	PgUtil::SaveTexture(PgUtil::PathFromClientFolder(_CurrentLayer->BlendFileName), _CurrentLayer->BlendTexture);
}