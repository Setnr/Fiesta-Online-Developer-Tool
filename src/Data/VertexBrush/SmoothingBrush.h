#pragma once

#include "../Brush.h"

class ColorBrush : public VertexBrush
{
public:
	ColorBrush(TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) : VertexBrush(kWorld, HTDOrbNode, BrushSize) {}
	virtual void DrawInternal()
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::ColorPicker4("Color", (float*)&color);

		if (ImGui::IsKeyDown((ImGuiKey)VK_CONTROL) && ImGui::IsKeyPressed((ImGuiKey)0x43))  // ctrl c 
			color = CurColor;

		ImGui::EndChild();
	}
	virtual const char* GetName() { return "Color"; }

	virtual void Update()
	{
		IniFile& _InitFile = kWorld->GetIniFile();;
		static float LastUpdate = 0.0f;
		float CurTime = NiGetCurrentTimeInSec();
		if (LastUpdate + 0.05f > CurTime)
			return;
		LastUpdate = CurTime;
		int middlew = _Intersect.x / _InitFile.OneBlock_width;
		int middleh = _Intersect.y / _InitFile.OneBlock_height;
		std::vector<std::vector<TerrainWorld::PointInfos>>& HTD = kWorld->GetHTD();
		for (int w = middlew - BrushSize; w <= middlew + BrushSize && w < (int)HTD.size(); w++)
		{
			if (w < 0)
				continue;
			for (int h = middleh - BrushSize; h <= middleh + BrushSize && h < (int)HTD[w].size(); h++)
			{
				if (h < 0)
					continue;
				if (!((w - middlew) * (w - middlew) + (h - middleh) * (h - middleh) <= BrushSize * BrushSize))
					continue;
				HTD[w][h].VertexColor = color;
				for (auto point : kWorld->GetHTDPoints(w, h))
				{
					if (point.NiColor)
						*point.NiColor = color;
					if (point.NiGeometry)
						point.NiGeometry->MarkAsChanged(NiGeometryData::COLOR_MASK);
				}

			}
		}

	}

private:
	NiColorA color;
};