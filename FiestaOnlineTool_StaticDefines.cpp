#pragma once
#include "FiestaOnlineTool.h"

#include <string>
#include <filesystem>

#include <iostream>
#include <fstream>
#include "SHNManager.h"
FiestaOnlineTool* FiestaOnlineTool::_Tool = NULL;

int PgWinMgr::iScreenLeftPos;
int PgWinMgr::iScreenRightPos;

int PgWinMgr::iScreenWidth;
int PgWinMgr::iScreenHeight;

int PgWinMgr::iScreenTopPos;
int PgWinMgr::iScreenBottomPos;

std::string PgUtil::FolderPath;
std::string PgUtil::ApplicationPath;

NiApplication* NiApplication::Create()
{
    char Path[513];
    GetModuleFileNameA(NULL, Path, sizeof(Path));
    PgUtil::ApplicationPath = std::filesystem::path(Path).parent_path().string();

    std::string ressys = PgUtil::CreateFullFilePathFromApplicationFolder(".\\ressystem");
    if (!std::filesystem::exists(ressys)) 
    {
        PgUtil::FolderPath = "";
    }
    else
        PgUtil::FolderPath = PgUtil::ApplicationPath;
    NiSourceTexture::SetDestroyAppDataFlag(false);
    return NiNew FiestaOnlineTool;
}