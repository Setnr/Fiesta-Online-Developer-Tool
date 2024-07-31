#include "LayerEditWindow.h"
#include <ImGui/imgui.h>
#include "../../NiApplication/FiestaOnlineTool.h"
#define TextureWidth 512.f
#define TextureHeight 512.f
LayerEditWindow::LayerEditWindow(TerrainWorldPtr World) : kWorld(World) 
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
	NiPoint3 Intersect = NiPoint3::ZERO;
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
			Intersect = results.GetAt(0)->GetIntersection();
		}
	}
	OrbNode->SetTranslate(Intersect);


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
			if (ImGui::DragInt("BrushSize", &BrushSize, 1.f, 0, 100)) 
			{
				OrbNode->SetScale((50.f / 160.f) * BrushSize);
			}
			std::string Coords = "Mouse Pos \nX: " + std::to_string(static_cast<int>(Intersect.x)) + "\nY: " + std::to_string(static_cast<int>(Intersect.y));
			ImGui::Text(Coords.c_str());
			ImGui::Checkbox("Show Texture", &ShowTexture);
			ImGui::EndChild();
		}
		if(pScreenElementTextureEdit)
			UpdateTexturePos();
		if(pScreenElementTextureEdit && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemActive())
		{
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


			IniFile& _InitFile = kWorld->GetIniFile();
			int xPixel = Intersect.x / _InitFile.OneBlock_width;
			int yPixel = Intersect.y / _InitFile.OneBlock_height;
			static int LastxPixel = 0;
			static int LastYPixel = 0;
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && xPixel != LastxPixel && yPixel != LastYPixel)// && LastDrawTime + pow(io.Framerate, -0.5f) < CurTime && !ImGui::IsAnyItemActive() )
			{
				LastxPixel = xPixel;
				LastYPixel = yPixel;
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
			
			bool W_Key = ImGui::IsKeyDown((ImGuiKey)0x57);
			bool S_Key = ImGui::IsKeyDown((ImGuiKey)0x53);
			bool A_Key = ImGui::IsKeyDown((ImGuiKey)0x41);
			bool D_Key = ImGui::IsKeyDown((ImGuiKey)0x44);
			bool CtrlKey = ImGui::IsKeyDown((ImGuiKey)VK_CONTROL);
			if (io.MouseWheel != 0.0f)
			{
				float d1, d2; //Dead for FunctionCall
				if(PgUtil::HoveringScreenElement(pScreenElementTextureEdit,d1,d2))
				{
					Scale.x -= io.MouseWheel * 0.03f;
					Scale.y -= io.MouseWheel * 0.03f;

					if (Scale.x > 1.f || Scale.y > 1.f)
						Scale = NiPoint2(1.f, 1.f);
					if (Scale.x <= 0.f || Scale.y <= 0.f)
						Scale = NiPoint2(0.05f, 0.05f);
					TexturProp->SetScale(Scale);
				}
				else {
					if (io.MouseWheel > 0.0f)
					{
						if (BrushSize < 100)
							BrushSize++;
						OrbNode->SetScale((50.f / 160.f) * BrushSize);
					}
					if (io.MouseWheel < 0.0f)
					{
						if (BrushSize > 1)
							BrushSize--;
						else
							BrushSize = 1;
						OrbNode->SetScale((50.f / 160.f) * BrushSize);
					}
				}
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

	static ImVec2 LastPos = { 0,0 };
	static ImVec2 LastSize = { 0,0 };

	if (pos.x == LastPos.x && pos.y == LastPos.y && size.x == LastSize.x && size.y == LastSize.y)
		return;

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

	if (!OrbNode)
	{
		PgUtil::LoadNodeNifFile(PgUtil::CreateFullFilePathFromApplicationFolder(".\\FiestaOnlineTool\\HTDCircle.nif").c_str(), &OrbNode, NULL);
		NiWireframePropertyPtr ptr = NiNew NiWireframeProperty;
		ptr->SetWireframe(true);
		OrbNode->AttachProperty(ptr);
		NiNodePtr terrain = kWorld->GetGroundObjNode();
		if (terrain)
		{
			terrain->AttachChild(OrbNode);
			terrain->UpdateProperties();
			terrain->UpdateEffects();
			terrain->Update(0.0f);
		}
		else
			LogError("LayerEditWindow cant attach OrbNode because Terrain is empty");
	}
}

void LayerEditWindow::UpdateLayer(std::vector<std::vector<TerrainWorld::PointInfos>>& _HTD) 
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
			if (_HTD[w][h].GetHeight() >= MinHeight && _HTD[w][h].GetHeight() <= MaxHeight)
				pixel[PointOffsetNormal] = TerrainLayer::RGBAColor((char)0xFF);
			else
				pixel[PointOffsetNormal] = TerrainLayer::RGBAColor((char)0x0);
		}
	}
	data->MarkAsChanged();
}