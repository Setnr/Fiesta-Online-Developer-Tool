#pragma once
#include <string>

#include <NiDynamicTexture.h>
#include <vector>
#include <fstream>
#include <istream>
#include <filesystem>

#include "Logger.h"
#include "SHNStruct.h"
#include "PgUtil.h"
#include <map>
class ShineBlockData
{
public:
	ShineBlockData(MapInfo* MapInfo) : _Info(MapInfo)
	{
		std::string SHBDFilePath = PgUtil::CreateMapFolderPath(_Info->KingdomMap, _Info->MapFolderName, "shbd");
		std::ifstream SHBDFile;
		SHBDFile.open(SHBDFilePath, std::ios::binary);
		if (!SHBDFile.is_open())
		{
			LogError("Failed to open SHBD for " + _Info->MapFolderName);
			return;
		}
		SHBDFile.read((char*)&MapSize, sizeof(MapSize));
		SHBDFile.read((char*)&SHBDSize, sizeof(SHBDSize));
		for (int i = 0; i < SHBDSize * MapSize; i++)
		{
			char data;
			SHBDFile.read(&data, sizeof(data));
			Data.push_back(data);
		}
		SHBDFile.close();
	}

	void AppendToTexture(NiDynamicTexturePtr texture, int x, int y, int width, int height);
	void Save() 
	{
		auto start = std::chrono::steady_clock::now();
		std::string SHBDFilePath = PgUtil::CreateMapFolderPath(_Info->KingdomMap, _Info->MapFolderName, "shbd");
		std::ofstream SHBDFile;
		if (std::filesystem::exists(PgUtil::CreateMapFolderPath(_Info->KingdomMap, _Info->MapFolderName, "shbd.bak")))
			std::filesystem::remove(PgUtil::CreateMapFolderPath(_Info->KingdomMap, _Info->MapFolderName, "shbd.bak"));
		std::filesystem::copy(PgUtil::CreateMapFolderPath(_Info->KingdomMap, _Info->MapFolderName, "shbd"), PgUtil::CreateMapFolderPath(_Info->KingdomMap, _Info->MapFolderName, "shbd.bak"));
		SHBDFile.open(SHBDFilePath, std::ios::binary);
		if (!SHBDFile.is_open())
		{
			LogError("Failed to open and save SHBD for " + _Info->MapFolderName);
			return;
		}
		SHBDFile.write((char*)&MapSize, sizeof(MapSize));
		SHBDFile.write((char*)&SHBDSize, sizeof(SHBDSize));
		for (int i = 0; i < Data.size(); i++)
			SHBDFile.write(&Data[i], sizeof(char));
		SHBDFile.close();
		auto diff = std::chrono::steady_clock::now() - start;
		std::ostringstream oss;
		oss << "Successfully safed SHBD for " << _Info->MapName << "("
			<< std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms)";
		LogInfo(oss.str());
	}
	unsigned int GetMapSize() { return MapSize; }
	unsigned int GetSHBDSize() { return SHBDSize; }
	std::vector<char>& GetDataRefrence() { return Data; }
private:
	MapInfo* _Info;
	unsigned int MapSize;
	unsigned int SHBDSize;
	std::vector<char> Data;
};