#include "Logger.h"

Logger Logger::_Logger;

Logger::Logger() {
	LogInfo("StartUp DeveloperTools by Set");
}

void Logger::Log(ImColor col, std::string msg)
{
	std::lock_guard<std::mutex> lock(InfoListLock);
	InfoList.push_back({ col,msg });
	if (InfoList.size() >= 24)
		InfoList.erase(InfoList.begin());
}
void Logger::Draw()
{
	_Logger.DrawInternal();

}
void Logger::DrawInternal()
{
	std::lock_guard<std::mutex> lock(InfoListLock);
	for (auto it : InfoList)
	{
		ImGui::TextColored(it.first, it.second.c_str());
	}
}