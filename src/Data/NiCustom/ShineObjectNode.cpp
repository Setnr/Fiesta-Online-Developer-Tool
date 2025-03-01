#include "ShineObjectNode.h"
#include "NiBoundingBox.h"
#include <Logger.h>

NiImplementRTTI(ShineObjectNode, NiPickable);
NiImplementCreateClone(ShineObjectNode);

//---------------------------------------------------------------------------
void ShineObjectNode::CopyMembers(
    ShineObjectNode* pDest, NiCloningProcess& kCloning)
{
    NiPickable::CopyMembers(pDest, kCloning);
    LogInfo("Clone ShineObject Node");
}

ShineObjectNode::ShineObjectNode(ShineObjectPtr BaseObject) : NiPickable(), _ShineObject(BaseObject)
{
    SetFilePathOrName(BaseObject->GetName());
}
ShineObjectNode::ShineObjectNode() {}

void ShineObjectNode::UpdateActor(NiActorManagerPtr NewActor)
{
    bool BoundingBox = BoundingBoxIsVisible();
    if (BoundingBox)
        HideBoundingBox();

    this->DetachChild(_Actor->GetNIFRoot());
    _Actor = NewActor;
    this->AttachChild(_Actor->GetNIFRoot(), 0);

    this->CompactChildArray();
    this->UpdateProperties();
    this->UpdateEffects();
    this->Update(0.0f);

    if (BoundingBox)
        ShowBoundingBox();
}