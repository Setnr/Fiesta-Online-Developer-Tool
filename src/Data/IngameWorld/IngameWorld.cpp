#include "IngameWorld.h"
#include <Logger.h>
#include <filesystem>
#include <PgUtil.h>
#include "WorldChanges/FogChange.h"
#include <NiPick.h>
#include <FiestaOnlineTool.h>
#include "Data/NPCData/NPCData.h"
#include "Data/SHN/Mobloader.h"
#include "NiCustom/ShineNPCNode.h"

#pragma region Gizmo-Calc
glm::vec4 ConvertQuatToAngleAxis(glm::quat q)
{
	if (q.w == 1.f)
	{
		return { 0.f, 1.f, 0.f, 0.f };
	}

	float_t angle_rad = acos(q.w) * 2;

	float_t x = q.x / sin(angle_rad / 2);
	float_t y = q.y / sin(angle_rad / 2);
	float_t z = q.z / sin(angle_rad / 2);

	return { angle_rad, x, y, z };
}
#pragma endregion

IngameWorld::IngameWorld(MapInfo* Info) : _MapInfo(Info)
{

	if (!InitScene())
		return;
	if (!InitCamera())
		return;
	if (!InitSkyCtrl())
		return;
	if (!InitLightFog())
		return;
	if (!InitShadow())
		return; 

	_SHBD = NiNew ShineBlockData();
	_SHMD = NiNew ShineMapData();
	_INI = NiNew ShineIni();

	if (_MapInfo->KingdomMap == -1)
	{
		/*New Map*/
		int MapSize = 256;
		_SHBD->CreateEmpty(MapSize);
		_SHMD->CreateEmpty();
		_INI->CreateEmpty(_MapInfo,MapSize);
		_HTD = NiNew HeightTerrainData;
		_HTD->ResizeHTD(MapSize, MapSize);
		CreateAndAttachTerrain();
	}
	else
	{
		if (!_SHBD->Load(_MapInfo))
			LogError("Failed to Load SHBD for " + _MapInfo->MapName);
		if (!_SHMD->Load(_MapInfo))
			LogError("Failed to Load SHMD for " + _MapInfo->MapName);

		std::string Path = PgUtil::PathFromClientFolder(PgUtil::GetMapFolderPath(_MapInfo->KingdomMap, _MapInfo->MapFolderName) + _MapInfo->MapFolderName + ".ini");
		if (std::filesystem::exists(Path))
			if (!_INI->Load(Path))
				LogError("Failed to Load INI for " + _MapInfo->MapName);
		if (_INI->GetHTDPath() != "")
			if (LoadHTD())
				CreateAndAttachTerrain();
	}

	

	for (auto obj : _SHMD->GetSkyList())
		m_spSkyScene->AttachChild(obj);

	for (auto obj : _SHMD->GetWaterList())
		m_spWaterScene->AttachChild(obj);

	for (auto obj : _SHMD->GetGlobalGroundObjectList())
		m_spGroundObject->AttachChild(obj);

	for (auto obj : _SHMD->GetGroundObjectListList())
		m_spGroundObject->AttachChild(obj);

	for (auto obj : _SHMD->GetCollisionObjectListList())
		m_spGroundCollidee->AttachChild(obj);

	SetFogColor(_SHMD->GetFogColor(),false);
	SetFogDepth(_SHMD->GetFogDepth(),false);
	SetGlobalLight(_SHMD->GetGlobalLight(), false);
	SetGlobalLightNode(_SHMD->GetGlobalLightNode(), false);
	SetFarFrustum(_SHMD->GetFarFrustum(), false);
	SetDirectionalLightAmbientColor(_SHMD->GetDirectionalLightAmbientColor(), false);
	SetDirectionalLightDiffuseColor(_SHMD->GetDirectionalLightDiffuseColor(), false);

	m_spGroundCollidee->Update(0.f);
	m_spGroundCollidee->UpdateProperties();
	m_spGroundCollidee->UpdateEffects();
	m_spGroundCollidee->Update(0.f);

	m_spWorldScene->Update(0.f);
	m_spWorldScene->UpdateProperties();
	m_spWorldScene->UpdateEffects();
	m_spWorldScene->Update(0.f);
	PgUtil::LookAndMoveAtWorldPoint(m_spCamera, NiPoint3(_MapInfo->RegenX, _MapInfo->RegenY, 500));

	LoadNPCS();
}

