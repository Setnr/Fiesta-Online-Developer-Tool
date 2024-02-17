#pragma once
#include <NiApplication.h>
#include <NiCursor.h>
#include "PgWinMgr.h"
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
};