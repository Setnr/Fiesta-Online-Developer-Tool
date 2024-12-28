#pragma once
#include "EditMode.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include "ImGui/ImGuizmo.h"

NiSmartPointer(TerrainMode);
class TerrainMode : public EditMode
{
	NiDeclareRTTI;
	TerrainMode(IngameWorldPtr World, EditorScenePtr Scene) : EditMode(World, (FiestaScenePtr)&* Scene)
	{
		Camera = World->GetCamera();
		MouseOrb = PgUtil::LoadNifFile(PgUtil::PathFromApplicationFolder(".\\FiestaOnlineTool\\HTDCircle.nif").c_str());
		NiWireframePropertyPtr ptr = NiNew NiWireframeProperty;
		ptr->SetWireframe(true);
		MouseOrb->AttachProperty(ptr);
		MouseOrb->SetScale((50.f / 160.f) * _BrushSize);

		_BaseNode = kWorld->GetWorldNode();
		//_BaseNode = NiNew NiNode;
		_BaseNode->AttachChild(MouseOrb);

		MouseOrb->UpdateEffects();
		MouseOrb->UpdateProperties();
		MouseOrb->Update(0.f);

		SetBrushSize(_BrushSize);
		_FarFrustum = kWorld->GetFarFrustum();
		_FogDepth = kWorld->GetFogDepth();
		kWorld->SetFarFrustum(50000.f, false);
		kWorld->SetFogDepth(0.0f, false);
	}
	~TerrainMode()
	{
		kWorld->SetFarFrustum(_FarFrustum, false);
		kWorld->SetFogDepth(_FogDepth, false);
		_BaseNode->DetachChild(MouseOrb);
		_BaseNode = NULL;
		MouseOrb = NULL;
		Light = NULL;
		Camera = NULL;
		SetShowElements(true);
	}
	virtual void Draw();
	virtual void Update(float fTime);
	virtual void ProcessInput();

	int GetBrushSize() { return _BrushSize; }
	void SetBrushSize(int Size) { _BrushSize = Size; MouseOrb->SetScale((6.25f / 160.f) * _BrushSize); }
	void SetShowElements(bool Show);
	bool GetShowElements() { return _ShowElements; }
	void SetWalkable(bool Walkable) { _Walkable = Walkable; }
	bool GetWalkable() { return _Walkable; }
protected:
	int _BrushSize = 1;
	bool _ShowElements = true;
	bool _Walkable = true;
	float _FarFrustum, _FogDepth;

	bool _Update = false;

	virtual void UpdateMouseIntersect();

	NiNodePtr _BaseNode;
	NiAmbientLightPtr Light;
	NiNodePtr MouseOrb;
	NiCameraPtr Camera;
	NiPoint3 MouseIntersect;

};