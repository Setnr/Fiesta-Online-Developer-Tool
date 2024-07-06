#pragma once
#include <NiMainLibType.h>
#include <NiMain.h>

#include <iostream>
#include <string>
#include "../Data/IngameWorld.h"
#define SettingsPath ".\\FiestaOnlineTool\\Settings.ini"
NiSmartPointer(FiestaScene);
class FiestaScene : public NiRefObject
{
	//NiDeclareRootRTTI(FiestaScene);
public:
	FiestaScene() 
	{
		Yaw = 0.0f;
		Pitch = 0.0f;
		Roll = 0.0f;
		memset(&FPS, 0, sizeof(FPS));
		values_offset = 0;
	};
	~FiestaScene() 
	{
	}
	virtual bool SetupScene(NiNodePtr& m_spScene, NiCameraPtr& m_spCamerea);
	virtual void Draw(NiRenderer* renderer) {}
	virtual void DrawImGui();
	virtual void CreateMenuBar() {};
	virtual void Update(float fTime)
	{
		if (BaseNode)
			BaseNode->Update(fTime);
	}
	virtual void UpdateCamera(float fTime);
	bool CanBeSwitched() { return CanSwitch; }
	void StartImGuiFrame();
	void EndImGuiFrame();
protected:
	NiNodePtr BaseNode;
	NiCameraPtr Camera;

	float Yaw;
	float Pitch;
	float Roll;

	float FPS[90];
	int values_offset;
	bool ShowAbout = false;
	virtual void ShowAboutWindow();
	void LookAndMoveAtWorldPoint(NiPoint3 Point)
	{
		Camera->SetTranslate(Point + NiPoint3(250.f, 250.f, 250.f));
		Camera->Update(0.0f);
		Camera->LookAtWorldPoint(Point, TerrainWorld::ms_kUpDir);
		Camera->LookAtWorldPoint(Point, TerrainWorld::ms_kUpDir);
		NiMatrix3 mat = Camera->GetRotate();
		mat.ToEulerAnglesXYZ(Roll, Yaw, Pitch); //Roll Yaw Pitch Steuerung gespiegelt
		Camera->Update(0.0f);
	}

	std::atomic<bool> CanSwitch = false;

};

