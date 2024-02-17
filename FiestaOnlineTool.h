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
		NiDelete cursor;
		//NiDelete m_spInterfaceCamera;
		//NiDelete Interface;
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
	NiNode* LoadNifFile(const char* File, NiTexturePalette* pTexturePalette);
	char CatchCamera(NiAVObject*, NiCameraPtr*);
	bool CreateRenderer(HWND hWnd);
	NiAlphaAccumulatorPtr Sorter;

	NiActionMap* CreateNewActionMap(const char* pcName);
	NiActionMap* CreateInitActionMap();
	static FiestaOnlineTool* _Tool;
	NiInputSystem::CreateParams* GetInputCreationParameters();

	NiActionMap* EmptyActionMap;
	NiActionMap* StartActionMap;

	NiInputKeyboardPtr m_spKeyboard;
	NiInputMousePtr m_spMouse;
	NiNode* ServerList;
	NiCursor* cursor;
	NiNode* LoginInputPanel;
	NiCamera* InterfaceCamera;
	NiNodePtr Interface;
	PgWinMgr* Pgg_kWinMgr;
};