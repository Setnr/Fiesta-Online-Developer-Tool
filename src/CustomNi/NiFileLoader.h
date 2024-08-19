#pragma once
#include "PgUtil.h"
#include <NiNode.h>
#include "ImGui/imgui.h"
#include "ImGui/imfilebrowser.h"
#include "../CustomNi/NiBoundingBox.h"
class NiFileLoader 
{
public:
	NiFileLoader() : fileDialog(ImGuiFileBrowserFlags_NoTitleBar | ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_NoModal)
	{
		fileDialog.SetTitle("Select Nif-File");
		fileDialog.SetTypeFilters({ ".nif" });
		fileDialog.SetPwd(PgUtil::CreateFullFilePathFromBaseFolder(""));
		fileDialog.SetWindowPos(400, 300);
		fileDialog.SetWindowSize(400, 350);
		BaseNode = NiNew NiNode;
		Camera = NiNew NiCamera;
		NiFrustum WorldFrustum(-0.25, 0.25, 0.1875, -0.1875, 1.0, 6000.0, false);
		float m_fCameraFOV = 50.0f;
		float fTop = m_fCameraFOV / 180.0 * 0.5 * NI_PI;
		float fTopa = tan(fTop);
		float v4 = fTopa;
		float fRight = fTopa;
		float v5 = 1600; /*TODO DYNAMIC*/
		float v6 = 900;
		float fTopb;
		if (900 >= (double)1600)
		{
			fTopb = v4 * (v6 / v5);
			v4 = fTopb;
		}
		else
		{
			fRight = v5 / v6 * v4;
		}
		float fTopc = -fRight;
		float v7 = fTopc;
		WorldFrustum.m_fLeft = fTopc;
		WorldFrustum.m_fRight = fRight;
		WorldFrustum.m_fTop = v4;
		float fTopd = -v4;
		WorldFrustum.m_fBottom = fTopd;
		Camera->SetViewFrustum(WorldFrustum); //World::SetCameraFOV
		m_spAmbientLight = NiNew NiAmbientLight;
		m_spAmbientLight->SetAmbientColor(NiColor(0.792157f, 0.792157f, 0.792157f));
		m_spAmbientLight->SetDiffuseColor(NiColor::WHITE);
		m_spAmbientLight->SetSpecularColor(NiColor::BLACK);

		m_spAmbientLight->AttachAffectedNode(BaseNode);

		BaseNode->AttachProperty(NiNew NiVertexColorProperty);
		BaseNode->AttachChild(m_spAmbientLight);
		BaseNode->UpdateEffects();
		BaseNode->UpdateProperties();
		BaseNode->Update(0.f);

		Camera->SetTranslate(NiPoint3(250.f, 250.f, 250.f));
		Camera->Update(0.0f);
		Camera->LookAtWorldPoint(NiPoint3::ZERO, TerrainWorld::ms_kUpDir);
		Camera->Update(0.0f);

	}
	~NiFileLoader() 
	{
	};

	void Prepare(NiNodePtr LoadToScene, bool Pickable = false, NiPoint3 pos = NiPoint3::ZERO)
	{
		fileDialog.Open();
		_Pickable = Pickable;/* Change back*/
		_Pos = pos;
		_LoadToScene = LoadToScene;
		//BaseNode->DetachAllEffects();
		//BaseNode->DetachAllProperties();
		if (SelectedObj)
			BaseNode->DetachChild(SelectedObj);
		SelectedObj = NULL;
	}
	bool ShowInternalScene() { return fileDialog.IsOpened(); }
	void Update(float fTime) {
		Camera->Update(fTime);
		if (SelectedObj)
		{
			NiMatrix3 mat = SelectedObj->GetRotate();
			float Roll, Yaw, Pitch;
			mat.ToEulerAnglesXYZ(Roll, Yaw, Pitch);
			mat.FromEulerAnglesXYZ(Roll, Yaw, Pitch + 0.01f);
			SelectedObj->SetRotate(mat);
		}
		BaseNode->Update(fTime);
	}
	void DrawScene(NiRenderer* renderer) 
	{
		//Currently Interacts with Real Map, No Clue how to fix that xD
		if (!SelectedObj)
			return;

		NiVisibleArray m_kVisible;
		NiCullingProcess m_spCuller(&m_kVisible);
		NiDrawScene(Camera, BaseNode, m_spCuller,&m_kVisible);
	}

