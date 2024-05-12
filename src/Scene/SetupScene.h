#pragma once
#include "FiestaScene.h"
#include <NiThread.h>
#include "PgUtil.h"
#include "ImGui/imgui.h"
#include "ImGui/imfilebrowser.h"
#include <iostream>
#include <fstream>
#include "Logger.h"
#include "Settings.h"
class SetupLoader
{
public:
	SetupLoader() : fileDialog(ImGuiFileBrowserFlags_SelectDirectory)
	{
		fileDialog.SetTitle("Select Game-Client-Folder");
		fileDialog.SetTypeFilters({ ".exe", ".bin" });
		fileDialog.SetPwd(PgUtil::CreateFullFilePathFromBaseFolder(""));
	}
	~SetupLoader() = default;

	void Prepare()
	{
		fileDialog.Open();
	}

	bool DrawImGui()
	{
		fileDialog.Display();
		return fileDialog.HasSelected();
	}
	void UpdateSettingsFile()
	{
		std::string File = fileDialog.GetSelected().string();
		if (File.find(".exe") != std::string::npos || File.find(".bin") != std::string::npos)
		{
			NiMessageBox::DisplayMessage("Please Select the Folder!", "Info");
			fileDialog.ClearSelected();
			return;
		}
		Settings::SetClientPath(File);
		Settings::SaveSettings();
	
		NiMessageBox::DisplayMessage("Updated Settings\n Restart Tool now!", "Info");
		fileDialog.ClearSelected();
		return;
	}
private:
	ImGui::FileBrowser fileDialog;
};
NiSmartPointer(SetupScene);
class StartUpScene : public FiestaScene
{
public:
	StartUpScene() {
		_Loader.Prepare();
	}
	~StartUpScene()
	{
	}

	void Draw(NiRenderer* renderer)
	{

	}
	bool SetupScene(NiNodePtr& m_spScene, NiCameraPtr& m_spCamerea) { return true; };
	void Update(float fTime){}
	virtual void DrawImGui();
protected:
	static SetupLoader _Loader;
};



