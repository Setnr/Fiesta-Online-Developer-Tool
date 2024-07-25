#include "Brush.h"

class PixelBrush : public HTDTextureBrush 
{
public:
	PixelBrush(std::shared_ptr<TerrainLayer> Layer, TerrainWorldPtr kWorld, NiNodePtr HTDOrbNode, int BrushSize) :
		HTDTextureBrush(Layer,kWorld, HTDOrbNode, BrushSize){}

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
				pixel[PointOffsetNormal] = TerrainLayer::RGBAColor((char)BrushColor);

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
		float Col = static_cast<float>(BrushColor) / 255;
		ImGui::ColorButton("", ImVec4(Col, Col, Col, Col)); 
		ImGui::SameLine();
		ImGui::DragInt("Color", &BrushColor, 1.f, 0, 255);
		ImGui::EndChild();
		
	}
	int BrushColor = 0x0;
};