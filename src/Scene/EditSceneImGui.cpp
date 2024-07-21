#pragma once
#include "EditScene.h"

#include <future>
#include "../Data/SHNManager.h"

#include "ImGuiConstantFuncs.h"
#include "NiFileLoader.h"

#include <NiPick.h>
#include "FiestaOnlineTool.h"
#include "MapCreateScene.h"


glm::vec4 ConvertQuatToAngleAxis(glm::quat q)
{
	if (q.w == 1.f)
	{
		return { 0.f, 1.f, 0.f, 0.f };
	}

	float_t angle_rad = acos(q.w) * 2;

	float_t x = q.x / sin(angle_rad / 2);
	float_t y = q.y / sin(angle_rad / 2);
	float_t z = q.z / sin(angle_rad / 2);

	return { angle_rad, x, y, z };
}

void EditScene::DrawImGui()
{
    FiestaScene::DrawImGui();
	if(Settings::GeneralView() && CurrentEditMode != EditMode::HTDG)
		DrawGeneralInfoWindow();
	switch (CurrentEditMode)
	{
	case EditMode::SHBD:
		DrawSHBDEditor();
		break;

	case EditMode::SHMD:
		DrawSHMDWindow();
		DrawGizmo();
		MiddleMouseButtonMenu();
		break;
	case EditMode::HTDG:
	{
		HTDBrushPtr n = _HTDBrush->Draw();
		if (n) 
		{
			_HTDBrush = n;
		}
	}
		break;
	default:
		break;
	}

	_MapMenu.RenderMenu();
	if (ShowSettingsMenu)
		ShowSettings();
	if (ShowAbout)
		ShowAboutWindow();
}

void EditScene::CreateMenuBar()
{
    if (ImGui::BeginMenu("File"))
    {
		_MapMenu.ShowMenuBar();
        if (ImGui::MenuItem("Settings"))
        {
            ShowSettingsMenu = true;
        }
        if (ImGui::MenuItem("Save SHMD", 0, false, kWorld != NULL))
        {
            kWorld->SaveSHMD();
        }
        if (ImGui::MenuItem("Save SHBD", 0, false, kWorld != NULL))
        {
            kWorld->SaveSHBD();
        }
		if (ImGui::MenuItem("Save HTDG", 0, false, kWorld != NULL))
		{
			kWorld->SaveHTDG();
		}
		if (ImGui::MenuItem("Export HTD as NIF", 0, false, kWorld != NULL))
		{
			PgUtil::SaveNode(kWorld->GetFolderPath() + "Export.nif", kWorld->GetTerrainScene());
		}
        ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("General View"))
		{
			Settings::SwitchGeneralView();
		}
		
		ImGui::EndMenu();
	}
	ImGui::SameLine(ImGui::GetWindowWidth() - 130);
	std::string mode = GetEditMode();
	ImVec2 size(100, 20);
	if (ImGui::Button(mode.c_str(),size)) 
	{
		ImGui::OpenPopup("ChangeEditMode");
	}
	ImGui::SameLine(ImGui::GetWindowWidth() - 130);
	if (ImGui::BeginPopup("ChangeEditMode")) 
	{
		if (ImGui::Button("None"))
		{
			UpdateEditMode(EditMode::None);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("SHMD"))
		{
			UpdateEditMode(EditMode::SHMD);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("SHBD"))
		{
			UpdateEditMode(EditMode::SHBD);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("HTDG"))
		{
			UpdateEditMode(EditMode::HTDG);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::IsKeyPressed((ImGuiKey)VK_TAB, false))
	{
		int m = (int)CurrentEditMode;
		m++;
		UpdateEditMode((EditMode)m);
	}
	ImGui::SameLine(ImGui::GetWindowWidth() - 30);
	ImGui::Text("(?)");
	if (ImGui::IsItemClicked()) 
	{
		ShowAbout = true;
	}
}


