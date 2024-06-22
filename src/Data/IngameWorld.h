#pragma once
#include "../Data/SHNStruct.h"
#include "../Data/IniFile.h"
#include "../Data/SHBD/SHBD.h"
#include "TerrainWorld.h"

NiSmartPointer(World);
class World : public TerrainWorld
{
public:
	World(MapInfo* MapInfo);
	~World() 
	{
		
	};


	bool HasMapInfo() { return _Info != nullptr; }
	MapInfo* GetMapInfo() { return _Info; }
	void SetSHBDVisiblity(bool visible)
	{
		if (SHBDNode && GetGroundObjNode())
		{
			if (visible)
				GetGroundObjNode()->AttachChild(SHBDNode);
			else
				GetGroundObjNode()->DetachChild(SHBDNode);
		}
	}
	NiNodePtr GetSHBDNode() { return SHBDNode; }
#pragma region GlobalStatics
	static NiPoint3 ms_kUpDir; // idb
	static NiPoint3 ms_kDownDir; // idb
	static NiPoint3 ms_kNorthDir; // idb
	static NiPoint3 ms_kEastDir; // idb
	static NiPoint3 ms_kWestDir; // idb
	static NiPoint3 ms_kDefaultDirectionalLightDir; // idb
#pragma endregion

	void ResetSHBD(NiPoint3 SpawnPoint);
	void SaveSHBD();
	void SaveSHMD();
	void SaveSHMDEntry(std::ofstream&, NiNodePtr, const char*);
	void SaveSHMDLight(std::ofstream&, NiColor, const char*);
	void SaveSHMDFog(std::ofstream&, float, NiColor);
	void SaveSHMDFrustum(std::ofstream&, NiFrustum);
	void SaveSHMDGroundObjects(std::ofstream&, NiNodePtr);
	void SaveSHMDGlobalGroundObjects(std::ofstream&, NiNodePtr);
	NiPoint3 GetSpawnPoint();
	void CreateBrushTexture(NiPoint3& BrushPositon, int BrushSize, bool MoveStatus);

	void ShowHTDG(bool Show, NiNodePtr OrbNode);
	void SaveHTDG();
	struct HTDHelper {
		float Height;
		std::vector<NiPoint3*> Vec;
		std::vector<NiGeometryData*> Shape;
	};
	std::vector<std::vector<HTDHelper>>& GetHTD() { return _HTD; }
	IniFile& GetIni() { return _InitFile; }

private:
	bool LoadTerrain();
	bool LoadSHMD();
	bool LoadSHBD();

private:
	MapInfo* _Info;
	IniFile _InitFile;

	ShineBlockData _SHBD;
	NiNodePtr SHBDNode;
	std::vector<std::vector<NiPixelDataPtr>> TextureConnector;

	std::vector<std::vector<HTDHelper>> _HTD;

	std::pair<int, int> ResetPoint;

	unsigned int Blocked;
	unsigned int Walkable;
	unsigned int BrushColor;
	int TextureSize = 128;
	float PixelSize;
};
