#pragma once
#include "Brush.h"
#include "../../NiApplication/FiestaOnlineTool.h"
NiSmartPointer(TextureBrush);
class TextureBrush : public HTDBrush 
{
#define TextureWidth 200
#define TextureHeight 100
public:
	TextureBrush(TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) : HTDBrush(kWorld, HTDOrbNode, BrushSize) {  }
	~TextureBrush(){ this->Show(false); }
	virtual void Show(bool ShowTexture) 
	{
		if (!pkScreenTexture)
			return;
		if (ShowTexture)
		{
			FiestaOnlineTool::AddScreenTexture(pkScreenTexture);
		}
		else
		{
			FiestaOnlineTool::RemoveScreenTexture(pkScreenTexture);
		}

	}
	virtual void DrawTextureBrush()
	{
		if (ImGui::Selectable("Open Noise Preview"))
		{
			_Show = !_Show;
			Show(_Show);
		}
		ImGui::SliderFloat("Level Change", &ChangeSize, -50.f, 50.f);
		if(ImGui::DragInt("Texture Size", &MapSize, 1.f, 0, 256) && MapSize <= 256)
		{
			this->CreateTexture();
		}
		
	}
	void CreateTexture();
	void Init(){ CreateTexture(); }
	virtual void Update()
	{
		IniFile& _InitFile = kWorld->GetIniFile();;
		std::vector<std::vector<World::HTDHelper>>& HTD = kWorld->GetHTD();
		int middlew = _Intersect.x / _InitFile.OneBlock_width;
		int middleh = _Intersect.y / _InitFile.OneBlock_height;

		RGBAColor* PixelColorA = (RGBAColor*)data->GetPixels();
		int wh = -1;
		
		for (int w = middlew - BrushSize; w <= middlew + BrushSize && w < (int)HTD.size(); w++)
		{
			wh++;
			if (w < 0)
				continue;
			int hh = -1;
			for (int h = middleh - BrushSize; h <= middleh + BrushSize && h < (int)HTD[w].size(); h++)
			{
				hh++;
				if (h < 0)
					continue;
				if (!((w - middlew) * (w - middlew) + (h - middleh) * (h - middleh) <= BrushSize * BrushSize))
					continue;
				float XAnteil = static_cast<float>(wh) / (BrushSize * 2 + 1);
				float YAnteil = static_cast<float>(hh) / (BrushSize * 2 + 1);
				int XA = XAnteil * data->GetWidth();
				int YA = YAnteil * data->GetHeight();
				float z = ((static_cast<float>(PixelColorA[XA + YA * data->GetWidth()].r) / 0xFF ) - .5f) * ChangeSize;
				for (auto point : HTD[w][h].Vec)
					if (point)
						point->z += z;
				for (auto shape : HTD[w][h].Shape)
					if (shape)
						shape->MarkAsChanged(NiGeometryData::VERTEX_MASK);
			}
		}
	}
protected:
	struct RGBAColor
	{
		RGBAColor(float c) {
			r = c * 0xFF;
			g = r;
			b = r;
			a = 0xFF;
		}
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

	};
	virtual void UpdatePixelData() = 0;
	float ChangeSize = 1.f;
	bool _Show = false;
	int MapSize = 128;
	NiNodePtr Node;
	NiPixelDataPtr data;
	NiSourceTexturePtr pkTexture;
	NiScreenTexturePtr pkScreenTexture;
};