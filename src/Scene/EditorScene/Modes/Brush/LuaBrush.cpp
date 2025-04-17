#include "LuaBrush.h"
#include <ScreenElements/LuaElement/LuaFunctions/LuaElementFunctions.h>
#include <Data/IngameWorld/WorldChanges/FogChange.h>

NiImplementRTTI(LuaBrush, Brush);

LuaBrush::LuaBrush(std::string FileName) : _FileName(FileName)
{
	Script = luaL_newstate();
	luaL_openlibs(Script);
	if (luaL_loadfilex(Script, FileName.c_str(), 0))
	{
		LogLua(_FileName, lua_tostring(Script, -1));
		
	}
	else 
	{
		if (lua_pcallk(Script, 0, 0, 0, 0, 0))
			LogLua(_FileName, lua_tostring(Script, -1));

		SetFunctions(Script);

		lua_getglobal(Script, "BrushName");
		if (lua_isstring(Script, 1))
			_BrushName = lua_tostring(Script, 1);
		else
			_BrushName = "Unknown Name";
		 
	}
}

void LuaBrush::Draw()
{
	lua_getglobal(Script, "render");
	lua_pushinteger(Script, (long long)this);
	if (lua_pcallk(Script, 1, 0, 0, 0, 0))
		LogLua(_FileName, lua_tostring(Script, -1));
}
void LuaBrush::RunAlgorithm(int middelw, int middleh,float z, int SizeW, int SizeH, int BrushSize,IngameWorldPtr World,float ZAtClick)
{
	lua_getglobal(Script, "algorithm");
	   
	lua_pushinteger(Script, middelw);
	lua_pushinteger(Script, middleh);
	lua_pushnumber(Script, z);
	lua_pushnumber(Script, ZAtClick);
	lua_pushinteger(Script, SizeW);
	lua_pushinteger(Script, SizeH);
	lua_pushinteger(Script, BrushSize); 
	lua_pushinteger(Script, (long long)&*World);
	lua_pushinteger(Script, (long long)this);

	 
	if (lua_pcallk(Script, 9, 0, 0, 0, 0))
		LogLua(_FileName, lua_tostring(Script, -1));
}

void LuaBrush::RecreateHTD(IngameWorldPtr world, float MapHeight)
{
	if (!_Noise || !world->HasHTD())
		return;
	HeightTerrainData _OldData = *world->GetHTD(); 
	HeightTerrainDataPtr data = world->GetHTD();
	auto ini = world->GetShineIni();
	data->ResizeHTD(ini->GetMapHeight()-1 , ini->GetMapWidth()-1);
	for (int w = 0; w < ini->GetMapWidth(); w++)
	{
		for (int h = 0; h < ini->GetMapHeight(); h++)
		{
			data->SetHTD(w, h, _Noise->GetNoise((float)w, (float)h) * MapHeight);
		}
	}
	world->ShowTerrain(world->GetShowTerrain());
	HTDGChangePtr Change = NiNew HTDGChange(world, _OldData, *data);
	world->AttachStack(NiSmartPointerCast(WorldChange, Change));
}