#pragma once
#include "HTDBrush.h"
NiSmartPointer(World);
class ChangeBrush : public HTDBrush
{
public:
	ChangeBrush(NiNodePtr HTDOrbNode, int BrushSize) : HTDBrush(HTDOrbNode, BrushSize){}
	virtual void DrawInternal() 
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SliderFloat("Level Change", &ChangeSize, -50.f, 50.f);
		ImGui::EndChild();
	}
	virtual const char* GetName() { return "Change"; }

	virtual void UpdateHTD(IniFile& _InitFile, std::vector<std::vector<World::HTDHelper>>& HTD)
	{
		static float LastUpdate = 0.0f;
		float CurTime = NiGetCurrentTimeInSec();
		if (LastUpdate + 0.05f > CurTime)
			return;
		LastUpdate = CurTime;
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
						point->z += ChangeSize;
				for (auto shape : HTD[w][h].Shape)
					if (shape)
						shape->MarkAsChanged(NiGeometryData::VERTEX_MASK);
			}
		}

	}

private:
	float ChangeSize = 10.f;
};