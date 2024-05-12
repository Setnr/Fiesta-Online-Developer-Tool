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
    NiSourceTexture::SetDestroyAppDataFlag(false);
    return NiNew FiestaOnlineTool;
}