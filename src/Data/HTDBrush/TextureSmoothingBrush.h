#include "Brush.h"

class TextureSmoothingBrush : public HTDTextureBrush
{
public:
	TextureSmoothingBrush(std::shared_ptr<TerrainLayer> Layer, TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) :
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
		float KernelValue = 1 / static_cast<float>(KernelSize * KernelSize);
		std::vector<std::vector<float>> KernelMap((BrushSize + 1) * 2);
		for (int w = 0; w < (BrushSize + 1) * 2; w++)
		{
			KernelMap[w] = std::vector<float>((BrushSize + 1) * 2);
		}
		int WHelper = 0;
		int HHelper = 0;
		for (int w = xPixel - BrushSize; w <= xPixel + BrushSize && w < static_cast<int>(data->GetWidth()); w++)
		{
			if (w - KernelSize / 2 < 0)
				continue;
			for (int h = yPixel - BrushSize; h <= yPixel + BrushSize && h < static_cast<int>(data->GetHeight()); h++)
			{
				if (h - KernelSize / 2 < 0)
					continue;
				if (!((w - xPixel) * (w - xPixel) + (h - yPixel) * (h - yPixel) <= BrushSize * BrushSize))
					continue;
				float Sum = 0.f;
				int StartI = -1 * (KernelSize / 2);
				int EndI = KernelSize / 2;
				int StartJ = -1 * (KernelSize / 2);
				int EndJ = KernelSize / 2;
				for (int i = StartI; i <= EndI; i++)
				{
					for (int j = StartJ; j <= EndJ; j++)
					{
						int XPart = w + i;

						int PreFullLines = Layer->BlendTexture->GetWidth() * (h + j);
						int YPartNormal = PreFullLines;
						int PointOffsetNormal = XPart + YPartNormal;
						Sum += static_cast<float>(pixel[PointOffsetNormal].r) / 255.f * KernelValue;
					}
				}
				KernelMap[WHelper][HHelper] = Sum;
				HHelper++;
			}
			HHelper = 0;
			WHelper++;
		}
		for (int cycle = 0; cycle < Cycles; cycle++)
		{
			WHelper = 0;
			HHelper = 0;
			for (int w = xPixel - BrushSize; w <= xPixel + BrushSize && w + KernelSize / 2 < static_cast<int>(data->GetWidth()); w++)
			{
				if (w - KernelSize / 2 < 0)
					continue;
				for (int h = yPixel - BrushSize; h <= yPixel + BrushSize && h + KernelSize / 2 < static_cast<int>(data->GetHeight()); h++)
				{
					if (h - KernelSize / 2 < 0)
						continue;
					if (!((w - xPixel) * (w - xPixel) + (h - yPixel) * (h - yPixel) <= BrushSize * BrushSize))
						continue;
					int XPart = w;

					int PreFullLines = Layer->BlendTexture->GetWidth() * h;
					int YPartNormal = PreFullLines;
					int PointOffsetNormal = XPart + YPartNormal;
					pixel[PointOffsetNormal] = TerrainLayer::RGBAColor(static_cast<char>(KernelMap[WHelper][HHelper] * 255.f));
					HHelper++;
				}
				HHelper = 0;
				WHelper++;
			}
		}
		data->MarkAsChanged();
		//kWorld->CreateTerrainLayer(Layer);
	}
private:
	virtual void DrawInternal()
	{
		ImGui::BeginChild("BrushChildR", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		HTDTextureBrush::DrawInternal();
		ImGui::DragInt("Kernel Size", &KernelSize, 2.f, 3, 21);
		ImGui::DragInt("Cycles", &Cycles, 1.f, 1, 20);
		ImGui::EndChild();
	}
	int KernelSize = 3;
	int Cycles = 5;
};