#pragma once
#include "TerrainMode.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include "ImGui/ImGuizmo.h"

NiSmartPointer(SHBDMode);
class SHBDMode : public TerrainMode
{
	NiDeclareRTTI;
	SHBDMode(IngameWorldPtr World, EditorScenePtr Scene) : TerrainMode(World, (FiestaScenePtr)&* Scene)
	{
		ScreenElements.push_back(NiNew LuaElement(Scene, "EditorELements/SHBD.lua"));

		_BaseNode->DetachChild(MouseOrb);//We need to remove it from the Terrain ebcause this mode does use it on its own

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

		
		_SHBDNode = NiNew NiNode;
		_BaseNode->AttachChild(_SHBDNode);
		_BaseNode->AttachChild(MouseOrb);
		SetBrushSize(1);
	}
	~SHBDMode()
	{
		_SHBDNode = NULL;
	}
	virtual void Draw();
	virtual void Update(float fTime);
	virtual void ProcessInput();
	virtual std::string GetEditModeName() { return "SHBD"; }
	
	void SetWalkable(bool Walkable) { _Walkable = Walkable; }
	bool GetWalkable() { return _Walkable; }
	void SetBrushSize(int Size) { _BrushSize = Size; MouseOrb->SetScale((6.25f / 160.f) * _BrushSize); }
private:
	void UpdateSHBD();

	NiNodePtr _SHBDNode;

	void CreateSHBDNode();
	void UpdateMouseIntersect();

	std::vector<std::vector<NiPixelDataPtr>> TextureConnector;
	int TextureSize = 128;

	std::vector<char> _Data;
	unsigned int Walkable, Blocked;

};