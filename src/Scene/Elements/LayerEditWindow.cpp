#include "LayerEditWindow.h"
#include <ImGui/imgui.h>
#include "../../NiApplication/FiestaOnlineTool.h"
#define TextureWidth 512.f
#define TextureHeight 512.f
LayerEditWindow::LayerEditWindow(TerrainWorldPtr& World) : kWorld(World) 
{
	
}
LayerEditWindow::~LayerEditWindow() 
{
	kWorld = 0;
	if(pScreenElementTextureEdit)
	{
		pScreenElementTextureEdit = NULL;
		//FiestaOnlineTool::RemoveScreenElemets(pScreenElementTextureEdit);
	}
	if(pScreenElementTexturePreview)
		FiestaOnlineTool::RemoveScreenElemets(pScreenElementTexturePreview);
}
bool LayerEditWindow::Show() 
{
	bool UpdateTerrainLayer = false; //Set to True if Layer Needs to be recreated;
	ImGuiIO& io = ImGui::GetIO();
	if (!_Show)
		return UpdateTerrainLayer;
	ImGui::SetNextWindowSize(ImVec2(200.f, 300.f));
	if (ImGui::Begin("Change Texture",0,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
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
			float Col = static_cast<float>(BrushColor) / 255;
			ImGui::SameLine(); ImGui::ColorButton("Preview Color", ImVec4(Col, Col, Col, Col));
			ImGui::DragInt("BrushSize", &BrushSize, 1.f, 0, 100);
			ImGui::EndChild();
		}
		if(pScreenElementTextureEdit)
		{
			UpdateTexturePos();
			NiTexturingPropertyPtr prop = (NiTexturingProperty*)pScreenElementTextureEdit->GetProperty(NiProperty::TEXTURING);
			auto TexturProp = prop->GetBaseTextureTransform();
			if (!TexturProp) 
			{
				LogError("Something went Horribly Wrong and Texture doesnt have a BaseTextureTransform");
				return false;
			}
			float x, y;
			auto data = this->Layer->BlendTexture->GetSourcePixelData();
			TerrainLayer::RGBAColor* pixel = (TerrainLayer::RGBAColor*)data->GetPixels();
			NiPoint2 Scale = TexturProp->GetScale();
			auto Translate = TexturProp->GetTranslate();
			//float MaxValue = Scale.x;

			//y = y * Scale.x + (1.f - Scale.x) - Translate.x;
			//x = x * Scale.x + Translate.y;
			//int yPixel = data->GetHeight() - (data->GetHeight() * y);  //Currently no clue why x and y are wrong order
			//int xPixel = (data->GetWidth() * x);

			static float LastDrawTime;
			float CurTime = NiGetCurrentTimeInSec();

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && LastDrawTime + pow(io.Framerate, -0.5f) < CurTime && !ImGui::IsAnyItemActive() && !ImGui::IsAnyItemHovered())
			{
				LastDrawTime = CurTime;

				NiPoint3 kOrigin, kDir;
				long X, Y;
				FiestaOnlineTool::GetMousePosition(X, Y);
				if (kWorld->GetCamera()->WindowPointToRay(X, Y, kOrigin, kDir))
				{
					NiPick _Pick;
					_Pick.SetPickType(NiPick::FIND_FIRST);
					_Pick.SetSortType(NiPick::SORT);
					_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
					_Pick.SetFrontOnly(true);
					_Pick.SetReturnNormal(true);
					_Pick.SetObserveAppCullFlag(true);
					_Pick.SetTarget(kWorld->GetTerrainScene());
					if (_Pick.PickObjects(kOrigin, kDir, true))
					{
						NiPick::Results& results = _Pick.GetResults();
						NiPoint3 Intersect = results.GetAt(0)->GetIntersection();
						IniFile& _InitFile = kWorld->GetIniFile();
						int xPixel = Intersect.x / _InitFile.OneBlock_width;
						int yPixel = Intersect.y / _InitFile.OneBlock_height;

						int wh = -1;
						for (int w = xPixel - BrushSize; w <= xPixel + BrushSize && w < static_cast<int>(data->GetWidth()); w++)
						{
							wh++;
							if (w < 0)
								continue;
							int hh = -1;
							for (int h = yPixel - BrushSize; h <= yPixel + BrushSize && h < static_cast<int>(data->GetHeight()); h++)
							{
								hh++;
								if (h < 0)
									continue;
								if (!((w - xPixel) * (w - xPixel) + (h - yPixel) * (h - yPixel) <= BrushSize * BrushSize))
									continue;
								int XPart = w;

								int PreFullLines = Layer->BlendTexture->GetWidth() * h;
								int YPartNormal = PreFullLines;
								int PointOffsetNormal = XPart + YPartNormal;
								pixel[PointOffsetNormal] = TerrainLayer::RGBAColor((char)BrushColor);

							}
						}

						UpdateTerrainLayer = true;
						data->MarkAsChanged();


					}
				}

			}
			bool W_Key = ImGui::IsKeyDown((ImGuiKey)0x57);
			bool S_Key = ImGui::IsKeyDown((ImGuiKey)0x53);
			bool A_Key = ImGui::IsKeyDown((ImGuiKey)0x41);
			bool D_Key = ImGui::IsKeyDown((ImGuiKey)0x44);
			bool CtrlKey = ImGui::IsKeyDown((ImGuiKey)VK_CONTROL);
			if (io.MouseWheel != 0.0f)
			{
				Scale.x -= io.MouseWheel * 0.03f;
				Scale.y -= io.MouseWheel * 0.03f;

				if (Scale.x > 1.f || Scale.y > 1.f)
					Scale = NiPoint2(1.f, 1.f);
				if (Scale.x <= 0.f || Scale.y <= 0.f)
					Scale = NiPoint2(0.05f, 0.05f);
				TexturProp->SetScale(Scale);
			}

			if (PgUtil::HoveringScreenElement(pScreenElementTextureEdit, x, y))
			{
				if (CtrlKey || io.MouseWheel != 0.0f)
				{
					NiPoint2 Scale = TexturProp->GetScale();
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
					if (Translate.y < 0.f)
						Translate.y = 0.f;


					if (Translate.x > 1.f * ( 1.f - Scale.x))
						Translate.x = 1.f * (1.f - Scale.x);
					if (Translate.y > 1.f * (1.f - Scale.y))
						Translate.y = 1.f * (1.f - Scale.y);
					TexturProp->SetTranslate(Translate);
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
	pScreenElementTextureEdit->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
	pScreenElementTextureEdit->SetVertex(0, 1, NiPoint2(fRight, fBottom));
	pScreenElementTextureEdit->SetVertex(0, 2, NiPoint2(fRight, fTop));
	pScreenElementTextureEdit->SetVertex(0, 3, NiPoint2(fLeft, fTop));
	pScreenElementTextureEdit->UpdateBound();
}

std::shared_ptr<TerrainLayer>  LayerEditWindow::NewLayer(std::string BlendFileName, float Width, float Height)
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

void LayerEditWindow::ChangeLayer(std::shared_ptr<TerrainLayer> Layer) 
{
	this->Layer = Layer;
	_Show = true;
	if(pScreenElementTextureEdit)
		pScreenElementTextureEdit = NULL;
	//FiestaOnlineTool::RemoveScreenElemets(pScreenElementTextureEdit);
	Layer->BlendTexture->SetStatic(false);
	pScreenElementTextureEdit = PgUtil::CreateScreenElement(TextureWidth, TextureHeight, Layer->BlendTexture);
	pScreenElementTextureEdit->UpdateProperties();
	//FiestaOnlineTool::AddScreenElemets(pScreenElementTextureEdit);
	UpdateTexturePreview();
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