#include "FiestaOnlineTool.h"
#include "FiestaOnlineTool_GeneralHeaders.h"

void FiestaOnlineTool::CheckInterfaceForHit() {

	tagPOINT kPoint;
	GetCursorPos(&kPoint);
	ScreenToClient(this->GetWindowReference(), &kPoint);
	Pgg_kWinMgr->CheckForHit(kPoint);
}
void FiestaOnlineTool::CheckInterfaceForClick() {

	tagPOINT kPoint;
	GetCursorPos(&kPoint);
	ScreenToClient(this->GetWindowReference(), &kPoint);
	Pgg_kWinMgr->CheckForClick(kPoint);
}
void FiestaOnlineTool::UpdateInterface()
{
	Pgg_kWinMgr->Update();
}
