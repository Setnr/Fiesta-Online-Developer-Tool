#pragma once

#include <NiApplication.h>
#include <NiCursor.h>
#include "PgWinMgr.h"
#include "FiestaScene.h"
#include <atomic>
#include "ImGui/imgui.h"
#include <mutex>
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "FiestaOnlineTool_GeneralHeaders.h"
#include "Settings.h"

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
	static bool GetResolution(unsigned int& Width, unsigned int& Height)
	{
		if (!_Tool)
			return false;

		Height = _Tool->GetAppWindow()->GetHeight();
		Width = _Tool->GetAppWindow()->GetWidth();
		return true;
	}
	static float GetFPSCap() 
	{
		if (!_Tool)
			return 0.0f;
		return _Tool->GetMaxFrameRate();
	}
	static void SetFPSCap(float fps)
	{
		if (!_Tool)
			return ;
		Settings::SetFPSCap(fps);
		_Tool->SetMaxFrameRate(fps);
	}
	static void UpdateScene(FiestaScenePtr Scene);
	static FiestaOnlineTool* _Tool;
	FiestaScenePtr NewScene;
	void UpdateSceneInternal(FiestaScenePtr Scene);
	void UpdateSceneInternal();
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
	static void AddScreenTexture(NiScreenTexturePtr texture) {
		if (_Tool)
			_Tool->GetScreenTextures().AddTail(texture);
	}
	static void RemoveScreenTexture(NiScreenTexturePtr texture) {
		if (_Tool)
			_Tool->GetScreenTextures().Remove(texture);
	}
	static void AddScreenElemets(NiScreenElementsPtr element) {
		if (_Tool)
			_Tool->GetScreenElements().AddTail(element);
	}
	static void RemoveScreenElemets(NiScreenElementsPtr element) {
		if (_Tool)
			_Tool->GetScreenElements().Remove(element);
	}
private: 
	std::mutex SceneLock;
	std::mutex UpdateLock;
	FiestaScenePtr _Scene;
	static unsigned int ShaderErrorCallback(const char* pacError,NiShaderError eError, bool bRecoverable);
	virtual bool RunShaderParser();
	static unsigned int RunParser(const char* pcLibFile, NiRenderer* pkRenderer, const char* pcDirectory,bool bRecurseSubFolders);
	virtual bool RegisterShaderLibraries();
	static bool LibraryClassCreate(const char* pcLibFile, NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[], bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary);
	static bool EffectLibraryClassCreate(const char* pcLibFile, NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[], bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary);

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
	float m_fLastUpdateTime;

};
