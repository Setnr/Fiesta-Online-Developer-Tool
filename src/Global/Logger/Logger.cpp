#include "Logger.h"
#include <NiMain.h> //;
Logger Logger::_Logger;

Logger::Logger() {
	LogInfo("StartUp DeveloperTools by Set");
}

void Logger::Log(ImColor col, std::string msg)
{
	std::lock_guard<std::mutex> lock(InfoListLock);
	InfoList.push_back({ col,msg, NiGetCurrentTimeInSec() });
	if (InfoList.size() >= 24)
		InfoList.erase(InfoList.begin());
}
void Logger::Draw()
{
	_Logger.DrawInternal();

}
void Logger::DrawInternal()
{
	std::lock_guard<std::mutex> lock(ErrorListLock);
	for (auto it = InfoList.begin(); it != InfoList.end();)
	{
		if (std::get<float>(*it) + 25.0 < NiGetCurrentTimeInSec())
		{
			it = InfoList.erase(it);
			continue;
		}
		ImGui::TextColored(std::get<ImColor>(*it), std::get<std::string>(*it).c_str());
		it++;
	}
}
void Logger::ErrorLog(ImColor col, std::string msg)
{
	std::lock_guard<std::mutex> lock(ErrorListLock);
	ErrorList.push_back({ col,msg, NiGetCurrentTimeInSec() });
	if (ErrorList.size() >= 24)
		ErrorList.erase(ErrorList.begin());
}

void Logger::DrawError() 
{
	_Logger.DrawErrorInternal();
}

void Logger::DrawErrorInternal() 
{
	auto io = ImGui::GetIO();
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	ImGui::SetNextWindowPos(ImVec2(0, 25));
	ImGui::SetNextWindowSize(ImVec2(1000, 0));
	ImGui::Begin("ErrorLogger", NULL, flags);
	{
		std::lock_guard<std::mutex> lock(ErrorListLock);
		for (auto it = ErrorList.begin(); it != ErrorList.end();)
		{
			if (std::get<float>(*it) + 15.0 < NiGetCurrentTimeInSec()) 
			{
				it = ErrorList.erase(it);
				continue;
			}
			ImGui::TextColored(std::get<ImColor>(*it), std::get<std::string>(*it).c_str());
			it++;
		}
	}
	ImGui::End();
}