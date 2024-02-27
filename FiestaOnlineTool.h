#pragma once
#include <NiApplication.h>
#include <NiCursor.h>
#include "PgWinMgr.h"
#include "FiestaScene.h"
class FiestaOnlineTool : public NiApplication
{
public:
	FiestaOnlineTool();
	~FiestaOnlineTool() 
	{
		ShowCursor(true);
	}
	virtual bool CreateRenderer();
	virtual bool Initialize();
	virtual bool CreateInputSystem();
	virtual void OnIdle();
	virtual void ProcessInput();
	virtual void Terminate() 
	{
		m_spActionMap = 0;
		m_spScene = 0;
		_Scene = 0;
		NiApplication::Terminate();
	}

	void CheckInterfaceForHit() {

		tagPOINT kPoint;
		GetCursorPos(&kPoint);
		ScreenToClient(this->GetWindowReference(), &kPoint);
		Pgg_kWinMgr->CheckForHit(kPoint);
	}
	void CheckInterfaceForClick() {

		tagPOINT kPoint;
		GetCursorPos(&kPoint);
		ScreenToClient(this->GetWindowReference(), &kPoint);
		Pgg_kWinMgr->CheckForClick(kPoint);
	}
	void UpdateInterface()
	{
		Pgg_kWinMgr->Update();
	}

	static bool GetRotateCamera() 
	{
		if (_Tool)
			return _Tool->MoveCamera;
		return false;
	}
	static bool GetPositionDelta(int& iX, int& iY, int& iZ) 
	{
		if (_Tool) 
		{
			return _Tool->m_spMouse->GetPositionDelta(iX, iY, iZ);
		}
		return false;
	}
private: 

	FiestaScenePtr _Scene;

	static bool HandleMouseMovement(NiActionData* pkActionData);
	void DrawCursor();
	bool CreateRenderer(HWND hWnd);
	NiAlphaAccumulatorPtr Sorter;

	NiActionMapPtr CreateNewActionMap(const char* pcName);
	NiActionMapPtr CreateInitActionMap();
	static FiestaOnlineTool* _Tool;
	NiInputSystem::CreateParams* GetInputCreationParameters();

	NiActionMapPtr m_spActionMap;

	NiInputKeyboardPtr m_spKeyboard;
	NiInputMousePtr m_spMouse;
	NiCursorPtr cursor;
	NiNodePtr LoginInputPanel;
	NiPointer<PgWinMgr> Pgg_kWinMgr;
	float m_fHeading;
	float m_fCamPitch;
	bool MoveCamera;
	void EnableCameraMove() { MoveCamera = true; }
	void DisableCameraMove() { MoveCamera = false; }
	float m_fLastUpdateTime;
};