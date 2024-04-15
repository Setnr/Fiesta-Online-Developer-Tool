#pragma once
#include <NiSample.h>
#include <NiCursor.h>
#include "PgWinMgr.h"
#include "FiestaScene.h"


class FiestaOnlineTool : public NiSample
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
	static bool IsMoveKeyPressed(bool &W_Key, bool& S_Key, bool& A_Key, bool& D_Key)
	{
		bool ret = false;
		if (_Tool) 
		{
			if (_Tool->m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_W)) 
			{
				W_Key = true;
				ret = true;
			}
			if (_Tool->m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_S))
			{
				S_Key = true;
				ret = true;
			}
			if (_Tool->m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_A))
			{
				A_Key = true;
				ret = true;
			}
			if (_Tool->m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_D))
			{
				D_Key = true;
				ret = true;
			}
		}
		return ret;
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
	virtual bool RegisterShaderParsers() 
	{
		char Path[513];

		char acProgramPath[NI_MAX_PATH];

		PgUtil::CreateFullFilePathFromBaseFolder(acProgramPath, ".\\shader\\");

		if (!NiShaderFactory::LoadAndRunParserLibrary(".\\NSFParserLibDX9" NI_DLL_SUFFIX ".dll", 
			acProgramPath,true))
		{
			NiMessageBox("Failed to load shader library!", "ERROR");
			return false;
		}
		return true;
	}
	virtual bool RegisterShaderLibraries() 
	{
		char Path[513];

		char acShaderPath[NI_MAX_PATH];
		char acProgramPath[NI_MAX_PATH];
		int iDirectoryCount = 1;
		char* apcDirectories[1];

		PgUtil::CreateFullFilePathFromBaseFolder(acShaderPath, "");
		apcDirectories[0] = acShaderPath;
		PgUtil::CreateFullFilePathFromBaseFolder(acProgramPath, ".\\shader\\");

		NiShaderFactory::AddShaderProgramFileDirectory(acProgramPath);
		

		if (!NiShaderFactory::LoadAndRegisterShaderLibrary(".\\NSBShaderLibDX9" NI_DLL_SUFFIX ".dll", 
			iDirectoryCount, apcDirectories,true))
		{
			NiMessageBox("Failed to load shader library!", "ERROR");
			return false;
		}
		if (!NiShaderFactory::LoadAndRegisterShaderLibrary(".\\NiD3DXEffectShaderLibDX9" NI_DLL_SUFFIX ".dll", 
			iDirectoryCount, apcDirectories,true))
		{
			NiMessageBox("Failed to load shader library!", "ERROR");
			return false;
		}
		return true;
	}
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

class Engine3D
{
public:
	static char Init(NiCamera* pkCamera);
	static void ShaderRunParserCallback(const char* pcLibFile, NiRenderer* pkRenderer, const char* pcDirectory, bool bRecurseSubFolders);
	static BOOL ShaderClassCreateCallback(const char* pcLibFile, NiRenderer* pkRenderer, int iDirectoryCount, char** apcDirectories, bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary);
	static bool FXLibraryClassCreate(const char* pcLibFile, NiRenderer* pkRenderer, int iDirectoryCount, char** apcDirectories, bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary);
	static bool GetEnableCarToon();
	static void EnableCarToon(bool bEnable);
	bool Terminate();
	static unsigned int ShaderErrorCallback(const char* pcError,NiShaderError eError, bool bRecoverable) {
		NiMessageBox(pcError, "Shader Error");
		NiOutputDebugString("ERROR: ");
		NiOutputDebugString(pcError);

		return 0;
	}
};