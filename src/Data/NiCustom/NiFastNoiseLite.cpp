#include "NiFastNoiseLite.h"
#include "Logger.h"
#include "PgUtil.h"
#include "FiestaOnlineTool.h"

void NiFastNoiseLite::CreateTexture(int Size)
{
	const NiPixelFormat* TexturePixelFormat = &NiPixelFormat::RGBA32;
	NiPixelDataPtr data = NiNew NiPixelData(Size, Size, *TexturePixelFormat);

	auto pixeloffset = (unsigned int*)data->GetPixels();

	for (int h = 0; h < Size; h++) {
		for (int w = 0; w < Size; w++)
		{ 
			unsigned int* NewPtr = (pixeloffset + (h * data->GetWidth()) + w);
			char Value = ((GetNoise((float)w, (float)h) + 1.f) / 2.f ) * 255.f;
			
			unsigned int Color = (Value << TexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & TexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
			Color |= (Value << TexturePixelFormat->GetShift(NiPixelFormat::COMP_GREEN)) & TexturePixelFormat->GetMask(NiPixelFormat::COMP_GREEN);
			Color |= (Value << TexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & TexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
			Color |= (Value << TexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & TexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);
			*pixeloffset = Color;
			pixeloffset++;
		}
	}
	_Texture = NiSourceTexture::Create(data);

	_ScreenElement = PgUtil::CreateScreenElement(Size, Size, _Texture);
	auto& io = ImGui::GetIO();
	float fLeft = 1.f - Size / io.DisplaySize.x;
	float fTop = 1.f - Size / io.DisplaySize.y;
	float fBottom = 1.f;
	float fRight = 1.f;
	_ScreenElement->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
	_ScreenElement->SetVertex(0, 1, NiPoint2(fRight, fBottom));
	_ScreenElement->SetVertex(0, 2, NiPoint2(fRight, fTop));
	_ScreenElement->SetVertex(0, 3, NiPoint2(fLeft, fTop));
	_ScreenElement->UpdateBound();
}
void NiFastNoiseLite::Draw()
{
	unsigned int Width = _Texture->GetWidth();
	unsigned int Height = _Texture->GetHeight();
	ImGui::SetNextWindowSize(ImVec2(Width,20.0));
	if (ImGui::Begin("Noise Preview")) 
	{
		auto WindowPos = ImGui::GetWindowPos();
		auto& io = ImGui::GetIO();
		
		unsigned int Width = _Texture->GetWidth();
		unsigned int Height = _Texture->GetHeight();

		float fLeft = WindowPos.x / io.DisplaySize.x;
		float fTop = (WindowPos.y + 20) / io.DisplaySize.y;

		float fBottom = fTop + (Height / io.DisplaySize.y);
		float fRight = fLeft + (Width / io.DisplaySize.x);

		_ScreenElement->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
		_ScreenElement->SetVertex(0, 1, NiPoint2(fRight, fBottom));
		_ScreenElement->SetVertex(0, 2, NiPoint2(fRight, fTop));
		_ScreenElement->SetVertex(0, 3, NiPoint2(fLeft, fTop));
		_ScreenElement->UpdateBound();

		auto renderer = FiestaOnlineTool::GetRenderer();
		if(renderer)
			_ScreenElement->Draw(renderer);
		ImGui::End();
	}
} 