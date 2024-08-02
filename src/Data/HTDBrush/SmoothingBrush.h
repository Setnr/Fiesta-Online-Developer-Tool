#pragma once
#include "Brush.h"
#include <vector>
NiSmartPointer(World);
class SmoothingBrush : public HTDBrush
{
public:
	SmoothingBrush(TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) : HTDBrush(kWorld, HTDOrbNode, BrushSize) {}
	virtual void DrawInternal()
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::DragInt("Kernel Size", &KernelSize, 2.f, 3, 21);
		ImGui::DragInt("Cycles", &Cycles, 1.f, 1, 20);
		ImGui::EndChild();
	}
	virtual const char* GetName() { return "Smoothing"; }

	virtual void Update()
	{
		float CurTime = NiGetCurrentTimeInSec();
		if (UpdateTime + 0.05f > CurTime)
			return;
		UpdateTime = CurTime;


		IniFile& _InitFile = kWorld->GetIniFile();;
		std::vector<std::vector<TerrainWorld::PointInfos>>& HTD = kWorld->GetHTD();

		int middlew = _Intersect.x / _InitFile.OneBlock_width;
		int middleh = _Intersect.y / _InitFile.OneBlock_height;
		float KernelValue = 1 / static_cast<float>(KernelSize * KernelSize);

		int StartI = -1 * (KernelSize / 2);
		int EndI = KernelSize / 2;
		int StartJ = -1 * (KernelSize / 2);
		int EndJ = KernelSize / 2;

		for (int cycle = 0; cycle < Cycles; cycle++)
		{
				
			struct PointData { float height; int w; int h; };
			std::vector <std::pair<PointData, TerrainWorld::HTDPointData>> UpdateVector;
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
					for (int i = StartI; i <= EndI; i++)
					{
						for (int j = StartJ; j <= EndJ; j++)
						{
							Sum += HTD[w - i][h - j].GetHeight() * KernelValue;
							
						}
					}

					float NewHDTGWeight = Sum - HTD[w][h].HTDHeight;
					for (TerrainWorld::HTDPointData& point : kWorld->GetHTDPoints(w, h))
					{
						UpdateVector.push_back({ {NewHDTGWeight,w,h }, point });
					}
				}
			}
			for (auto& data : UpdateVector)
			{
				PointData d = data.first;
				auto point = data.second;

				HTD[d.w][d.h].HTDGHeight = d.height;
				if (point.NiPoint)
					point.NiPoint->z = HTD[d.w][d.h].GetHeight();
				if (point.NiGeometry)
					point.NiGeometry->MarkAsChanged(NiGeometryData::VERTEX_MASK);
			}
		}
	}

private:
	int KernelSize = 3;
	int Cycles = 1;
	float UpdateTime = 0.f;
};