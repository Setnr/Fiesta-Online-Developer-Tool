#include "NiSHMDPickable.h"
#include <NiCloningProcess.h>
#include "NiBoundingBox.h"

NiImplementRTTI(NiPickable, NiNode);
NiImplementCreateClone(NiPickable)
NiImplementRTTI(NiSHMDPickable, NiPickable);
NiImplementCreateClone(NiSHMDPickable);
void NiSHMDPickable::CopyMembers(
    NiSHMDPickable* pDest, NiCloningProcess& kCloning)
{
    NiPickable::CopyMembers(pDest, kCloning);
}
//---------------------------------------------------------------------------
void NiPickable::CopyMembers(
    NiPickable* pDest, NiCloningProcess& kCloning)
{
    NiNode::CopyMembers(pDest, kCloning);
    pDest->SetFilePathOrName(GetSHMDPath());
}

NiNodePtr NiPickable::ToNiNode()
{
    NiNodePtr ptr = NiNew NiNode;
    NiCloningProcess p;
    NiNode::CopyMembers(ptr, p);
    return ptr;
}


void NiPickable::HideBoundingBox() 
{
    this->DetachChild(BoundingBox);
    this->UpdateProperties();
    this->UpdateEffects();
    this->Update(0.0f);
    BoundingBox = NULL;
    return;
}

void NiPickable::ShowBoundingBox()
{
    BoundingBox = NiBoundingBox::CreateBoundingBox(this);
    if (!BoundingBox)
        return;

    this->AttachChild(BoundingBox);
    this->CompactChildArray();
    this->UpdateProperties();
    this->UpdateEffects();
    this->Update(0.0f);
    return;
}