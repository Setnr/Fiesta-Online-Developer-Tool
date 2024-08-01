#include "Brush.h"
#include "ChangeBrush.h"
#include "LevelingBrush.h"
#include "SmoothingBrush.h"
#include "PerlinBrush.h"
#include "WorleyBrush.h"
#include "HightBrush.h"
#include "PixelBrush.h"
#include "TextureSmoothingBrush.h"
#include "PerlinTextureBrush.h"
#include "./VertexBrush/ColorBrush.h"
#include "./VertexBrush/SmoothingBrush.h"

NiImplementRootRTTI(Brush);
NiImplementRTTI(HTDTextureBrush, Brush);
NiImplementRTTI(HTDBrush, Brush);
NiImplementRTTI(VertexBrush, Brush);

#define BrushSizeMax 100
#define BrushSizeMin 0

BrushPtr HTDBrush::Draw()
{
	BrushPtr ptr;
	ImGuiIO& io = ImGui::GetIO();
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

	int h = 295;
	int w = 400;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w, io.DisplaySize.y - h));
	ImGui::SetNextWindowSize(ImVec2(w, h));

	if (io.MouseWheel > 0.0f)
	{
		if (BrushSize < BrushSizeMax)
			BrushSize++;
		Node->SetScale((50.f / 160.f) * BrushSize);
	}
	if (io.MouseWheel < 0.0f) 
	{
		if (BrushSize > BrushSizeMin)
			BrushSize--;
		else
			BrushSize = 1;
		Node->SetScale((50.f / 160.f) * BrushSize);
	}

	if (ImGui::Begin("HTDG-Editor"))
	{
		ImGui::LabelText("Current Brush", "%s", this->GetName());
		if (ImGui::SliderInt("BrushSize", &BrushSize, BrushSizeMin, BrushSizeMax))
			Node->SetScale((50.f / 160.f) * BrushSize);

		ImGui::LabelText("", "X: %.2f, Y: %.2f, Z: %.2f", _Intersect.x, _Intersect.y, _Intersect.z);
		
		ImGui::BeginChild("BrushChildL", ImVec2(100, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		bool s;
		if (ImGui::Selectable("Change Brush", &s))
			ptr = NiNew ChangeBrush(kWorld, Node, BrushSize);
		if (ImGui::Selectable("Leveling Brush", &s))
			ptr = NiNew LevelingBrush(kWorld, Node, BrushSize);
		if (ImGui::Selectable("Height Brush", &s))
			ptr = NiNew HeightBrush(kWorld, Node, BrushSize);
		if (ImGui::Selectable("Smoothing Brush", &s))
			ptr = NiNew SmoothingBrush(kWorld, Node, BrushSize);

		if (ImGui::Selectable("Perlin Brush", &s))
			ptr = NiNew PerlinBrush(kWorld, Node, BrushSize);
		if (ImGui::Selectable("Worley Brush", &s))
			ptr = NiNew WorleyBrush(kWorld, Node, BrushSize);
		
		ImGui::EndChild();
		ImGui::SameLine();
		this->DrawInternal();
		ImGui::End();
	}
	if (ptr)
		ptr->Init();
	return ptr;
}

BrushPtr HTDTextureBrush::Draw()
{
	if (!Layer)
		return NULL;
	BrushPtr ptr;
	ImGuiIO& io = ImGui::GetIO();
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

	int h = 250;
	int w = 380;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w, io.DisplaySize.y - h - 230));
	ImGui::SetNextWindowSize(ImVec2(w, h));

	if (io.MouseWheel > 0.0f)
	{
		if (BrushSize < BrushSizeMax)
			BrushSize++;
		Node->SetScale((50.f / 160.f) * BrushSize);
	}
	if (io.MouseWheel < 0.0f)
	{
		if (BrushSize > BrushSizeMin)
			BrushSize--;
		else
			BrushSize = 1;
		Node->SetScale((50.f / 160.f) * BrushSize);
	}

	if (ImGui::Begin("Color-Editor"))
	{
		ImGui::LabelText("Current Brush", "%s", this->GetName());
		if (ImGui::SliderInt("BrushSize", &BrushSize, BrushSizeMin, BrushSizeMax))
			Node->SetScale((50.f / 160.f) * BrushSize);

		ImGui::LabelText("", "X: %.2f, Y: %.2f, Z: %.2f", _Intersect.x, _Intersect.y, _Intersect.z);
		if (ImGui::DragFloat("Min Height", &MinHeight,1.f,-5000.f,MaxHeight))
		{
			UpdateViaHeight();
		}
		if (ImGui::DragFloat("Max Height", &MaxHeight, 1.f, MinHeight, 5000.f))
		{
			UpdateViaHeight();
		}
		ImGui::BeginChild("BrushChildL", ImVec2(100, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		bool s;
		if (ImGui::Selectable("Pixel Brush", &s))
			ptr = NiNew PixelBrush(Layer, kWorld, Node, BrushSize);
		if (ImGui::Selectable("Smoothing Brush", &s))
			ptr = NiNew TextureSmoothingBrush(Layer, kWorld, Node, BrushSize);
		if (ImGui::Selectable("Perlin Brush", &s))
			ptr = NiNew PerlinTextureBrush(Layer, kWorld, Node, BrushSize);

		ImGui::EndChild();
		ImGui::SameLine();
		this->DrawInternal();
		ImGui::End();
	}
	if (ptr)
		ptr->Init();
	return ptr;
}

std::shared_ptr<TerrainLayer>  HTDTextureBrush::NewLayer(std::string BlendFileName, float Width, float Height)
{
	auto Layer = std::make_shared<TerrainLayer>();
	Layer->Name = std::to_string(LayerCt) + " New Layer";
	LayerCt++;
	Layer->DiffuseFileName = ".\\resmap\\fieldTexture\\grass_01.dds";
	Layer->BlendFileName = BlendFileName;
	Layer->StartPos_X = .0f;
	Layer->StartPos_Y = .0f;
	Layer->Width = Width + 1;
	Layer->Height = Height + 1;
	Layer->UVScaleDiffuse = 5.0f;
	Layer->UVScaleBlend = 1.0f;
	Layer->CreateTexture();
	kWorld->CreateTerrainLayer(Layer);

	ChangeLayer(Layer);

	return Layer;
}

void HTDTextureBrush::ChangeLayer(std::shared_ptr<TerrainLayer> Layer)
{
	this->Layer = Layer;
	HideScreenElement(pScreenElementTextureEdit);

	Layer->BlendTexture->SetStatic(false);

	pScreenElementTextureEdit = PgUtil::CreateScreenElement(256, 256, Layer->BlendTexture);
	pScreenElementTextureEdit->UpdateProperties();

	auto& io = ImGui::GetIO();
	float fLeft = 1.f - 256 / io.DisplaySize.x;
	float fTop = 1.f - 736 / io.DisplaySize.y;
	float fBottom = fTop + 256 / io.DisplaySize.y;
	float fRight = 1.f;
	pScreenElementTextureEdit->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
	pScreenElementTextureEdit->SetVertex(0, 1, NiPoint2(fRight, fBottom));
	pScreenElementTextureEdit->SetVertex(0, 2, NiPoint2(fRight, fTop));
	pScreenElementTextureEdit->SetVertex(0, 3, NiPoint2(fLeft, fTop));
	pScreenElementTextureEdit->UpdateBound();
	ShowScreenElement(pScreenElementTextureEdit);
	UpdateTexturePreview();
}

BrushPtr VertexBrush::Draw()
{
	BrushPtr ptr;
	ImGuiIO& io = ImGui::GetIO();
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

	int h = 295;
	int w = 400;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w, io.DisplaySize.y - h));
	ImGui::SetNextWindowSize(ImVec2(w, h));

	if (io.MouseWheel > 0.0f)
	{
		if (BrushSize < BrushSizeMax)
			BrushSize++;
		Node->SetScale((50.f / 160.f) * BrushSize);
	}
	if (io.MouseWheel < 0.0f)
	{
		if (BrushSize > BrushSizeMin)
			BrushSize--;
		else
			BrushSize = 1;
		Node->SetScale((50.f / 160.f) * BrushSize);
	}

	if (ImGui::Begin("VertexColor-Editor"))
	{
		ImGui::LabelText("Current Brush", "%s", this->GetName());
		if (ImGui::SliderInt("BrushSize", &BrushSize, BrushSizeMin, BrushSizeMax))
			Node->SetScale((50.f / 160.f) * BrushSize);

		ImGui::LabelText("", "X: %.2f, Y: %.2f, Z: %.2f", _Intersect.x, _Intersect.y, _Intersect.z);
		ImGui::SameLine();
		ImGui::ColorButton("Current Color", *(ImVec4*)&CurColor);
		ImGui::BeginChild("BrushChildL", ImVec2(100, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		bool s;
		if (ImGui::Selectable("Color Brush", &s))
			ptr = NiNew ColorBrush(kWorld, Node, BrushSize);
		if (ImGui::Selectable("Smoothing Brush", &s))
			ptr = NiNew VertexSmoothingBrush(kWorld, Node, BrushSize);

		ImGui::EndChild();
		ImGui::SameLine();
		this->DrawInternal();
		ImGui::End();
	}
	if (ptr)
		ptr->Init();
	return ptr;
}
