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
	void UpdateSHBDData(int offset, int Shift, bool Status);
	void CreateEmpty(int Size);
	bool IsWalkable(int w, int h);
	
private:
	unsigned int MapSize; //Größer der Map Generell
	unsigned int SHBDSize; //Anzahl an Bytes für eine Breite der Map
	std::vector<char> Data;
};

