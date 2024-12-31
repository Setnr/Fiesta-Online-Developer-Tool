#pragma once
#include "EditMode.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include "ImGui/ImGuizmo.h"

NiSmartPointer(SHMDMode);
class SHMDMode : public EditMode
{
	NiDeclareRTTI;
	SHMDMode(IngameWorldPtr World, EditorScenePtr Scene) : EditMode(World, (FiestaScenePtr)&*Scene)
	{
		ScreenElements.push_back(NiNew LuaElement(Scene, "EditorELements/SHMD.lua"));
		ScreenElements.push_back(NiNew LuaElement(Scene, "EditorELements/SHMDObject.lua"));
	}
	~SHMDMode() 
	{
		for (auto ptr : SelectedObjects) {
			ptr->HideBoundingBox();
		}
		SelectedObjects.clear();
	}
	virtual void Draw();
	virtual void Update(float fTime);
	virtual void ProcessInput();
	virtual std::string GetEditModeName() { return "SHMD"; }
	void SelectObject(NiPickablePtr Obj, bool append = false) 
	{
		if (!Obj)
			return;

		auto it = std::find(SelectedObjects.begin(), SelectedObjects.end(), Obj);
		if (it != SelectedObjects.end())
		{
			return;
		}
		if (!append && !SelectedObjects.empty())
		{
			for (auto ptr : SelectedObjects) {
				ptr->HideBoundingBox();
			}
			SelectedObjects.clear();
		}
		Obj->ShowBoundingBox();
		SelectedObjects.push_back(Obj);
	}
	void ClearSelectedObjects() 
	{
		if (!SelectedObjects.empty())
		{
			for (auto ptr : SelectedObjects) {
				ptr->HideBoundingBox();
			}
			SelectedObjects.clear();
		}
	}
	bool HasSelectedObject() { return !SelectedObjects.empty(); }
	ImGuizmo::OPERATION GetOperationMode() { return OperationMode; }
	void SetOperationMode(ImGuizmo::OPERATION Mode) { OperationMode = Mode; }
	std::vector<NiPickablePtr> GetSelectedNodes() { return SelectedObjects; }
	NiPoint3 GetSnapSize() { return SnapSize; }
	void SetSnapSize(float Size){ SnapSize = NiPoint3(Size, Size, Size); }
	void SetSnapeMovement(bool Snap) { SnapMovement = Snap; }
	bool GetSnapMovement() { return SnapMovement; }
	void CreateAddElement(std::string type);
	void UpdateScale(float Scale);
private:
	ImGuizmo::OPERATION OperationMode = ImGuizmo::OPERATION::TRANSLATE;
	void DrawGizmo();
	bool SnapMovement = false;
	NiPoint3 SnapSize = NiPoint3(2.5f, 2.5f, 2.5f);
	std::vector<NiPickablePtr> SelectedObjects;
	NiPickablePtr CopyObject = NULL;
	
};