IngameWorld::IngameWorld(MapInfo* Info, int MapSize) : _MapInfo(Info)
{
	if (!InitScene())
		return;
	if (!InitCamera())
		return;
	if (!InitSkyCtrl())
		return;
	if (!InitLightFog())
		return;
	if (!InitShadow())
		return;
	std::string MapFolderPath = PgUtil::PathFromClientFolder(PgUtil::GetMapFolderPath(_MapInfo->KingdomMap, _MapInfo->MapFolderName));

	if (std::filesystem::exists(MapFolderPath)) 
	{
		std::filesystem::remove_all(MapFolderPath);
	}
	std::filesystem::create_directory(MapFolderPath);


	_SHBD = NiNew ShineBlockData();
	_SHMD = NiNew ShineMapData();
	_INI = NiNew ShineIni();

	_SHBD->CreateEmpty(MapSize);
	_SHMD->CreateEmpty();
	_INI->CreateEmpty(_MapInfo, MapSize);
	_HTD = NiNew HeightTerrainData;
	_HTD->ResizeHTD(MapSize, MapSize);
	CreateAndAttachTerrain();

	for (auto obj : _SHMD->GetSkyList())
		m_spSkyScene->AttachChild(obj);

	for (auto obj : _SHMD->GetWaterList())
		m_spWaterScene->AttachChild(obj);

	for (auto obj : _SHMD->GetGlobalGroundObjectList())
		m_spGroundObject->AttachChild(obj);

	for (auto obj : _SHMD->GetGroundObjectListList())
		m_spGroundObject->AttachChild(obj);

	for (auto obj : _SHMD->GetCollisionObjectListList())
		m_spGroundCollidee->AttachChild(obj);

	SetFogColor(_SHMD->GetFogColor(), false);
	SetFogDepth(_SHMD->GetFogDepth(), false);
	SetGlobalLight(_SHMD->GetGlobalLight(), false);
	SetGlobalLightNode(_SHMD->GetGlobalLightNode(), false);
	SetFarFrustum(_SHMD->GetFarFrustum(), false);
	SetDirectionalLightAmbientColor(_SHMD->GetDirectionalLightAmbientColor(),false);
	SetDirectionalLightDiffuseColor(_SHMD->GetDirectionalLightDiffuseColor(), false);

	m_spGroundCollidee->Update(0.f);
	m_spGroundCollidee->UpdateProperties();
	m_spGroundCollidee->UpdateEffects();
	m_spGroundCollidee->Update(0.f);

	m_spWorldScene->Update(0.f);
	m_spWorldScene->UpdateProperties();
	m_spWorldScene->UpdateEffects();
	m_spWorldScene->Update(0.f);

	PgUtil::LookAndMoveAtWorldPoint(m_spCamera, NiPoint3(_MapInfo->RegenX, _MapInfo->RegenY, 500));

	LoadNPCS();
}
void IngameWorld::SetFogColor(NiColor kColor, bool Backup) 
{
	NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
	if(Backup)
		AttachStack(NiNew FogChangeColor(this, fog->GetFogColor(), kColor));
	fog->SetFogColor(kColor);
	fog->SetFog(true);
	_SHMD->SetFogColor(kColor);
}
void IngameWorld::SetFogDepth(float depth, bool Backup)
{
	NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
	if (depth < 0.0)
		depth = 0.0;
	if (Backup)
		AttachStack(NiNew FogChangeDepth(this, _SHMD->GetFogDepth(), depth));
	fog->SetDepth(depth);
	_SHMD->SetFogDepth(depth);
}
void IngameWorld::SetGlobalLightNode(NiNodePtr LightNode, bool Backup) 
{
	if (!LightNode)
		return;
	if (Backup)
		AttachStack(NiNew GlobalLightNodeChange(this, NiSmartPointerCast(NiNode,m_spLightArea->GetAt(0)), LightNode));
	m_spLightArea->RemoveAllChildren();
	m_spLightArea->AttachChild(LightNode);
	_SHMD->SetGlobalLightNode(LightNode);
}
void IngameWorld::SetFarFrustum(float Frustum, bool Backup)
{
	if (Backup)
		AttachStack(NiNew ChangeFarFrustum(this, m_kWorldFrustum.m_fFar, Frustum));
	m_kWorldFrustum.m_fFar = Frustum;
	_SHMD->SetFarFrustum(Frustum);
}
void IngameWorld::SetDirectionalLightAmbientColor(NiColor kColor, bool Backup)
{
	if (Backup)
		AttachStack(NiNew ChangeDirectionalAmbientLightColor(this, _SHMD->GetDirectionalLightAmbientColor(), kColor));
	m_spMapDirectionalLight->SetAmbientColor(kColor);
	_SHMD->SetDirectionalLightAmbientColor(kColor);

}
void IngameWorld::SetDirectionalLightDiffuseColor(NiColor kColor, bool Backup)
{
	if (Backup)
		AttachStack(NiNew ChangeDirectionalDiffuseLightColor(this, _SHMD->GetDirectionalLightDiffuseColor(), kColor));
	m_spMapDirectionalLight->SetDiffuseColor(kColor);
	_SHMD->SetDirectionalLightDiffuseColor(kColor);
}
void IngameWorld::SetBackgroundColor(NiColor kColor, bool Backup)
{
	if (Backup)
		AttachStack(NiNew ChangeBackgroundColor(this, _SHMD->GetBackgroundColor(), kColor));
	_SHMD->SetBackgroundColor(kColor);
}
void IngameWorld::SetGlobalLight(NiColor kColor, bool Backup)
{
	if (Backup)
		AttachStack(NiNew ChangeGlobalLightColor(this, _SHMD->GetGlobalLight(), kColor));
	m_spAmbientLight->SetAmbientColor(kColor);
	_SHMD->SetGlobalLight(kColor);
} 
NiPoint3 IngameWorld::GetWorldPoint() 
{
	NiPoint3 kOrigin, kDir;
	auto Point = FiestaOnlineTool::CurrentMousePosition();
	if (m_spCamera->WindowPointToRay(Point.x, Point.y, kOrigin, kDir))
	{
		NiPick _Pick;
		_Pick.SetPickType(NiPick::FIND_FIRST);
		_Pick.SetSortType(NiPick::SORT);
		_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
		_Pick.SetFrontOnly(true);
		_Pick.SetReturnNormal(true);
		_Pick.SetObserveAppCullFlag(true);
		_Pick.SetTarget(m_spGroundScene);
		if (_Pick.PickObjects(kOrigin, kDir, true))
		{
			NiPick::Results& results = _Pick.GetResults();
			if (results.GetSize() > 0)
				return results.GetAt(0)->GetIntersection();
		}
	}
}
NiPickablePtr IngameWorld::PickObject() 
{
	NiPoint3 kOrigin, kDir;
	auto Point = FiestaOnlineTool::CurrentMousePosition();
	if (m_spCamera->WindowPointToRay(Point.x, Point.y, kOrigin, kDir))
	{
		NiPick _Pick;
		_Pick.SetPickType(NiPick::FIND_FIRST);
		_Pick.SetSortType(NiPick::SORT);
		_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
		_Pick.SetFrontOnly(true);
		_Pick.SetReturnNormal(true);
		_Pick.SetObserveAppCullFlag(true);
		_Pick.SetTarget(GetWorldNode());
		if (_Pick.PickObjects(kOrigin, kDir, true))
		{
			NiPick::Results& results = _Pick.GetResults();

			std::set<NiPickable*> PossibleObjects;

			for (int i = 0; i < results.GetSize(); i++)
			{
				auto record = results.GetAt(i);
				auto obj = record->GetAVObject();
				if (NiIsKindOf(NiPickable, obj))
				{
					PossibleObjects.insert((NiPickable*)obj);
				}
				else
				{
					//We only earch Upwards, 
					//if we search Downwards we can click the terrain and select the first 
					//NiPickable which must not be the actually targeted Node
					//Picking only works when compiled with the define of PICKABLEOBJECTS being true
					while (obj = obj->GetParent())
					{
						if (NiIsKindOf(NiPickable, obj))
						{
							PossibleObjects.insert((NiPickable*)obj);
							break;
						}
					}
				}
			}
			if (PossibleObjects.size() != 0)
			{
				return *PossibleObjects.begin();

			}
			return NULL;
		}
	}
	return NULL;
}
void IngameWorld::UpdatePos(std::vector<NiPickablePtr> Node, NiPoint3 PosChange, bool Backup)
{
	if (Backup)
		AttachStack(NiNew UpdateNodePos(this, Node, PosChange));

	for (auto obj : Node)
	{
		NiPoint3 NewPos = obj->GetTranslate() + PosChange;
		if (NiIsKindOf(ShineObjectNode, obj))
		{
			UpdateZCoord(NewPos);
			ShineObjectNodePtr mob = NiSmartPointerCast(ShineObjectNode, obj);
			mob->UpdatePos(NewPos);
		}else
			obj->SetTranslate(NewPos);
	}
}
void IngameWorld::UpdateRotation(std::vector<NiPickablePtr> Node, glm::vec3 RotationChange, bool Backup)
{
	if (Backup)
		AttachStack(NiNew UpdateNodeRotation(this, Node, RotationChange));
	
	for (auto obj : Node)
	{
		float angle, x, y, z;
		obj->GetRotate().ExtractAngleAndAxis(angle, x, y, z);
		glm::vec3 SelectedObjAngels = glm::degrees(glm::vec3{ eulerAngles(glm::angleAxis((float)angle, glm::vec3(x, y, z))) });
		SelectedObjAngels -= RotationChange;

		if (abs(SelectedObjAngels[0]) > 180.f)
		{
			SelectedObjAngels[0] = -SelectedObjAngels[0] + 2 * fmod(SelectedObjAngels[0], 180.f);
		}

		if (abs(SelectedObjAngels[1]) > 180.f)
		{
			SelectedObjAngels[1] = -SelectedObjAngels[1] + 2 * fmod(SelectedObjAngels[1], 180.f);
		}

		if (abs(SelectedObjAngels[2]) > 180.f)
		{
			SelectedObjAngels[2] = -SelectedObjAngels[2] + 2 * fmod(SelectedObjAngels[2], 180.f);
		}

		glm::vec4 angleAxis = ConvertQuatToAngleAxis(glm::quat(glm::radians(SelectedObjAngels)));

		NiMatrix3 m;
		m.MakeRotation(angleAxis[0], angleAxis[1], angleAxis[2], angleAxis[3]);

		if (NiIsKindOf(ShineObjectNode, obj))
		{
			ShineObjectNodePtr mob = NiSmartPointerCast(ShineObjectNode, obj);
			mob->UpdateRotation(angleAxis[3] * angleAxis[0] * 180.0 / NI_PI );
		}else
			obj->SetRotate(m);
	}
}
void IngameWorld::RemoveSky(NiNodePtr Node, bool Backup)
{
	if (Backup)
		AttachStack(NiNew AttachingNode(this, Node, &IngameWorld::RemoveSky, &IngameWorld::AddSky));
	_SHMD->RemoveSky(Node); m_spSkyScene->DetachChild(Node); m_spSkyScene->CompactChildArray();
	m_spSkyScene->UpdateProperties();
	m_spSkyScene->UpdateEffects();
	m_spSkyScene->Update(0.f);
}
void IngameWorld::RemoveWater(NiNodePtr Node, bool Backup)
{
	if (Backup)
		AttachStack(NiNew AttachingNode(this, Node, &IngameWorld::RemoveWater, &IngameWorld::AddWater));
	_SHMD->RemoveWater(Node); m_spWaterScene->DetachChild(Node); m_spWaterScene->CompactChildArray();
	m_spWaterScene->UpdateProperties();
	m_spWaterScene->UpdateEffects();
	m_spWaterScene->Update(0.f);
}
void IngameWorld::RemoveGroundObject(NiNodePtr Node, bool Backup)
{
	if (Backup)
		AttachStack(NiNew AttachingNode(this, Node, &IngameWorld::RemoveGroundObject, &IngameWorld::AddGroundObject));
	_SHMD->RemoveGroundObject(Node); m_spGroundObject->DetachChild(Node); m_spGroundObject->CompactChildArray();
	m_spGroundObject->UpdateProperties();
	m_spGroundObject->UpdateEffects();
	m_spGroundObject->Update(0.f);
}
void IngameWorld::AddSky(NiNodePtr Node, bool Backup)
{
	if (Backup)
		AttachStack(NiNew AttachingNode(this, Node, &IngameWorld::AddSky, &IngameWorld::RemoveSky));
	_SHMD->AddSky(Node); m_spSkyScene->AttachChild(Node);
	m_spSkyScene->UpdateProperties();
	m_spSkyScene->UpdateEffects();
	m_spSkyScene->Update(0.f);
}
void IngameWorld::AddWater(NiNodePtr Node, bool Backup)
{
	if (Backup)
		AttachStack(NiNew AttachingNode(this, Node, &IngameWorld::AddWater, &IngameWorld::RemoveWater));
	_SHMD->AddWater(Node); m_spWaterScene->AttachChild(Node);
	m_spWaterScene->UpdateProperties();
	m_spWaterScene->UpdateEffects();
	m_spWaterScene->Update(0.f);
}
void IngameWorld::AddGroundObject(NiNodePtr Node, bool Backup)
{
	if (Backup)
		AttachStack(NiNew AttachingNode(this, Node, &IngameWorld::AddGroundObject, &IngameWorld::RemoveGroundObject));
	_SHMD->AddGroundObject(Node); m_spGroundObject->AttachChild(Node);
	m_spGroundObject->UpdateProperties();
	m_spGroundObject->UpdateEffects();
	m_spGroundObject->Update(0.f);
}
void IngameWorld::RemoveObject(std::vector<NiPickablePtr> objs, bool Backup) 
{
	if (Backup)
		AttachStack(NiNew AttachingNodeList(this, objs, &IngameWorld::RemoveObject, &IngameWorld::AddObject));
	for (auto obj : objs) 
	{
		m_spGroundObject->DetachChild(obj);
		_SHMD->RemoveObject(NiSmartPointerCast(NiNode, obj));
	}
	m_spGroundObject->CompactChildArray();
	m_spGroundObject->UpdateProperties();
	m_spGroundObject->UpdateEffects();
	m_spGroundObject->Update(0.f);
}
void IngameWorld::AddObject(std::vector<NiPickablePtr> objs, bool Backup)
{
	if (Backup)
		AttachStack(NiNew AttachingNodeList(this, objs, &IngameWorld::AddObject, &IngameWorld::RemoveObject));
	for (auto obj : objs)
	{
		m_spGroundObject->AttachChild(obj);
		_SHMD->AddObject(NiSmartPointerCast(NiNode, obj));
	}
	m_spGroundObject->CompactChildArray();
	m_spGroundObject->UpdateProperties();
	m_spGroundObject->UpdateEffects();
	m_spGroundObject->Update(0.f);
}
void IngameWorld::AddShineObject(ShineObjectNodePtr obj, bool Backup) 
{
	if (Backup) {
		LogError("No ShineObject Backup added yet");
	}
	m_spGroundObject->AttachChild(obj);
	m_spGroundObject->CompactChildArray();
	m_spGroundObject->UpdateProperties();
	m_spGroundObject->UpdateEffects();
	m_spGroundObject->Update(0.f);
}
void IngameWorld::UpdateScale(std::vector<NiPickablePtr> Node, float Scale, bool Backup) 
{
	if (Backup)
		AttachStack(NiNew UpdateNodeScale(this, Node, Scale));
	for (auto obj : Node)
	{
		float OldScale = obj->GetScale();
		obj->SetScale(OldScale - Scale);
	}
}
void IngameWorld::ShowSHMDElements(bool Show) 
{
	if (Show) 
	{
		m_spGroundScene->AttachChild(m_spGroundObject);
		m_spGroundScene->AttachChild(m_spWaterScene);
		m_spGroundScene->AttachChild(m_spLightArea);
	}
	else
	{
		m_spGroundScene->DetachChild(m_spGroundObject);
		m_spGroundScene->DetachChild(m_spWaterScene);
		m_spGroundScene->DetachChild(m_spLightArea);
	}
	m_spGroundScene->Update(0.f);
}
void IngameWorld::ReplaceObjects(std::vector<NiPickablePtr> OldNodes, std::vector<NiPickablePtr> NewNodes, bool Backup)
{
	RemoveObject(OldNodes, Backup);
	AddObject(NewNodes, Backup);
}
void IngameWorld::ShowTerrain(bool show)
{
	m_spGroundTerrain->RemoveAllChildren();
	if (show) 
	{
		CreateAndAttachTerrain();
	}
	else 
	{
		auto layerlist = _INI->GetLayers();
		if(layerlist.size() == 0)
		{
			LogError("No TerrainLayers exist!");
			return;
		}
		CreateTerrainLayer(layerlist.at(0));
		
	}

	m_spGroundTerrain->Update(0.f);
	m_spGroundTerrain->UpdateProperties();
	m_spGroundTerrain->UpdateEffects();
	m_spGroundTerrain->Update(0.f);
	_ShowTerrain = show;

}
void IngameWorld::SetHTD(int w, int h, float level) 
{
	
	if (w >= _INI->GetMapWidth() || h >= _INI->GetMapHeight() || w < 0 || h < 0)
		return;
	auto htdlevel = _HTD->GetHTD(w, h);
	auto htdglevel = _HTD->GetHTDG(w, h);
	_HTD->SetHTDG(w, h, level - htdlevel);

	int XBlocks = (_INI->GetMapWidth() - 1) / _INI->GetQuadsWide();
	int YBlocks = (_INI->GetMapHeight() - 1) / _INI->GetQuadsHigh();

	if (_INI->GetLayers().size() * XBlocks * YBlocks != m_spGroundTerrain->GetChildCount())
	{
		LogError("Count Missmatchter for Layers and Terrain-Childs");
		LogError("This can be caused if you have have offsets for textures like Adelia!!");
		return;
	}

	int SubLayerW = w / _INI->GetQuadsWide();
	int SubLayerH = h / _INI->GetQuadsHigh();

	int SubW = w % (_INI->GetQuadsWide());
	int SubH = h % (_INI->GetQuadsHigh());

	int CurWith = _INI->GetQuadsWide() + 1;

	if (NiIsKindOf(NiNode, m_spGroundTerrain) && m_spGroundTerrain)
	{
		NiNodePtr Layer = NiSmartPointerCast(NiNode, m_spGroundTerrain);
		struct POS
		{
			NiAVObjectPtr obj;
			int w;
			int h;
		};
		std::vector<POS> LayerList;
		for (int i = 0; i < _INI->GetLayers().size(); i++)
		{
			if (SubW == 0 && SubLayerW >= 1)
			{
				LayerList.push_back({ Layer->GetAt(SubLayerH + (SubLayerW - 1) * YBlocks + i * YBlocks * XBlocks), _INI->GetQuadsWide(), SubH });
			}
			if (SubH == 0 && SubLayerH >= 1)
			{
				LayerList.push_back({ Layer->GetAt(SubLayerH + SubLayerW * YBlocks - 1 + i * YBlocks * XBlocks), SubW, _INI->GetQuadsHigh() });
			}
			if (SubW == 0 && SubLayerW >= 1 && SubH == 0 && SubLayerH >= 1)
				LayerList.push_back({ Layer->GetAt(SubLayerH + (SubLayerW - 1) * YBlocks - 1 + i * YBlocks * XBlocks), _INI->GetQuadsWide(), _INI->GetQuadsHigh() });
			LayerList.push_back({ Layer->GetAt(SubLayerH + SubLayerW * YBlocks + i * YBlocks * XBlocks), SubW, SubH });
		}
		for (auto _Layer : LayerList)
		{
			if (!_Layer.obj)
				continue;
			if (NiIsKindOf(NiTriShape, _Layer.obj))
			{
				NiTriShapePtr shape = NiSmartPointerCast(NiTriShape, _Layer.obj);
				NiGeometryData* data = shape->GetModelData();

				NiPoint3* Points = shape->GetVertices();
				NiPoint3* RelevantPoint = NULL;
				NiColorA* col = shape->GetColors();
				NiColorA* RelevantCol = NULL;
				if (_Layer.h <= 1)
				{

					if ((_Layer.h == 1 && _Layer.w == 0) || (_Layer.h == 0 && (_Layer.w == 0 || _Layer.w == 1)))
					{
						if (_Layer.h == 0 && _Layer.w == 0)
						{
							RelevantPoint = &Points[0];
							RelevantCol = &col[0];
						}
						if (_Layer.h == 0 && _Layer.w == 1)
						{
							RelevantPoint = &Points[1];
							RelevantCol = &col[1];
						}
						if (_Layer.h == 1 && _Layer.w == 0)
						{
							RelevantPoint = &Points[2];
							RelevantCol = &col[2];
						}
					}
					else
					{
						if (_Layer.h == 1)
						{
							RelevantPoint = &Points[(_Layer.w + 1) * 2 - 1];
							RelevantCol = &col[(_Layer.w + 1) * 2 - 1];
						}
						if (_Layer.h == 0)
						{
							RelevantPoint = &Points[(_Layer.w) * 2];
							RelevantCol = &col[(_Layer.w) * 2];
						}
					}

				}
				else
				{
					RelevantPoint = &Points[_Layer.w + _Layer.h * CurWith];
					RelevantCol = &col[_Layer.w + _Layer.h * CurWith];
				}

				RelevantPoint->z = level;
				data->MarkAsChanged(NiGeometryData::VERTEX_MASK);
			}
			else
			{
				LogError("Ah NiTriShape was expected and GetHTDPoints havent recieved one");
			}
		}
	}
	
		

	
}
float IngameWorld::GetHTD(int w, int h) 
{
	auto htdlevel = _HTD->GetHTD(w, h);
	auto htdglevel = _HTD->GetHTDG(w, h); 
	return htdlevel + htdglevel;
}

