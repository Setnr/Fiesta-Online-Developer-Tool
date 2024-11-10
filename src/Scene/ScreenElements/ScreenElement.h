#pragma once
#include <NiMain.h>

NiSmartPointer(ScreenElement);
class ScreenElement : public NiRefObject
{
	NiDeclareRootRTTI(ScreenElement);
public:
	virtual bool Draw() = 0;
};

