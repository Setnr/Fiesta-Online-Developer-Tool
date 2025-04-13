#pragma once
#include "EditMode.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include "ImGui/ImGuizmo.h"
#include "ShineObject/ShineObject.h"


NiSmartPointer(NPCEditMode);
class NPCEditMode : public EditMode
{
	NiDeclareRTTI;
public:
	NPCEditMode(IngameWorldPtr World, EditorScenePtr Scene) : EditMode(World, (FiestaScenePtr)&* Scene)
	{
		ScreenElements.push_back(NiNew LuaElement(Scene, "EditorELements/NPCEditMode.lua"));
	} 
	~NPCEditMode() 
	{
		ClearSelectedObjects();
		kWorld->GateSpawnPoints(false);
	}
	virtual void Draw();
	virtual void Update(float fTime);
	virtual void ProcessInput();
	virtual std::string GetEditModeName() { return "NPC"; }
	void SelectObject(NiPickablePtr Obj, bool append = false)
	{
		if (!Obj || !NiIsKindOf(ShineObject, Obj))
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
	void SetSnapSize(float Size) { SnapSize = NiPoint3(Size, Size, Size); }
	void SetSnapeMovement(bool Snap) { SnapMovement = Snap; }
	bool GetSnapMovement() { return SnapMovement; }
private:
	ImGuizmo::OPERATION OperationMode = ImGuizmo::OPERATION::TRANSLATE_X | ImGuizmo::OPERATION::TRANSLATE_Y;
	void DrawGizmo();
	bool SnapMovement = false;
	NiPoint3 SnapSize = NiPoint3(2.5f, 2.5f, 2.5f);
	std::vector<NiPickablePtr> SelectedObjects;
	NiPickablePtr CopyObject = NULL;
};
