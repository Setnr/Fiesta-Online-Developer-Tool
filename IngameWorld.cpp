#include "IngameWorld.h"

NiPoint3 World::ms_kUpDir = NiPoint3(0.0, 0.0, 1.0); 
NiPoint3 World::ms_kDownDir = NiPoint3(0.0, 0.0, -1.0); 
NiPoint3 World::ms_kNorthDir = NiPoint3(0.0, 1.0, 0.0); 
NiPoint3 World::ms_kEastDir = NiPoint3(1.0, 0.0, 0.0); 
NiPoint3 World::ms_kWestDir = NiPoint3(-1.0, 0.0, 0.0); 
NiPoint3 World::ms_kDefaultDirectionalLightDir = NiPoint3(0.5, 0.69999999, -0.40000001); 

bool World::InitScene() 
{
	m_spLightArea = NiNew NiNode;
	m_spFog = NiNew NiNode;
	m_spWorldScene = NiNew NiNode;
	m_spCharScene = NiNew NiNode;
	m_spDropItemScene = NiNew NiNode;
	m_spGroundScene = NiNew NiNode;
	m_spShadowScene = NiNew NiNode;;
	m_spGroundObject = NiNew NiNode;
	m_spDirectionalLightScene = NiNew NiNode;
	m_spMinihouseScene = NiNew NiNode;
	m_spNormalLightScene = NiNew NiNode;
	m_spAbstateCharScene = NiNew NiNode;
	m_spWeatherEffectScene = NiNew NiNode;
	m_spGroundTerrain = NiNew NiSortAdjustNode;
	m_spAllGroundScene = NiNew NiNode;
	m_spBuildingScene = NiNew NiSortAdjustNode;
	m_spBDAni = NiNew NiNode;
	m_spBDNotAni = NiNew NiNode;
	m_spSkyScene = NiNew NiNode;
	m_spWaterScene = NiNew NiNode;
	m_spEffectScene = NiNew NiNode;
	m_spGroundCollidee = NiNew NiNode;
	m_spGroundObjectCollidee = NiNew NiNode;
	m_spBuildingCollidee = NiNew NiNode;
	m_spUseCameraCollidee = NiNew NiNode;



	m_spLightArea->SetName("m_spLightArea");
	m_spFog->SetName("m_spFog");
	m_spWorldScene->SetName("m_spWorldScene");
	m_spCharScene->SetName("m_spCharScene");
	m_spDropItemScene->SetName("m_spDropItemScene");
	m_spGroundScene->SetName("m_spGroundScene");
	m_spShadowScene->SetName("m_spShadowScene");
	m_spGroundObject->SetName("m_spGroundObject");
	m_spDirectionalLightScene->SetName("m_spDirectionalLightScene");
	m_spMinihouseScene->SetName("m_spMinihouseScene");
	m_spNormalLightScene->SetName("m_spNormalLightScene");
	m_spAbstateCharScene->SetName("m_spAbstateCharScene");
	m_spWeatherEffectScene->SetName("m_spWeatherEffectScene");
	m_spGroundTerrain->SetName("m_spGroundTerrain");
	m_spAllGroundScene->SetName("m_spAllGroundScene");
	m_spBuildingScene->SetName("m_spBuildingScene");
	m_spSkyScene->SetName("m_spSkyScene");
	m_spWaterScene->SetName("m_spWaterScene");
	m_spGroundCollidee->SetName("m_spGroundCollidee");
	m_spGroundObjectCollidee->SetName("m_spGroundObjectCollidee");
	m_spBuildingCollidee->SetName("m_spBuildingCollidee");


	m_spBuildingScene->SetName("[PN]");
	m_spBuildingScene->AttachChild(m_spGroundObject);
	m_spBuildingScene->AttachChild(m_spBDNotAni);
	m_spBuildingScene->AttachChild(m_spBDAni);
	m_spGroundTerrain->AttachChild(m_spGroundObject);
	m_spGroundObject->AttachChild(m_spGroundObjectCollidee);
	m_spGroundScene->AttachChild(m_spGroundTerrain);
	m_spAllGroundScene->AttachChild(m_spGroundScene);
	m_spAllGroundScene->AttachChild(m_spBuildingScene);
	m_spNormalLightScene->AttachChild(m_spAllGroundScene);
	m_spNormalLightScene->AttachChild(m_spDirectionalLightScene);
	m_spDirectionalLightScene->AttachChild(m_spCharScene);
	m_spNormalLightScene->AttachChild(m_spShadowScene);
	m_spNormalLightScene->AttachChild(m_spDropItemScene);
	m_spDirectionalLightScene->AttachChild(m_spMinihouseScene);
	m_spNormalLightScene->AttachChild(m_spWaterScene);
	m_spNormalLightScene->AttachChild(m_spEffectScene);
	m_spWorldScene->AttachChild(m_spNormalLightScene);
	m_spWorldScene->AttachChild(m_spAbstateCharScene);
	m_spWorldScene->AttachChild(m_spLightArea);
	m_spWorldScene->AttachChild(m_spFog);
	m_spShadowScene->AttachProperty(NiNew NiAlphaProperty);
	m_spShadowScene->AttachProperty(NiNew NiVertexColorProperty);
	m_spShadowScene->AttachProperty(NiNew NiFogProperty);
	m_spWorldScene->AttachProperty(NiNew NiVertexColorProperty);
	m_spWorldScene->AttachProperty(NiNew NiMaterialProperty);
	m_spWorldScene->AttachProperty(NiNew NiFogProperty);
	m_spSkyScene->AttachProperty(NiNew NiZBufferProperty);
	return true;
}
bool World::InitCamera() 
{
	m_spCamera = NiNew NiCamera;
	m_bCameraOrtho = false;
	m_spCamera->SetViewFrustum(NiFrustum(-0.25, 0.25, 0.1875, -0.1875, 1.0, 60000.0, m_bCameraOrtho)); //World::SetCameraFOV
	m_kCameraCtrl.InitDirection(ms_kUpDir, ms_kEastDir, ms_kNorthDir);
	m_kCameraCtrl.InitZoomMinMax(40.0, 600.0);
	m_kCameraCtrl.m_fAngleUDMax = 295.0;
	m_kCameraCtrl.m_fAngleUDMin = 89.0;
	m_kCameraCtrl.SetZoom(300.0);
	m_kCameraCtrl.AddRotate(0.0, 35.0);
	m_kCamOscillation.Clear();
	m_bControllerMode = 0;
	m_kLookAtDelta.x = 0.0;
	m_kLookAtDelta.y = 0.0;
	m_kLookAtDelta.z = 0.0;
	m_kCameraPosDelta.x = 0.0;
	m_kCameraPosDelta.y = 0.0;
	m_kCameraPosDelta.z = 0.0;
	m_dwOldTime = 0;
	m_fAngleUDDelta = -1.0;
	m_fAngleLRDelta = -1.0;
	m_fDist = 0.0;
	m_kLookAtPos = NiPoint3::ZERO;
	return true;
}
bool World::InitSkyCtrl()
{
	return m_kSkyCtrl.Init();
}
bool World::InitLightFog()
{
	m_spAmbientLight = NiNew NiAmbientLight;
	m_spAmbientLight->SetAmbientColor(NiColor(0.2, .2, .2));
	m_spAmbientLight->SetDiffuseColor(NiColor::WHITE);
	m_spAmbientLight->SetSpecularColor(NiColor::BLACK);
	m_spAmbientLight->AttachAffectedNode(m_spNormalLightScene);
	m_spNormalLightScene->AttachChild(m_spAmbientLight);
	m_spDirectionalLight = NiNew NiDirectionalLight;
	m_spDirectionalLight->SetDimmer(1.0);
	m_spDirectionalLight->SetAmbientColor(NiColor::BLACK);
	m_spDirectionalLight->SetDiffuseColor(NiColor::WHITE);
	m_spDirectionalLight->SetSpecularColor(NiColor::BLACK);
	m_spDirectionalLight->AttachAffectedNode(m_spDirectionalLightScene);
	m_spDirectionalLightScene->AttachChild(m_spDirectionalLight);
	m_spMapDirectionalLight = NiNew NiDirectionalLight;
	m_spMapDirectionalLight->SetAmbientColor(NiColor(.75, .75, .75));
	m_spMapDirectionalLight->SetDiffuseColor(NiColor::WHITE);
	m_spMapDirectionalLight->SetSpecularColor(NiColor::WHITE);
	return 1;
}