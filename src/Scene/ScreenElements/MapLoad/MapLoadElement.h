#pragma once
#include <Scene/ScreenElements/ScreenElement.h>
#include <Scene/EditorScene/EditorScene.h>

NiSmartPointer(MapLoadElement);
class MapLoadElement : public ScreenElement
{
public:

	MapLoadElement(EditorScenePtr Scene) : _Scene(Scene){}
	bool Draw();
private:
	EditorScenePtr _Scene;
	int MapSize = 256;

};

