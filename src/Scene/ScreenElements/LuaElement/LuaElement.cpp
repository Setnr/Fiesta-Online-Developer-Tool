#include "LuaElement.h"
#include "LuaFunctions/LuaElementFunctions.h"

LuaElement::LuaElement(EditorScenePtr Scene, std::string FileName, ImVec2 Pos) : _Scene(Scene), _FileName(FileName), _Pos(Pos)
{
	Script = luaL_newstate();
	luaL_openlibs(Script);
	std::string Path = PgUtil::PathFromApplicationFolder("./FiestaOnlineTool/SceneElements/" + _FileName);
	if (luaL_loadfilex(Script, Path.c_str(), 0)) 
	{
		LogLua(_FileName , lua_tostring(Script, -1));
	}
	else 
	{
		if (lua_pcallk(Script, 0, 0, 0, 0, 0))
			LogLua(_FileName, lua_tostring(Script, -1));

		SetFunctions(Script);

		lua_getglobal(Script, "prepare");
		lua_pushinteger(Script, (long long)this);
		if (lua_pcallk(Script, 1, 0, 0, 0, 0))
			LogLua(_FileName, lua_tostring(Script, -1));

		lua_getglobal(Script, "WindowName");
		if (lua_isstring(Script, 1))
			WindowName = lua_tostring(Script, 1);
	}
} 

bool LuaElement::Draw()
{  
	bool ret = true;
	if (_Pos.x != -1 && _Pos.y != -1)
		ImGui::SetNextWindowPos(_Pos);
	if(ImGui::Begin(WindowName.c_str(),&open, flags))
	{ 
		lua_getglobal(Script, "render");
		lua_pushinteger(Script, (long long) this);
		if (lua_pcall(Script, 1, 1, 0) != 0)
			LogLua(_FileName, lua_tostring(Script, -1));
		if (lua_isboolean(Script, -1))
			ret = lua_toboolean(Script, -1);
		lua_pop(Script, 1);
	}
	ImGui::End();

	return ret && open;
}