void IngameWorld::SetVertexColor(int w, int h, NiColorA Color)
{
	if (w < 0 || h < 0 || w >= _INI->GetMapWidth() || h >= _INI->GetMapHeight())
		return;
	_INI->SetColor(w, h, Color);
	std::vector<std::shared_ptr<TerrainLayerData>>& layers = _INI->GetLayers();
	for (int layerid = 0; layerid < layers.size(); layerid++)
	{
		auto QuadsHigh = _INI->GetQuadsHigh();
		auto QuadsWide = _INI->GetQuadsWide();

		auto BlockX = w / QuadsWide;
		auto InternalBlockX = w % QuadsWide;
		auto BlockXMax = (_INI->GetMapWidth() - 1) / _INI->GetQuadsWide();

		auto BlockY = h / QuadsHigh;
		auto InternalBlockY = h % QuadsHigh;
		auto BlockYMax = (_INI->GetMapHeight() - 1) / _INI->GetQuadsHigh();

		auto ShapeID = BlockY + BlockX * BlockYMax + layerid * BlockYMax * BlockXMax;
		if (ShapeID > m_spGroundTerrain->GetChildCount() || ShapeID < 0) 
		{
			return;
		}
		if (BlockY > 0 && InternalBlockY == 0)
		{
			SetVertexColorInternal(ShapeID - 1, InternalBlockX, _INI->GetQuadsHigh(), Color);
		}
		
		SetVertexColorInternal(ShapeID, InternalBlockX, InternalBlockY, Color);
	}
}

