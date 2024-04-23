#pragma once
#include <NiMain.h>
#include <vector>
#include <set>
#include <map>
#include <list>
#include "PgUtil.h"

class World 
{
public:
	~World();
private:

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
	NiRenderer* m_pkRenderer;
	float m_fCameraFOV;
	bool m_bCameraOrtho;
	NiFrustum m_kSkyFrustum;
	NiFrustum m_kWorldFrustum;
	NiAmbientLightPtr m_spAmbientLight;
	NiDirectionalLightPtr m_spDirectionalLight;
	NiDirectionalLightPtr m_spMapDirectionalLight;

public:
	bool InitScene();
	bool InitCamera();
	bool InitSkyCtrl();
	bool InitLightFog();
	bool InitShadow() { return true; }

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
		m_spGroundObject->AttachChild(obj, 1);
	}
	void AttachGroundCollidee(NiNodePtr obj)
	{
		m_spGroundObjectCollidee->AttachChild(obj, 1);
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
	NiNodePtr GetGroundObjNode() {
		return m_spGroundObject;
	}
	void SetFogColor(NiColor kColor) 
	{
		NiFogProperty* fog = (NiFogProperty*) m_spWorldScene->GetProperty(NiProperty::FOG);
		fog->SetFogColor(kColor);
	}
	void SetFogDepth(float fDepth)
	{
		NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
		if (fDepth < 0.0)
			fDepth = 0.0;
		fog->SetDepth(fDepth);
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
	void SetMapDirectionalLightDiffuseColor(NiColor kColor)
	{
		m_spMapDirectionalLight->SetDiffuseColor(kColor);
	}
	void SetAmbientLightAmbientColor(NiColor kColor)
	{
		m_spAmbientLight->SetAmbientColor(kColor);
	}
	NiCameraPtr GetCamera() { return m_spCamera; }

	float GetFOV() { return this->m_fCameraFOV; }
	static NiPoint3 ms_kUpDir; // idb
	static NiPoint3 ms_kDownDir; // idb
	static NiPoint3 ms_kNorthDir; // idb
	static NiPoint3 ms_kEastDir; // idb
	static NiPoint3 ms_kWestDir; // idb
	static NiPoint3 ms_kDefaultDirectionalLightDir; // idb
};
