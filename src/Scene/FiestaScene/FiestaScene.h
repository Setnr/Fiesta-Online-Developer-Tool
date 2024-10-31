#pragma once
#include <NiMainLibType.h>
#include <NiMain.h>
#include <NiRTTI.h>

#include <iostream>
#include <string>

NiSmartPointer(FiestaScene);

class FiestaScene : public NiRefObject
{
	NiDeclareRootRTTI(FiestaScene);
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
		BaseNode = NULL;
		Camera = NULL;
	}
	virtual bool SetupScene();
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
	virtual void Terminate(){}
	virtual void ProcessInput(){}
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

	bool EnableKeyStrokes = false;
	std::atomic<bool> CanSwitch = false;

};
