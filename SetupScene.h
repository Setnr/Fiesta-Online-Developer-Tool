#pragma once
#include "FiestaScene.h"
#include <NiThread.h>
#include "PgUtil.h"
#include "ImGui/imgui.h"
#include "ImGui/imfilebrowser.h"
#include <iostream>
#include <fstream>
class SetupLoader
{
public:
	SetupLoader() : fileDialog(ImGuiFileBrowserFlags_SelectDirectory)
	{
		fileDialog.SetTitle("Select Game-Client");
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
		std::ofstream Settings;
		std::string Path = PgUtil::CreateFullFilePathFromApplicationFolder(SettingsPath);

		Settings.open(Path, std::ios::out);
		Settings << "#ClientPath " << File;
		Settings.close();

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


