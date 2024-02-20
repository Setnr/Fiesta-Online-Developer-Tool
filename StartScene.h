#pragma once
#include "FiestaScene.h"
NiSmartPointer(StartScene);
class StartScene : public FiestaScene
{
public:
	StartScene();
	~StartScene() 
	{
		TerminateThread(_Thread,0);
		CloseHandle(_Thread);
	}
	void SetHWND(HWND Handle) { _HWND = Handle; }
	void RunThread();
private:
	HANDLE _Thread;
	HWND _HWND;

	bool Loaded;
};



