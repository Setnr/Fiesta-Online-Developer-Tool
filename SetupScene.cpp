#include "SetupScene.h"

SetupLoader StartUpScene::_Loader;

void StartUpScene::DrawImGui()
{
	if (_Loader.DrawImGui()) 
	{
		_Loader.UpdateSettingsFile();
	}
}