#pragma once
#include "FiestaScene.h"
#include "../Data/IniFile.h"
#include "Elements/MapMenu.h"
#include "Elements/LayerEditWindow.h"
#include "../CustomNi/TextureFileLoader.h"

NiSmartPointer(MapTextureScene);
class MapTextureScene : public FiestaScene
{
public:

	MapTextureScene(std::string FilePath, std::string SubPath) : _LayerEdit(kWorld), _SubPath(SubPath)
	{
		kWorld = NiNew TerrainWorld(FilePath);
		Camera = kWorld->GetCamera();
		LookAndMoveAtWorldPoint(kWorld->GetSpawnPoint());
		kWorld->SetAmbientLightAmbientColor(NiColor::WHITE);
		CanSwitch = true;


	}
	~MapTextureScene() 
	{
		return;
	}
	virtual void Draw(NiRenderer* renderer);
	virtual void DrawImGui();
	virtual void CreateMenuBar();
	virtual void UpdateCamera(float fTime);
	virtual void Update(float fTime);
	
	std::shared_ptr<TerrainLayer> SelectedLayer;

private:
	void SaveMap();
	TerrainWorldPtr kWorld;
	MapMenu _MapMenu;
	LayerEditWindow _LayerEdit;
	std::string _SubPath;
	TextureFileLoader loader;

};

