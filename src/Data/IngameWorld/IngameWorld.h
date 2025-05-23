#pragma once
#include <SHN/SHNStruct.h>
#include <NiMain.h>
#include <HeightTerrainData/HeightTerrainData.h>
#include <ShineIni/ShineIni.h>
#include <ShineBlockData/ShineBlockData.h>
#include <ShineMapData/ShineMapData.h>
#include <deque>
#include <Data/IngameWorld/WorldChanges/WorldChanges.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "ShineObject/ShineObject.h"
NiSmartPointer(IngameWorld);

class IngameWorld : public NiRefObject
{
public:
	enum WorldIntersectType {
		HTD, 
		GroundCollision,
		GroundScene
	};
	IngameWorld(MapInfo* Info);
	IngameWorld(MapInfo* Info, int MapSize);
	NiNodePtr GetWorldNode() { return m_spWorldScene; }
	NiCameraPtr GetCamera() { return m_spCamera; }

	void SetFogColor(NiColor kColor, bool Backup = true);
	void SetFogDepth(float depth, bool Backup = true);
	void SetGlobalLightNode(NiNodePtr LightNode, bool Backup = true);
	void SetFarFrustum(float Frustum, bool Backup = true);
	void SetDirectionalLightAmbientColor(NiColor kColor, bool Backup = true);
	void SetDirectionalLightDiffuseColor(NiColor kColor, bool Backup = true);
	void SetBackgroundColor(NiColor kColor, bool Backup = true);
	void SetGlobalLight(NiColor kColor, bool Backup = true);
	std::vector<NiNodePtr>& GetSkyNodeList() { return _SHMD->GetSkyList(); }
	std::vector<NiNodePtr>& GetWaterNodeList() { return _SHMD->GetWaterList(); }
	std::vector<NiNodePtr>& GetGlobalGroundObjectList() { return _SHMD->GetGlobalGroundObjectList(); }
	std::set<NiNodePtr>& GetGroundObjects() { return _SHMD->GetGroundObjectListList(); }
	void RemoveSky(NiNodePtr Node, bool Backup = true);
	void RemoveWater(NiNodePtr Node, bool Backup = true);
	void RemoveGroundObject(NiNodePtr Node, bool Backup = true);
	void AddSky(NiNodePtr Node, bool Backup = true);
	void AddWater(NiNodePtr Node, bool Backup = true);
	void AddGroundObject(NiNodePtr Node, bool Backup = true);
	void RemoveObject(std::vector<NiPickablePtr>, bool Backup = true);
	void AddObject(std::vector<NiPickablePtr>, bool Backup = true);
	NiFrustum GetSkyFrustum() { return m_kSkyFrustum; }
	NiFrustum GetWorldFrustum() { return m_kWorldFrustum; }
	NiColor GetBackgroundColor() { return _SHMD->GetBackgroundColor(); }
	NiColor GetDirectionalLightAmbientColor() { return _SHMD->GetDirectionalLightAmbientColor(); }
	NiColor GetGlobalLight() { return _SHMD->GetGlobalLight(); }
	NiColor GetFogColor() { return _SHMD->GetFogColor(); }
	float GetFogDepth() { return _SHMD->GetFogDepth(); }
	float GetFarFrustum() { return _SHMD->GetFarFrustum(); }
	NiColor GetDirectionalLightDiffuseColor() { return _SHMD->GetDirectionalLightDiffuseColor(); }
	void SaveSHMD() { 
		if (_SHMD)
			if (!_SHMD->Save(_MapInfo))
				LogError("Failed to save SHMD");
	}
	void SaveSHBD() 
	{ 
		std::string Path = PgUtil::PathFromClientFolder(
			PgUtil::GetMapFolderPath(_MapInfo->KingdomMap, _MapInfo->MapFolderName)
			+ _MapInfo->MapFolderName + ".shbd");
			
		if (_SHBD)
			if (!_SHBD->Save(Path))
				LogError("Failed to save SHBD");
	}
	void SaveHTD() 
	{
		std::string Path = PgUtil::PathFromClientFolder(
			PgUtil::GetMapFolderPath(_MapInfo->KingdomMap, _MapInfo->MapFolderName)
			+ _MapInfo->MapFolderName + ".htd");
		if(_HTD)
			if (!_HTD->Save(Path)) 
				LogError("Failed to save HTD");
	}
	void SaveIni() 
	{
		std::string Path = PgUtil::PathFromClientFolder(
			PgUtil::GetMapFolderPath(_MapInfo->KingdomMap, _MapInfo->MapFolderName)
			+ _MapInfo->MapFolderName + ".ini");
		if (_INI)
			if (!_INI->Save(Path))
				LogError("Failed to save INI");
	}
	void Undo()
	{
		if (UnDoStack.empty())
			return;
		auto revert = UnDoStack.back();
		revert->Undo();
		ReDoStack.push_back(revert);
		UnDoStack.pop_back();
	}
	void Redo() 
	{
		if (ReDoStack.empty())
			return;
		auto redo = ReDoStack.back();
		redo->Redo();
		UnDoStack.push_back(redo);
		ReDoStack.pop_back();
	}
	void ClearStacks()
	{
		if(!UnDoStack.empty())
			UnDoStack.clear();
		if (!ReDoStack.empty())
			ReDoStack.clear();
	}
	NiPickablePtr PickObject();
	void UpdatePos(std::vector<NiPickablePtr> Node, NiPoint3 PosChange, bool Backup = true);
	void UpdateRotation(std::vector<NiPickablePtr> Node, glm::vec3 RotationChange, bool Backup = true);
	void UpdateScale(std::vector<NiPickablePtr> Node, float Scale, bool Backup = true);
	float GetFOV() { return m_fCameraFOV; }
	ShineBlockDataPtr GetSHBD() { return _SHBD; }
	ShineIniPtr GetShineIni() { return _INI; }
	NiPoint3 GetWorldPoint(WorldIntersectType point = WorldIntersectType::GroundScene);
	void ReplaceObjects(std::vector<NiPickablePtr> OldNodes, std::vector<NiPickablePtr> NewNodes, bool Backup = true);
	void ShowSHMDElements(bool Show);
	void UpdateSHBDData(std::vector<char> newData) 
	{
		_SHBD->UpdateSHBDData(newData);
	}
	void UpdateCollide() { m_spGroundCollidee->Update(0.0); }
	NiNodePtr GetCollide() { return m_spGroundCollidee; }
	void AttachStack(WorldChangePtr change)
	{
		if (UnDoStack.size() == 0)
		{
			UnDoStack.push_back(change);
			ReDoStack.clear();
			return;
		}
		auto topobject = UnDoStack.back();
		if (!change->IsKindOf(topobject->GetRTTI()) || topobject->ExtraCheck(change))
		{
			if (UnDoStack.size() > 25)
				UnDoStack.pop_front();
			UnDoStack.push_back(change);
			ReDoStack.clear();
		}
		else
			topobject->Update(change);
	}
	void ShowTerrain(bool show);
	NiSortAdjustNodePtr GetTerrainNode() { return m_spGroundTerrain; }
	bool HasHTD() { return _HTD; }
	HeightTerrainDataPtr GetHTD() { return _HTD; }
	void SetHTD(int w, int h, float level);
	float GetHTD(int w, int h);
	void CreateAndAttachTerrain();
	void SetHTD(HeightTerrainData data) 
	{
		*_HTD = data;
	}
	bool GetShowTerrain() { return _ShowTerrain; }
	MapInfo* GetMapInfo() { return _MapInfo; }
	void UpdateTerrain() {

		m_spGroundTerrain->Update(0.f);
		m_spGroundTerrain->UpdateProperties();
		m_spGroundTerrain->UpdateEffects();
		m_spGroundTerrain->Update(0.f);
	}
	void SetVertexColor(int w, int h, NiColorA Color);
	NiColorA GetVertexColor(int w, int h) {
		return _INI->GetColor(w, h);
	}
	void CreateShadows(NiColorA Color, NiColorA SunLight);
	void CalculateRay(NiPoint3& StartPoint, NiPoint3& SunVector, std::vector<bool>& Shadowed, float minh);
	void SaveVertex();
	std::vector<ShineObjectPtr> LoadNPCS();
	void AddShineObject(ShineObjectPtr obj, bool Update = true);
	void RemoveShineObject(ShineObjectPtr obj, bool Backup = true);
	bool UpdateZCoord(NiPoint3& Pos);
	NiPickablePtr CreateNewNPC();
	void GateSpawnPoints(bool show);

private:

