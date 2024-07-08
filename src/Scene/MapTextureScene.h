#pragma once
#include "FiestaScene.h"
#include "../Data/IniFile.h"
#include "Elements/MapMenu.h"
#include "Elements/LayerEditWindow.h"

NiSmartPointer(MapTextureScene);
class MapTextureScene : public FiestaScene
{
public:

	MapTextureScene(std::string FilePath) : kWorld(FilePath), _LayerEdit(&kWorld)
	{
		Camera = kWorld.GetCamera();
		LookAndMoveAtWorldPoint(NiPoint3(0.0f, 0.0f, 0.0f));
		kWorld.SetAmbientLightAmbientColor(NiColor::WHITE);
		PgUtil::SaveNode(PgUtil::CreateFullFilePathFromBaseFolder(".\\Test.nif"), kWorld.GetTerrainScene());
		CanSwitch = true;
	}

	virtual void Draw(NiRenderer* renderer);
	virtual void DrawImGui();
	virtual void CreateMenuBar();
	virtual void UpdateCamera(float fTime);
	//virtual void Update(float fTime);
	
	std::shared_ptr<TerrainLayer> SelectedLayer;

private:
	TerrainWorld kWorld;
	MapMenu _MapMenu;
	LayerEditWindow _LayerEdit;
};

