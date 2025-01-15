#include "TextureMode.h"
#include <FiestaOnlineTool.h>
#include <filesystem>
#include "Logger.h"
#include "Brush/LuaBrush.h"
#include <Data/IngameWorld/WorldChanges/FogChange.h>
#include <Scene/ScreenElements/LoadObject/LoadDiffuseFile.h>
NiImplementRTTI(TextureMode, TerrainBrushMode);

void TextureMode::ProcessInput()
{
	auto _Ini = kWorld->GetShineIni();
	auto layerlist = _Ini->GetLayers();
	if (layerlist.size() < 0 || _CurrentLayer == layerlist[0])
	{
		return;
	}
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

	static char buffer[25];
	ImGui::BeginChild("BrushChildLayers", ImVec2(200, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	auto _Ini = kWorld->GetShineIni();
	if (ImGui::Button("Recreate Terrain"))
		kWorld->ShowTerrain(true);
	if (ImGui::Button("Delete Selected Layer")) 
	{
		if (_CurrentLayer)
		{
			_Ini->DeleteLayer(_CurrentLayer);
			kWorld->AttachStack(NiNew LayerDelete(kWorld, _CurrentLayer));
			_ScreenElement = NULL;
			_CurrentLayer = NULL;
			kWorld->ShowTerrain(kWorld->GetShowTerrain());
		}
	}
	if (ImGui::Button("Add New Layer"))
	{
		_CurrentLayer = _Ini->CreateNewLayer(kWorld->GetMapInfo());
		_ScreenElement = PgUtil::CreateScreenElement(_CurrentLayer->BlendTexture->GetWidth(), _CurrentLayer->BlendTexture->GetHeight(), _CurrentLayer->BlendTexture);
		_CurrentLayer->BlendTexture->SetStatic(false);

		kWorld->AttachStack(NiNew LayerAdd(kWorld, _CurrentLayer));
		kWorld->ShowTerrain(kWorld->GetShowTerrain());
	}
	if (ImGui::Button("Save Layer")) 
	{
		SaveSelectedLayer();
	}
	std::vector<std::shared_ptr<TerrainLayerData>>& LayerList = _Ini->GetLayers();
	for (int i = 0; i < LayerList.size(); i++)
	{
		auto layer = LayerList.at(i);
		if (ImGui::ArrowButton((layer->Name + "Up").c_str(), ImGuiDir_Up))
		{
			if (i > 0)
			{
				LayerList[i] = LayerList[i - 1];
				LayerList[i - 1] = layer;
			}
		}
		ImGui::SameLine();
		if (ImGui::ArrowButton((layer->Name + "Down").c_str(), ImGuiDir_Down))
		{
			if (i < LayerList.size() - 1)
			{
				LayerList[i] = LayerList[i + 1];
				LayerList[i + 1] = layer;
			}
		}
		ImGui::SameLine();
		if (ImGui::Selectable(layer->Name.c_str(), layer == _CurrentLayer))
		{
			_CurrentLayer = layer;
			if (NiIsKindOf(LuaBrush, _CurrentBrush) && _CurrentLayer)
			{
				LuaBrushPtr ptr = NiSmartPointerCast(LuaBrush, _CurrentBrush);
				ptr->SetPointer((long long)_CurrentLayer.get(), "SetLayer");
			}
			_ScreenElement = PgUtil::CreateScreenElement(_CurrentLayer->BlendTexture->GetWidth(), _CurrentLayer->BlendTexture->GetHeight(), _CurrentLayer->BlendTexture);
			layer->BlendTexture->SetStatic(false);
		}
		
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("LayerRight");
	std::string msg = "DiffuseFile: ";
	if (_CurrentLayer )
	{
		strcpy_s(buffer, sizeof(buffer), _CurrentLayer->Name.c_str());
		if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			_CurrentLayer->Name = buffer;
		msg += _CurrentLayer->DiffuseFileName;
		if (ImGui::Selectable(msg.c_str(), false))
		{
			auto element = NiNew LoadDiffuseFile(_CurrentLayer, kWorld);
			ScreenElements.push_back(element);
		}
		ImGui::DragFloat("StartPos_X", &_CurrentLayer->StartPos_X);
		ImGui::DragFloat("StartPos_Y", &_CurrentLayer->StartPos_Y);
		ImGui::DragFloat("UVScaleDiffuse", &_CurrentLayer->UVScaleDiffuse);
		ImGui::DragFloat("UVScaleBlend", &_CurrentLayer->UVScaleBlend);
	}
	auto layerlist = _Ini->GetLayers();
	if (layerlist.size() < 0 || _CurrentLayer == layerlist[0])
	{
		ImGui::Text("Ground Layer is not intended to be changed because of the size missmatch\n and the fact that the game need a all white base layer!");
	}
	else 
	{
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
	ImGui::EndChild();
	auto WindowPos = ImGui::GetWindowPos();
	auto Size = ImGui::GetWindowSize();

	auto& io = ImGui::GetIO();

	if (!_ScreenElement)
		return;
	unsigned int Width = _CurrentLayer->BlendTexture->GetWidth();
	unsigned int Height = _CurrentLayer->BlendTexture->GetHeight();

	float fLeft = (WindowPos.x +Size.x- Width) / io.DisplaySize.x;
	float fTop = (WindowPos.y - Height) / io.DisplaySize.y;

	float fBottom = fTop + (Height / io.DisplaySize.y);
	float fRight = fLeft + (Width / io.DisplaySize.x);

	_ScreenElement->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
	_ScreenElement->SetVertex(0, 1, NiPoint2(fRight, fBottom));
	_ScreenElement->SetVertex(0, 2, NiPoint2(fRight, fTop));
	_ScreenElement->SetVertex(0, 3, NiPoint2(fLeft, fTop));
	_ScreenElement->UpdateBound();

	auto renderer = FiestaOnlineTool::GetRenderer();
	if (renderer)
		_ScreenElement->Draw(renderer);
}

void TextureMode::SaveSelectedLayer() 
{
	if (!_CurrentLayer)
		return;
	PgUtil::SaveTexture(PgUtil::PathFromClientFolder(_CurrentLayer->BlendFileName), _CurrentLayer->BlendTexture);
}