	bool DrawImGui() 
	{
		fileDialog.Display();

		if (fileDialog.GetPwd() != fileDialog.GetSelected())
		{
			std::string File = fileDialog.GetSelected().string();
			std::string BasePath = PgUtil::CreateFullFilePathFromBaseFolder("");
			std::string UsePath = File.substr(BasePath.length() + 1);
			if(!SelectedObj || UsePath != LastUsePath)
			{
				LastUsePath = UsePath;
				NiNodePtr nif = PgUtil::LoadNifFile(File.c_str(), 0, _Pickable);
				
				if (NiIsKindOf(NiPickable, nif))
				{
					nif->SetTranslate(NiPoint3::ZERO);
					NiPickable* ptr = (NiPickable*)&*nif;
					ptr->SetSHMDPath(UsePath.c_str());
				}
				else
					nif->SetName(UsePath.c_str());
				if(SelectedObj)
					BaseNode->DetachChild(SelectedObj);

				SelectedObj = nif;

				BaseNode->AttachChild(SelectedObj);

				NiPoint3 MinPoint(0.f, 0.f, 0.f);
				NiPoint3 MaxPoint(0.f, 0.f, 0.f);
				NiMatrix3 WorldRotate;
				NiBoundingBox::GetNewVerticesData(SelectedObj, MinPoint, MaxPoint, WorldRotate);

				Camera->SetTranslate(MaxPoint * 1.5f);
				Camera->Update(0.0f);
				Camera->LookAtWorldPoint(NiPoint3::ZERO, TerrainWorld::ms_kUpDir);
				Camera->Update(0.0f);

				auto io = ImGui::GetIO();
				NiPoint3 kOrigin, kDir;
				if (this->Camera->WindowPointToRay(io.DisplaySize.x * 0.75f, io.DisplaySize.y * 0.5f, kOrigin, kDir)) 
				{
					SelectedObj->SetTranslate(kOrigin + (kOrigin.x	* kDir * 6.f));
				}

				BaseNode->UpdateEffects();
				BaseNode->UpdateProperties();
				BaseNode->Update(0.0f);
				
			}
		}

		return fileDialog.HasSelected();
	}
	NiNodePtr Load() 
	{
		if(_LoadToScene)
		{
			SelectedObj->SetTranslate(_Pos);
			_LoadToScene->AttachChild(SelectedObj);
			_LoadToScene->UpdateEffects();
			_LoadToScene->UpdateProperties();
			_LoadToScene->Update(0.0f);
			_LoadToScene = NULL; // This Line resolves a NiDx9RendererCrash xD
			//The Renderer crashes because he releases its Shaders but if this is not cleared it still holds
			// a deep reference to a shader which cases the RendererDestructor to crash
		}
		fileDialog.ClearSelected();

		NiMatrix3 mat;
		mat.FromEulerAnglesXYZ(0.f, 0.f, 0.f);
		SelectedObj->SetRotate(mat);

		NiNodePtr obj = SelectedObj;
		BaseNode->DetachChild(SelectedObj);
		SelectedObj = NULL;
		return obj;
	}
private:
	NiNodePtr _LoadToScene;
	NiPoint3 _Pos;
	bool _Pickable;
	ImGui::FileBrowser fileDialog;
	NiNodePtr BaseNode;
	NiNodePtr SelectedObj;
	NiCameraPtr Camera;
	std::string LastUsePath;
	NiAmbientLightPtr m_spAmbientLight;

	
};


class IniFileLoader
{
public:
	IniFileLoader() : fileDialog(ImGuiFileBrowserFlags_NoTitleBar | ImGuiFileBrowserFlags_CloseOnEsc)
	{
		fileDialog.SetTitle("Select Ini-File");
		fileDialog.SetTypeFilters({ ".ini" });
		fileDialog.SetPwd(PgUtil::CreateFullFilePathFromBaseFolder(""));
	}
	~IniFileLoader()
	{
	};

	void Prepare()
	{
		fileDialog.Open();
		
	}

	bool DrawImGui()
	{
		fileDialog.Display();
		return fileDialog.HasSelected();
	}
	std::string Load()
	{
		std::string path = fileDialog.GetSelected().string();
		fileDialog.ClearSelected();
		return path;
		
	}
	std::string GetFolderPath() { return fileDialog.GetPwd().string(); }
private:
	ImGui::FileBrowser fileDialog;
};