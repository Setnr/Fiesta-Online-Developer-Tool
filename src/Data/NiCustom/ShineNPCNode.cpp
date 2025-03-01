#include "ShineNPCNode.h"
#include <Logger.h>

NiImplementRTTI(ShineNPCNode, ShineMobNode);
NiImplementCreateClone(ShineNPCNode);
ShineNPCNode::ShineNPCNode(ShineNPCPtr BaseObject) 
	: ShineMobNode(NiSmartPointerCast(ShineMob, BaseObject)) 
{
    if(_Actor)
        _Actor->ActivateSequence(101000);
}
void ShineNPCNode::CopyMembers(
    ShineNPCNode* pDest, NiCloningProcess& kCloning)
{
    ShineMobNode::CopyMembers(pDest, kCloning);
    LogInfo("Clone ShineNPCNode Node");
}

ShineNPCNode::ShineNPCNode(){}

void ShineNPCNode::DrawObjectMenu() 
{
    ShineMobNode::DrawObjectMenu();

    if (!NiIsKindOf(ShineNPC, _ShineObject))
    {
        LogError("ShineNPCNode doesnt have a ShineNPC Object");
        return; 
    }
    ShineNPCPtr npc = NiSmartPointerCast(ShineNPC, _ShineObject);
    ImGui::Checkbox("Has Menu", npc->HasMenu());
    npc->DrawRole();
}

void ShineNPCNode::UpdateActor(NiActorManagerPtr NewActor) 
{
    ShineMobNode::UpdateActor(NewActor);
    _Actor->ActivateSequence(101000);
}