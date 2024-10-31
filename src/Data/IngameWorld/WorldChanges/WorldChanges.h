#pragma once
#include <NiRTTI.h>
#include <NiMain.h>

NiSmartPointer(WorldChange);
class WorldChange : public NiRefObject
{
public:
	NiDeclareRootRTTI(WorldChange);
	WorldChange() = default;
	virtual void Undo() {}
	virtual void Update(WorldChangePtr) {}
	virtual void Redo() {}
	virtual bool ExtraCheck(WorldChangePtr) { return false; }
};