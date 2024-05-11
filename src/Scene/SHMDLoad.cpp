#include "EditorScene.h"

#include <fstream>
#include <filesystem>
#include "Logger.h"
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
		std::string FullFilePath = PgUtil::CreateFullFilePathFromBaseFolder(FilePath);

		NiNodePtr sky;
		PgUtil::LoadNodeNifFile(FullFilePath.c_str(), &sky, NULL);
		sky->SetName(FilePath.c_str());
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
		std::string FullFilePath = PgUtil::CreateFullFilePathFromBaseFolder(FilePath);

		NiNodePtr water;
		PgUtil::LoadNodeNifFile(FullFilePath.c_str(), &water, NULL);
		water->SetName(FilePath.c_str());
		{
			std::lock_guard<std::mutex> lock(WorldLock);
			kWorld.AttachWater(water);
		}
	}
	return true;
}

void RemoveCDAndM(NiNodePtr& obj) 
{
	std::vector<NiAVObjectPtr> Removes;
	for (int i = 0; i < obj->GetChildCount(); i++) 
	{
		NiAVObjectPtr child = obj->GetAt(i);
		if (!child)
			continue;
		if(NiIsKindOf(NiGeometry,child))
		{
			auto Name = child->GetName();
			if (Name.Contains("#CD") || Name.Contains("#M"))
			{
				Removes.push_back(child);
			}
		}
		else if(NiIsKindOf(NiNode,child))
		{
			NiNodePtr p = (NiNode*)(NiAVObject*)child;
			RemoveCDAndM(p);
		}
	}
	for (auto rem : Removes)
	{
		obj->DetachChild(rem);
	}
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
		std::string FullFilePath = PgUtil::CreateFullFilePathFromBaseFolder(FilePath);

		NiNodePtr Ground = PgUtil::LoadNifFile(FullFilePath.c_str(), NULL);
		Ground->SetName(FilePath.c_str());
		RemoveCDAndM(Ground);
		{
			std::lock_guard<std::mutex> lock(WorldLock);
			kWorld.AttachGroundObj(Ground); 
		}
	}
	return true;
}

bool EditorScene::LoadGlobalLight(std::ifstream& SHMD, std::string& GlobalLightNifPath)
{
	std::string line;
	SHMD >> line;
	if (line != "GlobalLight")
		return false;
	float r, g, b;
	if (std::filesystem::exists(GlobalLightNifPath))
	{
		NiNodePtr Light = PgUtil::LoadNifFile(GlobalLightNifPath.c_str());
		std::lock_guard<std::mutex> lock(WorldLock);
		kWorld.SetGlobalLight(Light);
	}
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
	SHMD >> depth >> r >> g >> b;
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
	kWorld.SetBackgroundColor(r, g, b);
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
		std::vector<ObjectPosition> Positions;
		if (!LoadOneObject(SHMD, Positions))
		{
			NiMessageBox::DisplayMessage("Failed to Load SHMD Object", "ERROR");
			return false;
		}
		ObjectList.push_back({ line,Positions });
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