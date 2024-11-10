#pragma once
#include <Scene/ScreenElements/ScreenElement.h>
#include <Scene/EditorScene/EditorScene.h>
#include "lua.hpp"

NiSmartPointer(LuaElement);
class LuaElement : public ScreenElement
{
	NiDeclareRTTI;
public:
	LuaElement(EditorScenePtr Scene, std::string FileName, ImVec2 Pos = {-1,-1});
	bool Draw();
	EditorScenePtr GetScene() { return _Scene; }
	void AddFlag(int flag) { flags |= flag; }
	std::string GetFileName() { return _FileName; }
private:
	EditorScenePtr _Scene;

	std::string _FileName;
	lua_State* Script;
	std::string WindowName;
	bool open = true;
	int flags = 0; 
	ImVec2 _Pos;
};

