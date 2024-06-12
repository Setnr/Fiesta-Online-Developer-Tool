#pragma once
#include "HTDBrush.h"
#include "PerlinNoise.hpp"
#include "../../NiApplication/FiestaOnlineTool.h"
#include "ImGui/imgui.h"
NiSmartPointer(PerlinBrush);
class PerlinBrush : public HTDBrush
{
#define TextureWidth 200
#define TextureHeight 100
public:
	PerlinBrush(NiNodePtr HTDOrbNode, int BrushSize) : HTDBrush(HTDOrbNode, BrushSize)
	{
		ImGuiIO& io = ImGui::GetIO();
		data = NiNew NiPixelData(TextureWidth, TextureHeight, NiPixelFormat::RGBA32);
		UpdatePixelData();
		NiTexture::FormatPrefs BlendPref;
		BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
		BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
		BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;
		pkTexture = NiSourceTexture::Create(data, BlendPref);
		pkTexture->SetStatic(false);
		pkScreenTexture = NiNew NiScreenTexture(pkTexture);
		pkScreenTexture->AddNewScreenRect(-1 * (int)pkTexture->GetHeight() - 45, -1 * (int)pkTexture->GetWidth() - 70, pkTexture->GetWidth(), pkTexture->GetHeight(), 0, 0);
		FiestaOnlineTool::AddScreenTexture(pkScreenTexture);
	}
	~PerlinBrush() {
		FiestaOnlineTool::RemoveScreenTexture(pkScreenTexture);
	}
	virtual void DrawInternal() 
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SliderFloat("Level Change", &ChangeSize, -50.f, 50.f);
		if(ImGui::InputInt("Perlin Seed",(int*) &seed) || ImGui::InputInt("Perlin Ocatves", &Octaves))
		{
			perlin = siv::PerlinNoise(seed);
			UpdatePixelData();
		}
		
		ImGui::EndChild();

	}
	virtual const char* GetName() { return "Perlin Brush"; }

	virtual void UpdateHTD(IniFile& _InitFile, std::vector<std::vector<World::HTDHelper>>& HTD, NiPoint3 InterSect)
	{
			int middlew = InterSect.x / _InitFile.OneBlock_width;
		int middleh = InterSect.y / _InitFile.OneBlock_height;
		for (int w = middlew - BrushSize; w <= middlew + BrushSize && w < (int)HTD.size(); w++)
		{
			if (w < 0)
				continue;
			for (int h = middleh - BrushSize; h <= middleh + BrushSize && h < (int)HTD[w].size(); h++)
			{
				if (h < 0)
					continue;
				if (!((w - middlew) * (w - middlew) + (h - middleh) * (h - middleh) <= BrushSize * BrushSize))
					continue;
				float z = perlin.octave2D_01(static_cast<float>(w), static_cast<float>(h), Octaves) * ChangeSize;
				for (auto point : HTD[w][h].Vec)
					if (point)
						point->z += z;
				for (auto shape : HTD[w][h].Shape)
					if (shape)
						shape->MarkAsChanged(NiGeometryData::VERTEX_MASK);
			}
		}
	}
	void UpdatePixelData() 
	{
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
		RGBAColor* PixelColorA = (RGBAColor*)data->GetPixels();
		int i = 0;
		for(int w = 0; w < TextureWidth; w++)
		{
			for (int h = 0; h < TextureHeight; h++)
			{
				PixelColorA[i] = RGBAColor(perlin.octave2D_01(static_cast<float>(w), static_cast<float>(h), Octaves));
				i++;
			}
		}
		data->MarkAsChanged();
	}
private:
	float ChangeSize = 1.f;
	int Octaves = 16;
	siv::PerlinNoise::seed_type seed = 123456u;
	siv::PerlinNoise perlin{ seed };
	NiPixelDataPtr data;
	NiSourceTexturePtr pkTexture;
	NiScreenTexturePtr pkScreenTexture;
};