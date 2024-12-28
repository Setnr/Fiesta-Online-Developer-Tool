#pragma once
#include "TerrainMode.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include "ImGui/ImGuizmo.h"

NiSmartPointer(HTDGMode);
class HTDGMode : public TerrainMode
{
	NiDeclareRTTI;
	HTDGMode(IngameWorldPtr World, EditorScenePtr Scene) : TerrainMode(World, Scene)
	{

		kWorld->ShowTerrain(false);
		ScreenElements.push_back(NiNew LuaElement(Scene, "EditorELements/HTDG.lua"));
	} 
	~HTDGMode()
	{
		kWorld->ShowTerrain(true);
	} 
	virtual void Update(float fTime);
	virtual void ProcessInput();
	virtual std::string GetEditModeName() { return "HTDG"; }

private:
};