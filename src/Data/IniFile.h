#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <NiSourceTexture.h>
#include <filesystem>
#include "PgUtil.h"

class TerrainLayer 
{
public:
	struct RGBColor {
	public:
		RGBColor(unsigned char _r, unsigned char _g, unsigned char _b) {
			r = _r;
			g = _g;
			b = _b;
		}
		RGBColor(unsigned char c) {
			r = c;
			g = c;
			b = c;
		}
		bool operator==(const RGBColor& c)
		{
			return c.b == b && c.g == g && c.r == r;
		}
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
	struct RGBAColor {
	public:
		RGBAColor(){}
		RGBAColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) {
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}
		RGBAColor(NiColorA& Col) 
		{
			r = static_cast<unsigned char>(Col.r * 255.f);
			g = static_cast<unsigned char>(Col.g * 255.f);
			b = static_cast<unsigned char>(Col.b * 255.f);
			a = static_cast<unsigned char>(Col.a * 255.f);
		}
		RGBAColor(RGBColor c) {
			r = c.r;
			g = c.g;
			b = c.b;
			a = 0xFF;
		}
		RGBAColor(unsigned char c) {
			r = c;
			g = c;
			b = c;
			a = c;
		}
		bool operator==(const RGBAColor& c)
		{
			return c.a == a && c.b == b && c.g == g && c.r == r;
		}
		bool operator!=(const RGBAColor& c)
		{
			return c.a != a || c.b != b || c.g != g || c.r != r;
		}
		bool operator>(const RGBColor& c)
		{
			return r > c.r && g > c.g && b > c.b;
		}
		bool operator==(const RGBColor& c)
		{
			return c.b == b && c.g == g && c.r == r;
		}
		bool operator!=(const RGBColor& c)
		{
			return c.b != b || c.g != g || c.r != r;
		}
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};
	TerrainLayer() = default;
	~TerrainLayer();
	bool Load(std::ifstream& File);

	void CreateTexture();
	std::string Name;
	std::string DiffuseFileName;
	std::string BlendFileName;
	float StartPos_X;
	float StartPos_Y;
	float Width;
	float Height;
	float UVScaleDiffuse;
	float UVScaleBlend;

	NiPixelDataPtr pixldata;
	NiSourceTexturePtr BlendTexture;
	NiSourceTexturePtr BaseTexture;
	bool LoadDiffuseFile()
	{
		NiTexture::FormatPrefs BasePref;
		BasePref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::PIX_DEFAULT;
		BasePref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::MIP_DEFAULT;
		BasePref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

		std::string acFileName = PgUtil::CreateFullFilePathFromBaseFolder(DiffuseFileName);
		BaseTexture = NiSourceTexture::Create(acFileName.c_str(), BasePref);
		if (BaseTexture == NULL)
		{
			NiMessageBox::DisplayMessage("BaseTexture is Nullptr", "");
			return false;
		}
		return true;
	}
};
class IniFile 
{
public:
	IniFile(std::string FilePath);
	~IniFile() {
		LayerList.clear();
	}
	bool Load();
	void UpdateFilePath(std::string FilePath) { _FilePath = FilePath; }
	void Save(bool Backup = false)
	{
		auto start = std::chrono::steady_clock::now();
		std::ofstream IniFile;
		if(Backup)
		{
			std::string OrgPath = _FilePath;
			_FilePath += ".auto.bak";
			if (std::filesystem::exists(_FilePath))
				std::filesystem::remove(_FilePath);

			IniFile.open(_FilePath);
			_FilePath = OrgPath;
		}else
			IniFile.open(_FilePath);
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
			
			PgUtil::SaveTexture(PgUtil::CreateFullFilePathFromBaseFolder(layer->BlendFileName), layer->BlendTexture);
		}
		IniFile << "#END_FILE" <<std::endl<< "//Made With Fiesta Developer Tools by Set" << std::endl;
		
		IniFile.close();
		LogTime("Saved Inifile ", start);
	}

	std::string FileType;
	std::string HeightFileName;
	std::string VertexColorTexture;
	std::string _FilePath;
	std::string Version;

	float OneBlock_width;
	float OneBlock_height;

	int QuadsWide;
	int QuadsHigh;
	int HeightMap_width;
	int HeightMap_height;

	std::vector<std::shared_ptr<TerrainLayer>> LayerList;

private:
	bool LoadPGFile(std::ifstream& File) 
	{
		std::string line;

		File >> line;
		if (line == ":")
			File >> line;
		FileType = line;
		if (line != "HeightMap")
		{
			std::string msg = _FilePath + "\nIs not a HeightMap";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadFileVer(std::ifstream& File)
	{
		std::string line;
		File >> line;
		if (line == ":")
			File >> Version;
		else
		{
			std::string msg = _FilePath + "\nWrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadHeightFileName(std::ifstream& File)
	{
		std::string line;
		File >> line;
		if (line == ":")
		{
			File >> HeightFileName;
			HeightFileName = HeightFileName.substr(1, HeightFileName.length() - 2);
		}
		else
		{
			std::string msg = _FilePath + "\nWrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadVerTextTexture(std::ifstream& File)
	{
		std::string line;
		File >> line;
		if (line == ":")
		{
			File >> VertexColorTexture;
			VertexColorTexture = VertexColorTexture.substr(1, VertexColorTexture.length() - 2);
		}
		else
		{
			std::string msg = _FilePath + "\nWrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadHeightMapWidth(std::ifstream& File)
	{
		std::string line;
		File >> line;
		if (line == ":")
			File >> HeightMap_width;
		else
		{
			std::string msg = _FilePath + "\nWrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadHeightMapHeight(std::ifstream& File)
	{
		std::string line;
		File >> line;
		if (line == ":")
			File >> HeightMap_height;
		else
		{
			std::string msg = _FilePath + "\nWrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadOneBlockWidht(std::ifstream& File)
	{
		std::string line;
		File >> line;
		if (line == ":")
			File >> OneBlock_width;
		else
		{
			std::string msg = _FilePath + "\nWrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadOneBlockHeight(std::ifstream& File)
	{
		std::string line;
		File >> line;
		if (line == ":")
			File >> OneBlock_height;
		else
		{
			std::string msg = _FilePath + "\nWrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadQuadsWide(std::ifstream& File)
	{
		std::string line;
		File >> line;
		if (line == ":")
			File >> QuadsWide;
		else
		{
			std::string msg = _FilePath + "\nWrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadQuadsHigh(std::ifstream& File)
	{
		std::string line;
		File >> line;
		if (line == ":")
			File >> QuadsHigh;
		else
		{
			std::string msg = _FilePath + "\nWrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadLayer(std::ifstream& File)
	{
		std::string line;
		auto Layer = std::make_shared<TerrainLayer>();
		if (!Layer->Load(File))
		{
			std::string msg = _FilePath + "Failed to Load a Layer";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		LayerList.push_back(Layer);
		return true;
	}
	
};