#include "IngameWorld.h"
#include "../Logger/Logger.h"
#include <chrono>
#include <future>
World::World(MapInfo* MapInfo) : _Info(MapInfo) , TerrainWorld(PgUtil::CreateFilePathFromMapInfo(MapInfo->KingdomMap, MapInfo->MapFolderName, "ini")) , _SHBD(MapInfo)
{
	auto start = std::chrono::steady_clock::now();


	if (!LoadSHMD())
		return;
	if (!LoadSHBD())
		return;

	if (!GetWorldScene())
		return;

	GetWorldScene()->UpdateEffects();
	GetWorldScene()->UpdateProperties();
	GetWorldScene()->Update(0.0);

	auto diff = std::chrono::steady_clock::now() - start;
	std::ostringstream oss;
	oss << "Successfully Loaded " << _Info->MapName << "("
		<< std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms)";
	LogInfo(oss.str());
}

void World::CreateBrushTexture(NiPoint3& BrushPositon, int BrushSize, bool MoveStatus)
{

	std::vector<char> SHBDData = _SHBD.GetDataRefrence();

	for (int h = ResetPoint.second - BrushSize; h <= ResetPoint.second + BrushSize && h < _SHBD.GetSHBDSize(); h++)
	{
		if (h < 0)
			continue;
		int i = _SHBD.GetMapSize() * h;
		for (int w = ResetPoint.first - BrushSize; w <= ResetPoint.first + BrushSize && w < _SHBD.GetSHBDSize(); w++)
		{
			if (w < 0)
				continue;
			int TextureH = h / TextureSize;
			int TextureW = w / TextureSize;
			NiPixelDataPtr pixleData = TextureConnector[TextureH][TextureW];
			pixleData->MarkAsChanged();
			int TextureInternH = h % TextureSize;
			int TextureInternW = w % TextureSize;
			unsigned int* NewPtr = ((unsigned int*)pixleData->GetPixels()) + (TextureInternH * pixleData->GetWidth()) + TextureInternW;

			int hges = h;
			int wges = w;

			int index = hges * _SHBD.GetSHBDSize() + wges;

			size_t charIndex = index / 8;
			if (charIndex < SHBDData.size())
			{
				size_t bitIndex = index % 8;

				if ((SHBDData[charIndex] >> bitIndex) & 0x1)
					*NewPtr = Blocked;
				else
					*NewPtr = Walkable;
			}
		}
	}

	int middlew = BrushPositon.x / PixelSize;
	int middleh = BrushPositon.y / PixelSize;

	for (int h = middleh - BrushSize; h <= middleh + BrushSize && h < _SHBD.GetSHBDSize(); h++)
	{
		if (h < 0)
			continue;
		int i = _SHBD.GetMapSize() * h;

		for (int w = middlew - BrushSize; w <= middlew + BrushSize && w < _SHBD.GetSHBDSize(); w++)
		{
			if (w < 0)
				continue;
			int TextureH = h / TextureSize;
			int TextureW = w / TextureSize;
			NiPixelDataPtr pixleData = TextureConnector[TextureH][TextureW];
			pixleData->MarkAsChanged();
			int TextureInternH = h % TextureSize;
			int TextureInternW = w % TextureSize;
			unsigned int* NewPtr = ((unsigned int*)pixleData->GetPixels()) + (TextureInternH * pixleData->GetWidth()) + TextureInternW;
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				int offset = i + (w / 8);
				int Shift = w % 8;

				_SHBD.UpdateSHBDData(offset, Shift, MoveStatus);

			}
			*NewPtr = BrushColor;
		}
	}
	ResetPoint = std::pair<int, int>(middlew, middleh);


}

