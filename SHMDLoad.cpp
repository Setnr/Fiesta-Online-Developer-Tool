#include "EditorScene.h"

#include <fstream>

bool EditorScene::CheckSHMDVersion(std::ifstream& SHMD) 
{
	std::string line;
	SHMD >> line;
	if (line == "shmd0_5")
		return true;
	return false;
}

bool EditorScene::LoadSky(std::ifstream& SHMD)
{
	std::string line;
	SHMD >> line;
	if (line != "Sky")
		return false;
	int SkyCounter;
	SHMD >> SkyCounter;
	for (int i = 0; i < SkyCounter; i++) 
	{
		std::string FilePath;
		SHMD >> FilePath;
		FilePath = PgUtil::CreateFullFilePathFromBaseFolder(FilePath);

		NiNodePtr sky;
		PgUtil::LoadNodeNifFile(FilePath.c_str(), &sky, NULL);
		{
			std::lock_guard<std::mutex> lock(WorldLock);
			kWorld.AttachSky(sky); 
		}
	}
	return true;
}

bool EditorScene::LoadWater(std::ifstream& SHMD)
{
	std::string line;
	SHMD >> line;
	if (line != "Water")
		return false;
	int WaterCounter;
	SHMD >> WaterCounter;
	for (int i = 0; i < WaterCounter; i++)
	{
		std::string FilePath;
		SHMD >> FilePath;
		FilePath = PgUtil::CreateFullFilePathFromBaseFolder(FilePath);

		NiNodePtr water;
		PgUtil::LoadNodeNifFile(FilePath.c_str(), &water, NULL);
		{
			std::lock_guard<std::mutex> lock(WorldLock);
			kWorld.AttachWater(water);
		}
	}
	return true;
}

bool EditorScene::LoadGroundObject(std::ifstream& SHMD)
{
	std::string line;
	SHMD >> line;
	if (line != "GroundObject")
		return false;
	int GroundCounter;
	SHMD >> GroundCounter;
	for (int i = 0; i < GroundCounter; i++)
	{
		std::string FilePath;
		SHMD >> FilePath;
		FilePath = PgUtil::CreateFullFilePathFromBaseFolder(FilePath);

		NiNodePtr Ground;
		PgUtil::LoadNodeNifFile(FilePath.c_str(), &Ground, NULL);
		{
			std::lock_guard<std::mutex> lock(WorldLock);
			kWorld.AttachGroundObj(Ground); 
		}
	}
	return true;
}

bool EditorScene::LoadGlobalLight(std::ifstream& SHMD)
{
	std::string line;
	SHMD >> line;
	if (line != "GlobalLight")
		return false;
	float r, g, b;
	SHMD >> r >> g >> b;
	{
		std::lock_guard<std::mutex> lock(WorldLock);
		kWorld.SetAmbientLightAmbientColor(NiColor(r, g, b));
	}
	return true;
}

bool EditorScene::LoadFog(std::ifstream& SHMD)
{
	std::string line;
	SHMD >> line;
	if (line != "Fog")
		return false;
	float r, g, b, depth;
	SHMD >> r >> g >> b >> depth;
	{ 
		std::lock_guard<std::mutex> lock(WorldLock);
		kWorld.SetFogColor(NiColor(r, g, b));
		kWorld.SetFogDepth(depth);
	}
	return true;
}

bool EditorScene::LoadBackGroundColor(std::ifstream& SHMD)
{
	std::string line;
	SHMD >> line;
	if (line != "BackGroundColor")
		return false;
	float r, g, b;
	SHMD >> r >> g >> b;
	/* Currently Unk Where to Put that Color*/
	return true;
}

bool EditorScene::LoadFrustum(std::ifstream& SHMD)
{
	std::string line;
	SHMD >> line;
	if (line != "Frustum")
		return false;
	float Frustum;
	SHMD >> Frustum;
	{
		std::lock_guard<std::mutex> lock(WorldLock);
		kWorld.SetFarFrumstum(Frustum);
	}
	return true;
}

 bool EditorScene::LoadGlobalObjects(std::ifstream& SHMD, std::vector<std::pair<std::string, std::vector<EditorScene::ObjectPosition>>>& ObjectList)
{
	std::string line;
	SHMD >> line;
	while (line != "DataObjectLoadingEnd")
	{
		std::string Path = PgUtil::CreateFullFilePathFromBaseFolder(line);
		std::vector<ObjectPosition> Positions;
		if (!LoadOneObject(SHMD, Positions))
		{
			NiMessageBox::DisplayMessage("Failed to Load SHMD Object", "ERROR");
			return false;
		}
		ObjectList.push_back({ Path,Positions });
		SHMD >> line;
	}
	return true;
}

bool EditorScene::LoadOneObject(std::ifstream& SHMD, std::vector<ObjectPosition>& Positions)
{
	int counter;
	SHMD >>  counter;
	for (int i = 0; i < counter; i++) 
	{
		ObjectPosition p;
		SHMD >> p.pos.x >> p.pos.y >> p.pos.z >> p.quater.m_fX >> p.quater.m_fY >> p.quater.m_fZ >> p.quater.m_fW >> p.Scale;
		Positions.push_back(p);
	}
	return true;
}

bool EditorScene::LoadDirectionLightAmbient(std::ifstream& SHMD)
{
	std::string line;
	SHMD >> line;
	if (line != "DirectionLightAmbient")
		return false;
	float r, g, b;
	SHMD >> r >> g >> b;
	{
		std::lock_guard<std::mutex> lock(WorldLock);
		kWorld.SetMapDirectionalLightAmbientColor(NiColor(r, g, b));
	}
	return true;
}

bool EditorScene::LoadDirectionLightDiffuse(std::ifstream& SHMD)
{
	std::string line;
	SHMD >> line;
	if (line != "DirectionLightDiffuse")
		return false;
	float r, g, b;
	SHMD >> r >> g >> b;
	{
		std::lock_guard<std::mutex> lock(WorldLock);
		kWorld.SetMapDirectionalLightDiffuseColor(NiColor(r, g, b));
	}
	return true;
}