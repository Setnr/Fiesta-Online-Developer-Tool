#pragma once
#include "HTDBrush.h"
NiSmartPointer(World);
class HightBrush : public HTDBrush
{
public:
	HightBrush(NiNodePtr HTDOrbNode, int BrushSize) : HTDBrush(HTDOrbNode, BrushSize) {}
	virtual void DrawInternal()
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::DragFloat("Set Height to:", &HightValue, 1.f, -5000.f, 5000.f);
		ImGui::EndChild();
	}
	virtual const char* GetName() { return "Hight"; }
	
	virtual void UpdateHTD(IniFile& _InitFile, std::vector<std::vector<World::HTDHelper>>& HTD)
	{
		if (!InitMouse)
			return;
		int middlew = _Intersect.x / _InitFile.OneBlock_width;
		int middleh = _Intersect.y / _InitFile.OneBlock_height;
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
				for (auto point : HTD[w][h].Vec)
					if (point)
						point->z = HightValue;
				for (auto shape : HTD[w][h].Shape)
					if (shape)
						shape->MarkAsChanged(NiGeometryData::VERTEX_MASK);
			}
		}

	}

private:
	float HightValue = 10.f;
};