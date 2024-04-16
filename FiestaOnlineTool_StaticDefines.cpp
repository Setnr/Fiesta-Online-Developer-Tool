#pragma once
#include "FiestaOnlineTool.h"

#include <string>
#include <filesystem>
FiestaOnlineTool* FiestaOnlineTool::_Tool = NULL;

int PgWinMgr::iScreenLeftPos;
int PgWinMgr::iScreenRightPos;

int PgWinMgr::iScreenWidth;
int PgWinMgr::iScreenHeight;

int PgWinMgr::iScreenTopPos;
int PgWinMgr::iScreenBottomPos;

std::string PgUtil::FolderPath;

NiApplication* NiApplication::Create()
{
    char Path[513];
    GetModuleFileNameA(NULL, Path, sizeof(Path));
    PgUtil::FolderPath = std::filesystem::path(Path).parent_path().string();

    NiSourceTexture::SetDestroyAppDataFlag(false);
    return NiNew FiestaOnlineTool;
}