#pragma once
#include "TerrainBrushMode.h"
#include "EditorScene/EditorScene.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>

NiSmartPointer(TextureMode);
class TextureMode : public TerrainBrushMode
{
	NiDeclareRTTI;
	TextureMode(IngameWorldPtr World, EditorScenePtr Scene) : TerrainBrushMode(World, (FiestaScenePtr)&* Scene)
	{
		kWorld->ShowTerrain(true);
		ScreenElements.push_back(NiNew LuaElement(Scene, "EditorELements/Texture.lua"));
		LoadBrushes("TextureBrushes");
	}
	~TextureMode()
	{
		_CurrentLayer = nullptr;
		_Data = NULL;
		_ScreenElement = NULL;
	}

	virtual void ProcessInput();
	void DrawLayers();
	virtual std::string GetEditModeName() { return "Texture"; }
	void SaveSelectedLayer();

private:

	std::shared_ptr<TerrainLayerData> _CurrentLayer;
	NiPixelDataPtr _Data;
	NiScreenElementsPtr _ScreenElement;
};