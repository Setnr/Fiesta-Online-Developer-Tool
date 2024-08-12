#pragma once
#include "../Brush.h"

class VertexSmoothingBrush : public VertexBrush
{
public:
	VertexSmoothingBrush(TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) : VertexBrush(kWorld, HTDOrbNode, BrushSize){}
	virtual void DrawInternal() 
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::DragInt("Kernel Size", &KernelSize,2.f,3,21);
		ImGui::DragInt("Cycles", &Cycles, 1.f, 1, 20);
		ImGui::EndChild();
	}
	virtual const char* GetName() { return "Smoothing"; }

	virtual void Update()
	{
		float CurTime = NiGetCurrentTimeInSec();
		if (LastUpdate + 0.05f > CurTime)
			return;
		LastUpdate = CurTime;

		IniFile& _InitFile = kWorld->GetIniFile();;
		std::vector<std::vector<TerrainWorld::PointInfos>>& HTD = kWorld->GetHTD();
		
		int middlew = _Intersect.x / _InitFile.OneBlock_width;
		int middleh = _Intersect.y / _InitFile.OneBlock_height;

		float KernelValue = 1 / static_cast<float>(KernelSize * KernelSize);

		int StartI = -1 * (KernelSize / 2);
		int EndI = KernelSize / 2;
		int StartJ = -1 * (KernelSize / 2);
		int EndJ = KernelSize / 2;

		for(int cycle = 0; cycle < Cycles; cycle++)
		{
			std::vector <std::pair<NiColorA, TerrainWorld::HTDPointData>> UpdateVector;
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

					float SumR = 0.f;
					float SumG = 0.f;
					float SumB = 0.f;
					float SumA = 0.f;
					for (int i = StartI; i <= EndI; i++)
					{
						for (int j = StartJ; j <= EndJ; j++)
						{
							SumR += HTD[w - i][h - j].VertexColor.r * KernelValue;
							SumG += HTD[w - i][h - j].VertexColor.g * KernelValue;
							SumB += HTD[w - i][h - j].VertexColor.b * KernelValue;
							SumA += HTD[w - i][h - j].VertexColor.a * KernelValue;
						}
					}
					NiColorA color(SumR, SumG, SumB, SumA);
					kWorld->UpdateTerrainVertexColor(w, h, color);
					for (auto point : kWorld->GetHTDPoints(w, h))
					{
						UpdateVector.push_back({ color , point});
					}

				}
			}
			for (auto data : UpdateVector) 
			{
				auto point = data.second;
				if (point.NiColor)
				{
					*point.NiColor = data.first;
				}
				if (point.NiGeometry)
					point.NiGeometry->MarkAsChanged(NiGeometryData::COLOR_MASK);
			}
		}
	}

private:
	int KernelSize = 3;
	int Cycles = 5;
};