#pragma once
#include "ShineMobNode.h"
#include "ShineObject/ShineNPC.h"
NiSmartPointer(ShineNPCNode);
class ShineNPCNode : public ShineMobNode
{
	NiDeclareRTTI;
	NiDeclareClone(ShineNPCNode);
public:
	explicit ShineNPCNode(ShineNPCPtr BaseObject);
	virtual void DrawObjectMenu();
protected:
	ShineNPCNode();
	virtual void UpdateActor(NiActorManagerPtr NewActor);
};