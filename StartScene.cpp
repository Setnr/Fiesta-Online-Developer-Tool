#include "StartScene.h"
#include "PgUtil.h"
#include <NiThread.h>
#include "EditorScene.h"
DWORD WINAPI HandleLoading(void*  ptr)
{
    StartScene* p = (StartScene * )ptr;
    p->RunThread();
    return 1;
}
StartScene::StartScene() 
{
    _procedure = NULL;
    _Thread = NULL;
    Loaded = false;
    NiSortAdjustNodePtr sortNode = NiNew NiSortAdjustNode;
    sortNode->SetSortingMode(NiSortAdjustNode::SORTING_INHERIT);
    BaseNode = sortNode;
    BaseNode->SetName("Scene BaseNode");


    std::string Path = PgUtil::CreateFullFilePathFromBaseFolder(".\\resmenu\\account\\LoginBackground.nif");
    NiNodePtr NiN = PgUtil::LoadNifFile(Path.c_str(), 0);
    BaseNode->AttachChild(NiN, 1);

    return;
    _procedure = NiNew StartSceneBackgroundThread(this);
    _Thread = NiThread::Create(_procedure);
    NIASSERT(_Thread)
    _Thread->SetPriority(NiThread::NORMAL);
    _Thread->Resume();
}

void StartScene::RunThread()
{
    char FileNameBuffer[MAX_PATH];
    char FilePathBuffer[MAX_PATH];
    OPENFILENAMEA l = { 0x0 };

    ZeroMemory(FileNameBuffer, sizeof(FileNameBuffer));
    ZeroMemory(FilePathBuffer, sizeof(FilePathBuffer));
    ZeroMemory(&l, sizeof(l));

    l.lStructSize = sizeof(OPENFILENAMEA);
    l.hwndOwner = NULL;
    l.lpstrFilter = "SHMD-File\0*.shmd";
    l.lpstrFile = FilePathBuffer;
    l.nMaxFile = MAX_PATH;
    l.lpstrTitle = "Open SHMD-File";
    l.lpstrDefExt = "shmd";
    l.lpstrFileTitle = FileNameBuffer;
    l.nMaxFileTitle = MAX_PATH;
    l.lpstrInitialDir = NULL;
    l.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | 0x02000000;

    if (GetOpenFileNameA(&l))
    {

        LoadedScene = NiNew EditorScene(FilePathBuffer, FileNameBuffer);
        LoadingFinished();
    }
    else
    {
        DWORD error = CommDlgExtendedError();
        char ErrorMessage[1024];
        NiSprintf(ErrorMessage, sizeof(ErrorMessage), "Failed to create OpenFileDialog with Error %x", error);
        NiMessageBox::DisplayMessage(ErrorMessage, "Error");
    }
}