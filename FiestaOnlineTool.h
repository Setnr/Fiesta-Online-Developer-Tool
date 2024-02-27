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

	void DoCamera() 
	{
		if (!MoveCamera)
			return;
		int iX, iY, iZ;
		if (m_spMouse->GetPositionDelta(iX, iY, iZ))
		{
			UtilDebugString("UpdateCamera")
			NiMatrix3 kv = m_spCamera->GetRotate();
			NiPoint3 translate =m_spCamera->GetTranslate();
			float CurrentPitch;
			float CurrentYaw;
			float CurrentRoll;
			float Angle;
			kv.ExtractAngleAndAxis(Angle, CurrentRoll, CurrentYaw, CurrentPitch);

			UtilDebugString("Angle %f ,CurrentRoll %f ,CurrentYaw %f ,CurrentPitch %f ", Angle, CurrentRoll,CurrentYaw,CurrentPitch)
			UtilDebugString("translate.x %f ,translate.y %f ,translate.z %f ", translate.x, translate.y, translate.z)

			unsigned int uiAppHeight = NiApplication::ms_pkApplication->
				GetAppWindow()->GetHeight();
			// in pixels to avoid the sensitivity changing with screen resolution
			//CameraCtrl::CalCulateCamera
			unsigned int uiAppWidth = NiApplication::ms_pkApplication->
				GetAppWindow()->GetWidth();
			if (uiAppHeight > 0 && uiAppWidth > 0)
			{
				float fPitchDelta = NI_PI * 0.375f * (float)(iY)
					/ (float)uiAppHeight;
				float fHeadingDelta = NI_PI * 0.5f * (float)(iX)
					/ (float)uiAppWidth;
				UtilDebugString("fPitchDelta %f", fPitchDelta)
				NiMatrix3 kv2(NiPoint3::ZERO, NiPoint3::ZERO, NiPoint3::ZERO);
				kv2.MakeRotation(Angle, CurrentRoll, CurrentYaw + fHeadingDelta, CurrentPitch + fPitchDelta);

				m_spCamera->SetRotate(kv2);
				m_spCamera->Update(0.0f);
			}

		}
	}
};