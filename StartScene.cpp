#include "StartScene.h"
#include "PgUtil.h"
#include <NiThread.h>
DWORD WINAPI HandleLoading(void*  ptr)
{
    StartScene* p = (StartScene * )ptr;
    p->RunThread();
    return 1;
}
StartScene::StartScene() 
{
    BaseNode->SetSortingMode(NiSortAdjustNode::SORTING_INHERIT);

    NiNodePtr NiN = PgUtil::LoadNifFile(".\\resmenu\\account\\LoginBackground.nif", 0);

    BaseNode->AttachChild(NiN, 1);

    UMBAUEN AUF NITHREAD WEIL WILL NUTZEN NISHIT

    _Thread = CreateThread(NULL, 0, HandleLoading, this, 0, NULL);
}

void StartScene::RunThread() 
{
    char szFileNameIN[MAX_PATH];
    char buffer[MAX_PATH];
    OPENFILENAMEA l = { 0x0 };

    ZeroMemory(szFileNameIN, sizeof(szFileNameIN));
    ZeroMemory(buffer, sizeof(buffer));
    ZeroMemory(&l, sizeof(l));

    l.lStructSize = sizeof(OPENFILENAMEA);
    l.hwndOwner = NULL;
    l.lpstrFilter = "SHMD-File\0*.shmd";
    l.lpstrFile = buffer;
    l.nMaxFile = MAX_PATH;
    l.lpstrTitle = "Open SHMD-File";
    l.lpstrDefExt = "shmd";
    l.lpstrFileTitle = szFileNameIN;
    l.nMaxFileTitle = MAX_PATH;
    l.lpstrInitialDir = NULL;
    l.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | 0x02000000;

    if (GetOpenFileNameA(&l)) 
    {
        UtilDebugString(szFileNameIN);
        UtilDebugString(buffer);
    }
    else 
    {
        DWORD error = CommDlgExtendedError();
        char ErrorMessage[1024];
        NiSprintf(ErrorMessage, sizeof(ErrorMessage), "Failed to create OpenFileDialog with Error %x",  error);
        NiMessageBox::DisplayMessage(ErrorMessage, "Error");
    }