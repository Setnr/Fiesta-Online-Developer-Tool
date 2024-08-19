#pragma once
#include "FiestaScene.h"
#include "../Data/IngameWorld.h"
#include "../Data/HTDBrush/Brush.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Elements/MapMenu.h"
#include "../CustomNi/NiFileLoader.h"
#include "ImGui/ImGuizmo.h"
#include "../Settings/Settings.h"
#include "../CustomNi/TextureFileLoader.h"
#include "Elements/LayerEditWindow.h"

NiSmartPointer(EditScene);
class EditScene :  public FiestaScene
{
public:
	EditScene()
	{
		CanSwitch = true;
		SaveTime = NiGetCurrentTimeInSec();
	}

	virtual void Draw(NiRenderer* renderer);
	virtual void DrawImGui();
	virtual void CreateMenuBar();
	virtual void UpdateCamera(float fTime);
	virtual void Update(float fTime);
	void LoadMap(MapInfo* Info);
	void HideShowLoadMenu() { _MapMenu.HideMenu(); }
	void ShowLoadMenu() { _MapMenu.ShowMenu(); }
private:
	void RecreateBoundingBox();
	void FindBoundingData(NiNodePtr Node, NiPoint3& MinPoint, NiPoint3& MaxPoint);
	WorldPtr kWorld;
	std::mutex UpDateWorldLock;
	WorldPtr UpDateWorld;
	float SaveTime;
	MapMenu _MapMenu;
	bool ShowSettingsMenu = false;
	void ShowSettings();
	void ShowAboutWindow();
	void DrawGeneralInfoWindow();
	void DrawSceneNode(NiNodePtr Node);
	NiNodePtr GeneralInfoNode;
	NiNodePtr BoundingBox;
	void DrawGeneralInfoNode();
	void UpdateGeneralInfoNode(NiNodePtr Node);
	void DrawGizmo();
	void SelectObject();
	void SHMDChangeGizmoStatus();
	void SHMDCopyPaste();
	void SHMDDelete();
	void MiddleMouseButtonMenu();
	void DrawSHMDWindow();
	void DrawSHMDHeader(std::string Name, NiNodePtr Node);
	NiPickablePtr  SelectedObj;
	NiPickablePtr  CopyObj;
	glm::vec3 SelectedObjAngels;
	float LastPasteTime = 0.0f;
	ImGuizmo::OPERATION OperationMode = ImGuizmo::OPERATION::TRANSLATE;
	bool SnapMove = false;
	NiPoint3 SnapSize = NiPoint3(2.5f,2.5f,2.5f);

	void UpdateSHBDTexture();
	void MoveViaMiddleMouse();
	void MoveSHBDTexture(float fTime);
	void DrawSHBDEditor();

	std::shared_ptr<TerrainLayer> SelectedLayer;

	TextureFileLoader loader;

	void DrawTextureEditor();

	void SaveAll() 
	{
		if (kWorld == NULL)
			return;
		kWorld->SaveSHMD();
		kWorld->SaveSHBD();
		kWorld->SaveHTDG();
		kWorld->GetIniFile().Save();
		kWorld->SaveVertexColor();
	}
	void SaveAllBackUp() 
	{
		if (kWorld == NULL)
			return;
		kWorld->SaveSHMD(true);
		kWorld->SaveSHBD(true);
		kWorld->SaveHTDG(true);
		kWorld->GetIniFile().Save(true);
		kWorld->SaveVertexColor(true);
	}
	int BrushSize = 0;
	bool MoveStatus = false;

	BrushPtr _Brush;
	NiNodePtr HTDOrbNode;

	void CopyObject();

	enum EditMode : int
	{
		None,
		SHMD,
		SHBD,
		HTDG,
		Texture,
		VertexColor,
		Max
	};
	EditMode CurrentEditMode = None;

	void ResetCamera();

	void UpdateEditMode(EditMode mode) 
	{
		if (!kWorld)
			return;
		if (mode >= EditMode::Max)
			mode = None;
		CurrentEditMode = mode;
		
		kWorld->SetSHBDVisiblity(mode == SHBD);
		kWorld->ShowHTDG(mode == HTDG || mode == Texture || mode == VertexColor, Settings::ShowSHMD(), HTDOrbNode);

		switch (mode)
		{
		case EditScene::HTDG:
			if(!NiIsKindOf(HTDBrush,_Brush))
				_Brush = NiNew HTDBrush(NiSmartPointerCast(TerrainWorld,kWorld), HTDOrbNode, 5);
			break;
		case EditScene::Texture:
			if (!NiIsKindOf(HTDTextureBrush, _Brush))
				_Brush = NiNew HTDTextureBrush(SelectedLayer,NiSmartPointerCast(TerrainWorld, kWorld), HTDOrbNode, 5);
			break;
		case EditScene::VertexColor:
			_Brush = NiNew VertexBrush(NiSmartPointerCast(TerrainWorld, kWorld), HTDOrbNode, 5);
			break;
		default:
			break;
		}

		_Brush->Show(mode == HTDG || mode == Texture || mode == VertexColor);
	}
	std::string GetEditMode() 
	{
		switch (CurrentEditMode) {
		case None: return "None";
		case SHMD: return "SHMD";
		case SHBD: return "SHBD";
		case HTDG: return "HTDG";
		case Texture: return "Texture";
		case VertexColor: return "VertexColor";
		default: return "Inactive";
		}
	}
	std::string GetEditMode(EditMode mode)
	{
		switch (mode) {
		case None: return "None";
		case SHMD: return "SHMD";
		case SHBD: return "SHBD";
		case HTDG: return "HTDG";
		case Texture: return "Texture";
		case VertexColor: return "VertexColor";
		default: return "Inactive";
		}
	}
	NiFileLoader MiddleMouseLoader; 
	NiFileLoader SHMDWindowLoader;

	void RemoveCDAndM(NiNodePtr& obj, World* kWorld)
	{
		std::vector<NiAVObjectPtr> Removes;
		for (int i = 0; i < obj->GetChildCount(); i++)
		{
			NiAVObjectPtr child = obj->GetAt(i);
			if (!child)
				continue;
			if (NiIsKindOf(NiGeometry, child))
			{
				auto Name = child->GetName();
				if (Name.Contains("#CD") || Name.Contains("#M"))
				{
					Removes.push_back(child);
				}
			}
			else if (NiIsKindOf(NiNode, child))
			{
				NiNodePtr p = (NiNode*)(NiAVObject*)child;
				RemoveCDAndM(p, kWorld);
			}
		}
		for (auto rem : Removes)
		{
			obj->DetachChild(rem);
			if (NiIsKindOf(NiNode, rem))
				kWorld->AttachGroundCollidee((NiNodePtr)(NiNode*)&*rem);
		}
	}
};

