#pragma once
#include <NiMain.h>
#include <vector>
#include <set>
#include <map>
#include <list>
#include "PgUtil.h"
struct Oscillation
{
	float m_fCurrTime;
	float m_fNextPeakTime;
	float m_fPeriod;
	float m_fAmplitude;
	float m_fTargetAmplitude;
	float m_fValue;
	float m_fStartTime;
	float m_fPhase;
	float m_fDampingMagnitude;
};
struct OscillationEx : Oscillation
{
	enum EffectAxisEnum : __int32
	{
		SHAKE_LEFTRIGHT = 0x0,
		SHAKE_UPDOWN = 0x1,
		MAX_EFFECTAXISENUM = 0x2,
	};
	EffectAxisEnum m_eAxis;
	void Clear()
	{
		this->m_fTargetAmplitude = 0.0;
		this->m_fAmplitude = 0.0;
		this->m_fValue = 0.0;
		this->m_fCurrTime = 3.4028235e38;
		this->m_eAxis = SHAKE_LEFTRIGHT;
	}
};
struct stFogInfo : NiRefObject
{
	NiColor m_kColor;
};

struct CameraCtrl
{
	float m_fZoom;
	float m_fZoomMax;
	float m_fZoomMin;
	float m_fZoomDef;
	float m_fMsDeltaX;
	float m_fMsDeltaY;
	std::list<float> m_lstTraceCam;
	NiPoint3 m_kLookAtPos;
	float m_fAngleUD;
	float m_fAngleUDMax;
	float m_fAngleUDMin;
	float m_fAngleLR;
	NiPoint3 m_kUpDir;
	NiPoint3 m_kRightDir;
	NiPoint3 m_kFrontDir;
	float m_fZoomRatePerTick;
	float m_fLastUpdateTime;
	float m_fTargetZoom;
	float m_fTargetAngleLR;
	unsigned int m_uiCheckRayTile;
	void InitDirection(const NiPoint3 kUpDir, const NiPoint3 kRightDir, const NiPoint3 kFrontDir) 
	{
		this->m_kUpDir = kUpDir;
		this->m_kRightDir = kRightDir;
		this->m_kFrontDir = kFrontDir;
	}
	void InitZoomMinMax(const float fZoomMin, const float fZoomMax)
	{
		this->m_fZoomMin = fZoomMin;
		this->m_fZoomMax = fZoomMax;
	}
	void SetZoom(float fZoom) 
	{
		if (fZoom < m_fZoomMin)
		{
			m_fTargetZoom = m_fZoomMin;
			return;
		}
		if (fZoom > m_fZoomMax)
		{
			m_fTargetZoom = m_fZoomMax;
			return;
		}
		m_fTargetZoom = fZoom;
	}
	void AddRotate(int iMsDeltaX, int iMsDeltaY)
	{
		if (m_lstTraceCam.size())
		{
			this->m_fMsDeltaX = 0.0;
			this->m_fMsDeltaY = 0.0;
		}
		else
		{
			this->m_fMsDeltaX = (double)iMsDeltaX * 0.699999988079071 * 0.5;
			this->m_fMsDeltaY = 0.5 * (0.699999988079071 * (double)iMsDeltaY);
		}
	}
};
struct stGlobalLightInfo : NiRefObject
{
	NiColor m_kColor;
};
struct SkyCtrl
{
	NiNodePtr m_spNode;
	int m_nMaxTime;
	char Init()
	{
		return 1;
	}
};
class World 
{
	struct ExtraLightScene
	{
		unsigned int dwRGB;
		NiNodePtr spScene;
		NiNodePtr spLightScene;
	};
	struct AniObjectType
	{
		float fMaxTime;
		float fStartTime;
	};
	bool m_bCameraMove;
	bool m_bChangeFogFlag;
	bool m_bMagicCircle;
	float m_fMaxHeight;
	float m_fMinHeight;
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
	CameraCtrl m_kCameraCtrl;
	float m_fCameraFOV;
	bool m_bCameraOrtho;
	NiFrustum m_kSkyFrustum;
	NiFrustum m_kWorldFrustum;
	float m_fNearPlaneRadius;
	OscillationEx m_kCamOscillation;
	NiPoint3 m_kLookAtDelta;
	NiPoint3 m_kCameraPosDelta;
	bool m_bControllerMode;
	bool m_bAutoCameraMode;
	unsigned int m_dwOldTime;
	float m_fAngleUDDelta;
	float m_fAngleLRDelta;
	float m_fDist;
	NiPoint3 m_kLookAtPos;
	SkyCtrl m_kSkyCtrl;
	NiAmbientLightPtr m_spAmbientLight;
	NiDirectionalLightPtr m_spDirectionalLight;
	NiDirectionalLightPtr m_spMapDirectionalLight;
	NiColor m_kBackGroundColor;
	unsigned int m_eCharShadowLevel;
	NiNodePtr m_spMoveTargetPointNode;
	unsigned __int8 m_MapType;
	NiNodePtr m_spCharSelectMarkNode;
	NiPointer<NiSourceTexture> m_spMiniMapTexture;
	std::set<NiPointer<NiAVObject>> m_setChar;
	std::set<NiAVObject*> m_vecFadeObject;
	std::set<NiAVObject*> m_vecCameraIntersectObject;
	std::set<NiAVObject*> m_vecUpdateFadeObject;
	NiPointer<NiTexturePalette> m_spTexturePalette;
	NiNodePtr m_spCameraShakeNode;
	ExtraLightScene m_kExtraLightScene[30];
	std::list<NiAVObject*> m_vecAniObject;
	std::map<NiAVObject*, NiBound> m_mapBound;
	std::map<NiAVObject*, AniObjectType> m_mapBoundTime;
	std::map<int, NiPointer<stGlobalLightInfo>> m_mapGlobalLight;
	std::map<int, NiPointer<stFogInfo>> m_mapFogInfo;
	int m_globalLightIndex;

public:
	bool InitScene();
	bool InitCamera();
	bool InitSkyCtrl();
	bool InitLightFog();
	bool InitShadow() { return true; }

