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
	InitializeScene() : fileDialog(ImGuiFileBrowserFlags_SelectDirectory | ImGuiFileBrowserFlags_NoModal) 
	{
		fileDialog.SetTitle("Select Game-Client-Folder");
		fileDialog.SetTypeFilters({ ".exe", ".bin" });
		fileDialog.SetPwd(PgUtil::PathFromApplicationFolder(""));
		fileDialog.Open();
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
	ImGui::FileBrowser fileDialog;
};
