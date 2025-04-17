#pragma once
#include <FiestaScene/FiestaScene.h>
#include <PgUtil.h>
#include "ImGui/imgui.h"
#include "ImGui/imfilebrowser.h"

NiSmartPointer(InitializeScene);

class InitializeScene : public FiestaScene
{
	NiDeclareRTTI;
public:
	InitializeScene() : 
		ClientFolderPath(ImGuiFileBrowserFlags_SelectDirectory) ,
		ServerFolderPath(ImGuiFileBrowserFlags_SelectDirectory)
	{
		ClientFolderPath.SetTitle("Select Game-Client-Exe");
		ClientFolderPath.SetTypeFilters({ ".exe", ".bin" });
		ClientFolderPath.SetPwd(PgUtil::PathFromApplicationFolder(""));
		ClientFolderPath.Open();
		ServerFolderPath.SetTitle("Select 9Data Folder");
		ServerFolderPath.SetTypeFilters({});
		ServerFolderPath.SetPwd(PgUtil::PathFromApplicationFolder(""));
	};
	virtual void Draw(NiRenderer* renderer) 
	{
		NiVisibleArray m_kVisible;
		NiCullingProcess m_spCuller(&m_kVisible);
		NiDrawScene(Camera, BaseNode, m_spCuller);
	}
	virtual void DrawImGui();
	virtual void UpdateCamera(float fTime){};
private:
	bool Init = false;
	ImGui::FileBrowser ClientFolderPath;
	ImGui::FileBrowser ServerFolderPath;
};
