#pragma once
#include "PgUtil.h"
#include <NiNode.h>
#include "ImGui/imgui.h"
#include "ImGui/imfilebrowser.h"
class NiFileLoader 
{
public:
	NiFileLoader() : fileDialog(ImGuiFileBrowserFlags_NoTitleBar | ImGuiFileBrowserFlags_CloseOnEsc)
	{
		fileDialog.SetTitle("Select Nif-File");
		fileDialog.SetTypeFilters({ ".nif" });
		fileDialog.SetPwd(PgUtil::CreateFullFilePathFromBaseFolder(""));
	}
	~NiFileLoader() 
	{
	};

	void Prepare(NiNodePtr LoadToScene, bool Pickable = false, NiPoint3 pos = NiPoint3::ZERO)
	{
		fileDialog.Open();
		_Pickable = Pickable;
		_Pos = pos;
		_LoadToScene = LoadToScene;
	}

	bool DrawImGui() 
	{
		fileDialog.Display();
		return fileDialog.HasSelected();
	}
	NiNodePtr Load() 
	{
		std::string File = fileDialog.GetSelected().string();
		std::string BasePath = PgUtil::CreateFullFilePathFromBaseFolder("");
		NiNodePtr nif = PgUtil::LoadNifFile(File.c_str(), 0, _Pickable);
		if(NiIsKindOf(NiPickable,nif))
		{
			nif->SetTranslate(_Pos);
			NiPickable* ptr = (NiPickable*)&*nif;
			ptr->SetSHMDPath(File.substr(BasePath.length() + 1).c_str());
		}else
			nif->SetName(File.substr(BasePath.length() + 1).c_str());
		_LoadToScene->AttachChild(nif);
		_LoadToScene->UpdateEffects();
		_LoadToScene->UpdateProperties();
		_LoadToScene->Update(0.0f);
		_LoadToScene = NULL; // This Line resolves a NiDx9RendererCrash xD
		//The Renderer crashes because he releases its Shaders but if this is not cleared it still holds
		// a deep reference to a shader which cases the RendererDestructor to crash
		fileDialog.ClearSelected();
		return nif;
	}
private:
	NiNodePtr _LoadToScene;
	NiPoint3 _Pos;
	bool _Pickable;
	ImGui::FileBrowser fileDialog;
};