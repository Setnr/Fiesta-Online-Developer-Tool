#pragma once
#include "TerrainBrushMode.h"
#include "EditorScene/EditorScene.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>

NiSmartPointer(HTDGMode);
class HTDGMode : public TerrainBrushMode
{
	NiDeclareRTTI;
	HTDGMode(IngameWorldPtr World, EditorScenePtr Scene) : TerrainBrushMode(World, (FiestaScenePtr)&*Scene)
	{
		kWorld->ShowTerrain(false);
		ScreenElements.push_back(NiNew LuaElement(Scene, "EditorELements/HTDG.lua"));
		LoadBrushes("HTDGBrushes");
	} 
	~HTDGMode()
	{
		kWorld->ShowTerrain(true);
	} 

	virtual void ProcessInput();
	
	virtual std::string GetEditModeName() { return "HTDG"; }

private:

	
};