#include "Settings.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "IniReader.h"
#include "Logger.h"
Settings Settings::_Settings;
static const char SettingsSubPath[] = ".\\FiestaOnlineTool\\Settings.ini";
Settings::Settings() : _FirstStartUp(false)
{
	//Prepare Application Path
	char Path[513];
	GetModuleFileNameA(NULL, Path, sizeof(Path));
	PgUtil::ApplicationPath = std::filesystem::path(Path).parent_path().string();

	std::string SettingsPath = PgUtil::CreateFullFilePathFromApplicationFolder(SettingsSubPath);
	if (!std::filesystem::exists(std::filesystem::path(SettingsPath)))
	{
		_FirstStartUp = true;
		_WindowWidth = 1600;
		_WindowHeight = 900;
		_FullScreen = false;
		return;
	}
	INIReader reader(SettingsPath);
	PgUtil::FolderPath = reader.GetString("BaseInformation", "ClientPath", "");
	if(PgUtil::FolderPath == "" || !std::filesystem::exists(PgUtil::FolderPath)) {
		_FirstStartUp = true;
	}
	_WindowWidth = reader.GetInteger("Window", "Width", 1600);
	_WindowHeight = reader.GetInteger("Window", "Height", 900);
	_FullScreen = reader.GetBoolean("Window", "FullScreen", false);
}

void Settings::SaveSettings() 
{
	std::ofstream file;
	std::string SettingsPath = PgUtil::CreateFullFilePathFromApplicationFolder(SettingsSubPath);
	file.open(SettingsPath);
	if (!file.is_open())
	{
		LogError("Failed to Safe Settings");
		return;
	}
	file << "[BaseInformation]" << std::endl;
	file << "ClientPath = " << PgUtil::FolderPath << std::endl << std::endl;
	file << "[Window]" << std::endl;
	file << "Width = " << _Settings._WindowWidth << std::endl;
	file << "Height = " << _Settings._WindowHeight << std::endl;
	if(_Settings._FullScreen)
		file << "FullScreen = true" << std::endl;
	else
		file << "FullScreen = false" << std::endl;

	file.close();

	LogInfo("Settings safed");
	//ClientPath PgUtil::FolderPath;
	//ApplicationPath PgUtil::ApplicationPath -> no need to safe;
}

void Settings::SetResolution(std::string Resoultion)
{
	auto offset = Resoultion.find(" x ");
	std::string w = Resoultion.substr(0, offset);
	std::string h = Resoultion.substr(offset + 3);
	_Settings._WindowWidth = atoi(w.c_str());
	_Settings._WindowHeight = atoi(h.c_str());
	//ClientPath PgUtil::FolderPath;
	//ApplicationPath PgUtil::ApplicationPath -> no need to safe;
}