#include "PgWinObj.h"
#include "PgWin.h"

const NiRTTI PgWin::ms_RTTI("PgWin",NULL);
const NiRTTI PgWinObj::ms_RTTI("PgWinObj", NULL);
const NiRTTI PgWinHoverObj::ms_RTTI("PgWinHoverObj", &PgWinObj::ms_RTTI);
const NiRTTI PgWinButtonObj::ms_RTTI("PgWinButtonObj", &PgWinHoverObj::ms_RTTI);

bool PgWinButtonObj::Click(PgWin* pWin)
{
	if (_ClickedSprite->GetParent())
		return false;
	ParentObj->AttachChild(_ClickedSprite);
	_HoveredSprite->DetachParent();
	_RegularSprite->DetachParent();
	pWin->OnCommand(ClickID);
	return true;
}