#include "StartScene.h"
#include "PgUtil.h"
StartScene::StartScene() 
{
    BaseNode->SetSortingMode(NiSortAdjustNode::SORTING_INHERIT);

    NiNodePtr NiN = PgUtil::LoadNifFile(".\\resmenu\\account\\LoginBackground.nif", 0);

    BaseNode->AttachChild(NiN, 1);
}