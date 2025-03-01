#include "ShineMobNode.h"
#include <Logger.h>
#include "SHN/SHNStruct.h"
#include "SHN/MobLoader.h"

NiImplementRTTI(ShineMobNode, ShineObjectNode);
NiImplementCreateClone(ShineMobNode);

//---------------------------------------------------------------------------
void ShineMobNode::CopyMembers(
    ShineMobNode* pDest, NiCloningProcess& kCloning)
{
    ShineObjectNode::CopyMembers(pDest, kCloning);
    LogInfo("Clone ShineMobNode Node");
}

ShineMobNode::ShineMobNode(ShineMobPtr BaseObject) : ShineObjectNode(NiSmartPointerCast(ShineObject,BaseObject))
{

    _Actor = MobLoader::GetNodeByName(BaseObject->GetName());
    if (!_Actor) 
    {
        LogError("Cant create Mob for " + BaseObject->GetName());
        return;
    }
    this->AttachChild(_Actor->GetNIFRoot(), 0);

    this->CompactChildArray();
    this->UpdateProperties();
    this->UpdateEffects();
    this->Update(0.0f);
}
ShineMobNode::ShineMobNode(){}
void ShineMobNode::DrawObjectMenu()
{
    if (!NiIsKindOf(ShineMob, _ShineObject))
    {
        LogError("ShineMobNode doesnt have a ShineMob Object");
        return;
    }
    ShineMobPtr mob = NiSmartPointerCast(ShineMob, _ShineObject);
    char buffer[33];
    strcpy_s(buffer, mob->GetMobIndex().c_str());
    if(ImGui::InputText("MobInx", buffer,sizeof(buffer)))
    {
        NiActorManagerPtr NewNode = MobLoader::GetNodeByName(buffer);
        if (NewNode) 
        {
            mob->SetMobIndex(buffer);
            UpdateActor(NewNode);
        }
    }
}
