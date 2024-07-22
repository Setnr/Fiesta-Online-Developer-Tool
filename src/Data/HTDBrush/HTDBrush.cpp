#include "HTDBrush.h"
#include "ChangeBrush.h"
#include "LevelingBrush.h"
#include "SmoothingBrush.h"
#include "PerlinBrush.h"
#include "WorleyBrush.h"
#include "HightBrush.h"
HTDBrushPtr HTDBrush::Draw()
{
	HTDBrushPtr ptr;
	ImGuiIO& io = ImGui::GetIO();
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

	int h = 295;
	int w = 400;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w, io.DisplaySize.y - h));
	ImGui::SetNextWindowSize(ImVec2(w, h));

	if (io.MouseWheel > 0.0f)
	{
		if (BrushSize <  20)
			BrushSize++;
		Node->SetScale((50.f / 160.f) * BrushSize);
	}
	if (io.MouseWheel < 0.0f) 
	{
		if (BrushSize > 1)
			BrushSize--;
		else
			BrushSize = 1;
		Node->SetScale((50.f / 160.f) * BrushSize);
	}

	if (ImGui::Begin("HTDG-Editor"))
	{
		ImGui::LabelText("Current Brush", "%s", this->GetName());
		if (ImGui::SliderInt("BrushSize", &BrushSize, 1, 20))
			Node->SetScale((50.f / 160.f) * BrushSize);

		ImGui::LabelText("", "X: %.2f, Y: %.2f, Z: %.2f", _Intersect.x, _Intersect.y, _Intersect.z);
		
		ImGui::BeginChild("BrushChildL", ImVec2(100, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		bool s;
		if (ImGui::Selectable("Change Brush", &s))
			ptr = NiNew ChangeBrush(Node, BrushSize);
		if (ImGui::Selectable("Leveling Brush", &s))
			ptr = NiNew LevelingBrush(Node, BrushSize);
		if (ImGui::Selectable("Hight Brush", &s))
			ptr = NiNew HightBrush(Node, BrushSize);
		if (ImGui::Selectable("Smoothing Brush", &s))
			ptr = NiNew SmoothingBrush(Node, BrushSize);

		if (ImGui::Selectable("Perlin Brush", &s))
			ptr = NiNew PerlinBrush(Node, BrushSize);
		if (ImGui::Selectable("Worley Brush", &s))
			ptr = NiNew WorleyBrush(Node, BrushSize);
		
		ImGui::EndChild();
		ImGui::SameLine();
		this->DrawInternal();
		ImGui::End();
	}
	if (ptr)
		ptr->Init();
	return ptr;
	}