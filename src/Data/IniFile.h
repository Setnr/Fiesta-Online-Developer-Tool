#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <NiSourceTexture.h>

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
};
class IniFile 
{
public:
	IniFile(std::string FilePath);
	~IniFile() {
		LayerList.clear();
	}
	bool Load();


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