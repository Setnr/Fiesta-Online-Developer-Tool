#pragma once
#include <string>
#include "PgUtil.h"


#define Setting(type,FunctionName,VariableName) public:\
	static type FunctionName(){ return _Settings.VariableName;}\
private:\
	type VariableName;

class Settings
{
public:
	Settings();
	static void SetClientPath(std::string& Path) 
	{
		PgUtil::FolderPath = Path;
	}
	static void SaveSettings();
	static void SetResolution(std::string Resoultion);
	static void SetFullScreen(bool b) { _Settings._FullScreen = b; }
	static void SetFPSCap(float f) { _Settings._FPSCap = f; }
	static void SetPasteDelay(float f) { _Settings._PasteDelay = f; }
	static void SetShowSHMD(bool status) { _Settings._ShowSHMD = status; }
	static std::string GetResoultion() 
	{
		return std::string( std::to_string(_Settings._WindowWidth) + " x " + std::to_string(_Settings._WindowHeight));
	}
	static void SetSaveInterval(float Interval) { _Settings._SaveInterval = Interval; }
	static void SwitchGeneralView() { _Settings._GeneralView = !_Settings._GeneralView; }
private:
	static Settings _Settings;

	Setting(bool, FirstStartUp,_FirstStartUp);
	Setting(int, WindowWidth, _WindowWidth);
	Setting(int, WindowHeight, _WindowHeight);
	Setting(bool, FullScreen, _FullScreen);
	Setting(float, FPSCap, _FPSCap);
	Setting(float, PasteDelay, _PasteDelay);
	Setting(bool, GeneralView, _GeneralView);
	Setting(bool, ShowSHMD, _ShowSHMD);
	Setting(float, SaveInterval, _SaveInterval);

};