void EditScene::ShowAboutWindow() 
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 Size(500, 250);
	ImVec2 Pos(io.DisplaySize.x / 2 - Size.x / 2, io.DisplaySize.y / 2 - Size.y / 2);
	ImGui::SetNextWindowPos(Pos);
	ImGui::SetNextWindowSize(Size);
	if (ImGui::Begin("About", &ShowAbout,ImGuiWindowFlags_NoCollapse)) 
	{
		ImGui::Text("Fiesta Online DeveloperTool by SetNr12");
		ImGui::Text("Switch Current Mode with Tab");
		ImGui::Text("Press R to Reset the Camera");
		ImGui::Text("Controls for the current mode:");
		ImGui::Text("");
		switch (CurrentEditMode)
		{
		case SHMD:
			ImGui::Text("Move Camera with WASD");
			ImGui::Text("Rotate Camera with Right Click");
			ImGui::Text("Open SHMDMenu with Middle Mouse Click");
			ImGui::Text("Select Objects with Left Click");
			ImGui::Text("Copy and Paste with ctrl + c/v");
			ImGui::Text("Move Up/Down with Q/E");
			ImGui::Text("Switch to Translate Mode ctrl + w");
			ImGui::Text("Switch to Rotate Mode ctrl + e");
			ImGui::Text("Enable/Disable Snap Mode ctrl + d");
			break;
		case SHBD:
			ImGui::Text("Move Camera with WASD");
			ImGui::Text("Rotate Camera with Right Click");
			ImGui::Text("Move Up/Down with Q/E");
			ImGui::Text("Scroll to Move SHBD Up/Down");
			ImGui::Text("Change SHBD with Left Click");
			break;

		case None:
		default:
			ImGui::Text("Select a Mode to see the matching controls");
			break;
		}
		ImGui::End();
	}
}

void EditScene::MiddleMouseButtonMenu()
{
	if (MiddleMouseLoader.DrawImGui())
	{
		NiNodePtr obj = MiddleMouseLoader.Load();
		if (NiIsKindOf(NiPickable, obj))
			SelectedObj = (NiPickable*)(NiNode*)obj;

	}
	if (ImGui::BeginPopupContextVoid(0, ImGuiPopupFlags_MouseButtonMiddle))
	{
		if (ImGui::Selectable("Add Sky"))
			MiddleMouseLoader.Prepare(kWorld->GetSkyNode());
		if (ImGui::Selectable("Add Water"))
			MiddleMouseLoader.Prepare(kWorld->GetWaterNode());
		if (ImGui::Selectable("Add Global Object"))
			MiddleMouseLoader.Prepare(kWorld->GetGroundObjNode());
		if (ImGui::Selectable("Add Movable Object"))
		{
			NiPoint3 kOrigin, kDir;
			long X, Y;
			FiestaOnlineTool::GetMousePosition(X, Y);
			if (this->Camera->WindowPointToRay(X, Y, kOrigin, kDir))
			{
				NiPick _Pick;
				_Pick.SetPickType(NiPick::FIND_FIRST);
				_Pick.SetSortType(NiPick::SORT);
				_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
				_Pick.SetFrontOnly(true);
				_Pick.SetReturnNormal(true);
				_Pick.SetObserveAppCullFlag(true);
				_Pick.SetTarget(kWorld->GetWorldScene());
				if (_Pick.PickObjects(kOrigin, kDir, true))
				{
					NiPick::Results& results = _Pick.GetResults();
					for (int i = 0; i < results.GetSize(); i++)
					{
						auto result = results.GetAt(i);
						MiddleMouseLoader.Prepare(kWorld->GetGroundObjNode(), PICKABLEOBJECTS, result->GetIntersection());
					}
				}
				else
				{
					_Pick.SetTarget(kWorld->GetWorldScene());
					if (_Pick.PickObjects(kOrigin, kDir, true))
					{
						NiPick::Results& results = _Pick.GetResults();
						for (int i = 0; i < results.GetSize(); i++)
						{
							auto result = results.GetAt(i);
							MiddleMouseLoader.Prepare(kWorld->GetGroundCollidee(), PICKABLEOBJECTS, result->GetIntersection());
						}

					}
				}
			}
		}
		if (SelectedObj)
		{
			if (ImGui::Selectable("Copy Selected Obj"))
			{
				if (NiIsKindOf(NiPickable, SelectedObj))
				{
					NiPickablePtr Obj = (NiPickable*)SelectedObj->Clone();
					NiNodePtr ptr = (NiNode*)&*Obj;
					kWorld->AttachGroundObj(ptr);

					Obj->SetName(SelectedObj->GetName());
					Obj->SetDefaultCopyType(Obj->COPY_UNIQUE);

					Obj->SetSelectiveUpdateRigid(true);
					auto node = kWorld->GetGroundCollidee();
					node->UpdateEffects();
					node->UpdateProperties();
					node->Update(0.0);
					SelectedObj = Obj;
				}
				else
					LogError("Can´t clone selcted Object");
			}
			if (ImGui::Selectable("Delete Selected Obj"))
			{
				SelectedObj->GetParent()->DetachChild(SelectedObj);
				SelectedObj = NULL;
			}
		}
		ImGui::EndPopup();
	}
}

