#pragma once
#include "TerrainBrushMode.h"
#include "EditorScene/EditorScene.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>

NiSmartPointer(VertexMode);
class VertexMode : public TerrainBrushMode
{
	NiDeclareRTTI;
	VertexMode(IngameWorldPtr World, EditorScenePtr Scene) : TerrainBrushMode(World, (FiestaScenePtr)&* Scene)
	{
		ScreenElements.push_back(NiNew LuaElement(Scene, "EditorELements/VertexColor.lua"));
		LoadBrushes("VertexBrushes");
	}
	~VertexMode()
	{
		//kWorld->ShowTerrain(true);
	}

	virtual void ProcessInput();

	virtual std::string GetEditModeName() { return "VertexColor"; }

private:
	NiPixelDataPtr _Data;

};