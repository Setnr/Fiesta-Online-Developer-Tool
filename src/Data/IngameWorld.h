#pragma once
#include <NiMain.h>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <mutex>
#include "PgUtil.h"

#include "../Data/SHNStruct.h"
#include "../Data/IniFile.h"
#include "../Data/SHBD/SHBD.h"

NiSmartPointer(World);
class World : public NiRefObject
{
public:
	World(MapInfo* MapInfo);
	~World() 
	{
		
	};

private:

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
public:
#pragma region Init
	bool InitScene();
	bool InitCamera();
	bool InitSkyCtrl();
	bool InitLightFog();
	bool InitShadow() { return true; }

	bool LoadTerrain();
	bool LoadSHMD();
	bool LoadSHBD();
#pragma endregion
#pragma region SetterAndGetter
	void SetBackgroundColor(float r, float g, float b)
	{
		BackgroundColor = NiColor(r, g, b);
	}
	NiColor& GetBackgroundColor() {
		return BackgroundColor;
	}

	void AttachSky(NiNodePtr sky)
	{
		m_spSkyScene->AttachChild(sky,1);
		m_spSkyScene->UpdateProperties();
		m_spSkyScene->UpdateEffects();
		m_spSkyScene->Update(0.0);
	}
	void AttachWater(NiNodePtr water)
	{
		m_spWaterScene->AttachChild(water, 1);
	}
	void AttachGroundObj(NiNodePtr& obj)
	{
		static std::mutex AttachGroundObjLock;
		std::lock_guard<std::mutex> lock(AttachGroundObjLock);
		m_spGroundObject->AttachChild(obj, 1);
	}
	void AttachGroundCollidee(NiNodePtr obj)
	{
		m_spGroundObjectCollidee->AttachChild(obj, 1);
	}
	NiNodePtr GetGroundCollidee()
	{
		return m_spGroundObjectCollidee;
	}
	void AttachGroundTerrain(NiTriShapePtr obj)
	{
		m_spGroundTerrain->AttachChild(obj, 1);
	}
	NiNodePtr GetWorldScene() {
		return m_spWorldScene;
	}
	NiSortAdjustNodePtr GetTerrainScene() {
		return m_spGroundTerrain;
	}
	NiNodePtr GetSkyNode() {
		return m_spSkyScene;
	}
	NiNodePtr GetWaterNode() 
	{
		return m_spWaterScene;
	}
	NiNodePtr GetGroundObjNode() {
		return m_spGroundObject;
	}
	void SetFogColor(NiColor kColor) 
	{
		NiFogProperty* fog = (NiFogProperty*) m_spWorldScene->GetProperty(NiProperty::FOG);
		fog->SetFogColor(kColor);
		fog->SetFog(true);
	}
	const NiColor& GetFogColor()
	{
		NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
		return fog->GetFogColor();
	}
	float GetFogDepth() 
	{
		NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
		return fog->GetDepth();
	}
	void SetFogDepth(float fDepth)
	{
		NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
		if (fDepth < 0.0)
			fDepth = 0.0;
		fog->SetDepth(fDepth);
	}
	void SetGlobalLight(NiNodePtr LightNode) {
		this->m_spLightArea->AttachChild(LightNode);
	}
	void SetFarFrumstum(float Frustum)	
	{
		m_kWorldFrustum.m_fFar = Frustum;
	}
	NiFrustum& GetSkyFrustum() {
		return m_kSkyFrustum;
	}
	NiFrustum& GetWorldFrustum() {
		return m_kWorldFrustum;
	}
	void SetMapDirectionalLightAmbientColor(NiColor kColor)
	{
		m_spMapDirectionalLight->SetAmbientColor(kColor);
	}
	const NiColor& GetMapDirectionalLightAmbientColor()
	{
		return m_spMapDirectionalLight->GetAmbientColor();
	}
	void SetMapDirectionalLightDiffuseColor(NiColor kColor)
	{
		m_spMapDirectionalLight->SetDiffuseColor(kColor);
	}
	const NiColor& GetMapDirectionalLightDiffuseColor()
	{
		return m_spMapDirectionalLight->GetDiffuseColor();
	}
	void SetAmbientLightAmbientColor(NiColor kColor)
	{
		m_spAmbientLight->SetAmbientColor(kColor);
	}
	const NiColor& GetAmbientLightAmbientColor()
	{
		return m_spAmbientLight->GetAmbientColor();
	}
	NiCameraPtr GetCamera() { return m_spCamera; }

	float GetFOV() { return this->m_fCameraFOV; }

	bool WasLoadedSuccessfully() { return LoadedSuccessfully; }
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
#pragma endregion
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
	bool LoadedSuccessfully = false;
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
