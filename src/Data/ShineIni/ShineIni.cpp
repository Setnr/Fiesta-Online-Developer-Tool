#include "ShineIni.h"
#include <Logger.h>
#include <filesystem>
#include <fstream>

void TerrainLayerData::CreateTexture()
{
	NiImageConverterPtr conv = NiImageConverter::GetImageConverter();

	std::string acFileName = PgUtil::PathFromClientFolder(BlendFileName);
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
	BlendTexture->SetStatic(false);
	LoadDiffuseFile();
}
void TerrainLayerData::LoadDiffuseFile()
{
	NiTexture::FormatPrefs BasePref;
	BasePref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::PIX_DEFAULT;
	BasePref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::MIP_DEFAULT;
	BasePref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

	std::string acFileName = PgUtil::PathFromClientFolder(DiffuseFileName);
	BaseTexture = NiSourceTexture::Create(acFileName.c_str(), BasePref);
	if (BaseTexture == NULL)
	{
		LogError("Failed to Create BaseTexture from\n" + DiffuseFileName);
		return;
	}
	return;
}
bool ShineIni::Load(std::string Path) 
{
	if (!std::filesystem::exists(Path))
	{
		LogError("No Ini to load for:\n " + Path);
		return false;
	}
	std::ifstream File;
	File.open(Path);
	if(!File.is_open())
	{
		LogError("Failed to open Ini for:\n " + Path);
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

	File.close();

	if (HeightFileName == "")
		return true;
	NiImageConverter* conv = NiImageConverter::GetImageConverter();
	std::string VertexShadowPath = PgUtil::PathFromClientFolder(GetVertexColor());
	VertexShadowImage = conv->ReadImageFile(VertexShadowPath.c_str(), 0);
	
	if (VertexShadowImage == NULL)
		LogError("Failed to Load ShadowImage:\n" + GetVertexColor());
	return true;
}
bool TerrainLayerData::Load(std::ifstream& File)
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
bool ShineIni::Save(std::string Path) 
{
	std::ofstream IniFile;

	if (std::filesystem::exists(Path)) 
	{
		if (std::filesystem::exists(Path + ".bak"))
			std::filesystem::remove(Path + ".bak");
		std::filesystem::copy(Path, Path + ".bak");
	}

	IniFile.open(Path);
	if (!IniFile.is_open()) 
	{
		LogError("Failed to save\n" + Path);
		return false;
	}
	IniFile << std::fixed << std::setprecision(1);
	IniFile << "#PGFILE\t:\t" << FileType << std::endl;
	IniFile << "#FILE_VER\t:\t" << Version << std::endl << std::endl;

	IniFile << "#HeightFileName\t:\t\"" << HeightFileName << "\"" << std::endl;
	IniFile << "#VerTexColorTexture\t:\t\"" << VertexColorTexture << "\"" << std::endl << std::endl;

	IniFile << "#HEIGHTMAP_WIDTH\t:\t" << HeightMap_width << std::endl;
	IniFile << "#HEIGHTMAP_HEIGHT\t:\t" << HeightMap_height << std::endl << std::endl;

	IniFile << "#OneBlockWidth\t:\t" << OneBlock_width << "f" << std::endl;
	IniFile << "#OneBlockHeight\t:\t" << OneBlock_height << "f" << std::endl << std::endl;

	IniFile << "#QuadsWide\t:\t" << QuadsWide << std::endl;
	IniFile << "#QuadsHigh\t:\t" << QuadsHigh << std::endl << std::endl;

	for (auto layer : LayerList)
	{
		IniFile << "#Layer" << std::endl;
		IniFile << "{" << std::endl;
		IniFile << "	#Name\t:" << layer->Name << std::endl;
		IniFile << "	#DiffuseFileName\t:\"" << layer->DiffuseFileName << "\"" << std::endl;
		IniFile << "	#BlendFileName\t:\"" << layer->BlendFileName << "\"" << std::endl;
		IniFile << "	#StartPos_X\t:" << layer->StartPos_X << "f" << std::endl;
		IniFile << "	#StartPos_Y\t:" << layer->StartPos_Y << "f" << std::endl;
		IniFile << "	#Width\t:" << layer->Width << "f" << std::endl;
		IniFile << "	#Height\t:" << layer->Height << "f" << std::endl;
		IniFile << "	#UVScaleDiffuse\t:" << layer->UVScaleDiffuse << "f" << std::endl;
		IniFile << "	#UVScaleBlend\t:" << layer->UVScaleBlend << "f" << std::endl;
		IniFile << "}" << std::endl;

		PgUtil::SaveTexture(PgUtil::PathFromClientFolder(layer->BlendFileName), layer->BlendTexture,true);
	}
	IniFile << "#END_FILE" << std::endl << "//Made With Fiesta Developer Tools by Set" << std::endl;

	IniFile.close();
	return true;
}
void ShineIni::CreateEmpty(MapInfo* Info, int MapSize) 
{
	FileType = "HeightMap";
	Version = "0.01";

	HeightFileName = PgUtil::GetMapFolderPath(Info->KingdomMap, Info->MapFolderName) + Info->MapName + ".htd";
	VertexColorTexture = PgUtil::GetMapFolderPath(Info->KingdomMap, Info->MapFolderName) + Info->MapName + ".bmp";
	HeightMap_width = MapSize + 1;
	HeightMap_height = MapSize + 1;
	OneBlock_width = 50.f;
	OneBlock_height = 50.f;
	QuadsWide = 64;
	QuadsHigh = 64;
	auto data = std::make_shared<TerrainLayerData>();
	CreateNewLayer(Info, MapSize);


	auto pixldata = LayerList[0]->BlendTexture->GetSourcePixelData();
	auto pixl = pixldata->GetPixels();
	for (size_t i = 0; i < pixldata->GetSizeInBytes(); i++)
	{
		pixl[i] = 0xFF;
	}
	pixldata->MarkAsChanged();
	//std::string VertexShadowPath = PgUtil::PathFromClientFolder(GetVertexColor());
	VertexShadowImage = NiNew NiPixelData(HeightMap_width, HeightMap_height, NiPixelFormat::RGBA32);
	auto pixels = VertexShadowImage->GetPixels();
	for (size_t i = 0; i < VertexShadowImage->GetSizeInBytes(); i++)
	{
		pixels[i] = 0xFF;
	}
}
void ShineIni::SetColor(int w, int h, NiColorA Color) 
{

	if (!VertexShadowImage)
	{
		LogError("No VertexColors loaded!");
		return;
	}
	NiPixelFormat format = VertexShadowImage->GetPixelFormat();
	if (format == NiPixelFormat::RGB24)
	{
		TerrainLayerData::RGBColor* VertexColorArray = NULL;
		VertexColorArray = (TerrainLayerData::RGBColor*)VertexShadowImage->GetPixels();

		VertexColorArray[w + (VertexShadowImage->GetHeight() - h - 1) * VertexShadowImage->GetWidth()] = TerrainLayerData::RGBColor(Color);
	}
	else
	{
		if (format == NiPixelFormat::RGBA32)
		{
			TerrainLayerData::RGBAColor* VertexColorArrayA = NULL;
			VertexColorArrayA = (TerrainLayerData::RGBAColor*)VertexShadowImage->GetPixels();
			VertexColorArrayA[w + (VertexShadowImage->GetHeight() - h - 1) * VertexShadowImage->GetWidth()] = TerrainLayerData::RGBAColor(Color);;
		}
		else
		{
			LogError("Not Supported NiPixelFormat for VertexColorTexture");
		}
	}
}
NiColorA ShineIni::GetColor(int w, int h) 
{
	if (!VertexShadowImage)
	{
		LogError("No VertexColors loaded!");
		return NiColorA::WHITE;
	}
	if (w < 0 || h < 0 || w >= VertexShadowImage->GetWidth() || h >= VertexShadowImage->GetHeight())
		return NiColorA::WHITE;
	NiPixelFormat format = VertexShadowImage->GetPixelFormat();
	if (format == NiPixelFormat::RGB24)
	{
		TerrainLayerData::RGBColor* VertexColorArray = NULL;
		VertexColorArray = (TerrainLayerData::RGBColor*)VertexShadowImage->GetPixels();

		TerrainLayerData::RGBColor col = VertexColorArray[w + (VertexShadowImage->GetHeight() - h - 1) * VertexShadowImage->GetWidth()];
		return NiColorA(col.r / 255.f, col.g / 255.f, col.b / 255.f, 1.0f);
	}
	else
	{
		if (format == NiPixelFormat::RGBA32)
		{
			TerrainLayerData::RGBAColor* VertexColorArrayA = NULL;
			VertexColorArrayA = (TerrainLayerData::RGBAColor*)VertexShadowImage->GetPixels();
			TerrainLayerData::RGBAColor col = VertexColorArrayA[w + (VertexShadowImage->GetHeight() - h - 1) * VertexShadowImage->GetWidth()];
			return NiColorA(col.r / 255.f, col.g / 255.f, col.b / 255.f,1.0f);
		}
		else
		{
			LogError("Not Supported NiPixelFormat for VertexColorTexture");
			return NiColorA::WHITE;
		}
	}
}
void TerrainLayerData::SetColor(int w, int h, float Color)
{
	if (w < 0 || h < 0 || w >= BlendTexture->GetWidth() || h >= BlendTexture->GetHeight())
		return;
	const NiPixelFormat* format = BlendTexture->GetPixelFormat();

	NiPixelDataPtr data = BlendTexture->GetSourcePixelData();
	int XPart = w ;

	int PreFullLines = BlendTexture->GetWidth() * h ;
	int YPartNormal = PreFullLines;
	int PointOffsetNormal = XPart + YPartNormal; 

	auto VertexColorArray = data->GetPixels();

	unsigned char* pixlptr = data->operator()(w, h);
	if (!pixlptr)
		return;
	for (int i = 0; i < format->GetBitsPerPixel() / 8; i++)
	{
		pixlptr[i] = static_cast<unsigned char>(Color * 255.f);
	}

}

float TerrainLayerData::GetColor(int w, int h)
{
	if (w < 0 || h < 0 || w >= BlendTexture->GetWidth() || h >= BlendTexture->GetHeight())
		return 0.f;
	NiPixelFormat format = *this->BlendTexture->GetPixelFormat();

	NiPixelDataPtr data = BlendTexture->GetSourcePixelData();
	int XPart = w;

	int PreFullLines = BlendTexture->GetWidth() * h;
	int YPartNormal = PreFullLines;
	int PointOffsetNormal = XPart + YPartNormal;

	float Color = 0.f;

	if (format.GetFormat() == NiPixelFormat::RGB24.GetFormat())
	{
		TerrainLayerData::RGBColor* VertexColorArray = NULL;
		VertexColorArray = (TerrainLayerData::RGBColor*)data->GetPixels();

		auto col = VertexColorArray[PointOffsetNormal];
		Color = static_cast<float>(col.r) / 255.f;
	}
	else
	{
		if (format.GetFormat() == NiPixelFormat::RGBA32.GetFormat())
		{
			TerrainLayerData::RGBAColor* VertexColorArrayA = NULL;
			VertexColorArrayA = (TerrainLayerData::RGBAColor*)data->GetPixels();
			auto col = VertexColorArrayA[PointOffsetNormal];
			Color = static_cast<float>(col.r) / 255.f;
		}
		else
		{
			LogError("Not Supported NiPixelFormat for BlendTexture");
			return 0.f;
		}
	}
	return Color;
}
void ShineIni::AddLayer(std::shared_ptr<TerrainLayerData> Layer) 
{
	LayerList.push_back(Layer);
}
void ShineIni::DeleteLayer(std::shared_ptr<TerrainLayerData> Layer) 
{
	auto l = std::find(LayerList.begin(), LayerList.end(), Layer);
	LayerList.erase(l);
}
std::shared_ptr<TerrainLayerData> ShineIni::CreateNewLayer(MapInfo* Info, int size) 
{
	std::shared_ptr<TerrainLayerData> layer = std::make_shared<TerrainLayerData>();
	layer->Width = size;
	layer->Height = size;
	layer->StartPos_X = 0.f;
	layer->StartPos_Y = 0.f;
	layer->UVScaleBlend = 1.f;
	layer->UVScaleDiffuse = 5.f;
	layer->Name = "New Layer";
	layer->DiffuseFileName = ".\\resmap\\fieldtexture\\\grass_01.dds";
	layer->BlendFileName = PgUtil::GetMapFolderPath(Info->KingdomMap, Info->MapName) + "Name.bmp";
	layer->LoadDiffuseFile();

	NiTexture::FormatPrefs BlendPref;
	BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
	BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
	BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

	NiPixelDataPtr pixldata = NiNew NiPixelData(layer->Width, layer->Height, NiPixelFormat::RGBA32);
	unsigned char* pixl = pixldata->GetPixels();
	for (size_t i = 0; i < pixldata->GetSizeInBytes(); i++)
		pixl[i] = 0x0;
	layer->BlendTexture = NiSourceTexture::Create(pixldata, BlendPref);
	layer->BlendTexture->SetStatic(false);
	AddLayer(layer);
	return layer;
}

void TerrainLayerData::SaveBlendFile(MapInfo* Info) 
{
	BlendFileName = PgUtil::GetMapFolderPath(Info->KingdomMap, Info->MapName) + Name +".bmp";
	PgUtil::SaveTexture(PgUtil::PathFromClientFolder(BlendFileName), BlendTexture,true);
}