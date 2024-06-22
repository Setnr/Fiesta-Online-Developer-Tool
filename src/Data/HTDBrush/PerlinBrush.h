#pragma once
#include "TextureBrush.h"
#include "PerlinNoise.hpp"
#include "ImGui/imgui.h"
NiSmartPointer(PerlinBrush);
class PerlinBrush : public TextureBrush
{
public:
	PerlinBrush(NiNodePtr HTDOrbNode, int BrushSize) : TextureBrush(HTDOrbNode, BrushSize){}
	
	virtual void DrawInternal() 
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
		this->DrawTextureBrush();
		bool UpdateTexture = false;
		if (ImGui::DragInt("Seed", (int*)&seed)) 
			UpdateTexture = true;
		ImGui::SameLine();
		if (ImGui::Button("Random"))
		{
			seed = std::rand();
			UpdateTexture = true;
		}
		if(ImGui::InputInt("Ocatves", &Octaves) && Octaves < 257)
			UpdateTexture = true;
		if(UpdateTexture)
		{
			perlin = siv::PerlinNoise(seed);
			UpdatePixelData();
		}
		
		ImGui::EndChild();	

	}
	virtual const char* GetName() { return "Perlin Brush"; }

	/*virtual void UpdateHTD(IniFile& _InitFile, std::vector<std::vector<World::HTDHelper>>& HTD, NiPoint3 InterSect)
	{
		int middlew = InterSect.x / _InitFile.OneBlock_width;
		int middleh = InterSect.y / _InitFile.OneBlock_height;
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
				float z = perlin.octave2D_01(static_cast<float>(w), static_cast<float>(h), Octaves) * ChangeSize;
				for (auto point : HTD[w][h].Vec)
					if (point)
						point->z += z;
				for (auto shape : HTD[w][h].Shape)
					if (shape)
						shape->MarkAsChanged(NiGeometryData::VERTEX_MASK);
			}
		}
	}*/
	void UpdatePixelData() override;


private:
	float ChangeSize = 1.f;
	int Octaves = 16;
	siv::PerlinNoise::seed_type seed = 123456u;
	siv::PerlinNoise perlin{ seed };
	
};