void IngameWorld::SetVertexColorInternal(int ShapeID, int InternalBlockX, int InternalBlockY, NiColorA Color)
{
	auto obj = m_spGroundTerrain->GetAt(ShapeID);
	if (NiIsKindOf(NiTriShape, obj))
	{
		NiTriShapePtr shape = NiSmartPointerCast(NiTriShape, obj);
		NiGeometryDataPtr data = shape->GetModelData();
		NiColorA* ColorArray = data->GetColors();

		if (InternalBlockY <= 1)
		{
			if (InternalBlockX == 0 && InternalBlockY == 0)
			{
				ColorArray[0] = Color;
				ColorArray[1] = Color;
				ColorArray[2] = Color;
				ColorArray[3] = Color;
			}
			if (InternalBlockX > 0)
			{
				ColorArray[3 + InternalBlockX * 2] = Color;
				ColorArray[3 + InternalBlockX * 2 - 1] = Color;
			}
		}
		else 
		{
			if (InternalBlockX == 0)
			{
				ColorArray[(_INI->GetQuadsWide() + 1) * InternalBlockY + InternalBlockX] = Color;
			}
			ColorArray[(_INI->GetQuadsWide() + 1) * InternalBlockY + InternalBlockX+1] = Color;

		}

		data->MarkAsChanged(NiGeometryData::COLOR_MASK);
	}
}
void IngameWorld::CreateShadows(NiColorA Color, NiColorA SunLight)
{
	if (!HasHTD())
		return;
	NiPoint3 MiddlePoint = NiPoint3(_INI->GetMapWidth() * _INI->GetOneBlockWidht() / 2,
		_INI->GetMapHeight() * _INI->GetOneBlockHeight() / 2, 
		_HTD->GetMiddle()) + NiPoint3(250.f,0.f,0.f);
	NiPoint3 SunVector = MiddlePoint - GetCamera()->GetTranslate();
	SunVector.Unitize();
	SunVector *= 25.f;

	std::vector<bool> Shadowed(_INI->GetMapWidth() * _INI->GetMapHeight(),false);
	std::vector<NiPoint3> Normals(_INI->GetMapWidth() * _INI->GetMapHeight(), NiPoint3::ZERO);
	const float strength = 8.0f;
	const float dZ = 1.0 / strength;

	float MinH = std::numeric_limits<float>::max();

	for (int w = 0; w < _INI->GetMapWidth(); w++)
	{
		for (int h = 0; h < _INI->GetMapHeight(); h++)
		{
			if (GetHTD(w, h) < MinH)
				MinH = GetHTD(w, h);
			float tl = GetHTD(w - 1, h - 1); // top left
			float  l = GetHTD(w - 1, h);   // left
			float bl = GetHTD(w - 1, h + 1); // bottom left
			float  t = GetHTD(w, h - 1);   // top
			float  b = GetHTD(w, h + 1);   // bottom
			float tr = GetHTD(w + 1, h - 1); // top right
			float  r = GetHTD(w + 1, h);   // right
			float br = GetHTD(w + 1, h + 1); // bottom right

			// sobel filter
			const float dX = (tr + 2.0 * r + br) - (tl + 2.0 * l + bl);
			const float dY = (bl + 2.0 * b + br) - (tl + 2.0 * t + tr);

			NiPoint3 vec(dX, dY, dZ);
			vec.Unitize();
			vec.x += 1.f;
			vec.y += 1.f;
			vec.z += 1.f;
			vec = vec / 2;
			Normals[w + h * _INI->GetMapWidth()] = vec;

		}
	}
	int RaysSent = 0;

	int wStart = (SunVector.x < 0.0f) ? _INI->GetMapWidth() - 1 : 0;
	int wEnd = (SunVector.x < 0.0f) ? -1 : _INI->GetMapWidth();
	int wStep = (SunVector.x < 0.0f) ? -1 : 1;

	int hStart = (SunVector.y < 0.0f) ? _INI->GetMapHeight() - 1 : 0;
	int hEnd = (SunVector.y < 0.0f) ? -1 : _INI->GetMapHeight();
	int hStep = (SunVector.y < 0.0f) ? -1 : 1;

	for (int w = wStart; w != wEnd; w += wStep)
	{
		for (int h = hStart; h != hEnd; h += hStep)
		{
			if (!Shadowed[w + h * _INI->GetMapWidth()])
			{
				NiPoint3 CurPoint(w * 50.f + 25.f, h * 50.f + 25.f, GetHTD(w, h)); //We Start at the middle of the Point to make shit smoother
				CalculateRay(CurPoint, SunVector, Shadowed, MinH);
				RaysSent++;
			}
		}
	}
	PgUtil::MakePositiveVector(SunVector);

	for (int w = 0; w < _INI->GetMapWidth(); w++)
	{
		for (int h = 0; h < _INI->GetMapHeight(); h++)
		{
			int OffSet = w + h * _INI->GetMapWidth();
			auto FinalColor = Color * (Normals[OffSet].z + 1) / 2; //AmbientLight
			if (!Shadowed[OffSet])
			{
				FinalColor = FinalColor + SunLight * Normals[OffSet].Dot(SunVector);
			}
			FinalColor.a = 1.f;
			PgUtil::FixColor(FinalColor);
			SetVertexColor(w, h, FinalColor); //Add SunLight if Not Shadowed

		}
	}
	for(int cycle = 0; cycle < 3; cycle++)
	{
		for (int w = 0; w < _INI->GetMapWidth(); w++)
		{
			for (int h = 0; h < _INI->GetMapHeight(); h++)
			{
				NiColorA Color = NiColorA::BLACK;
				for (int i = -2; i <= 2; i++)
				{
					for (int j = -2; j <= 2; j++)
					{
						Color = Color + GetVertexColor(w - i, h - j) * 1 / (5 * 5);
					}
				}
				SetVertexColor(w, h, Color);
			}
		}
	}
}
void IngameWorld::CalculateRay(NiPoint3& StartPoint, NiPoint3& SunVector, std::vector<bool>& Shadowed, float minh) 
{
	NiPoint3 ActivePoint = StartPoint;
	if (SunVector.x == 0.0f && SunVector.y == 0.0f)
		return;
	const float invGridSize = 1.0f / 50.f;
	int x = static_cast<int>(StartPoint.x * invGridSize);
	int y = static_cast<int>(StartPoint.y * invGridSize);
	float MinH = minh * 25.f;
	int prevx = x;
	int prevy = y;

	while (true)
	{
		if (x != prevx || y != prevy)
		{
			if (x >= _INI->GetMapWidth() || x < 0 || y < 0 || y >= _INI->GetMapHeight() || GetHTD(x,y) > ActivePoint.z || ActivePoint.z < MinH)
				return;

			Shadowed[x + y *_INI->GetMapWidth()] = true;
			prevx = x;
			prevy = y;
		}
		ActivePoint += SunVector;
		x = static_cast<int>(ActivePoint.x * invGridSize);
		y = static_cast<int>(ActivePoint.y * invGridSize);
	}
}
void IngameWorld::SaveVertex() 
{
	PgUtil::SaveTexture(PgUtil::PathFromClientFolder(_INI->GetVertexColor()), _INI->GetVertexImage());
}

