#pragma once
#include "FiestaScene.h"
#include "../Data/IngameWorld.h"
#include "../Data/HTDBrush/HTDBrush.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Elements/MapMenu.h"
#include "../CustomNi/NiFileLoader.h"
#include "ImGui/ImGuizmo.h"
#include "../Settings/Settings.h"

NiSmartPointer(EditScene);
class EditScene :  public FiestaScene
{
public:
	EditScene() 
	{
		CanSwitch = true;
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
	int BrushSize = 0;
	bool MoveStatus = false;

	HTDBrushPtr _HTDBrush;
	NiNodePtr HTDOrbNode;

	enum EditMode : int
	{
		None,
		SHMD,
		SHBD,
		HTDG,
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
		kWorld->ShowHTDG(mode == HTDG , Settings::ShowSHMD(), HTDOrbNode);
		_HTDBrush->Show(mode == HTDG);
	}
	std::string GetEditMode() 
	{
		switch (CurrentEditMode) {
		case None: return "None";
		case SHMD: return "SHMD";
		case SHBD: return "SHBD";
		case HTDG: return "HTDG";
		default: return "Inactive";
		}
	}
	NiFileLoader MiddleMouseLoader; 
	NiFileLoader SHMDWindowLoader;
};

