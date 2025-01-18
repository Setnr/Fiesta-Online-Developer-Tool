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
	void SetPointer(long long ptr, const char* Func)
	{
		lua_getglobal(Script, Func);
		lua_pushinteger(Script, ptr);
		if (lua_pcallk(Script, 1, 0, 0, 0, 0))
			LogLua(_FileName, lua_tostring(Script, -1));
	}

private:
	EditorScenePtr _Scene;

	std::string _FileName;
	lua_State* Script;
	std::string WindowName;
	bool open = true;
	int flags = 0; 
	ImVec2 _Pos;
};

