#pragma once
#include "PgUtil.h"
#include <NiNode.h>
#include "ImGui/imgui.h"
#include "ImGui/imfilebrowser.h"
#include "../NiApplication/FiestaOnlineTool.h"
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
		pkScreenElement = NULL;
	}

	bool DrawImGui() 
	{
		fileDialog.Display();
		static bool LastOpenStatus = false;
		if (LastOpenStatus != fileDialog.IsOpened()) 
		{
			LastOpenStatus = fileDialog.IsOpened();
			if (!LastOpenStatus && pkScreenElement)
				FiestaOnlineTool::RemoveScreenElemets(pkScreenElement);
		}
		if (fileDialog.GetPwd() != fileDialog.GetSelected())
		{
			if(!pkScreenElement || !pkScreenElement->GetName().Contains(fileDialog.GetSelected().string().c_str()))
			{
				NiSourceTexturePtr ptr = NiSourceTexture::Create(fileDialog.GetSelected().string().c_str());
				if (pkScreenElement)
					FiestaOnlineTool::RemoveScreenElemets(pkScreenElement);
				pkScreenElement = PgUtil::CreateScreenElement(256, 256, ptr);
				pkScreenElement->SetName(fileDialog.GetSelected().string().c_str());
				FiestaOnlineTool::AddScreenElemets(pkScreenElement);
				auto& io = ImGui::GetIO();
				float fLeft = 1.f - 256 / io.DisplaySize.x;
				float fTop = 1.f - 256 / io.DisplaySize.y;
				float fBottom = 1.f;
				float fRight = 1.f;
				pkScreenElement->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
				pkScreenElement->SetVertex(0, 1, NiPoint2(fRight, fBottom));
				pkScreenElement->SetVertex(0, 2, NiPoint2(fRight, fTop));
				pkScreenElement->SetVertex(0, 3, NiPoint2(fLeft, fTop));
				pkScreenElement->UpdateBound();
			}
		}
		return fileDialog.HasSelected();
	}
	std::string Load() 
	{
		FiestaOnlineTool::RemoveScreenElemets(pkScreenElement);
		std::string File = fileDialog.GetSelected().string();
		std::string BasePath = PgUtil::CreateFullFilePathFromBaseFolder("");
		fileDialog.ClearSelected();
		return ".\\" + File.substr(BasePath.length() + 1);
	}
private:
	ImGui::FileBrowser fileDialog;
	NiScreenElementsPtr pkScreenElement;
};