#pragma once
#include <NiMain.h>

NiSmartPointer(ScreenElement);
class ScreenElement : public NiRefObject
{
public:
	virtual bool Draw() = 0;
};

