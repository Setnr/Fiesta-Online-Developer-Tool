#pragma once

#include "InitializeScene.h"


#include "ImGui/imgui.h"
#include <Settings.h>

NiImplementRTTI(InitializeScene, FiestaScene);

void InitializeScene::DrawImGui() 
{
	if (!fileDialog.IsOpened())
		fileDialog.Open();

	FiestaScene::DrawImGui();
	ImGuiIO& io = ImGui::GetIO();

	fileDialog.SetWindowPos(io.DisplaySize.x, io.DisplaySize.y);
	fileDialog.Display();
	if (fileDialog.HasSelected()) 
	{
		std::string File = fileDialog.GetSelected().string();
		if (File.find(".exe") != std::string::npos || File.find(".bin") != std::string::npos)
		{
			NiMessageBox::DisplayMessage("Please Select the Folder!", "Info");
			fileDialog.ClearSelected();
			return;
		}
		PgUtil::ClientFolderPath = File;
		Settings::SaveSettings();
		fileDialog.ClearSelected();
		NiMessageBox::DisplayMessage("Updated Settings\n Restart Tool now!", "Info");

		if (std::filesystem::exists(PgUtil::PathFromApplicationFolder(".\\FiestaOnlineTool\\AlphaTextureBlender.NSF")))
		{
			NiMessageBox::DisplayMessage("AlphaTextureBlender.NSF will be replaced in your client", "Shader Info");
			if (std::filesystem::exists(PgUtil::PathFromClientFolder(".\\shader\\AlphaTextureBlender.NSF.bak")))
				std::filesystem::remove(PgUtil::PathFromClientFolder(".\\shader\\AlphaTextureBlender.NSF.bak"));

			if (std::filesystem::exists(PgUtil::PathFromClientFolder(".\\shader\\AlphaTextureBlender.NSF")))
			{
				std::filesystem::copy(
					PgUtil::PathFromClientFolder(".\\shader\\AlphaTextureBlender.NSF"),
					PgUtil::PathFromClientFolder(".\\shader\\AlphaTextureBlender.NSF.bak"));
				std::filesystem::remove(PgUtil::PathFromClientFolder(".\\shader\\AlphaTextureBlender.NSF"));
			}
			std::filesystem::copy(
				PgUtil::PathFromApplicationFolder(".\\FiestaOnlineTool\\AlphaTextureBlender.NSF"),
				PgUtil::PathFromClientFolder(".\\shader\\AlphaTextureBlender.NSF"));
		}

		return;
	}
}