	void SetVertexColorInternal(int ShapeID, int InternalBlockX, int InternalBlockY, NiColorA Color);
	ShineBlockDataPtr _SHBD;
	ShineMapDataPtr _SHMD;
	ShineIniPtr _INI;
	HeightTerrainDataPtr _HTD;

	MapInfo* _MapInfo;

	bool InitScene();
	bool InitCamera();
	bool InitSkyCtrl();
	bool InitLightFog();
	bool InitShadow();

	bool LoadHTD();
	void CreateTerrainLayer(std::shared_ptr<TerrainLayerData> CurrentLayer);

	bool _ShowTerrain = true;
	std::deque<WorldChangePtr>UnDoStack;
	std::deque<WorldChangePtr>ReDoStack;

	std::vector<ShineObjectPtr> _GateSpawnPoints;

#pragma region WorldStructureNodes
	NiNodePtr m_spLightArea;
	NiNodePtr m_spFog;
	NiNodePtr m_spWorldScene;
	NiNodePtr m_spCharScene;
	NiNodePtr m_spDropItemScene;
	NiNodePtr m_spGroundScene;
	NiNodePtr m_spShadowScene;
	NiNodePtr m_spGroundObject;
	NiNodePtr m_spDirectionalLightScene;
	NiNodePtr m_spMinihouseScene;
	NiNodePtr m_spNormalLightScene;
	NiNodePtr m_spAbstateCharScene;
	NiNodePtr m_spWeatherEffectScene;
	NiSortAdjustNodePtr m_spGroundTerrain;
	NiNodePtr m_spAllGroundScene;
	NiSortAdjustNodePtr m_spBuildingScene;
	NiNodePtr m_spBDAni;
	NiNodePtr m_spBDNotAni;
	NiNodePtr m_spSkyScene;
	NiNodePtr m_spWaterScene;
	NiNodePtr m_spEffectScene;
	NiNodePtr m_spGroundCollidee;
	NiNodePtr m_spGroundObjectCollidee;
	NiNodePtr m_spBuildingCollidee;
	NiNodePtr m_spUseCameraCollidee;
	NiCameraPtr m_spCamera;
	float m_fCameraFOV;
	bool m_bCameraOrtho;
	NiFrustum m_kSkyFrustum;
	NiFrustum m_kWorldFrustum;
	NiAmbientLightPtr m_spAmbientLight;
	NiDirectionalLightPtr m_spDirectionalLight;
	NiDirectionalLightPtr m_spMapDirectionalLight;

	NiColor BackgroundColor;
#pragma endregion
};