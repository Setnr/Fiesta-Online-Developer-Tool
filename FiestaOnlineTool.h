#pragma once
#include <NiSample.h>
#include <NiCursor.h>
#include "PgWinMgr.h"
#include "FiestaScene.h"

#include "ImGui/imgui.h"

class FiestaOnlineTool : public NiSample
{
public:
	FiestaOnlineTool();
	~FiestaOnlineTool() 
	{
		ShowCursor(true);
		exit(0);
	}
	virtual bool CreateRenderer();
	virtual bool Initialize();
	virtual bool CreateInputSystem();
	virtual void OnIdle();
	virtual void ProcessInput();
	virtual void Terminate() 
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SaveIniSettingsToDisk(io.IniFilename);

		m_spActionMap = 0;
		m_spScene = 0;
		_Scene = 0;
		
		NiApplication::Terminate();
	}

	void CheckInterfaceForHit();
	void CheckInterfaceForClick();
	void UpdateInterface();

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
	static bool IsLeftClick() {
		return _Tool->m_spMouse->ButtonWasPressed(NiInputMouse::NIM_LEFT);
	}
	static bool GetPositionDelta(int& iX, int& iY, int& iZ) 
	{
		if (_Tool) 
		{
			return _Tool->m_spMouse->GetPositionDelta(iX, iY, iZ);
		}
		return false;
	}
	static bool GetMousePosition(long& X, long& Y) 
	{
		if (!_Tool)
			return false;
		tagPOINT kPoint;
		GetCursorPos(&kPoint);
		ScreenToClient(_Tool->GetRenderWindowReference(), &kPoint);
		X = kPoint.x;
		Y = kPoint.y;
		return true;
	}

	static FiestaOnlineTool* _Tool;
private: 

	FiestaScenePtr _Scene;

	static bool HandleMouseMovement(NiActionData* pkActionData);
	virtual bool RegisterShaderParsers() 
	{
		std::string acProgramPath =PgUtil::CreateFullFilePathFromBaseFolder(".\\shader\\");

		if (!NiShaderFactory::LoadAndRunParserLibrary(".\\NSFParserLibDX9" NI_DLL_SUFFIX ".dll", 
			acProgramPath.c_str(), true))
		{
			NiMessageBox("Failed to load shader library!", "ERROR");
			return false;
		}
		return true;
	}
	virtual bool RegisterShaderLibraries() 
	{
		int iDirectoryCount = 1;
		char* apcDirectories[1];

		apcDirectories[0] = (char*)PgUtil::FolderPath.c_str();
		std::string acProgramPath = PgUtil::CreateFullFilePathFromBaseFolder(".\\shader\\");

		NiShaderFactory::AddShaderProgramFileDirectory(acProgramPath.c_str());
		

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
