#pragma once
#include "FiestaScene.h"
#include "../Data/IniFile.h"

NiSmartPointer(MapTextureScene);
class MapTextureScene : public FiestaScene
{
public:

	MapTextureScene(std::string FilePath) : World(FilePath)
	{
		CanSwitch = true;
	}

	//virtual void Draw(NiRenderer* renderer);
	//virtual void DrawImGui();
	//virtual void CreateMenuBar();
	//virtual void UpdateCamera(float fTime);
	//virtual void Update(float fTime);
	
	//void LoadHTD(std::string HTDFilePath);

private:
	TerrainWorld World;

};

