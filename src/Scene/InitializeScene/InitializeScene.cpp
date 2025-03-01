#pragma once

#include "InitializeScene.h"


#include "ImGui/imgui.h"
#include <Settings.h>

NiImplementRTTI(InitializeScene, FiestaScene);

void InitializeScene::DrawImGui() 
{
	FiestaScene::DrawImGui();
	ImGuiIO& io = ImGui::GetIO();
	if (!Init)
	{
		Init = true;
		ClientFolderPath.Display();
	}
	if (ClientFolderPath.IsOpened())
	{
		ClientFolderPath.SetWindowPos(io.DisplaySize.x, io.DisplaySize.y);
		ClientFolderPath.Display();
		if (ClientFolderPath.HasSelected())
		{
			std::string File = ClientFolderPath.GetSelected().string();
			if (File.find(".exe") != std::string::npos || File.find(".bin") != std::string::npos)
			{
				NiMessageBox::DisplayMessage("Please Select the Folder!", "Info");
				ClientFolderPath.ClearSelected();
				return;
			}
			PgUtil::ClientFolderPath = File;
			ClientFolderPath.ClearSelected();

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
			ServerFolderPath.Open();
			ServerFolderPath.Display();
		}
	}
	if (ServerFolderPath.IsOpened()) 
	{
		ServerFolderPath.SetWindowPos(io.DisplaySize.x, io.DisplaySize.y);
		ServerFolderPath.Display();
		if (ServerFolderPath.HasSelected())
		{
			std::string File = ServerFolderPath.GetSelected().string();
			if (!File.contains("9Data")) 
			{
				NiMessageBox::DisplayMessage("Please Select the 9Data Folder!", "Info");
				ClientFolderPath.ClearSelected();
				return;
			}
			PgUtil::ServerFolderPath = File;
			Settings::SaveSettings();
			NiMessageBox::DisplayMessage("Updated Settings\n Restart Tool now!", "Info");
			ServerFolderPath.Close();
		}

	}
}
