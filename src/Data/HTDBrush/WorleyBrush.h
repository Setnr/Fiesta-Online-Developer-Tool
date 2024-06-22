#pragma once
#include "TextureBrush.h"
#include "WorleyBrush.h"
#include "ImGui/imgui.h"
#include "../Algorithms/worley.h"
NiSmartPointer(WorleyBrush);
class WorleyBrush : public TextureBrush
{
public:
	WorleyBrush(NiNodePtr HTDOrbNode, int BrushSize) : TextureBrush(HTDOrbNode, BrushSize), wg(Distance)
	{
		
	}
	
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
		if (ImGui::DragFloat("Color", &ColorAdjust)&& ColorAdjust < 550.f)
			UpdateTexture = true;
		if (ImGui::DragInt("Points", &NumPoints) && NumPoints < 25)
			UpdateTexture = true;
		if (ImGui::DragInt("Closes Point", &PointLoc,1.f,0, NumPoints) && PointLoc <= NumPoints)
			UpdateTexture = true;
		//if(ImGui::InputInt("Distance", &Distance))
		//	UpdateTexture = true;
		if(UpdateTexture && NumPoints > 0)
		{
			UpdatePixelData();
		}
		
		ImGui::EndChild();	

	}
	virtual const char* GetName() { return "Worley Brush"; }


	void UpdatePixelData() override;

private:
	DistType Distance = DistType::euclidean;
	int seed = 123456u;
	int NumPoints = 10;
	float ColorAdjust = 250.f;
	WorleyGrid wg;
	int PointLoc = 0;
};