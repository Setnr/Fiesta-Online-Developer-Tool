#include "TextureBrush.h"

void TextureBrush::CreateTexture()
{
	if (_Show)
		Show(false);
	ImGuiIO& io = ImGui::GetIO();
	data = NiNew NiPixelData(MapSize, MapSize, NiPixelFormat::RGBA32);
	this->UpdatePixelData();
	NiTexture::FormatPrefs BlendPref;
	BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
	BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
	BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;
	pkTexture = NiSourceTexture::Create(data, BlendPref);
	pkTexture->SetStatic(false);
	pkScreenTexture = NiNew NiScreenTexture(pkTexture);
	pkScreenTexture->AddNewScreenRect(-1 * (int)pkTexture->GetHeight() - 295, -1 * (int)pkTexture->GetWidth(), pkTexture->GetWidth(), pkTexture->GetHeight(), 0, 0);

	Show(_Show);
}