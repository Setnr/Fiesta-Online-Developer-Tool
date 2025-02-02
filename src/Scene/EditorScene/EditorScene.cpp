#pragma once

#include "EditorScene.h"

#include <SHN/SHNManager.h>

#include "ImGui/imgui.h"
#include <Settings.h>
#include <Logger.h>
#include <Scene/ScreenElements/MapLoad/MapLoadElement.h>
#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include <filesystem>
#include "EditorScene/Modes/SHMDMode.h"
#include "EditorScene/Modes/SHBDMode.h"
#include "EditorScene/Modes/HTDGMode.h"
#include "EditorScene/Modes/TextureMode.h"
#include "EditorScene/Modes/VertexMode.h"
#include <NiDX9Renderer.h>
NiImplementRTTI(EditorScene, FiestaScene);

EditorScene::EditorScene() 
{
	ScreenElements.push_back(NiNew MapLoadElement(this));
}
EditorScene::~EditorScene() 
{
	_EditMode = nullptr;
	ScreenElements.clear();
}
void EditorScene::DrawImGui()
{
	FiestaScene::DrawImGui();

	ImGuiIO& io = ImGui::GetIO();

	for (int i = 0; i < ScreenElements.size(); i++) 
	{
		if(!ScreenElements.at(i)->Draw())
		{
			ScreenElements.erase(std::remove(ScreenElements.begin(), ScreenElements.end(), ScreenElements.at(i)), ScreenElements.end());
			i--;
		}
	}
	if (_EditMode)
		_EditMode->Draw();
	return;
}
void EditorScene::LoadNewMap(MapInfo* info) 
{
	auto start = std::chrono::steady_clock::now();
	NewWorldLoaded = NiNew IngameWorld(info);
	LogTime(std::string("Loaded ") + info->MapName, start);
}
void EditorScene::CreateNewMap(MapInfo* info, int MapSize) 
{
	auto start = std::chrono::steady_clock::now();

	memcpy_s(info->MapFolderName, sizeof(info->MapFolderName), info->MapName, sizeof(info->MapName));
	NewWorldLoaded = NiNew IngameWorld(info,MapSize);
	LogTime(std::string("Loaded ") + info->MapName, start);
}
void EditorScene::CreateMenuBar() 
{
	if (!kWorld)
		return;
	if (ImGui::BeginMenu("File"))
	{
		if(ImGui::MenuItem("Open New Map", "ctrl + o"))
			ScreenElements.push_back(NiNew MapLoadElement(this));
		if (ImGui::MenuItem("Save SHMD"))
		{
			kWorld->SaveSHMD();
		}
		if (ImGui::MenuItem("Save SHBD"))
		{
			kWorld->SaveSHBD();
		}
		if (ImGui::MenuItem("Save HTD(G)"))
		{
			kWorld->SaveHTD();
		}
		if (ImGui::MenuItem("Save Ini"))
		{
			kWorld->SaveIni();
		}
		if (ImGui::MenuItem("Save Vertex"))
		{
			kWorld->SaveVertex();
		}
		if (ImGui::MenuItem("Save All"))
		{
			kWorld->SaveHTD();
			kWorld->SaveSHBD();
			kWorld->SaveSHMD();
			kWorld->SaveIni();
			kWorld->SaveVertex();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View")) 
	{
		std::string Path = PgUtil::PathFromApplicationFolder(".\\FiestaOnlineTool\\SceneElements");
		for (const auto& entry : std::filesystem::directory_iterator(Path)) 
		{
			auto Name = entry.path().filename();
			if (Name.string() == ("About.lua") || Name.string() == ("Test.lua") || Name.string().substr(Name.string().length() -3) != "lua")
				continue;
			if (ImGui::Selectable(Name.string().c_str()))
			{
				ScreenElements.push_back(NiNew LuaElement(this, Name.string()));
			}
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginCombo("Edit Mode", _EditMode->GetEditModeName().c_str()))
	{
		if (ImGui::Selectable("SHMD", NiIsKindOf(SHMDMode, _EditMode)))
		{
			_EditMode = NULL;
			_EditMode = NiNew SHMDMode(kWorld, this);
		}
		if (ImGui::Selectable("SHBD", NiIsKindOf(SHBDMode, _EditMode)))
		{
			_EditMode = NULL;
			_EditMode = NiNew SHBDMode(kWorld, this);
		}
		if (ImGui::Selectable("HTD", NiIsKindOf(HTDGMode, _EditMode)))
		{
			_EditMode = NULL;
			_EditMode = NiNew HTDGMode(kWorld, this);
		}
		if (ImGui::Selectable("Texture", NiIsKindOf(TextureMode, _EditMode)))
		{
			_EditMode = NULL;
			_EditMode = NiNew TextureMode(kWorld, this);
		}
		if (ImGui::Selectable("VertexColor", NiIsKindOf(VertexMode, _EditMode)))
		{
			_EditMode = NULL;
			_EditMode = NiNew VertexMode(kWorld, this);
		}
		ImGui::EndCombo();
	}
	ImGui::Checkbox("Freeze Time", &FreezeTime);

	ImGui::SameLine(ImGui::GetWindowWidth() - 30);
	ImGui::Text("(?)");
	if (ImGui::IsItemClicked())
	{
		ScreenElements.push_back(NiNew LuaElement(this, "About.lua"));
	}
}
void EditorScene::Update(float fTime) 
{
	if (FreezeTime)
		fTime = 0.f;
	if (NewWorldLoaded)
	{
		kWorld = NewWorldLoaded;
		NewWorldLoaded = NULL;
		_EditMode = NiNew SHMDMode(kWorld, this);
		BaseNode = kWorld->GetWorldNode();
		Camera = kWorld->GetCamera();

		NiMatrix3 mat = Camera->GetRotate();
		mat.ToEulerAnglesXYZ(Roll, Yaw, Pitch);
		Camera->Update(0.0f);
		EnableKeyStrokes = true;
	}

	FiestaScene::Update(fTime);
	if (kWorld)
	{
		for (auto sky : kWorld->GetSkyNodeList())
		{
			NiPoint3 translate(Camera->GetTranslate());
			sky->SetTranslate(translate);
			sky->Update(fTime / 10.f);
		}
		if (_EditMode)
			_EditMode->Update(fTime);
	}
}
void EditorScene::ProcessInput() 
{
	ImGuiIO& io = ImGui::GetIO();
	if (kWorld)
	{
		if (ImGui::IsKeyDown((ImGuiKey)VK_CONTROL)) 
		{
			if(ImGui::IsKeyReleased((ImGuiKey)0x5A))//Ctrl + Z
				kWorld->Undo();
			if (ImGui::IsKeyReleased((ImGuiKey)0x59))
				kWorld->Redo();
		}
		if (ImGui::IsKeyReleased((ImGuiKey)VK_TAB))
			UpdateEditMode();
		if (_EditMode)
			_EditMode->ProcessInput();
	}
	if (ImGui::IsKeyDown((ImGuiKey)VK_CONTROL) && ImGui::IsKeyPressed((ImGuiKey)0x4F))
		ScreenElements.push_back(NiNew MapLoadElement(this));
}	
void EditorScene::UpdateEditMode() 
{
	if (NiIsKindOf(SHMDMode, _EditMode))
	{
		_EditMode = NiNew SHBDMode(kWorld, this);
	}
	else if (NiIsKindOf(SHBDMode, _EditMode))
	{
		_EditMode = NULL; 
		_EditMode = NiNew HTDGMode(kWorld, this);
	}
	else if (NiIsKindOf(HTDGMode, _EditMode))
	{
		_EditMode = NiNew TextureMode(kWorld, this);
	}
	else if (NiIsKindOf(TextureMode, _EditMode))
	{
		_EditMode = NiNew VertexMode(kWorld, this);
	}
	else
		_EditMode = NiNew SHMDMode(kWorld, this);
}