#pragma once
#include <NiApplication.h>
#include <NiCursor.h>
#include "PgWin.h"
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
private: 
	enum MouseMovementActionCode 
	{
		XAxis,
		YAxis,
		ZAxis
	};

	static bool HandleMouseMovement(NiActionData* pkActionData);
	void DrawCursor();
	bool CreateRenderer(HWND hWnd);
	NiAlphaAccumulatorPtr Sorter;

	NiActionMap* CreateNewActionMap(const char* pcName);
	NiActionMap* CreateInitActionMap();
	static FiestaOnlineTool* _Tool;
	NiInputSystem::CreateParams* GetInputCreationParameters();

	NiActionMapPtr EmptyActionMap;
	NiActionMapPtr StartActionMap;

	NiInputKeyboardPtr m_spKeyboard;
	NiInputMousePtr m_spMouse;
	NiCursorPtr cursor;
	NiNodePtr LoginInputPanel;
	NiPointer<PgWinMgr> Pgg_kWinMgr;
};