void EditScene::DrawGizmo() 
{
	if (!SelectedObj)
		return;

	/*
	Huge Credits to Maki for helping me with this Code
	by giving me his :)
	*/


	float_t matrix[16]{};
	float matrixScale[3] = { 1.f, 1.f, 1.f };
	glm::vec3 tmpRotation{};
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::BeginFrame();
	NiPickable* SelectedNode = SelectedObj;


	NiPoint3 target = Camera->GetTranslate() + Camera->GetWorldDirection() * 10.f;
	const auto eye = Camera->GetTranslate();
	const auto up = glm::vec3{ 0, 0, 1 };

	const auto view = glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(target.x, target.y, target.z), up);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(kWorld->GetFOV()), (((float_t)io.DisplaySize.x / (float_t)io.DisplaySize.y)), Camera->GetViewFrustum().m_fNear, Camera->GetViewFrustum().m_fFar);

	ImGuizmo::RecomposeMatrixFromComponents(&SelectedNode->GetTranslate().x, &tmpRotation[0], matrixScale, (float*)matrix);

	ImGuizmo::SetRect((float_t)0, (float_t)0, (float_t)io.DisplaySize.x, (float_t)io.DisplaySize.y);
	if(SnapMove)
		ImGuizmo::Manipulate(&view[0][0], &projectionMatrix[0][0], OperationMode, ImGuizmo::MODE::WORLD, (float*)matrix, nullptr, &SnapSize.x);
	else
		ImGuizmo::Manipulate(&view[0][0], &projectionMatrix[0][0], OperationMode, ImGuizmo::MODE::WORLD, (float*)matrix, nullptr, nullptr);
	
	ImGuizmo::DecomposeMatrixToComponents((float*)matrix, (float*)&SelectedNode->GetTranslate().x, &tmpRotation[0], matrixScale);

	SelectedObjAngels -= tmpRotation;

	if (abs(SelectedObjAngels[0]) > 180.f)
	{
		SelectedObjAngels[0] = -SelectedObjAngels[0] + 2 * fmod(SelectedObjAngels[0], 180.f);
	}

	if (abs(SelectedObjAngels[1]) > 180.f)
	{
		SelectedObjAngels[1] = -SelectedObjAngels[1] + 2 * fmod(SelectedObjAngels[1], 180.f);
	}

	if (abs(SelectedObjAngels[2]) > 180.f)
	{
		SelectedObjAngels[2] = -SelectedObjAngels[2] + 2 * fmod(SelectedObjAngels[2], 180.f);
	}

	glm::vec4 angleAxis = ConvertQuatToAngleAxis(glm::quat(glm::radians(SelectedObjAngels)));

	NiMatrix3 m;
	m.MakeRotation(angleAxis[0], angleAxis[1], angleAxis[2], angleAxis[3]);

	//float angle, x, y, z;
	//SelectedNode->GetRotate().ExtractAngleAndAxis(angle, x, y, z);

	SelectedNode->SetRotate(m);
}

void EditScene::ShowSettings() 
{
	static ImGui::FileBrowser fileDialog(ImGuiFileBrowserFlags_SelectDirectory);
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
		Settings::SetClientPath(File);
		fileDialog.ClearSelected();
	}
	if (ImGui::Begin("Settings", &ShowSettingsMenu));
	{
		std::string ClientPath = "ClientPath: " + PgUtil::CreateFullFilePathFromBaseFolder("");
		ImGui::Text(ClientPath.c_str()); ImGui::SameLine();
		if (ImGui::Button("PlaceHolder"))
		{
			fileDialog.SetTitle("Select Game-Client-Folder");
			fileDialog.SetTypeFilters({ ".exe", ".bin" });
			fileDialog.SetPwd(PgUtil::CreateFullFilePathFromBaseFolder(""));
			fileDialog.Open();
		}
		static const char* items[]{ "640 x 360","1024 x 576","1138 x 640", "1600 x 900","1920 x 1080", "2560 x 1440" };
		int Selecteditem = 0;

		for (int i = 0; i < IM_ARRAYSIZE(items); i++)
		{
			if (std::string(items[i]) == Settings::GetResoultion())
				Selecteditem = i;
		}
		if (ImGui::Combo("ScreenResolution", &Selecteditem, items, IM_ARRAYSIZE(items)))
		{
			Settings::SetResolution(std::string(items[Selecteditem]));
			LogInfo("To apply Resoultion Changes please restart the Tool");
		}
		float fps = FiestaOnlineTool::GetFPSCap();
		if (ImGui::DragFloat("FPS Cap", &fps, 1.f, 30.f, 144.f))
			FiestaOnlineTool::SetFPSCap(fps);
		/*bool FullScreen = Settings::FullScreen();
		if (ImGui::Checkbox("FullScreen", &FullScreen))
		{
			Settings::SetFullScreen(FullScreen);
		}*/
		if (ImGui::Button("Safe Settings"))
		{
			Settings::SaveSettings();
			//FiestaOnlineTool::RecreateRenderer = true;
		}
		bool ShowSHMD = Settings::ShowSHMD();
		if (ImGui::Checkbox("Show SHMD in HTDG-Editor", &ShowSHMD))
			Settings::SetShowSHMD(ShowSHMD);
		ImGui::End();
	}
}

