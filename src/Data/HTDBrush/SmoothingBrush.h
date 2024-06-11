#pragma once
#include "HTDBrush.h"
#include <vector>
NiSmartPointer(World);
class SmoothingBrush : public HTDBrush
{
public:
	SmoothingBrush(NiNodePtr HTDOrbNode, int BrushSize) : HTDBrush(HTDOrbNode, BrushSize){}
	virtual void DrawInternal() 
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::DragInt("Kernel Size", &KernelSize,2.f,3,21);
		ImGui::EndChild();
	}
	virtual const char* GetName() { return "Smoothing"; }

	virtual void UpdateHTD(IniFile& _InitFile, std::vector<std::vector<World::HTDHelper>>& HTD, NiPoint3 InterSect)
	{
		static float LastUpdate = 0.0f;
		float CurTime = NiGetCurrentTimeInSec();
		if (LastUpdate + 0.25f > CurTime)
			return;
		LastUpdate = CurTime;
		int middlew = InterSect.x / _InitFile.OneBlock_width;
		int middleh = InterSect.y / _InitFile.OneBlock_height;

		float KernelValue = 1 / static_cast<float>(KernelSize * KernelSize);
		std::vector<std::vector<float>> KernelMap((BrushSize + 1) * 2);
		for (int w = 0; w < (BrushSize + 1) * 2; w++) 
		{
			KernelMap[w] = std::vector<float>((BrushSize + 1) * 2);
		}
		int WHelper = 0;
		int HHelper = 0;
		for (int w = middlew - BrushSize; w <= middlew + BrushSize && w + KernelSize / 2 < (int)HTD.size(); w++)
		{
			if (w - KernelSize / 2 < 0)
				continue;
			for (int h = middleh - BrushSize; h <= middleh + BrushSize && h + KernelSize / 2 < (int)HTD[w].size(); h++)
			{
				if (h - KernelSize / 2 < 0)
					continue;
				if (!((w - middlew) * (w - middlew) + (h - middleh) * (h - middleh) <= BrushSize * BrushSize))
					continue;
				float Sum = 0.f;
				int StartI = -1 * (KernelSize / 2);
				int EndI = KernelSize / 2;
				int StartJ = -1 * (KernelSize / 2);
				int EndJ = KernelSize / 2;
				for (int i = StartI; i <= EndI; i++)
				{
					for (int j = StartJ; j <= EndJ; j++)
					{
						Sum += HTD[w - i][h - j].Vec[0]->z * KernelValue;
					}
				}
				KernelMap[WHelper][HHelper] = Sum;
				HHelper++;
			}
			HHelper = 0;
			WHelper++;
		}

		WHelper = 0;
		HHelper = 0;
		for (int w = middlew - BrushSize; w <= middlew + BrushSize && w + KernelSize / 2 < (int)HTD.size(); w++)
		{
			if (w - KernelSize / 2 < 0)
				continue;
			for (int h = middleh - BrushSize; h <= middleh + BrushSize && h + KernelSize / 2 < (int)HTD[w].size(); h++)
			{
				if (h - KernelSize / 2 < 0)
					continue;
				if (!((w - middlew) * (w - middlew) + (h - middleh) * (h - middleh) <= BrushSize * BrushSize))
					continue;
				for (auto point : HTD[w][h].Vec)
					if (point)
						point->z = KernelMap[WHelper][HHelper];
				for (auto shape : HTD[w][h].Shape)
					if (shape)
						shape->MarkAsChanged(NiGeometryData::VERTEX_MASK);
				HHelper++;
			}
			HHelper = 0;
			WHelper++;
		}
		KernelMap.clear();
	}

private:
	int KernelSize = 3;
};