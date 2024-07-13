#include "LayerEditWindow.h"
#include <ImGui/imgui.h>
#include "../../NiApplication/FiestaOnlineTool.h"
#define TextureWidth 512.f
#define TextureHeight 512.f
LayerEditWindow::LayerEditWindow(TerrainWorldPtr World) : kWorld(World) 
{
	
}

bool LayerEditWindow::Show() 
{
	bool UpdateTerrainLayer = false; //Set to True if Layer Needs to be recreated;
	ImGuiIO& io = ImGui::GetIO();
	if (!_Show)
		return UpdateTerrainLayer;
	if (ImGui::Begin("Change Texture",0,ImGuiWindowFlags_NoCollapse ))
	{
		if (ImGui::BeginChild("Settings Preview")) 
		{
			if (ImGui::DragFloat("Min Height", &MinHeight))
			{
				UpdateTerrainLayer = true;
				UpdateLayer(kWorld->GetHTD());
			}
			if (ImGui::DragFloat("Max Height", &MaxHeight))
			{
				UpdateTerrainLayer = true;
				UpdateLayer(kWorld->GetHTD());
			}
			
			ImGui::DragInt("Color", &BrushColor, 1.f, 0, 255);
			ImGui::EndChild();
		}
		if(pkScreenTexture)
		{
			UpdateTexturePos();
			NiTexturingPropertyPtr prop = (NiTexturingProperty*)pkScreenTexture->GetProperty(NiProperty::TEXTURING);
			auto TexturProp = prop->GetBaseTextureTransform();
			if (!TexturProp) 
			{
				LogError("Something went Horribly Wrong and Texture doesnt have a BaseTextureTransform");
				return false;
			}
			float x, y;
			if(PgUtil::HoveringScreenElement(pkScreenTexture, x, y))
			{
				auto data = this->Layer->BlendTexture->GetSourcePixelData();
				TerrainLayer::RGBAColor* pixel = (TerrainLayer::RGBAColor*)data->GetPixels();
				NiPoint2 Scale = TexturProp->GetScale();
				auto Translate = TexturProp->GetTranslate();
				int yPixel = data->GetHeight() - (data->GetHeight() * y) + data->GetHeight() * Translate.x * Scale.y;  //Currently no clue why x and y are wrong order
				int xPixel = (data->GetWidth() * x) + data->GetWidth() * Translate.y * Scale.x;
				yPixel *= Scale.x;
				xPixel *= Scale.x;

				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					int XPart = xPixel;

					int PreFullLines = Layer->BlendTexture->GetWidth() * yPixel;
					int YPartNormal = PreFullLines;
					int PointOffsetNormal = XPart + YPartNormal;
					pixel[PointOffsetNormal] = TerrainLayer::RGBAColor((char)BrushColor);
					UpdateTerrainLayer = true;
					data->MarkAsChanged();


				}
				bool W_Key = ImGui::IsKeyDown((ImGuiKey)0x57);
				bool S_Key = ImGui::IsKeyDown((ImGuiKey)0x53);
				bool A_Key = ImGui::IsKeyDown((ImGuiKey)0x41);
				bool D_Key = ImGui::IsKeyDown((ImGuiKey)0x44);
				bool CtrlKey = ImGui::IsKeyDown((ImGuiKey)VK_CONTROL);
				if (CtrlKey)
				{
					float MoveValue = 0.03;
					if (W_Key)
						Translate.x += MoveValue;
					if (S_Key)
						Translate.x -= MoveValue;
					if (A_Key)
						Translate.y -= MoveValue;
					if (D_Key)
						Translate.y += MoveValue;
					if (Translate.x < 0.f)
						Translate.x = 0.f;
					if (Translate.x > 1.f)
						Translate.x = 1.f;
					if (Translate.y < 0.f)
						Translate.y = 0.f;
					if (Translate.y > 1.f)
						Translate.y = 1.f;
					TexturProp->SetTranslate(Translate);
				}
				if (io.MouseWheel != 0.0f)
				{
					Scale.x -= io.MouseWheel * 0.03f;
					Scale.y -= io.MouseWheel * 0.03f;
					if (Scale.x > 1.f || Scale.y > 1.f)
						Scale = NiPoint2(1.f, 1.f);
					if (Scale.x <= 0.f || Scale.y <= 0.f)
						Scale = NiPoint2(0.05f, 0.05f);
					TexturProp->SetScale(Scale);
					//pkScreenTexture->UpdateProperties();
				}
			}
		}
		ImGui::End();
	}
	return UpdateTerrainLayer;
}

