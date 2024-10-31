#pragma once
#include <PgUtil.h>

#include <filesystem>
#include <iostream>
#include <fstream>

#include "IniReader.h"
#include <Logger.h>
class Settings 
{

public:
	struct Resolution {
		int Height;
		int Width;
	};
	static bool Load()
	{
		char Path[513];
		GetModuleFileNameA(NULL, Path, sizeof(Path));
		PgUtil::ApplicationPath = std::filesystem::path(Path).parent_path().string();
		auto SettingsPath = PgUtil::PathFromApplicationFolder(".\\FiestaOnlineTool\\Settings.ini");
		if (!std::filesystem::exists(std::filesystem::path(SettingsPath))) 
		{
			return false;
		}
		INIReader reader(SettingsPath);
		PgUtil::ClientFolderPath = reader.GetString("BaseInformation", "ClientPath", "");
		if (PgUtil::ClientFolderPath == "" || !std::filesystem::exists(PgUtil::ClientFolderPath))
			_ClientIsSetup = false;
		else
			_ClientIsSetup = true;
		_Resolution.Width = reader.GetInteger("Window", "Width", 1600);
		_Resolution.Height = reader.GetInteger("Window", "Height", 900);
		_IsFullScreen = reader.GetBoolean("Window", "FullScreen", false);
		_SaveInternal = reader.GetReal("BaseInformation", "SaveInterval", 5.f);
		return true;

	}
	static void SaveSettings()
	{
		std::ofstream file;
		std::string SettingsPath = PgUtil::PathFromApplicationFolder(".\\FiestaOnlineTool\\Settings.ini");
		file.open(SettingsPath);
		if (!file.is_open())
		{
			LogError("Failed to Safe Settings");
			return;
		}

		file << "[Window]" << std::endl;
		file << "Width = " << _Resolution.Width << std::endl;
		file << "Height = " << _Resolution.Height << std::endl;
		if (_IsFullScreen)
			file << "FullScreen = true" << std::endl;
		else
			file << "FullScreen = false" << std::endl;

		file << "[BaseInformation]" << std::endl;
		file << "ClientPath = " << PgUtil::ClientFolderPath << std::endl;

		file << "SaveInterval = " << _SaveInternal << std::endl << std::endl;

		file.close();
	}
	static bool IsFullScreen() { return _IsFullScreen; };
	static void SetFullScreen(bool IsFullScreen) { _IsFullScreen = IsFullScreen; }
	static Resolution GetResolution() { return _Resolution; }
	static void SetResolution(int Height, int Width) { _Resolution.Height = Height; _Resolution.Width = Width; }
	static bool IsClientSetup() { return _ClientIsSetup; }
private:
	static bool _IsFullScreen;
	static bool _ClientIsSetup;
	static Resolution _Resolution;
	static float _SaveInternal;
};