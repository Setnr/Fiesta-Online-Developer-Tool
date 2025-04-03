#include "ShineMapData.h"
#include <fstream>
#include <PgUtil.h>
#include <Logger.h>
#include <filesystem>
#include <future>
#include <map>
#include <Data/NiCustom/NiSHMDPickable.h>
#include <NiCollisionData.h>

struct ObjectPosition
{
	ObjectPosition(){}
	ObjectPosition(NiPoint3 point, NiMatrix3 matrix, float s) 
	{
		pos = point;
		quater.FromRotation(matrix);
		Scale = s;
	}
	NiPoint3 pos;
	NiQuaternion quater;
	float Scale;
};

bool ShineMapData::Load(MapInfo* Info) 
{
	std::ifstream SHMD;
	std::string Path = PgUtil::PathFromClientFolder( PgUtil::GetMapFolderPath(Info->KingdomMap, Info->MapFolderName) + Info->MapFolderName + ".shmd");
	std::string GlobalLightPath = PgUtil::PathFromClientFolder(PgUtil::GetMapFolderPath(Info->KingdomMap, Info->MapFolderName) + "GlobalLight.nif");
	SHMD.open(Path, std::ios::binary);

	if (!SHMD.is_open()) 
	{
		LogError("Failed to open SHMD for\n" + Info->MapName);
		return false;
	}

	std::string line;
	int Counter;
	float r, g, b, depth;
	std::vector<std::pair<std::string, std::vector<ObjectPosition>>> ObjectList;

	SHMD >> line;
	if (line != "shmd0_5") 
	{
		LogError("Wrong SHMD-Version\n" + Info->MapName);
		return false;
	}

	SHMD >> line;
	if (line != "Sky")
	{
		LogError("SHMD doesnt contain at least one Sky-Object\n" + Info->MapName);
		return false;
	}
	SHMD >> Counter;
	for (int i = 0; i < Counter; i++)
	{
		std::string FilePath;
		SHMD >> FilePath;
		std::string FullFilePath = PgUtil::PathFromClientFolder(FilePath);

		NiNodePtr Node;
		PgUtil::LoadNodeNifFile(FullFilePath.c_str(), &Node, NULL);
		Node->SetName(FilePath.c_str());
		Node->Update(0.f);
		Node->UpdateProperties();
		Node->UpdateEffects();
		SkyList.push_back(Node);
	}

	SHMD >> line;
	if (line != "Water")
	{
		LogError("SHMD doesnt contain at least one Water-Objectr\n" + Info->MapName);
		return false;
	}
	SHMD >> Counter;
	for (int i = 0; i < Counter; i++)
	{
		std::string FilePath;
		SHMD >> FilePath;
		std::string FullFilePath = PgUtil::PathFromClientFolder(FilePath);

		NiNodePtr Node;
		PgUtil::LoadNodeNifFile(FullFilePath.c_str(), &Node, NULL);
		Node->SetName(FilePath.c_str());
		WaterList.push_back(Node);
	}

	SHMD >> line;
	if (line != "GroundObject")
	{
		LogError("SHMD doesnt contain at least one GroundObject-Objectr\n" + Info->MapName);
		return false;
	}
	SHMD >> Counter;
	for (int i = 0; i < Counter; i++)
	{
		std::string FilePath;
		SHMD >> FilePath;
		std::string FullFilePath = PgUtil::PathFromClientFolder(FilePath);

		NiNodePtr Node;
		PgUtil::LoadNodeNifFile(FullFilePath.c_str(), &Node, NULL);
		if(Node)
		{
			Node->SetName(FilePath.c_str());
			ExtractCollision(Node);
			GlobalGroundObjectList.push_back(Node);
		}
	}

	SHMD >> line;
	if (line != "GlobalLight")
	{
		LogError("SHMD doesnt contain a GlobalLight\n" + Info->MapName);
		return false;
	}

	if (std::filesystem::exists(GlobalLightPath))
	{
		GlobalLightNif = PgUtil::LoadNifFile<NiNode>(GlobalLightPath.c_str());
	}
	SHMD >> r >> g >> b;
	{
		GlobalLight = NiColor(r, g, b);
	}

	SHMD >> line;
	if (line != "Fog")
	{
		LogError("SHMD doesnt contain Fog data\n" + Info->MapName);
		return false;
	}
	SHMD >> depth >> r >> g >> b;
	Fog.Color = NiColor(r, g, b);
	Fog.Depth = depth;

	SHMD >> line;
	if (line != "BackGroundColor")
	{
		LogError("SHMD doesnt contain a BackGroundColor\n" + Info->MapName);
		return false;
	}

	SHMD >> r >> g >> b;
	{
		BackgroundColor = NiColor(r, g, b);
	}

	SHMD >> line;
	if (line != "Frustum")
	{
		LogError("SHMD doesnt contain Frustum data\n" + Info->MapName);
		return false;
	}

	SHMD >> Frustum;

	SHMD >> line;
	while (line != "DataObjectLoadingEnd" && !SHMD.eof())
	{
		std::vector<ObjectPosition> Positions;
		int counter;
		SHMD >> counter;
		for (int i = 0; i < counter; i++)
		{
			ObjectPosition p;
			SHMD >> p.pos.x >> p.pos.y >> p.pos.z >> p.quater.m_fX >> p.quater.m_fY >> p.quater.m_fZ >> p.quater.m_fW >> p.Scale;
			Positions.push_back(p);
		}
		ObjectList.push_back({ line,Positions });
		SHMD >> line;
	}

	if (SHMD.eof())
	{
		SHMD.close();
		return true;
	}
	SHMD >> line;
	if (line != "DirectionLightAmbient")
	{
		LogError("SHMD doesnt contain DirectionLightAmbient data\n" + Info->MapName);
		return false;
	}
	SHMD >> r >> g >> b;
	DirectionalLightAmbientColor = NiColor(r, g, b);

	if (SHMD.eof())
	{
		SHMD.close();
		return true;
	}
	SHMD >> line;
	if (line != "DirectionLightDiffuse")
	{
		LogError("SHMD doesnt contain DirectionLightDiffuse data\n" + Info->MapName);
		return false;
	}
	SHMD >> r >> g >> b;
	DirectionalLightDiffuseColor = NiColor(r, g, b);

	std::vector<std::shared_future<std::set<NiNodePtr>>> _ThreadList;
	for (auto obj : ObjectList)
	{
		auto future = std::async(std::launch::async, [this, obj]
			{
				NiSHMDPickablePtr MainObj;
				NiPickablePtr Obj;
				std::set<NiNodePtr> List;
				for (auto pos = obj.second.begin(); pos < obj.second.end(); pos++)
				{
					std::string Path = PgUtil::PathFromClientFolder(obj.first);
					if (!MainObj)
					{
						MainObj = PgUtil::LoadNifFile<NiSHMDPickable>(Path.c_str(), NULL);
						MainObj->SetFilePathOrName(obj.first);
						MainObj->ExtractCollision();

					}
					Obj = (NiPickable*)MainObj->Clone();
					NiNodePtr ptr = &*Obj;

					List.insert(ptr);

					Obj->SetFilePathOrName(obj.first);

					Obj->SetDefaultCopyType(Obj->COPY_UNIQUE);
					Obj->SetTranslate(pos->pos);
					NiMatrix3 m;
					pos->quater.ToRotation(m);
					Obj->SetRotate(m);
					Obj->SetScale(pos->Scale);

					Obj->DisableSorting();
					Obj->SetSelectiveUpdateRigid(true);

				}
				return  List ;
			});
		_ThreadList.push_back(future.share());
	}

	{
		using namespace std::chrono_literals;
		while (_ThreadList.size())
		{
			auto status = _ThreadList.at(_ThreadList.size() - 1).wait_for(20ms);
			if (status == std::future_status::ready)
			{
				for(auto node : _ThreadList.at(_ThreadList.size() - 1).get())
					GroundObjectList.insert(GroundObjectList.end(), node);
				_ThreadList.pop_back();
				continue;
			}
			Sleep(1);
		}
	}
	return true;
}
void ShineMapData::ExtractCollision(NiNodePtr obj)
{
	std::vector<NiAVObjectPtr> Removes;
	obj->CompactChildArray();
	for (int i = 0; i < obj->GetChildCount(); i++)
	{
		NiAVObjectPtr child = obj->GetAt(i);
		if (!child)
			continue;
		auto Name = child->GetName();
		if ((Name.Contains("#CD") || Name.Contains("#M")) && NiIsKindOf(NiAVObject,child))
		{
			Removes.push_back(child);
		}
		else
		{
			if (NiIsKindOf(NiNode, child))
			{
				ExtractCollision(NiSmartPointerCast(NiNode, child));
			}
			if (NiIsKindOf(NiTriStrips, child)) 
			{
				/*auto colidee = child->GetCollisionObject();
				if(colidee)
				{
					NiNode* node = NiNew NiNode;
					node->AttachChild(NiSmartPointerCast(NiAVObject, child->Clone()));
					CollisionObjectList.push_back(node);
				}*/
			}
		}
		
	}
	for (auto rem : Removes)
	{
		if (NiIsKindOf(NiAVObject, rem))
		{
			obj->DetachChild(rem);
			obj->CompactChildArray();

			if(rem->GetName().Contains("#CD"))
				CollisionObjectList.push_back(NiSmartPointerCast(NiAVObject,rem));
			else
				CameraCollisionObjectList.push_back(NiSmartPointerCast(NiAVObject, rem));

			rem->UpdateProperties();
			rem->UpdateEffects();
			rem->Update(0.0f);
		}
	}
	obj->UpdateProperties();
	obj->UpdateEffects();
	obj->Update(0.0f);
}
bool ShineMapData::Save(MapInfo* Info) 
{
	std::string Path = PgUtil::PathFromClientFolder(PgUtil::GetMapFolderPath(Info->KingdomMap, Info->MapFolderName) + Info->MapName + ".shmd");
	if (std::filesystem::exists(Path))
	{
		if (std::filesystem::exists(Path + ".bak"))
			std::filesystem::remove(Path + ".bak");
		std::filesystem::copy(Path, Path + ".bak");
	}
	std::ofstream file;
	file.open(Path);
	if (!file.is_open()) 
	{
		LogError("Failed to save\n" + Path);
		return false;
	}
	file << std::fixed << std::setprecision(6);
	file << "shmd0_5" << std::endl;

	file << "Sky " << std::to_string(SkyList.size()) << std::endl;
	for(int i = 0; i < SkyList.size(); i++)
		file << SkyList[i]->GetName() << std::endl;

	file << "Water " << std::to_string(WaterList.size()) << std::endl;
	for (int i = 0; i < WaterList.size(); i++)
		file << WaterList[i]->GetName() << std::endl;

	file << "GroundObject " << std::to_string(GlobalGroundObjectList.size()) << std::endl;
	for (int i = 0; i < GlobalGroundObjectList.size(); i++)
		file << GlobalGroundObjectList[i]->GetName() << std::endl;

	file << "GlobalLight " << GlobalLight.r << " " << GlobalLight.g << " " << GlobalLight.b << std::endl;

	file << "Fog " << Fog.Depth <<" " << Fog.Color.r << " " << Fog.Color.g << " " << Fog.Color.b << std::endl;

	file << "BackGroundColor " << BackgroundColor.r << " " << BackgroundColor.g << " " << BackgroundColor.b << std::endl;

	file << "Frustum " << Frustum << std::endl;

	std::map<std::string, std::vector<NiPickablePtr>> ObjectMap;
	for (auto child : GroundObjectList)
	{
		NiPickablePtr ptr = NiSmartPointerCast(NiPickable,child);
		auto it = ObjectMap.find(ptr->GetSHMDPath());
		if (it == ObjectMap.end())
		{
			std::vector<NiPickablePtr> vec;
			vec.push_back(ptr);
			ObjectMap.insert({ std::string(ptr->GetSHMDPath()), vec });
		}
		else
		{
			it->second.push_back(ptr);
		}
	}
	for (auto Info : ObjectMap)
	{
		file << Info.first << " " << std::to_string(Info.second.size()) << std::endl;
		for (auto obj : Info.second)
		{
			ObjectPosition ObjectData( obj->GetTranslate() ,  obj->GetRotate(), obj->GetScale());

			file << ObjectData.pos.x << " " << ObjectData.pos.y << " " << ObjectData.pos.z << " " << ObjectData.quater.m_fX << " " << ObjectData.quater.m_fY << " " << ObjectData.quater.m_fZ << " " << ObjectData.quater.m_fW << " " << ObjectData.Scale << std::endl;
		}
	}
	file << "DataObjectLoadingEnd" << std::endl;

	file << "DirectionLightAmbient " << DirectionalLightAmbientColor.r << " " << DirectionalLightAmbientColor.g << " " << DirectionalLightAmbientColor.b << std::endl;

	file << "DirectionLightDiffuse " << DirectionalLightDiffuseColor.r << " " << DirectionalLightDiffuseColor.g << " " << DirectionalLightDiffuseColor.b << std::endl;
	
	file.close();
	return true;
}
void ShineMapData::CreateEmpty() 
{
	Version = "shmd_05";
	GlobalLight = NiColor(0.792157f, 0.792157f, 0.792157f);
	Fog.Depth = 0.57;
	Fog.Color = NiColor(0.070588f, 0.541176f, 0.929412f);
	BackgroundColor = NiColor(0.000000f, 0.501961f, 1.000000f);
	Frustum = 5000.f;
	DirectionalLightAmbientColor = NiColor(0.000000f, 0.000000f, 0.000000f);
	DirectionalLightDiffuseColor = NiColor(1.000000f, 0.976471f, 0.901961f);
}