void LayerEditWindow::UpdateTexturePos() 
{
	auto& io = ImGui::GetIO();
	auto pos = ImGui::GetWindowPos();
	auto size = ImGui::GetWindowSize();
	float fLeft = (pos.x + size.x) / io.DisplaySize.x;
	float fTop = pos.y / io.DisplaySize.y;
	float fBottom = fTop + TextureHeight / io.DisplaySize.y;
	float fRight = TextureWidth / io.DisplaySize.x + fLeft;
	pkScreenTexture->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
	pkScreenTexture->SetVertex(0, 1, NiPoint2(fRight, fBottom));
	pkScreenTexture->SetVertex(0, 2, NiPoint2(fRight, fTop));
	pkScreenTexture->SetVertex(0, 3, NiPoint2(fLeft, fTop));
	pkScreenTexture->UpdateBound();
}

std::shared_ptr<TerrainLayer>  LayerEditWindow::NewLayer(std::string BlendFileName, float Width, float Height)
{
	auto Layer = std::make_shared<TerrainLayer>();
	Layer->Name = "00 New Layer";
	Layer->DiffuseFileName = ".\\resmap\\fieldTexture\\grass_01.dds";
	Layer->BlendFileName = BlendFileName;
	Layer->StartPos_X = .0f;
	Layer->StartPos_Y = .0f;
	Layer->Width = Width;
	Layer->Height = Height;
	Layer->UVScaleDiffuse = 5.0f;
	Layer->UVScaleBlend = 1.0f;
	Layer->CreateTexture();
	kWorld->CreateTerrainLayer(Layer);
	
	ChangeLayer(Layer);
	
	return Layer;
}

void LayerEditWindow::ChangeLayer(std::shared_ptr<TerrainLayer> Layer) 
{
	this->Layer = Layer;
	_Show = true;

	Layer->BlendTexture->SetStatic(false);
	FiestaOnlineTool::RemoveScreenElemets(pkScreenTexture);
	pkScreenTexture = PgUtil::CreateScreenElement(TextureWidth, TextureHeight, Layer->BlendTexture);
	pkScreenTexture->UpdateProperties();
	//FiestaOnlineTool::AddScreenElemets(pkScreenTexture);
}

void LayerEditWindow::UpdateLayer(std::vector<std::vector<TerrainWorld::HTDHelper>>& _HTD) 
{
	NiPixelDataPtr data = Layer->BlendTexture->GetSourcePixelData();
	TerrainLayer::RGBAColor* pixel = (TerrainLayer::RGBAColor*)data->GetPixels();
	for (int w = 0; w < data->GetWidth(); w++)
	{
		for (int h = 0; h < data->GetHeight(); h++)
		{
			int XPart = w;

			int PreFullLines = Layer->BlendTexture->GetWidth() * h;
			int YPartNormal = PreFullLines;
			int PointOffsetNormal = XPart + YPartNormal;
			if (_HTD[w][h].Height >= MinHeight && _HTD[w][h].Height <= MaxHeight)
				pixel[PointOffsetNormal] = TerrainLayer::RGBAColor((char)0xFF);
			else
				pixel[PointOffsetNormal] = TerrainLayer::RGBAColor((char)0x0);
		}
	}
	data->MarkAsChanged();
}