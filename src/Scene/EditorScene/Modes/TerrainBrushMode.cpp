#include "TerrainBrushMode.h"
#include <FiestaOnlineTool.h>
#include <filesystem>
#include "Logger.h"
#include "Brush/LuaBrush.h"
NiImplementRTTI(TerrainBrushMode, TerrainMode);

void TerrainBrushMode::Update(float fTime)
{
	TerrainMode::Update(fTime);

	if (_Update && _CurrentBrush && kWorld->HasHTD())
	{
		auto Ini = kWorld->GetShineIni();

		int middelw = MouseIntersect.x / Ini->GetOneBlockWidht();
		int middelh = MouseIntersect.y / Ini->GetOneBlockHeight();
		float z = MouseIntersect.z;
		
		int SizeW = Ini->GetMapWidth(); 
		int SizeH = Ini->GetMapHeight();
		if(NiIsKindOf(LuaBrush,_CurrentBrush))
		{
			LuaBrushPtr luabrush = NiSmartPointerCast(LuaBrush, _CurrentBrush);
			luabrush->RunAlgorithm(middelw, middelh,z, SizeW, SizeH, GetBrushSize(), kWorld);
		}
	}
} 
void TerrainBrushMode::ProcessInput()
{
	TerrainMode::ProcessInput();
}

void TerrainBrushMode::LoadBrushes(std::string BrushFolder)
{
	_Brushes.clear();
	auto Folder = PgUtil::PathFromApplicationFolder("./FiestaOnlineTool/SceneElements/EditorElements/" + BrushFolder);

	for (std::filesystem::directory_iterator itr(Folder); itr != std::filesystem::directory_iterator(); itr++)
	{
		auto brush = NiNew LuaBrush(itr->path().string());
		_Brushes.push_back(brush);
		brush->Init(kWorld);
	}
}

void TerrainBrushMode::DrawBrushes() 
{
	bool s;
	ImGui::BeginChild("BrushChildL", ImVec2(100, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (auto brush : _Brushes) 
	{
		if (ImGui::Selectable(brush->GetBrushName().c_str(),&s))
		{
			_CurrentBrush = brush;   
		}  
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("BrushChildR", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (_CurrentBrush)
		_CurrentBrush->Draw();
	ImGui::EndChild();

}