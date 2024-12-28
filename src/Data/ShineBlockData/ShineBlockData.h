#pragma once
#include <NiMain.h>
#include <SHN/SHNStruct.h>
#include <vector>
#include <iostream>
NiSmartPointer(ShineBlockData);
class ShineBlockData : public NiRefObject
{
public:
	ShineBlockData() = default;

	bool Load(MapInfo* Info);
	bool Save(std::string Path);
	void UpdateSHBDData(int w, int h, bool Walkable);
	void CreateEmpty(int Size);
	bool IsWalkable(int w, int h);
	unsigned int GetMapSize() { return MapSize; }
	unsigned int GetSHBDSize() { return SHBDSize; }
	void UpdateSHBDData(std::vector<char> NewData) 
	{
		Data = NewData;
		_HadDirectDataUpdate = true;
	}
	bool HadDirectUpdate()
	{
		bool b = _HadDirectDataUpdate;
		_HadDirectDataUpdate = false;
		return b;
	}
	std::vector<char> GetSHBDData() { return Data; }
private:
	unsigned int MapSize; //Größer der Map Generell
	unsigned int SHBDSize; //Anzahl an Bytes für eine Breite der Map
	std::vector<char> Data;
	bool _HadDirectDataUpdate = false;
};

