#pragma once
#include <NiMain.h>
#include <string>

#include "Brush.h"
#include "lua.hpp"

NiSmartPointer(LuaBrush);
class LuaBrush : public Brush 
{
	NiDeclareRTTI;

	LuaBrush(std::string FileName);

	virtual void Draw();
	virtual void RunAlgorithm(int middelw, int middleh,float z, int SizeW, int SizeH, int BrushSize,IngameWorldPtr World);
	std::string GetBrushName() 
	{
		return _BrushName;
	}
private:
	lua_State* Script;
	std::string _FileName;
	std::string _BrushName;
};
