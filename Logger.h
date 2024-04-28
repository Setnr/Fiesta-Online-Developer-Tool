#pragma once
#include <vector>
#include <mutex>
#include "ImGui/imgui.h"

class Logger 
{
private:

	std::vector<std::pair<ImColor, std::string>> InfoList;
	std::mutex InfoListLock;
public:
	static Logger _Logger;
	Logger();
	void Log(ImColor col, std::string msg);

	static void Draw();
	void DrawInternal();
};

#include <format>
#include <chrono>
#ifdef _DEBUG
constexpr std::string GetFunctionNameWithClass(const char* s)
{
	const std::string name(s);
	const auto bracket = name.rfind('(');
	const auto space = name.rfind(' ', bracket) + 1;

	if (const auto lambda = name.find("::<"); lambda != std::string::npos)
	{
		return name.substr(space, lambda - space);
	}

	return name.substr(space, bracket - space);
}

#define __FUNCNAME__ GetFunctionNameWithClass(__FUNCSIG__)

#define Msg(info) std::string("[" + std::format("{:%T}", std::chrono::system_clock::now()) + "] "  + __FUNCNAME__ + " -> " + info)

#define Debug(info) Logger::_Logger.Log(ImColor(0.0f,1.0f,1.0f),Msg(info))
#else
#define Msg(info) std::string("[" + std::format("{:%T}", std::chrono::system_clock::now()) + "] " + " -> " + info)
#define Debug(info) 
#endif

#define Error(info) Logger::_Logger.Log(ImColor(1.0f,0.0f,0.0f),Msg(info))
#define Warning(info) Logger::_Logger.Log(ImColor(1.0f,0.8f,0.0f),Msg(info))
#define Info(info) Logger::_Logger.Log(ImColor(1.f,0.706f,.035f),Msg(info))