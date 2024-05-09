#pragma once

#include <NiApplication.h>
#include <NiCursor.h>
#include "PgWinMgr.h"
#include "FiestaScene.h"

#include "ImGui/imgui.h"
#include <mutex>

#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

class FiestaOnlineTool : public NiApplication
{
public:
	FiestaOnlineTool();
	~FiestaOnlineTool() 
	{
		ShowCursor(true);
	}
	virtual bool Initialize();
	
	virtual void OnIdle();

	virtual bool CreateRenderer();

	virtual void Terminate() 
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SaveIniSettingsToDisk(io.IniFilename);

		m_spActionMap = 0;
		m_spScene = 0;
		_Scene = 0;

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		NiApplication::Terminate();
	}
	virtual bool OnDefault(NiEventRef pEventRecord);
	void CheckInterfaceForHit();
	void CheckInterfaceForClick();
	void UpdateInterface();


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
	static void UpdateScene(FiestaScenePtr Scene);
	static FiestaOnlineTool* _Tool;
	void UpdateSceneInternal(FiestaScenePtr Scene);
	static float GetLastUpdateTime() {
		return _Tool->_LastUpdateTime();
	}
	float _LastUpdateTime() {
		return m_fLastUpdateTime;
	}
	static void DisableCursor()
	{
		if (_Tool)
			_Tool->cursor->Show(false);
	}
	static void EnableCursor()
	{
		if (_Tool)
			_Tool->cursor->Show(true);
	}
private: 
	std::mutex SceneLock;
	FiestaScenePtr _Scene;

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
	NiAlphaAccumulatorPtr Sorter;


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
	void LoadSettings();
	float m_fLastUpdateTime;
};
