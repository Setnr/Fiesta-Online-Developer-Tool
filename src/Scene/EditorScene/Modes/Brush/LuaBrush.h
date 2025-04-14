#pragma once
#include <NiMain.h>
#include <string>

#include "Brush.h"
#include "lua.hpp"
#include "NiCustom/NiFastNoiseLite.h"

NiSmartPointer(LuaBrush);
class LuaBrush : public Brush 
{
	NiDeclareRTTI;

	LuaBrush(std::string FileName);
	~LuaBrush() {
		if (_Noise)
		{
			_Noise = NULL;
		}
	}
	virtual void Draw();
	virtual void RunAlgorithm(int middelw, int middleh,float z, int SizeW, int SizeH, int BrushSize,IngameWorldPtr World, float ZAtClick);
	std::string GetBrushName() 
	{
		return _BrushName;
	}
	void CreateNoise(int seed)  
	{
		_Noise = NiNew NiFastNoiseLite(seed);
	}
	void Init(IngameWorldPtr world){
		lua_getglobal(Script, "Init");
		lua_pushinteger(Script, (long long)this);
		lua_pushinteger(Script, (long long)&*world);
		if (lua_pcallk(Script, 2, 0, 0, 0, 0))
			LogLua(_FileName, lua_tostring(Script, -1));
	}
	void RecreateHTD(IngameWorldPtr world, float MapHeight);
	NiFastNoiseLitePtr GetNoise() { return _Noise; }
	void SetPointer(long long ptr, const char* Func) 
	{
		lua_getglobal(Script, Func);
		lua_pushinteger(Script, ptr);
		if (lua_pcallk(Script, 1, 0, 0, 0, 0))
			LogLua(_FileName, lua_tostring(Script, -1));

	}
	void SetColor(NiColorA Col) 
	{
		lua_getglobal(Script, "SetColor");
		lua_pushnumber(Script, Col.r);
		lua_pushnumber(Script, Col.g);
		lua_pushnumber(Script, Col.b);
		if (lua_pcallk(Script, 3, 0, 0, 0, 0))
			LogLua(_FileName, lua_tostring(Script, -1));
	}

private:
	lua_State* Script;
	std::string _FileName;
	std::string _BrushName;
	NiFastNoiseLitePtr _Noise;

};
