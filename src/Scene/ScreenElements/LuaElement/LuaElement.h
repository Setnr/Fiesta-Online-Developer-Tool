#pragma once
#include <Scene/ScreenElements/ScreenElement.h>
#include <Scene/EditorScene/EditorScene.h>
#include "lua.hpp"

NiSmartPointer(LuaElement);
class LuaElement : public ScreenElement
{
public:
	LuaElement(EditorScenePtr Scene, std::string FileName);
	bool Draw();
	EditorScenePtr GetScene() { return _Scene; }
	void AddFlag(int flag) { flags |= flag; }
private:
	EditorScenePtr _Scene;

	std::string _FileName;
	lua_State* Script;
	std::string WindowName;
	bool open = true;
	int flags = 0; 
};

