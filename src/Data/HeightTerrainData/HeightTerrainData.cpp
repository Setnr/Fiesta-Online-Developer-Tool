#include "HeightTerrainData.h"

#include <PgUtil.h>
#include <filesystem>
#include <fstream>

bool HeightTerrainData::Load(std::string Path, int Width, int Height)
{
	_Width = Width;
	_Height = Height;
	if (!std::filesystem::exists(Path))
	{
		LogError("No HTD to load for:\n " + Path);
		return false;
	}
	std::ifstream HTDFile;
	HTDFile.open(Path, std::ios::binary);

	if (!HTDFile.is_open()) 
	{
		LogError("Failed to load HTD for:\n " + Path);
		return false;
	}
	HTDFile.read((char*)&PointCounter, sizeof(PointCounter));
	if (PointCounter != _Width * _Height) 
	{
		LogError("HTDSize missmatch for:\n" + Path);
		HTDFile.close();
		return false;
	}
	HTD.resize(PointCounter);
	HTDFile.read((char*)&HTD[0], sizeof(float) * PointCounter);
	HTDFile.close();

	Path += "g";

	if (!std::filesystem::exists(Path))
	{
		HTDG.resize(HTD.size(),0.0);
		return true;
	}
	std::ifstream HTDGFile;
	HTDGFile.open(Path, std::ios::binary);

	if (!HTDGFile.is_open())
	{
		return true;
	}
	HTDGFile.read((char*)&PointCounter, sizeof(PointCounter));
	if (PointCounter != _Width * _Height)
	{
		LogError("HTGDSize missmatch for:\n" + Path);
	}
	HTDG.resize(PointCounter);
	HTDGFile.read((char*)&HTDG[0], sizeof(float) * PointCounter);
	HTDGFile.close();
	return true;
}

bool HeightTerrainData::Save(std::string Path) 
{
	std::string HTDPath = Path;
	std::string HTDGPath = Path + "g";

	if (std::filesystem::exists(HTDPath))
	{
		if (std::filesystem::exists(HTDPath + ".bak"))
			std::filesystem::remove(HTDPath + ".bak");
		std::filesystem::copy(HTDPath, HTDPath + ".bak");
	}
	std::ofstream HTDFile;
	HTDFile.open(HTDPath, std::ios::binary);
	if (!HTDFile.is_open())
	{
		LogError("Cant open file to save:\n" + HTDPath);
		return false;
	}
	int Size = _Width * _Height;
	if (Size != HTD.size()) 
	{
		LogError("MapSize doesnt match to Vector for:\n" + HTDPath);
		HTDFile.close();
		return false;
	}
	HTDFile.write((char*) & Size, sizeof(Size));
	HTDFile.write((char*)&HTD[0], sizeof(float) * Size);
	HTDFile.close();



	if (!HTDG.size())
	{
		return true;
	}
	if (HTDG.size() != HTD.size()) 
	{
		LogError("HTD and HTDG have a size missmatch \n" + HTDPath);
		return false;
	}
	if (std::filesystem::exists(HTDGPath))
	{
		if (std::filesystem::exists(HTDGPath + ".bak"))
			std::filesystem::remove(HTDGPath + ".bak");
		std::filesystem::copy(HTDGPath, HTDGPath + ".bak");
	}
	std::ofstream HTDGFile;
	HTDGFile.open(HTDGPath, std::ios::binary);
	if (!HTDGFile.is_open())
	{
		LogError("Cant open file to save:\n" + HTDGPath);
		return false;
	}
	if (Size != HTDG.size())
	{
		LogError("MapSize doesnt match to Vector for:\n" + HTDGPath);
		HTDGFile.close();
		return false;
	}
	HTDGFile.write((char*)&Size, sizeof(Size));
	HTDGFile.write((char*)&HTDG[0], sizeof(float) * Size);
	HTDGFile.close();
	return true;
}