void EditScene::DrawGeneralInfoWindow() 
{
	if (!kWorld)
		return;
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(250, 450));
	if (ImGui::Begin("Scene Overview", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoCollapse))
	{
		DrawSceneNode(kWorld->GetWorldScene());
		ImGui::End();
	}
	if (GeneralInfoNode)
		DrawGeneralInfoNode();
}

void EditScene::DrawGeneralInfoNode()
{
	ImGui::SetNextWindowPos(ImVec2(0, 470));
	ImGui::SetNextWindowSize(ImVec2(250, 250));
	if (ImGui::Begin("Node Overview", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoCollapse))
	{
		std::string Name;
		if (GeneralInfoNode->GetName())
			Name = GeneralInfoNode->GetName();
		else
			Name = "-";
		ImGui::Text("Name: "); ImGui::SameLine(); ImGui::Text(Name.c_str());

		auto Translate = GeneralInfoNode->GetTranslate();
		auto Rotation = GeneralInfoNode->GetRotate();
		float angle, x, y, z;
		Rotation.ExtractAngleAndAxis(angle, x, y, z);
		NiPropertyList& PropertyList = GeneralInfoNode->GetPropertyList();
		ImGui::Text("Pos \n\tX: %f, \n\tY: %f, \n\tZ: %f", Translate.x, Translate.y, Translate.z);
		ImGui::Text("Rotate \n\tAngle: %f, \n\tX: %f, \n\tY: %f, \n\tZ: %f", angle, x, y, z);
		ImGui::Text("Scale: %f", GeneralInfoNode->GetScale());
		ImGui::Text("Properties: %i", PropertyList.GetSize());
		ImGui::Text("Children: %i", GeneralInfoNode->GetChildCount());
		if (NiIsKindOf(NiPickable, GeneralInfoNode)) 
		{
			NiPickablePtr ptr = (NiPickable*)&*GeneralInfoNode;
			ImGui::Text("SHMD File Path \n\t%s", ptr->GetSHMDPath().c_str());
		}
		ImGui::End();
	}
}

void EditScene::DrawSceneNode(NiNodePtr Node)
{
	std::string Name = "";
	if (Node->GetName())
		Name = Node->GetName();
	else
		Name = "-";
	std::string Name2 = "##" + std::to_string((int)&*Node);
	Name += Name2;
	bool ExpandNode = ImGui::TreeNode(Name2.c_str());
	
	ImGui::SameLine();
	bool selected = false;
	if (Node == GeneralInfoNode)
		selected = true;
	if (ImGui::Selectable(Name.c_str(), selected, 0, ImVec2(150, 0)))
	{
		UpdateGeneralInfoNode(Node);
		LookAndMoveAtWorldPoint(GeneralInfoNode->GetTranslate());
		if (CurrentEditMode == EditMode::SHMD && NiIsKindOf(NiPickable, Node))
		{
			SelectedObj = (NiPickable*) &*Node;
		}
	}
	if (!ExpandNode)
		return;
	for (int i = 0; i < Node->GetChildCount(); i++)
	{
		auto child = Node->GetAt(i);
		if (child && NiIsKindOf(NiNode, Node))
		{
			NiNodePtr ptr = (NiNode*)child;
			DrawSceneNode(ptr);
		}
	}
	ImGui::TreePop();
}

