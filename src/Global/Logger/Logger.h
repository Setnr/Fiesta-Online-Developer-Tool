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
#define LogDebug(info) Logger::_Logger.Log(ImColor(0.0f,1.0f,1.0f),Msg(info))

#else
#define Msg(info) std::string("[" + std::format("{:%T}", std::chrono::system_clock::now()) + "] " + " -> " + info)
#define LogDebug(info) 
#endif

#define LogError(info) Logger::_Logger.Log(ImColor(1.0f,0.0f,0.0f),Msg(info))
#define LogWarning(info) Logger::_Logger.Log(ImColor(1.0f,0.8f,0.0f),Msg(info))
#define LogInfo(info) Logger::_Logger.Log(ImColor(.333f,0.776f,0.952f),Msg(info))
#define LogTime(info, start) auto diff = std::chrono::steady_clock::now() - start; \
							std::ostringstream oss;\
							oss << info << " " << std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms";\
							LogInfo(oss.str());
#define LogLua(File,msg)  Logger::_Logger.Log(ImColor(1.0f,0.8f,0.0f),Msg(File +".lua\n" + msg))

