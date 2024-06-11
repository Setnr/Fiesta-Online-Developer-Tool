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
	}
	int GetMapSize() { return MapSize; }
	int GetSHBDSize() { return SHBDSize; }
	std::vector<char>& GetDataRefrence() { return Data; }
	void UpdateSHBDData(int offset, int Shift, bool Status) 
	{
		if (offset >= Data.size())
			return;
		if (Status)
		{
			Data[offset] |= (1 << Shift);
		}
		else
		{
			Data[offset] &= ~(1 << Shift);
		}
	}
private:
	MapInfo* _Info;
	unsigned int MapSize;
	unsigned int SHBDSize;
	std::vector<char> Data;

};