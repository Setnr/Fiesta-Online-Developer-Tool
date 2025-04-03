#pragma once
#include <Scene/ScreenElements/ScreenElement.h>
#include <ImGui/imgui.h>
#include <ImGui/imfilebrowser.h>
#include <Data/IngameWorld/IngameWorld.h>
#include <NiDX9Renderer.h>
#include "Data/NiCustom/NiSHMDPickable.h"
class ReplaceObjects : public ScreenElement
{
	NiDeclareRTTI;
public:
	ReplaceObjects(IngameWorldPtr world, std::vector<NiPickablePtr> OldNodes) : _FileBrowser(ImGuiFileBrowserFlags_NoTitleBar | ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_NoModal)
	{
		_FileBrowser.SetWindowPos(0, 50);
		_World = world;
		_OldNodes = OldNodes;
		_FileBrowser.Open();
		_FileBrowser.SetPwd(AddSingleObject::GetInitPath());
		_FileBrowser.SetTypeFilters({ ".nif" });
		Camera = PgUtil::CreateNewCamera();
		_BaseNode = NiNew NiNode;
		Light = NiNew NiAmbientLight();
		Light->SetAmbientColor(NiColor(0.792157f, 0.792157f, 0.792157f));
		Light->SetDiffuseColor(NiColor::WHITE);
		Light->SetSpecularColor(NiColor::BLACK);
		Light->AttachAffectedNode(_BaseNode);
		_BaseNode->AttachProperty(NiNew NiVertexColorProperty);
		_BaseNode->AttachChild(Light);

		_BaseNode->UpdateEffects();
		_BaseNode->UpdateProperties();
		_BaseNode->Update(0.f);

		Camera->SetTranslate(NiPoint3(250.f, 250.f, 250.f));
		Camera->Update(0.0f);
		Camera->LookAtWorldPoint(NiPoint3::ZERO, NiPoint3(0.0, 0.0, 1.0));
		Camera->Update(0.0f);
	}
	virtual bool Draw()
	{
		_FileBrowser.Display();

		if (!_FileBrowser.IsOpened())
			return false;

		if (CurPath != _FileBrowser.GetSelected())
		{
			CurPath = _FileBrowser.GetSelected().string();
			if (CurPath.ends_with(".nif"))
			{
				NiSHMDPickablePtr ptr = PgUtil::LoadNifFile<NiSHMDPickable>(CurPath.c_str(), 0);
				ptr->ExtractCollision();
				if (NiIsKindOf(NiPickable, ptr))
				{
					_BaseNode->DetachChild(_Obj);
					_Obj = NiSmartPointerCast(NiPickable, ptr);
					_Obj->SetFilePathOrName(CurPath);

					auto io = ImGui::GetIO();
					NiPoint3 kOrigin, kDir;
					if (Camera->WindowPointToRay(io.DisplaySize.x * 0.65f, io.DisplaySize.y * 0.4f, kOrigin, kDir))
					{
						_Obj->SetTranslate(kOrigin + (kOrigin.x * kDir * 6.f)); //Set Position on Screen of Object
					}
					auto len = PgUtil::PathFromClientFolder("").length() + 1;
					_Obj->SetName(CurPath.substr(len).c_str());
					_BaseNode->AttachChild(_Obj);

					_BaseNode->UpdateEffects();
					_BaseNode->UpdateProperties();
					_BaseNode->Update(0.f);


					//ReScale Object to make sure it is visible 
					Scale = _Obj->GetScale();
					float NewScale = 250.f / _Obj->GetWorldBound().GetRadius();
					_Obj->SetScale(NewScale);
				}
			}
		}
		if (_Obj)
		{
			NiMatrix3 mat = _Obj->GetRotate();
			float Roll, Yaw, Pitch;
			mat.ToEulerAnglesXYZ(Roll, Yaw, Pitch);
			mat.FromEulerAnglesXYZ(Roll, Yaw, Pitch + 0.01f);
			_Obj->SetRotate(mat);
			_Obj->Update(0.0f);
		}
		if (_BaseNode)
		{
			NiRenderer* pkRenderer = NiDX9Renderer::GetRenderer();
			pkRenderer->ClearBuffer(0, NiRenderer::CLEAR_ZBUFFER);
			NiVisibleArray m_kVisible;
			NiCullingProcess m_spCuller(&m_kVisible);
			NiDrawScene(Camera, _BaseNode, m_spCuller);
		}
		if (_FileBrowser.HasSelected())
		{
			_Obj->SetScale(Scale);

			std::vector<NiPickablePtr> NewNodes;
			for (auto node : _OldNodes) 
			{
				NiObjectPtr NewObj = _Obj->Clone();
				if (NiIsKindOf(NiPickable, NewObj)) 
				{
					NiPickablePtr NewPick = NiSmartPointerCast(NiPickable, NewObj);
					NewPick->SetTranslate(node->GetTranslate());
					NewPick->SetRotate(node->GetRotate());
					NewPick->SetScale(node->GetScale());
					NewNodes.push_back(NewPick);
				}
			}

			_World->ReplaceObjects(_OldNodes, NewNodes);

			_World = NULL;
			_BaseNode = NULL;
			_Obj = NULL;
			Camera = NULL;
			Light = NULL;

			AddSingleObject::SetInitPath(_FileBrowser.GetPwd().string());
			return false;
		}
		return true;
	}

private:
	IngameWorldPtr _World;
	std::vector<NiPickablePtr> _OldNodes;

	ImGui::FileBrowser _FileBrowser;
	std::string CurPath = "";
	NiNodePtr _BaseNode;
	NiPickablePtr _Obj;
	NiCameraPtr Camera;
	NiAmbientLightPtr Light;
	float Scale;
};

