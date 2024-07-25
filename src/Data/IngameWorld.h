#pragma once
#include "../Data/SHNStruct.h"
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
	virtual std::string GetFolderPath()
	{
		if(HasMapInfo())
			return PgUtil::GetMapFolderPath(_Info->KingdomMap, _Info->MapFolderName);
		return _InitFile.HeightFileName.substr(0, _InitFile.HeightFileName.find_last_of("\\"));
	}
	NiNodePtr GetSHBDNode() { return SHBDNode; }


	void ResetSHBD(NiPoint3 SpawnPoint);
	void SaveSHBD(bool Backup = false);
	void SaveSHMD(bool Backup = false);
	void SaveSHMDEntry(std::ofstream&, NiNodePtr, const char*);
	void SaveSHMDLight(std::ofstream&, NiColor, const char*);
	void SaveSHMDFog(std::ofstream&, float, NiColor);
	void SaveSHMDFrustum(std::ofstream&, NiFrustum);
	void SaveSHMDGroundObjects(std::ofstream&, NiNodePtr);
	void SaveSHMDGlobalGroundObjects(std::ofstream&, NiNodePtr);
	NiPoint3 GetSpawnPoint();
	void CreateBrushTexture(NiPoint3& BrushPositon, int BrushSize, bool MoveStatus);

	void ShowHTDG(bool ShowOrbNode, bool ShowObjecs, NiNodePtr OrbNode);
	void SaveHTDG(bool Backup = false);

	std::vector<std::vector<HTDHelper>>& GetHTD() { return _HTD; }
	IniFile& GetIni() { return _InitFile; }

private:
	bool LoadSHMD();
	bool LoadSHBD();

private:
	MapInfo* _Info;

	ShineBlockData _SHBD;
	NiNodePtr SHBDNode;
	std::vector<std::vector<NiPixelDataPtr>> TextureConnector;


	std::pair<int, int> ResetPoint;

	unsigned int Blocked;
	unsigned int Walkable;
	unsigned int BrushColor;
	int TextureSize = 128;
	float PixelSize;
};
