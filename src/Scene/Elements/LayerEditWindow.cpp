#include "LayerEditWindow.h"
#include <ImGui/imgui.h>
#include "../../NiApplication/FiestaOnlineTool.h"
LayerEditWindow::LayerEditWindow(TerrainWorldPtr World) : kWorld(World) 
{

}

bool LayerEditWindow::Show() 
{
	bool ReturnValue = false; //Set to True if Layer Needs to be recreated;
	if (!_Show)
		return ReturnValue;
	if (ImGui::Begin("Change Texture",0,ImGuiWindowFlags_NoCollapse ))
	{
		if (ImGui::BeginChild("Settings Preview")) 
		{
			if (ImGui::DragFloat("Min Height", &MinHeight))
				ReturnValue = true;
			if (ImGui::DragFloat("Max Height", &MaxHeight))
				ReturnValue = true;
			ImGui::EndChild();
		}
		if(pkScreenTexture)
		{
			/*NiScreenTexture::ScreenRect& rect = pkScreenTexture->GetScreenRect(0);
			auto pos = ImGui::GetWindowPos();
			auto size = ImGui::GetWindowSize();
			rect.m_sPixTop = pos.y;
			rect.m_sPixLeft = pos.x + size.x;
			pkScreenTexture->MarkAsChanged(NiScreenTexture::VERTEX_MASK);*/
		}
		
		
		ImGui::End();
	}
	return ReturnValue;
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
	pkScreenTexture = NiNew NiScreenElements(NiNew NiScreenElementsData(false, true, 1));
	pkScreenTexture->Insert(4);
	pkScreenTexture->SetRectangle(0, 0.4f, 0.4f, 0.8f, 0.8f);
	pkScreenTexture->UpdateBound();
	pkScreenTexture->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);
	pkScreenTexture->SetColors(0, NiColorA::WHITE);
	float fLeft = 0.4f;
	float fBottom = 0.4f;
	float fRight = 0.8f;
	float fTop = 0.8f;
	pkScreenTexture->SetVertex(0, 0, NiPoint2(fLeft, 1.0f - fBottom));
	pkScreenTexture->SetVertex(0, 1, NiPoint2(fRight, 1.0f - fBottom));
	pkScreenTexture->SetVertex(0, 2, NiPoint2(fRight, 1.0f - fTop));
	pkScreenTexture->SetVertex(0, 3, NiPoint2(fLeft, 1.0f - fTop));
	pkScreenTexture->UpdateBound();
	
	NiTexturingPropertyPtr prop = NiNew NiTexturingProperty();
	prop->SetBaseTexture(Layer->BlendTexture);
	prop->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
	prop->SetBaseFilterMode(NiTexturingProperty::FILTER_BILERP);
	NiVertexColorProperty* pVertex = NiNew NiVertexColorProperty;
	pVertex->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
	pVertex->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
	pkScreenTexture->AttachProperty(pVertex);

	// use alpha blending
	NiAlphaProperty* pAlpha = NiNew NiAlphaProperty;
	pAlpha->SetAlphaBlending(true);
	pAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	//pAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
	pkScreenTexture->AttachProperty(pAlpha);
	pkScreenTexture->AttachProperty(prop);
	pkScreenTexture->UpdateProperties();
	pkScreenTexture->UpdateEffects();
	pkScreenTexture->Update(0.0f);
	
	FiestaOnlineTool::AddScreenElemets(pkScreenTexture);
	/*FiestaOnlineTool::RemoveScreenTexture(pkScreenTexture);
	pkScreenTexture = NiNew NiScreenTexture(Layer->BlendTexture);
	NiTexturingProperty* prop = pkScreenTexture->GetTexturingProperty();
	NiTextureTransform* transform = NiNew NiTextureTransform();
	transform->SetScale(NiPoint2(512/Layer->BlendTexture->GetWidth(), 512 / Layer->BlendTexture->GetWidth()));
	transform->SetCenter(NiPoint2(transform->GetScale() / 2));
	transform->SetRotate(5.f);
	prop->SetBaseTextureTransform(transform);
	auto text = prop->GetBaseTexture();
	pkScreenTexture->SetApplyMode(NiTexturingProperty::APPLY_REPLACE)dw;
	//pkScreenTexture->SetClampMode(NiTexturingProperty::WRAP_S_WRAP_T);
	//pkScreenTexture->SetApplyMode(NiTexturingProperty::APPLY_DECAL);

	pkScreenTexture->AddNewScreenRect(20, 0, 512, 512, 0, 0);
	FiestaOnlineTool::AddScreenTexture(pkScreenTexture);
	PgUtil::SaveNode(PgUtil::CreateFullFilePathFromBaseFolder(".\\Test.nif"), prop);*/
}

void LayerEditWindow::UpdateLayer(std::vector<std::vector<TerrainWorld::HTDHelper>>& _HTD) 
{
	NiPixelDataPtr data = Layer->BlendTexture->GetSourcePixelData();
	TerrainLayer::RGBAColor* pixel = (TerrainLayer::RGBAColor*)data->GetPixels();
	for (int w = 0; w < _HTD.size(); w++) 
	{
		for (int h = 0; h < _HTD[w].size(); h++)
		{
			int XPart = w;

			int PreFullLines = Layer->BlendTexture->GetWidth() * h;
			int YPartNormal = PreFullLines;
			int PointOffsetNormal = XPart + YPartNormal;
			if (_HTD[w][h].Height >= MinHeight && _HTD[w][h].Height <= MaxHeight)
				pixel[PointOffsetNormal] = TerrainLayer::RGBAColor(TerrainLayer::RGBColor(0xFF));
			else
				pixel[PointOffsetNormal] = TerrainLayer::RGBAColor(TerrainLayer::RGBColor(0x0));
		}
	}
	data->MarkAsChanged();
}