#pragma once
#include "TextureBrush.h"
#include "PerlinNoise.hpp"
#include "ImGui/imgui.h"
NiSmartPointer(PerlinBrush);
class PerlinBrush : public TextureBrush
{
public:
	PerlinBrush(TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) : TextureBrush(kWorld, HTDOrbNode, BrushSize){}
	
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

	void UpdatePixelData() override;


private:
	float ChangeSize = 1.f;
	int Octaves = 16;
	siv::PerlinNoise::seed_type seed = 123456u;
	siv::PerlinNoise perlin{ seed };
	
};