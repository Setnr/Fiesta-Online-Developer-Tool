#include "Brush.h"
#include "PerlinNoise.hpp"

class PerlinTextureBrush : public HTDTextureBrush
{
public:
	PerlinTextureBrush(std::shared_ptr<TerrainLayer> Layer, TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) :
		HTDTextureBrush(Layer, kWorld, HTDOrbNode, BrushSize) {}

	virtual void Update()
	{
		if (!Layer)
			return;
		IniFile& _InitFile = kWorld->GetIniFile();
		auto data = this->Layer->BlendTexture->GetSourcePixelData();
		TerrainLayer::RGBAColor* pixel = (TerrainLayer::RGBAColor*)data->GetPixels();
		int xPixel = _Intersect.x / _InitFile.OneBlock_width;
		int yPixel = _Intersect.y / _InitFile.OneBlock_height;
		int wh = -1;
		static int LastxPixel = 0;
		static int LastYPixel = 0;
		if (!(xPixel != LastxPixel && yPixel != LastYPixel))
			return;
		
		LastxPixel = xPixel;
		LastYPixel = yPixel;
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
				float PValue = perlin.octave2D_01(static_cast<float>(w) / data->GetWidth(), static_cast<float>(h) / data->GetHeight(), Octaves,persistance);
				PValue *= 255.f;
				pixel[PointOffsetNormal] = TerrainLayer::RGBAColor(static_cast<char>(PValue));
				

			}
		}
		data->MarkAsChanged();
		kWorld->CreateTerrainLayer(Layer);
	}
private:
	virtual void DrawInternal()
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		HTDTextureBrush::DrawInternal();
		bool UpdateTexture = false;
		if (ImGui::DragInt("Seed", (int*)&seed))
			UpdateTexture = true;
		ImGui::SameLine();
		if (ImGui::Button("Random"))
		{
			seed = std::rand();
			UpdateTexture = true;
		}
		ImGui::InputInt("Ocatves", &Octaves);
		if (UpdateTexture)
		{
			perlin = siv::PerlinNoise(seed);
		}
		ImGui::DragFloat("Persistance", &persistance, 0.01f, 0.0f, 1.0f);
		
		ImGui::EndChild();
	}
	int Octaves = 16;
	siv::PerlinNoise::seed_type seed = 123456u;
	siv::PerlinNoise perlin{ seed };
	float persistance = 0.99f;
};