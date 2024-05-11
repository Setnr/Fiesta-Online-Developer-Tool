#include <windows.h>



#include "FiestaOnlineTool.h"
/*

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CHAR:
    {
        switch ((unsigned char)wparam)
        {
        case 'w': // toggle wireframe mode
        case 'W':
            gs_spWireframe->SetWireframe(!gs_spWireframe->GetWireframe());
            break;

        case 'q': // terminate the application (ALT+F4 works also)
        case 'Q':
        case VK_ESCAPE:
            PostMessage(hWnd, WM_DESTROY, 0, 0);
            break;
        }
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    }

    return DefWindowProc(hWnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hPI, LPSTR cmdline, int iWinMode)
{
    FiestaOnlineTool tool;
    FiestaOnlineTool::SetTool(&tool);
    //***** basic window setup *****
    static wchar_t lpszName[] = L"Fiesta Dev Tool by Set";

    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hI;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = lpszName;
    RegisterClass(&wc);
   
    // create a window with client area 1024x768
    RECT rect = { 0, 0, 1600, 900 };
    DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    AdjustWindowRect(&rect, dwStyle, false);
    HWND hWnd = CreateWindow(lpszName, lpszName, dwStyle, 0, 0,
        rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hI, NULL);

    ShowWindow(hWnd, iWinMode);
    UpdateWindow(hWnd);

    //***** Gamebryo initialization *****
    NiInit();
    int iSuccess = 0;
    
    if (!tool.Init(hWnd))
        iSuccess = -1;

    if (!tool.MainLoop())
        iSuccess = -1;
    
    NiShutdown();

    return 0;
}*/
