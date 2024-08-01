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
		IniFile& _InitFile = kWorld->GetIniFile();;
		std::vector<std::vector<TerrainWorld::PointInfos>>& HTD = kWorld->GetHTD();
		
		int middlew = _Intersect.x / _InitFile.OneBlock_width;
		int middleh = _Intersect.y / _InitFile.OneBlock_height;

		float KernelValue = 1 / static_cast<float>(KernelSize * KernelSize);
		std::vector<std::vector<float>> KernelMapR((BrushSize + 1) * 2);
		std::vector<std::vector<float>> KernelMapG((BrushSize + 1) * 2);
		std::vector<std::vector<float>> KernelMapB((BrushSize + 1) * 2);
		std::vector<std::vector<float>> KernelMapA((BrushSize + 1) * 2);
		for (int w = 0; w < (BrushSize + 1) * 2; w++) 
		{
			KernelMapR[w] = std::vector<float>((BrushSize + 1) * 2);
			KernelMapG[w] = std::vector<float>((BrushSize + 1) * 2);
			KernelMapB[w] = std::vector<float>((BrushSize + 1) * 2);
			KernelMapA[w] = std::vector<float>((BrushSize + 1) * 2);
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
				float SumR = 0.f;
				float SumG = 0.f;
				float SumB = 0.f;
				float SumA = 0.f;
				int StartI = -1 * (KernelSize / 2);
				int EndI = KernelSize / 2;
				int StartJ = -1 * (KernelSize / 2);
				int EndJ = KernelSize / 2;
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
				KernelMapR[WHelper][HHelper] = SumR;
				KernelMapG[WHelper][HHelper] = SumG;
				KernelMapB[WHelper][HHelper] = SumB;
				KernelMapA[WHelper][HHelper] = SumA;
				HHelper++;
			}
			HHelper = 0;
			WHelper++;
		}

		for(int cycle = 0; cycle < Cycles; cycle++)
		{
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

					HTD[w][h].VertexColor.r = KernelMapR[WHelper][HHelper];
					HTD[w][h].VertexColor.g = KernelMapG[WHelper][HHelper];
					HTD[w][h].VertexColor.b = KernelMapB[WHelper][HHelper];
					HTD[w][h].VertexColor.a = KernelMapA[WHelper][HHelper];
					for (auto point : kWorld->GetHTDPoints(w, h))
					{
						if (point.NiColor)
						{
							point.NiColor->r = KernelMapR[WHelper][HHelper];
							point.NiColor->g = KernelMapG[WHelper][HHelper];
							point.NiColor->b = KernelMapB[WHelper][HHelper];
							point.NiColor->a = KernelMapA[WHelper][HHelper];
						}
						if (point.NiGeometry)
							point.NiGeometry->MarkAsChanged(NiGeometryData::COLOR_MASK);
					}

					
					HHelper++;
				}
				HHelper = 0;
				WHelper++;
			}
		}
		KernelMapR.clear();
		KernelMapG.clear();
		KernelMapB.clear();
		KernelMapA.clear();
	}

private:
	int KernelSize = 3;
	int Cycles = 5;
};