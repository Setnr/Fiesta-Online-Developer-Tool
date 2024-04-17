#include "IniFile.h"
#include "PgUtil.h"

#include <NiMessageBox.h>
TerrainLayer::~TerrainLayer()
{
	if (pixldata)
		NiDelete pixldata;
}

bool TerrainLayer::Load(std::ifstream& File)
{
	std::string line;
	while (File >> line) 
	{
		if (line == "{")
			continue;
		if (line == "#Name")
		{
			std::getline(File, Name);
			Name = Name.substr(Name.find(":") + 1, Name.length() - Name.find(":") - 1);
		}
		if (line == "#DiffuseFileName")
		{
			File >> DiffuseFileName;
			DiffuseFileName = DiffuseFileName.substr(2, DiffuseFileName.length() - 3);
		}
		if (line == "#BlendFileName")
		{
			File >> BlendFileName;
			BlendFileName = BlendFileName.substr(2, BlendFileName.length() - 3);
		}	
		if (line == "#StartPos_X")
		{
			File >> line;
			StartPos_X = atof(line.substr(1).c_str());
		}
		if (line == "#StartPos_Y")
		{
			File >> line;
			StartPos_Y = atof(line.substr(1).c_str());
		}
		if (line == "#Width")
		{
			File >> line;
			Width = atof(line.substr(1).c_str());
		}
		if (line == "#Height")
		{
			File >> line;
			Height = atof(line.substr(1).c_str());
		}
		if (line == "#UVScaleDiffuse")
		{
			File >> line;
			UVScaleDiffuse = atof(line.substr(1).c_str());
		}
		if (line == "#UVScaleBlend")
		{
			File >> line;
			UVScaleBlend = atof(line.substr(1).c_str());
		}
		if (line == "}")
		{
			this->CreateTexture();
			return true;
		}
	}
	std::string msg = "This area from TerrainLayer::Load shoud never be reached";
	NiMessageBox::DisplayMessage(msg.c_str(), "Error");
	return false;
}

void TerrainLayer::CreateTexture()
{
	NiImageConverter* conv = NiImageConverter::GetImageConverter();

	std::string acFileName = PgUtil::CreateFullFilePathFromBaseFolder(BlendFileName);
	std::cout << "BlendFileName for Texture " << acFileName << std::endl;

	NiPixelData* ReadImage = conv->ReadImageFile(acFileName.c_str(), 0);
	if (ReadImage->GetPixelFormat() != NiPixelFormat::RGB24)
	{
		NiMessageBox::DisplayMessage("NiPixelData From ReadImage in Layer::CreateTexture Has Wrong NiPixelFormat", "Error");
	}

	pixldata = NiNew NiPixelData(ReadImage->GetWidth(), ReadImage->GetHeight(), NiPixelFormat::RGBA32);
	RGBAColor* PixelColorA = (RGBAColor*)pixldata->GetPixels();
	RGBColor* PixelColor = (RGBColor*)ReadImage->GetPixels();
	if (ReadImage->GetSizeInBytes() / sizeof(RGBColor) != pixldata->GetSizeInBytes() / sizeof(RGBAColor))
	{
		NiMessageBox::DisplayMessage("Size MissMatch in Recreating NiPixelData for TerrainTexture", "Error");
	}

	for (int w = 0; w < ReadImage->GetWidth(); w++)
	{
		for (int h = 0; h < ReadImage->GetHeight(); h++)
		{
			int XPart = w;

			int PreFullLines = ReadImage->GetWidth() * h;
			int YPartNormal = PreFullLines;
			int PointOffsetNormal = XPart + YPartNormal;

			int YPartFlipped = ReadImage->GetWidth() * (ReadImage->GetHeight() - h - 1);
			int PointOffsetFlipped = XPart + YPartFlipped;

			PixelColorA[PointOffsetNormal] = RGBAColor(PixelColor[PointOffsetNormal].g);
		}
	}

	NiTexture::FormatPrefs BasePref;
	BasePref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::PIX_DEFAULT;
	BasePref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::MIP_DEFAULT;
	BasePref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

	NiTexture::FormatPrefs BlendPref;
	BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
	BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
	BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

	BlendTexture = NiSourceTexture::Create(pixldata, BlendPref);

	acFileName = PgUtil::CreateFullFilePathFromBaseFolder(DiffuseFileName);
	BaseTexture = NiSourceTexture::Create(acFileName.c_str(), BasePref);
	if (BaseTexture == NULL)
		NiMessageBox::DisplayMessage("BaseTexture is Nullptr", "");


}

IniFile::IniFile(std::string FilePath)
{
	_FilePath = FilePath;
}
bool IniFile::Load()
{
	std::ifstream File;
	File.open(_FilePath);

	if (!File.is_open()) 
	{
		std::string msg = "Failed to open IniFile:\n" + _FilePath;
		NiMessageBox::DisplayMessage(msg.c_str(), "Error");
		return false;
	}
	std::string line;
	std::string info;
	while (File >> line) 
	{
		if (line == "#PGFILE" && !LoadPGFile(File))
			return false;
		if(line == "#FILE_VER" && !LoadFileVer(File))
			return false;

		if (line == "#HeightFileName" && !LoadHeightFileName(File))
			return false;
		if (line == "#VerTexColorTexture" && !LoadVerTextTexture(File))
			return false;

		if (line == "#HEIGHTMAP_WIDTH" && !LoadHeightMapWidth(File))
			return false;
		if (line == "#HEIGHTMAP_HEIGHT" && !LoadHeightMapHeight(File))
			return false;

		if (line == "#OneBlockWidth" && !LoadOneBlockWidht(File))
			return false;
		if (line == "#OneBlockHeight" && !LoadOneBlockHeight(File))
			return false;

		if (line == "#QuadsWide" && !LoadQuadsWide(File))
			return false;
		if (line == "#QuadsHigh" && !LoadQuadsHigh(File))
			return false;


		if (line == "#Layer" && !LoadLayer(File))
			return false;
	}
	return true;
}