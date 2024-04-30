#pragma once
#include <NiMainLibType.h>
#include <NiMain.h>

#include <iostream>
#include <string>
NiSmartPointer(FiestaScene);
class FiestaScene : public NiRefObject
{
	//NiDeclareRootRTTI(FiestaScene);
public:
	FiestaScene() 
	{
		CanSwitch = false;
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
	void StartImGuiFrame();
	void EndImGuiFrame();
	virtual void Update(float fTime)
	{
		this->UpdateCamera(fTime);
		BaseNode->Update(fTime);
	}
	virtual void UpdateCamera(float fTime);
	bool CanBeSwitched() { return CanSwitch; }
protected:
	NiNodePtr BaseNode;
	NiCameraPtr Camera;

	float Yaw;
	float Pitch;
	float Roll;

	float FPS[90];
	int values_offset;
	std::atomic<bool> CanSwitch;
};