void World::SaveSHBD() 
{
	auto start = std::chrono::steady_clock::now();
	_SHBD.Save();
	auto diff = std::chrono::steady_clock::now() - start;
	std::ostringstream oss;
	oss << "Successfully safed SHBD for " << _Info->MapName << "("
		<< std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms)";
	LogInfo(oss.str());
}
void World::SaveSHMD() 
{
	auto start = std::chrono::steady_clock::now();
	if (std::filesystem::exists(PgUtil::CreateFilePathFromMapInfo(_Info->KingdomMap, _Info->MapFolderName, "shmd.bak")))
		std::filesystem::remove(PgUtil::CreateFilePathFromMapInfo(_Info->KingdomMap, _Info->MapFolderName, "shmd.bak"));

	std::filesystem::copy(PgUtil::CreateFilePathFromMapInfo(_Info->KingdomMap, _Info->MapFolderName, "shmd"), PgUtil::CreateFilePathFromMapInfo(_Info->KingdomMap, _Info->MapFolderName, "shmd.bak"));
	std::string _FilePath = PgUtil::CreateFilePathFromMapInfo(_Info->KingdomMap, _Info->MapFolderName, "shmd");
	std::ofstream file;
	file.open(_FilePath);
	file << std::fixed << std::setprecision(6);
	file << "shmd0_5" << std::endl;
	SaveSHMDEntry(file, GetSkyNode(), "Sky");
	SaveSHMDEntry(file, GetWaterNode(), "Water");
	SaveSHMDGlobalGroundObjects(file, GetGroundObjNode());
	SaveSHMDLight(file, GetAmbientLightAmbientColor(), "GlobalLight");
	SaveSHMDFog(file, GetFogDepth(), GetFogColor());
	SaveSHMDLight(file, GetBackgroundColor(), "BackGroundColor");
	SaveSHMDFrustum(file, GetWorldFrustum());
	SaveSHMDGroundObjects(file, GetGroundObjNode());
	SaveSHMDLight(file, GetMapDirectionalLightAmbientColor(), "DirectionLightAmbient");
	SaveSHMDLight(file, GetMapDirectionalLightDiffuseColor(), "DirectionLightDiffuse");
	file.close();
	auto diff = std::chrono::steady_clock::now() - start;
	std::ostringstream oss;
	oss << "Successfully safed SHMD for " << _Info->MapName << "("
		<< std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms)";
	LogInfo(oss.str());
}
void World::SaveSHMDEntry(std::ofstream& file, NiNodePtr objNode, const char* Name)
{
	objNode->CompactChildArray();
	file << Name << " " << std::to_string(objNode->GetChildCount()) << std::endl;
	for (int i = 0; i < objNode->GetChildCount(); i++)
	{
		file << objNode->GetAt(i)->GetName() << std::endl;
	}
}
void World::SaveSHMDLight(std::ofstream& file, NiColor color, const char* name)
{
	file << name << " " << color.r << " " << color.g << " " << color.b << std::endl;
}
void World::SaveSHMDFog(std::ofstream& file, float depth, NiColor color)
{
	file << "Fog " << depth << " " << color.r << " " << color.g << " " << color.b << std::endl;
}
void World::SaveSHMDFrustum(std::ofstream& file, NiFrustum frustum)
{
	file << "Frustum " << frustum.m_fFar << std::endl;
}
void World::SaveSHMDGroundObjects(std::ofstream& file, NiNodePtr node)
{
	node->CompactChildArray();
	std::map<std::string, std::vector<NiNodePtr>> ObjectMap;
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		auto child = node->GetAt(i);
		if (!NiIsKindOf(NiPickable, child))
			continue;
		NiPickable* ptr = (NiPickable*)child;
		auto it = ObjectMap.find(ptr->GetSHMDPath());
		if (it == ObjectMap.end())
		{
			std::vector<NiNodePtr> vec;
			vec.push_back((NiNode*)child);
			ObjectMap.insert({ std::string(ptr->GetSHMDPath()), vec });
		}
		else
		{
			it->second.push_back((NiNode*)child);
		}
	}
	for (auto Info : ObjectMap)
	{
		file << Info.first << " " << std::to_string(Info.second.size()) << std::endl;
		for (auto obj : Info.second)
		{
			NiPoint3 trans = obj->GetTranslate();
			NiMatrix3 matrix = obj->GetRotate();
			NiQuaternion quater;
			quater.FromRotation(matrix);
			float scale = obj->GetScale();

			file << trans.x << " " << trans.y << " " << trans.z << " " << quater.m_fX << " " << quater.m_fY << " " << quater.m_fZ << " " << quater.m_fW << " " << scale << std::endl;
		}
	}
	file << "DataObjectLoadingEnd" << std::endl;
}
void World::SaveSHMDGlobalGroundObjects(std::ofstream& file, NiNodePtr objNode)
{
	objNode->CompactChildArray();
	objNode->CompactChildArray();
	std::vector<NiNodePtr> ObjectMap;
	for (int i = 0; i < objNode->GetChildCount(); i++)
	{
		auto child = objNode->GetAt(i);
		if (!NiIsKindOf(NiNode, child))
			continue;
		auto& Name = child->GetName();
		if (Name.ContainsNoCase("resmap"))
		{
			NiNodePtr ptr = (NiNode*)child;
			ObjectMap.push_back(ptr);
		}
	}
	file << "GroundObject" << " " << std::to_string(ObjectMap.size()) << std::endl;
	for (auto Info : ObjectMap)
	{
		file << Info->GetName() << std::endl;
		
	}
}

