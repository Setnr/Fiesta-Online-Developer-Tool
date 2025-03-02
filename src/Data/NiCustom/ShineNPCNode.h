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

NiSmartPointer(GateSpawnPoint);
class GateSpawnPoint : public ShineMobNode 
{
	NiDeclareRTTI;
public:
	GateSpawnPoint(std::string Argument, ShineGate::LinkDataPtr LinkData);
	virtual void DrawObjectMenu();
private:
	std::string _Argument;
};