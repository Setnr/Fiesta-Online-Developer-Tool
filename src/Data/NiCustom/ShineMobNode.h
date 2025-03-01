#pragma once
#include "ShineObjectNode.h"
#include "ShineObject/ShineMob.h"
NiSmartPointer(ShineMobNode);
class ShineMobNode : public ShineObjectNode
{
	NiDeclareRTTI;
	NiDeclareClone(ShineMobNode);
public:
	explicit ShineMobNode(ShineMobPtr BaseObject);
	virtual void DrawObjectMenu();
protected:
	ShineMobNode();
};