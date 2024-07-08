#include "IniFile.h"

#include <NiMessageBox.h>
#include <filesystem>
TerrainLayer::~TerrainLayer()
{
	pixldata = 0;
	BlendTexture = 0;
	BaseTexture = 0;
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
			std::getline(File, line);
			std::string Started = line.substr(line.find("\"") + 1);
			DiffuseFileName = Started.substr(0, Started.find("\""));
			//DiffuseFileName = line.substr(line.find("\"") + 1, line.length() - line.find("\"") - 1);
		}
		if (line == "#BlendFileName")
		{
			std::getline(File, line);

			std::string Started = line.substr(line.find("\"") + 1);
			BlendFileName = Started.substr(0, Started.find("\""));
			//BlendFileName = line.substr(line.find("\"") + 1, line.length() - line.find("\"") - 1);
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
	NiImageConverterPtr conv = NiImageConverter::GetImageConverter();

	std::string acFileName = PgUtil::CreateFullFilePathFromBaseFolder(BlendFileName);
	std::cout << "BlendFileName for Texture " << acFileName << std::endl;

	NiPixelDataPtr ReadImage = conv->ReadImageFile(acFileName.c_str(), 0);
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

			PixelColorA[PointOffsetNormal] = RGBAColor(PixelColor[PointOffsetFlipped].g);
		}
	}

	NiTexture::FormatPrefs BlendPref;
	BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
	BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
	BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

	BlendTexture = NiSourceTexture::Create(pixldata, BlendPref);

	LoadDiffuseFile();
}

IniFile::IniFile(std::string FilePath)
{
	_FilePath = FilePath;
	OneBlock_height = 50.f;
	OneBlock_width = 50.f;
}
bool IniFile::Load()
{
	std::ifstream File;
	if (!std::filesystem::exists(_FilePath))
		return true;
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
		if (line.find("#PGFILE") != std::string::npos && !LoadPGFile(File))
		return false;
		if (line.find("#FILE_VER") != std::string::npos && !LoadFileVer(File))
			return false;

		if (line.find("#HeightFileName") != std::string::npos && !LoadHeightFileName(File))
			return false;
		if (line.find("#VerTexColorTexture") != std::string::npos && !LoadVerTextTexture(File))
			return false;

		if (line.find("#HEIGHTMAP_WIDTH") != std::string::npos && !LoadHeightMapWidth(File))
			return false;
		if (line.find("#HEIGHTMAP_HEIGHT") != std::string::npos && !LoadHeightMapHeight(File))
			return false;

		if (line.find("#OneBlockWidth") != std::string::npos && !LoadOneBlockWidht(File))
			return false;
		if (line.find("#OneBlockHeight") != std::string::npos && !LoadOneBlockHeight(File))
			return false;

		if (line.find("#QuadsWide") != std::string::npos && !LoadQuadsWide(File))
			return false;
		if (line.find("#QuadsHigh") != std::string::npos && !LoadQuadsHigh(File))
			return false;


		if (line.find("#Layer") != std::string::npos && !LoadLayer(File))
			return false;
	}
	return true;
}