void IngameWorld::LoadNPCS()
{
	std::vector<ShineNPCPtr> NPCS = NPCData::GetNPCsByMap(_MapInfo->MapName);
	for (auto npc : NPCS)
	{
		ShineNPCNodePtr mob = NiNew ShineNPCNode(npc);
		NiPoint3 pos = mob->GetPos();
		UpdateZCoord(pos);
		mob->UpdatePos(pos);
		
		mob->UpdateRotation(mob->GetRotation());
		if (NiIsKindOf(ShineObjectNode, mob))
			this->AddShineObject(NiSmartPointerCast(ShineObjectNode, mob), false);
	}
}
bool IngameWorld::UpdateZCoord(NiPoint3& Pos) 
{
	NiPoint3 Org = Pos;
	Org.z = -2500.;
	float z = -2500;

	NiPick _Pick;
	_Pick.SetPickType(NiPick::FIND_ALL);
	_Pick.SetSortType(NiPick::SORT);
	_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
	_Pick.SetFrontOnly(false);
	_Pick.SetObserveAppCullFlag(false);
	if (HasHTD()) 
	{
		_Pick.SetTarget(m_spGroundTerrain);
		if (_Pick.PickObjects(Org, NiPoint3(.0,.0,1.0), true))
		{
			NiPick::Results& results = _Pick.GetResults();
			for (int i = 0; i < results.GetSize(); i++) 
			{
				NiPick::Record* record = results.GetAt(0);
				if (!record)
					continue;
				if (z < record->GetIntersection().z) {
					z = record->GetIntersection().z;
				}
			}
		}
		_Pick.ClearResultsArray();
	}
	_Pick.SetTarget(m_spGroundCollidee);
	if (_Pick.PickObjects(Org, NiPoint3(.0, .0, 1.0), true))
	{
		NiPick::Results& results = _Pick.GetResults();
		for (int i = 0; i < results.GetSize(); i++)
		{
			NiPick::Record* record = results.GetAt(i);
			if (!record)
				continue;
			if (z < record->GetIntersection().z) {
				z = record->GetIntersection().z;
			}
		}
	}
	_Pick.ClearResultsArray();
	if (z != Pos.z)
	{
		Pos.z = z;
		return true;
	}
	return false;
}

NiPickablePtr IngameWorld::CreateNewNPC()
{
	NiPoint3 pos = GetWorldPoint();
	UpdateZCoord(pos);
	ShineNPCPtr npc = NPCData::CreateNewNPC(this->_MapInfo, pos);
	ShineNPCNodePtr mob = NiNew ShineNPCNode(npc);
	mob->UpdatePos(pos);
	mob->UpdateRotation(mob->GetRotation());
	if (NiIsKindOf(ShineObjectNode, mob))
		this->AddShineObject(NiSmartPointerCast(ShineObjectNode, mob), false);
	if (NiIsKindOf(NiPickable, mob))
		return NiSmartPointerCast(NiPickable, mob);
	else
		return nullptr;
}