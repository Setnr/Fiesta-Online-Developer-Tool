#pragma once
#include <NiMain.h>
#include <string>
#include "IngameWorld/IngameWorld.h"

NiSmartPointer(Brush);
class Brush : public NiRefObject
{
	NiDeclareRootRTTI(Brush);

	Brush();

	virtual void Draw();
	virtual void RunAlgorithm(IngameWorldPtr World);
	virtual std::string GetBrushName() { return "Brush::Brush"; }
private:
	
};
