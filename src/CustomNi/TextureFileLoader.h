#pragma once
#include "PgUtil.h"
#include <NiNode.h>
#include "ImGui/imgui.h"
#include "ImGui/imfilebrowser.h"
class TextureFileLoader 
{
public:
	TextureFileLoader() : fileDialog(ImGuiFileBrowserFlags_NoTitleBar | ImGuiFileBrowserFlags_CloseOnEsc)
	{
		fileDialog.SetTitle("Select Texture-File");
		fileDialog.SetTypeFilters({ ".dds" });
		fileDialog.SetPwd(PgUtil::CreateFullFilePathFromBaseFolder(""));
	}
	~TextureFileLoader()
	{
	};

	void Prepare()
	{
		fileDialog.Open();
	}

	bool DrawImGui() 
	{
		fileDialog.Display();
		if (fileDialog.HasSelected())
		{
			LogInfo(fileDialog.GetSelected().string());
		}
		return fileDialog.HasSelected();
	}
	std::string Load() 
	{
		std::string File = fileDialog.GetSelected().string();
		std::string BasePath = PgUtil::CreateFullFilePathFromBaseFolder("");
		fileDialog.ClearSelected();
		return ".\\" + File.substr(BasePath.length() + 1);
	}
private:
	ImGui::FileBrowser fileDialog;
};