void EditScene::DrawSHMDWindow() 
{
	ImGuiIO& io = ImGui::GetIO();
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

	int h = 295;
	int w = 578;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w, io.DisplaySize.y - h));
	ImGui::SetNextWindowSize(ImVec2(w, h));


	ImGui::Begin("SHMD-Editor", NULL, flags);
	DrawSHMDHeader("Sky", kWorld->GetSkyNode());
	DrawSHMDHeader("Water", kWorld->GetWaterNode());
	DrawSHMDHeader("GroundObject", kWorld->GetGroundObjNode());
	ImGui::ColorEdit3("Global Light", (float*)&kWorld->GetAmbientLightAmbientColor().r);
	ImGui::ColorEdit3("Fog Color", (float*)&kWorld->GetFogColor().r);

	float depth = kWorld->GetFogDepth();
	if (ImGui::DragFloat("Fog Alpha", &depth, 0.01, 0.f, 1.0f))
		kWorld->SetFogDepth(depth);
	ImGui::ColorEdit3("Background Color", (float*)&kWorld->GetBackgroundColor().r);
	ImGui::DragFloat("Frustum", &kWorld->GetWorldFrustum().m_fFar, 10.0f, 0.0f);

	ImGui::ColorEdit3("AmbientLight Color", (float*)&kWorld->GetMapDirectionalLightAmbientColor().r);
	ImGui::ColorEdit3("DiffuseLight Color", (float*)&kWorld->GetMapDirectionalLightDiffuseColor().r);
	

	ImGui::End();
	if (SHMDWindowLoader.DrawImGui())
	{
		NiNodePtr obj = SHMDWindowLoader.Load();
		if (NiIsKindOf(NiPickable, obj))
		{
			auto Parent = SelectedObj->GetParent();
			obj->SetTranslate(SelectedObj->GetTranslate());
			obj->SetRotate(SelectedObj->GetRotate());
			obj->SetScale(SelectedObj->GetScale());
			Parent->DetachChild(SelectedObj);
			Parent->AttachChild(obj);
			Parent->UpdateEffects();
			Parent->UpdateProperties();
			Parent->Update(0.0f);
			Parent->CompactChildArray();
			SelectedObj = (NiPickable*)(NiNode*)obj;
			UpdateGeneralInfoNode((NiNode*)&*SelectedObj);
		}
	}

	if (SelectedObj)
	{
		{
			auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

			int h2 = 150 + h;
			int w2 = 319;

			ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w2, io.DisplaySize.y - h2));
			ImGui::SetNextWindowSize(ImVec2(w2, h2 - h));

			ImGui::Begin("Object-Editor", NULL, flags);
			ImGui::RadioButton("Translate", (int*)&OperationMode, ImGuizmo::OPERATION::TRANSLATE);
			ImGui::SameLine();
			ImGui::RadioButton("Rotate", (int*)&OperationMode, ImGuizmo::OPERATION::ROTATE);

			ImGui::DragFloat3("Position", (float*)&SelectedObj->GetTranslate().x);
			ImGui::DragFloat3("Rotation", &SelectedObjAngels[0]);
			float Scale = SelectedObj->GetScale();
			if (ImGui::DragFloat("Scale", &Scale, 0.01f, 0.0f, 5.0f))
				SelectedObj->SetScale(Scale);

			ImGui::Checkbox("Snaping Movement", &SnapMove);
			if (ImGui::DragFloat("Snaping Step", &SnapSize.x))
			{
				SnapSize.y = SnapSize.x;
				SnapSize.z = SnapSize.x;
			}

			if (ImGui::Button("Change NIF of Object"))
				SHMDWindowLoader.Prepare(nullptr,NiIsKindOf(NiPickable, SelectedObj));
			ImGui::End();
		}
	}
	
}

void EditScene::DrawSHBDEditor() 
{
	ImGuiIO& io = ImGui::GetIO();
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

	int h = 110;
	int w = 200;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w, io.DisplaySize.y - h));
	ImGui::SetNextWindowSize(ImVec2(w, h));

	bool UpdateTexture = true;
	ImGui::Begin("SHBD-Editor", NULL, flags);

	if (ImGui::Checkbox("Set Blocked Area", &MoveStatus))
		UpdateTexture = false;
	bool Remove = !MoveStatus;
	if (ImGui::Checkbox("Remove Blocked Area", &Remove))
		MoveStatus = false;
	ImGui::SliderInt("BrushSize", &BrushSize, 0, 100);

	ImGui::End();
}

void EditScene::DrawSHMDHeader(std::string Name, NiNodePtr Node)
{
	if (ImGui::CollapsingHeader(Name.c_str()))
	{
		bool compact = false;
		int ChildCount = Node->GetChildCount();
		for (int i = 0; i < ChildCount; i++)
		{
			auto Object = Node->GetAt(i);
			if (NiIsKindOf(NiNode, Object) && !NiIsKindOf(NiPickable,Object))
			{
				ImGui::Text(Object->GetName(), "");
				ImGui::SameLine();
				if (ImGui::Button(std::string("Delete##" + Name + std::to_string(i)).c_str()))
				{
					Node->DetachChildAt(i);
					compact = true;
				}
			}
		}
		if (compact)
			Node->CompactChildArray();
	}
}

