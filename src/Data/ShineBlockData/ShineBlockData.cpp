#include "ShineBlockData.h"
#include <Logger.h>
#include <fstream>
#include <PgUtil.h>
#include <filesystem>

bool ShineBlockData::Load(MapInfo* Info) 
{
	std::string SHBDFilePath = PgUtil::PathFromClientFolder(PgUtil::GetMapFolderPath(Info->KingdomMap, Info->MapFolderName) + Info->MapFolderName + ".shbd");
	std::ifstream SHBDFile;
	SHBDFile.open(SHBDFilePath, std::ios::binary);
	if (!SHBDFile.is_open() || SHBDFile.bad() || SHBDFile.fail())
	{
		LogError("Failed to open SHBD for " + Info->MapFolderName);
		return false;
	}
	SHBDFile.read((char*)&MapSize, sizeof(MapSize));
	SHBDFile.read((char*)&SHBDSize, sizeof(SHBDSize));
	Data.resize(SHBDSize * MapSize);
	SHBDFile.read((char*)&Data[0], SHBDSize * MapSize);
	SHBDFile.close();
	return true;
}
void ShineBlockData::UpdateSHBDData(int offset, int Shift, bool Status)
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
bool ShineBlockData::Save(std::string Path)
{
	if (std::filesystem::exists(Path))
	{
		if (std::filesystem::exists(Path + ".bak"))
			std::filesystem::remove(Path + ".bak");
		std::filesystem::copy(Path, Path + ".bak");
	}
	std::ofstream SHBDFile;
	SHBDFile.open(Path, std::ios::binary);
	if (!SHBDFile.is_open())
	{
		LogError("Failed to open and save SHBD for:\n" + Path);
		return false;
	}
	SHBDFile.write((char*)&MapSize, sizeof(MapSize));
	SHBDFile.write((char*)&SHBDSize, sizeof(SHBDSize));
	SHBDFile.write((char*)&Data[0], SHBDSize * MapSize);
	SHBDFile.close();
	return true;
}
void ShineBlockData::CreateEmpty(int Size) 
{
	MapSize = Size;
	SHBDSize = Size / 8;
	Data.resize(MapSize * SHBDSize, 0x0);
}
bool ShineBlockData::IsWalkable(int w, int h)
{
	int offset = w / 8 * SHBDSize;
	int Shift = w % 8;
	return (Data[SHBDSize * offset] >> Shift) & 0x1; // Not Sure if Shift is correct!
}