void World::ShowHTDG(bool Show, NiNodePtr OrbNode)
{
	if (!Show)
	{
		m_spGroundScene->AttachChild(m_spGroundObject);
		m_spNormalLightScene->AttachChild(m_spWaterScene);
		m_spGroundScene->DetachChild(OrbNode);
	}
	else
	{
		m_spGroundScene->DetachChild(m_spGroundObject);
		m_spNormalLightScene->DetachChild(m_spWaterScene);
		m_spGroundScene->AttachChild(OrbNode);
		m_spGroundScene->CompactChildArray();
		m_spNormalLightScene->CompactChildArray();

	}

	m_spGroundScene->DetachChild(m_spGroundTerrain);
	m_spGroundScene->AttachChild(m_spGroundTerrain);
	m_spGroundScene->CompactChildArray();

	//m_spGroundScene->UpdateEffects();
	//m_spGroundScene->UpdateProperties();
	m_spGroundScene->Update(0.0f);
	OrbNode->UpdateEffects();
	OrbNode->UpdateProperties();
	OrbNode->Update(0.0f);
}

void World::SaveHTDG() 
{
	if (_InitFile.HeightFileName == "")
	{
		LogInfo("This map does not have a HTDG to save");
		return;
	}
	auto start = std::chrono::steady_clock::now();
	std::string HTDFilePath = PgUtil::CreateFullFilePathFromBaseFolder(_InitFile.HeightFileName);
	std::string HTDGFilePath = HTDFilePath + "G";
	if (std::filesystem::exists(HTDGFilePath + ".bak"))
		std::filesystem::remove(HTDGFilePath + ".bak");
	if(std::filesystem::exists(HTDGFilePath))
		std::filesystem::copy(HTDGFilePath, HTDGFilePath + ".bak");

	std::ofstream file;
	file.open(HTDGFilePath, std::ios::binary);
	int Size = _InitFile.HeightMap_height * _InitFile.HeightMap_width;
	file.write((char*) & Size, sizeof(Size));
	for (int h = 0; h < _InitFile.HeightMap_height; h++)
	{
		for (int w = 0; w < _InitFile.HeightMap_width; w++)
		{
			float Value = _HTD[w][h].Vec[0]->z - _HTD[w][h].Height;
			file.write((char*)&Value, sizeof(Value));
		}
	}
	file.close();
	auto diff = std::chrono::steady_clock::now() - start;
	std::ostringstream oss;
	oss << "Successfully safed HTDG for" << _Info->MapName << "("
		<< std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms)";
	LogInfo(oss.str());
}