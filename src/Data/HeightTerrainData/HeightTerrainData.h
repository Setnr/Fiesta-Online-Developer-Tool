#pragma once
#include <NiMain.h>
#include <SHN/SHNStruct.h>
#include <vector>
#include <iostream>
#include <Logger.h>

NiSmartPointer(HeightTerrainData);
class HeightTerrainData :    public NiRefObject
{
public:
	HeightTerrainData() = default;
	bool Load(std::string Path, int Width, int Height);
	bool Save(std::string Path);
	/*
	The Following Functions dont check if w or h are valid values
	Maybe this will get added need to be checked what the later Performance is saying
	*/
	inline float GetHTD(int w, int h) { return (HTD.size()) ? HTD[_Width * h + w] : 0.f;}
	inline float GetHTDG(int w, int h) { return (HTDG.size()) ? HTDG[_Width * h + w] : 0.f; }


	inline void SetHTD(int w, int h, float value) 
	{
		if (h > _Height || w > _Width || h < 0 || w < 0)
		{
			LogError("Will Not Set HTD Value due to OutOfBounds");
			return;
		}
		HTD[_Width * h + w] = value; 
	}
	inline void SetHTDG(int w, int h, float value) 
	{ 
		if (h > _Height || w > _Width || h < 0 || w < 0)
		{
			LogError("Will Not Set HTDG Value due to OutOfBounds");
			return;
		}
		HTDG[_Width * h + w] = value; 
	}
private:
	std::vector<float> HTD;
	std::vector<float> HTDG;
	int PointCounter = 0;
	int _Width;
	int _Height;
	
};

