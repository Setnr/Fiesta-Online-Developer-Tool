#pragma once
#include "FiestaOnlineTool.h"

FiestaOnlineTool* GlobalTool;

NiApplication* NiApplication::Create()
{
    NiSourceTexture::SetDestroyAppDataFlag(false);
    GlobalTool = NiNew FiestaOnlineTool;
    return GlobalTool;
}


void FiestaOnlineTool::DrawCursor()
{
    if (!GlobalTool)
        return;
    tagPOINT kPoint;
    GetCursorPos(&kPoint);
    ScreenToClient(GlobalTool->GetRenderWindowReference(), &kPoint);
    cursor->SetPosition(0.0, kPoint.x + 5, kPoint.y + 9);
    cursor->Draw();
}