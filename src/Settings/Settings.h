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
	static std::string GetResoultion() 
	{
		return std::string( std::to_string(_Settings._WindowWidth) + " x " + std::to_string(_Settings._WindowHeight));
	}
private:
	static Settings _Settings;

	Setting(bool, FirstStartUp,_FirstStartUp);
	Setting(int, WindowWidth, _WindowWidth);
	Setting(int, WindowHeight, _WindowHeight);
	Setting(bool, FullScreen, _FullScreen);

};