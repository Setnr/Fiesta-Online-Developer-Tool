#pragma once

#include <NiApplication.h>
#include <NiCursor.h>
#include <atomic>
#include <mutex>

#include <PgUtil.h>
#include <FiestaScene/FiestaScene.h>

#include "ImGui/imgui.h"
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

	virtual void Terminate()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SaveIniSettingsToDisk(io.IniFilename);
		if(_Scene)
			_Scene->Terminate();
		cursor = NULL;
		BoundingBox = NULL;
		_Scene = NULL;
		Sorter = NULL;

		m_spScene = 0;

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		NiApplication::Terminate();
	}
	virtual void OnIdle();
	virtual bool OnDefault(NiEventRef pEventRecord);
	static float GetDeltaTime() {return DeltaTime;}
	virtual void ProcessInput();
#pragma region ToolSetup
	bool Initialize();
	bool CreateRenderer();
	void LoadInitialScene();
	static void StartSHNLoadingThread();
	static void StartVersionCheckThread();
#pragma endregion
	static void DisableCursor() { if(cursor)cursor->Show(false); }
	static void EnableCursor() { if(cursor)cursor->Show(true); }
	static POINT CurrentMousePosition() { return FiestaOnlineTool::MousePos; }
	static NiNodePtr GetBoundingBox() 
	{
		auto newobj = FiestaOnlineTool::BoundingBox->CreateDeepCopy();
		if (NiIsKindOf(NiNode, newobj))
			return NiSmartPointerCast(NiNode,newobj);
		return  NULL;
	}
	static NiRendererPtr GetRenderer() { 
		FiestaOnlineTool* tool = (FiestaOnlineTool*)ms_pkApplication;
		auto renderer = tool->m_spRenderer;
		return renderer;
	}
private:
	static POINT MousePos;
	static NiNodePtr BoundingBox;
	static NiCursorPtr cursor;
	FiestaScenePtr _Scene;
	NiAlphaAccumulatorPtr Sorter;

	std::atomic_bool SHNLoaded;
	float m_fLastUpdateTime;
	static std::atomic<float> DeltaTime;

	void DrawCursor();
	bool WasPreviousFrameHovered = false;
#pragma region ShaderInit
	static unsigned int ShaderErrorCallback(const char* pacError, NiShaderError eError, bool bRecoverable);
	virtual bool RunShaderParser();
	static unsigned int RunParser(const char* pcLibFile, NiRenderer* pkRenderer, const char* pcDirectory, bool bRecurseSubFolders);
	virtual bool RegisterShaderLibraries();
	static bool LibraryClassCreate(const char* pcLibFile, NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[], bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary);
	static bool EffectLibraryClassCreate(const char* pcLibFile, NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[], bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary);
#pragma endregion
};