	void AttachSky(NiNode* sky)
	{
		m_spSkyScene->AttachChild(sky,1);
		m_spSkyScene->UpdateProperties();
		m_spSkyScene->UpdateEffects();
		m_spSkyScene->Update(0.0);
	}
	void AttachWater(NiNode* water)
	{
		m_spWaterScene->AttachChild(water, 1);
		m_spWaterScene->UpdateProperties();
		m_spWaterScene->UpdateEffects();
		m_spWaterScene->Update(0.0);
	}
	void AttachGroundObj(NiNode* obj)
	{
		m_spGroundObject->AttachChild(obj, 1);
		m_spGroundObject->UpdateProperties();
		m_spGroundObject->UpdateEffects();
		m_spGroundObject->Update(0.0);
	}
	void AttachGroundCollidee(NiNode* obj)
	{
		m_spGroundObjectCollidee->AttachChild(obj, 1);
		m_spGroundObjectCollidee->UpdateProperties();
		m_spGroundObjectCollidee->UpdateEffects();
		m_spGroundObjectCollidee->Update(0.0);
	}
	NiNode* GetWorldScene() {
		return m_spWorldScene;
	}
	NiNode* GetSkyNode() {
		return m_spSkyScene;
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
		NiFrustum frust; m_spCamera->GetViewFrustum();
		frust.m_fFar = Frustum * 1.0;
		m_spCamera->SetViewFrustum(frust);
	}
	void SetMapDirectionalLightAmbientColor(NiColor kColor)
	{
		m_spMapDirectionalLight->SetAmbientColor(kColor);
	}
	void SetMapDirectionalLightDiffuseColor(NiColor kColor)
	{
		m_spMapDirectionalLight->SetDiffuseColor(kColor);
	}
	NiCamera* GetCamera() { return m_spCamera; }
	static NiPoint3 ms_kUpDir; // idb
	static NiPoint3 ms_kDownDir; // idb
	static NiPoint3 ms_kNorthDir; // idb
	static NiPoint3 ms_kEastDir; // idb
	static NiPoint3 ms_kWestDir; // idb
	static NiPoint3 ms_kDefaultDirectionalLightDir; // idb
};
