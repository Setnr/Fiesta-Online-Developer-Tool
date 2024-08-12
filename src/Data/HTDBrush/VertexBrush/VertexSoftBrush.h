#pragma once
#include "../Brush.h"

class VertexSoftBrush : public VertexBrush
{
public:
	VertexSoftBrush(TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) : VertexBrush(kWorld, HTDOrbNode, BrushSize) {}
	virtual void DrawInternal()
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::DragFloat("Opacity", &color.a,0.01,0.f,1.f);
		ImGui::ColorPicker3("Color", (float*)&color);
		if (ImGui::IsKeyDown((ImGuiKey)VK_CONTROL) && ImGui::IsKeyPressed((ImGuiKey)0x43))  // ctrl c 
			color = CurColor;
		ImGui::EndChild();
		
	}
	virtual const char* GetName() { return "Soft"; }

	virtual void Update()
	{
		if (!InitMouse)
			return;
		float CurTime = NiGetCurrentTimeInSec();
		if (LastUpdate + 0.05f > CurTime)
			return;
		LastUpdate = CurTime;
		IniFile& _InitFile = kWorld->GetIniFile();;
		
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
				NiColorA col = color * color.a + OrgPoints[w][h].VertexColor * (1.f - color.a);
				kWorld->UpdateTerrainVertexColor(w, h, col);
				for (auto point : kWorld->GetHTDPoints(w, h))
				{
					if (point.NiColor)
						*point.NiColor = HTD[w][h].VertexColor;
					if (point.NiGeometry)
						point.NiGeometry->MarkAsChanged(NiGeometryData::COLOR_MASK);
				}

			}
		}
	}
	virtual void MouseClick() 
	{
		InitMouse = true; 
		OrgPoints = kWorld->GetHTD();
	}
	virtual void MouseRelease() { InitMouse = false; }
private:
	std::vector<std::vector<TerrainWorld::PointInfos>> OrgPoints;
	NiColorA color;
	float Deckkraft = 1.f;
};