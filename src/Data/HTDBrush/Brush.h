#pragma once
#include <NiNode.h>
#include "ImGui/imgui.h"
#include "../IniFile.h"
#include "../IngameWorld.h"
#include <NiRTTI.h>
#include "../../NiApplication/FiestaOnlineTool.h"
NiSmartPointer(Brush);
NiSmartPointer(HTDBrush);
NiSmartPointer(HTDTextureBrush);
class Brush : public NiRefObject 
{
	NiDeclareRootRTTI(Brush);
public:
	Brush(TerrainWorldPtr World,NiNodePtr HTDOrbNode,int BrushSize) : kWorld(World), Node(HTDOrbNode), BrushSize(BrushSize) {}
	virtual BrushPtr Draw() { return NULL; }
	
	virtual void Update(){}
	virtual void DrawInternal() {}
	virtual const char* GetName() { return "-"; }
	virtual void Show(bool Show){}
	virtual void Init() {}
	void SetIntersect(NiPoint3 Intersect) { _Intersect = Intersect; }
	virtual void MouseClick() { InitMouse = true; }
	void MouseRelease() { InitMouse = false; }
	NiPoint3 GetIntersect() { return _Intersect; }
	void UpdateWorld(TerrainWorldPtr World) { kWorld = World; }
protected:
	NiNodePtr Node;
	int BrushSize;
	bool InitMouse = false;
	NiPoint3 _Intersect;
	TerrainWorldPtr kWorld;
};

class HTDBrush : public Brush
{
	NiDeclareRTTI;
public:
	HTDBrush(TerrainWorldPtr kWorld,NiNodePtr HTDOrbNode, int BrushSize) : Brush(kWorld,HTDOrbNode, BrushSize){}
	virtual BrushPtr Draw();
};

class HTDTextureBrush : public Brush
{
	NiDeclareRTTI;

public:
	HTDTextureBrush(std::shared_ptr<TerrainLayer> Layer, TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) : 
		Brush(kWorld, HTDOrbNode, BrushSize) 
	{
		if(Layer)
			ChangeLayer(Layer);
	}
	~HTDTextureBrush() {
		Show(false);
	}
	virtual BrushPtr Draw();
	void UpdateViaHeight() 
	{
		std::vector<std::vector<TerrainWorld::HTDHelper>>& _HTD = kWorld->GetHTD();
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
	void ChangeLayer(std::shared_ptr<TerrainLayer> Layer);
	std::shared_ptr<TerrainLayer>  NewLayer(std::string BledFileName, float Width, float Height);
	void UpdateTexturePreview()
	{
		NiSourceTexturePtr ptr = NiSourceTexture::Create(PgUtil::CreateFullFilePathFromBaseFolder(Layer->DiffuseFileName).c_str());
		HideScreenElement(pScreenElementTexturePreview);
		pScreenElementTexturePreview = PgUtil::CreateScreenElement(128, 128, ptr);
		auto& io = ImGui::GetIO();
		float fLeft = 1.f - 128 / io.DisplaySize.x;
		float fTop = 1.f - 128 / io.DisplaySize.y;
		float fBottom = 1.f;
		float fRight = 1.f;
		pScreenElementTexturePreview->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
		pScreenElementTexturePreview->SetVertex(0, 1, NiPoint2(fRight, fBottom));
		pScreenElementTexturePreview->SetVertex(0, 2, NiPoint2(fRight, fTop));
		pScreenElementTexturePreview->SetVertex(0, 3, NiPoint2(fLeft, fTop));
		pScreenElementTexturePreview->UpdateBound();
		ShowScreenElement(pScreenElementTexturePreview);
	}
	virtual void Show(bool Show) 
	{
		if (Show) 
		{
			ShowScreenElement(pScreenElementTexturePreview);
			ShowScreenElement(pScreenElementTextureEdit);
		}
		else
		{
			HideScreenElement(pScreenElementTexturePreview);
			HideScreenElement(pScreenElementTextureEdit);
		}
	}
	void HideScreenElement(NiScreenElementsPtr obj) {
		if(obj)
			FiestaOnlineTool::RemoveScreenElemets(obj);
	}
	void ShowScreenElement(NiScreenElementsPtr obj) {
		if (obj)
		{
			FiestaOnlineTool::RemoveScreenElemets(obj);
			FiestaOnlineTool::AddScreenElemets(obj);
		}
	}
	bool UpdateTerrain() {
		bool ret = _UpdateTerrain;
		_UpdateTerrain = false;
		return ret;
	}
	virtual void DrawInternal() 
	{
		if (!pScreenElementTextureEdit)
			return;
		NiTexturingPropertyPtr prop = (NiTexturingProperty*)pScreenElementTextureEdit->GetProperty(NiProperty::TEXTURING);
		auto TexturProp = prop->GetBaseTextureTransform();
		if (!TexturProp)
		{
			LogError("Something went Horribly Wrong and Texture doesnt have a BaseTextureTransform");
			return;
		}

		NiPoint2 Scale = TexturProp->GetScale();
		auto Translate = TexturProp->GetTranslate();
		ImGuiIO& io = ImGui::GetIO();

		bool W_Key = ImGui::IsKeyDown((ImGuiKey)0x57);
		bool S_Key = ImGui::IsKeyDown((ImGuiKey)0x53);
		bool A_Key = ImGui::IsKeyDown((ImGuiKey)0x41);
		bool D_Key = ImGui::IsKeyDown((ImGuiKey)0x44);
		bool CtrlKey = ImGui::IsKeyDown((ImGuiKey)VK_CONTROL);
		if (io.MouseWheel != 0.0f)
		{
			float d1, d2; //Dead for FunctionCall
			if (PgUtil::HoveringScreenElement(pScreenElementTextureEdit, d1, d2))
			{
				Scale.x -= io.MouseWheel * 0.03f;
				Scale.y -= io.MouseWheel * 0.03f;

				if (Scale.x > 1.f || Scale.y > 1.f)
					Scale = NiPoint2(1.f, 1.f);
				if (Scale.x <= 0.f || Scale.y <= 0.f)
					Scale = NiPoint2(0.05f, 0.05f);
				TexturProp->SetScale(Scale);
			}
		}
		float x, y;
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


				if (Translate.x > 1.f * (1.f - Scale.x))
					Translate.x = 1.f * (1.f - Scale.x);
				if (Translate.y > 1.f * (1.f - Scale.y))
					Translate.y = 1.f * (1.f - Scale.y);
				TexturProp->SetTranslate(Translate);
			}

		}
	}
protected:
	void UpdateHTD();
	bool _UpdateTerrain = false;
	float MinHeight = 0.0f;
	float MaxHeight = 0.1f;
	std::shared_ptr<TerrainLayer> Layer;
	int LayerCt = 1;
	NiScreenElementsPtr pScreenElementTextureEdit;
	NiScreenElementsPtr pScreenElementTexturePreview;
};