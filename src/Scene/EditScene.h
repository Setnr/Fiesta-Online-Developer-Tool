#pragma once
#include "FiestaScene.h"
#include "../Data/IngameWorld.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"
class EditScene :    public FiestaScene
{
public:
	EditScene() = default;

	virtual void Draw(NiRenderer* renderer);
	virtual void DrawImGui();
	virtual void CreateMenuBar();
	virtual void UpdateCamera(float fTime);
	virtual void Update(float fTime);
private:
	void LoadMap(MapInfo* Info);
	void LookAndMoveAtWorldPoint(NiPoint3 Point);
	void RecreateBoundingBox();
	void FindBoundingData(NiNodePtr Node, NiPoint3& MinPoint, NiPoint3& MaxPoint);
	WorldPtr kWorld;
	std::mutex UpDateWorldLock;
	WorldPtr UpDateWorld;

	bool ShowLoadMenu = true;
	void ShowMapInfo();
	bool ShowSettingsMenu = false;
	void ShowSettings();
	bool ShowAbout = false;
	void ShowAboutWindow();

	void DrawGeneralInfoWindow();
	void DrawSceneNode(NiNodePtr Node);
	NiNodePtr GeneralInfoNode;
	NiNodePtr BoundingBox;
	void DrawGeneralInfoNode();
	void UpdateGeneralInfoNode(NiNodePtr Node);
	void DrawGizmo();
	void SelectObject();
	void SHMDCopyPaste();
	void SHMDDelete();
	void MiddleMouseButtonMenu();
	void DrawSHMDWindow();
	void DrawSHMDHeader(std::string Name, NiNodePtr Node);
	NiPickablePtr  SelectedObj;
	NiPickablePtr  CopyObj;
	glm::vec3 SelectedObjAngels;
	float LastPasteTime = 0.0f;

	void UpdateSHBDTexture();
	void MoveViaMiddleMouse();
	void MoveSHBDTexture(float fTime);
	void DrawSHBDEditor();
	int BrushSize = 0;
	void inline UpdateBrushSize() 
	{
		if (ImGui::SliderInt("BrushSize", &BrushSize, 0, 100))
			HTDOrbNode->SetScale((50.f / 160.f) * BrushSize);
	}
	bool MoveStatus = false;

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
		kWorld->ShowHTD(mode == HTDG, HTDOrbNode);
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
};

