#pragma once
#include "FiestaRendererTypeDef.h"
#include <atomic>
class FiestaRenderer 
{
public:
	explicit FiestaRenderer();
	~FiestaRenderer();
private:
	std::atomic<bool> Init = false;
	WinMain_Render WinMain;
};