#pragma once
#include <NiMain.h>
#include <iostream>
#include <vector>
#include <PgUtil.h>
#include <SHN/SHNStruct.h>
#include <fstream>
class TerrainLayerData
{
public:
	struct RGBAColor {
		RGBAColor(unsigned char val) { r = val; g = val; b = val; a = val; }
		RGBAColor(float col)
		{
			r = col * 0xFF;
			g = r;
			b = r;
			a = r;
		}
		RGBAColor(NiColorA col)
		{
			r = col.r * 255.f;
			g = col.g * 255.f;
			b = col.b * 255.f;
			a = col.a * 255.f;
		}
		unsigned char r, g, b, a;
	};
	struct RGBColor {
		RGBColor(float col) 
		{
			r = col * 0xFF;
			g = r;
			b = r;
		}
		RGBColor(NiColorA col) 
		{
			r = col.r * 255.f;
			g = col.g * 255.f;
			b = col.b * 255.f;
		}
		unsigned char r, g, b;
	};
	TerrainLayerData() = default;
	void SaveBlendFile(MapInfo* Info);
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

	void CreateTexture();
	void LoadDiffuseFile();

	bool Load(std::ifstream& File);

	void SetColor(int w, int h, float Color);
	float GetColor(int w, int h);
	void MarkAsChanged() 
	{
		NiPixelDataPtr data = BlendTexture->GetSourcePixelData();
		data->MarkAsChanged();
	}
};

NiSmartPointer(ShineIni);
class ShineIni : public NiRefObject
{
public:
	ShineIni() = default;

	bool Load(std::string Path);
	bool Save(std::string Path);
	std::string GetHTDPath() { return HeightFileName; }
	std::string GetVertexColor() { return VertexColorTexture; }
	int GetMapWidth() { return HeightMap_width; }
	int GetMapHeight() { return HeightMap_height; }
	int GetQuadsWide() { return QuadsWide; }
	int GetQuadsHigh() { return QuadsHigh; }
	void SetMapSize(int MapSize) 
	{
		HeightMap_width = MapSize + 1;
		HeightMap_height = MapSize + 1;
		for(auto layer : LayerList)
		{
			layer->Width = HeightMap_width;
			layer->Height = HeightMap_height;
		}
	}
	float GetOneBlockWidht() { return OneBlock_width; }
	float GetOneBlockHeight() { return OneBlock_height; }
	std::vector<std::shared_ptr<TerrainLayerData>>& GetLayers() { return LayerList; }
	void CreateEmpty(MapInfo* Info, int MapSize);
	NiColorA GetColor(int w, int h);
	void SetColor(int w, int h, NiColorA Color);
	void AddLayer(std::shared_ptr<TerrainLayerData> Layer);
	void DeleteLayer(std::shared_ptr<TerrainLayerData> Layer);
	std::shared_ptr<TerrainLayerData> CreateNewLayer(MapInfo* Info, int size);
	NiPixelDataPtr GetVertexImage() { return VertexShadowImage; }
	void SetVertexImage(NiPixelDataPtr Image) { VertexShadowImage = Image; }
private:
	std::string FileType;
	std::string HeightFileName;
	std::string VertexColorTexture;
	NiPixelDataPtr VertexShadowImage;
	std::string Version;

	float OneBlock_width = 50.f;
	float OneBlock_height = 50.f;

	int QuadsWide;
	int QuadsHigh;
	int HeightMap_width;
	int HeightMap_height;

	std::vector<std::shared_ptr<TerrainLayerData>> LayerList;
	bool LoadPGFile(std::ifstream& File)
	{
		std::string line;

		File >> line;
		if (line == ":")
			File >> line;
		FileType = line;
		if (line != "HeightMap")
		{
			std::string msg = "Is not a HeightMap";
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
			std::string msg = "Wrong Spaceing";
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
			std::string msg = "Wrong Spaceing";
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
			std::string msg = "Wrong Spaceing";
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
			std::string msg = "Wrong Spaceing";
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
			std::string msg = "Wrong Spaceing";
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
			std::string msg = "Wrong Spaceing";
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
			std::string msg = "Wrong Spaceing";
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
			std::string msg = "Wrong Spaceing";
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
			std::string msg = "Wrong Spaceing";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		return true;
	}
	bool LoadLayer(std::ifstream& File)
	{
		std::string line;
		auto Layer = std::make_shared<TerrainLayerData>();
		if (!Layer->Load(File))
		{
			std::string msg = "Failed to Load a Layer";
			NiMessageBox::DisplayMessage(msg.c_str(), "Error");
			return false;
		}
		LayerList.push_back(Layer);
		return true;
	}
};

