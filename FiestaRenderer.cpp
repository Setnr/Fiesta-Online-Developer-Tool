#include "FiestaRenderer.h"
#include <string>
FiestaRenderer::FiestaRenderer() 
{
    auto lib = LoadLibraryA("FiestaRenderer.dll");
    if (!lib)
    {
        auto err = GetLastError();
        std::string msg = "Failed to Load Library with Error: " + std::to_string(err);
        OutputDebugStringA(msg.c_str());
        return;
    }
    if (!(this->WinMain = (WinMain_Render)GetProcAddress(lib, "_WinMain2@16"))) 
    {
        auto err = GetLastError();
        std::string msg = "Failed to Load Function with Error: " + std::to_string(err);
        OutputDebugStringA(msg.c_str());
        return;
    }
	Init = true;
}