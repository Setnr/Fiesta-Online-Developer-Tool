#include "NiPickable.h"
#include "NiBoundingBox.h"
#include <NiCloningProcess.h>
NiImplementRTTI(NiPickable, NiNode);
NiImplementCreateClone(NiPickable);

//---------------------------------------------------------------------------
void NiPickable::CopyMembers(
    NiPickable* pDest, NiCloningProcess& kCloning)
{
    NiNode::CopyMembers(pDest, kCloning);
    pDest->SetSHMDPath(GetSHMDPath());
}

NiNodePtr NiPickable::ToNiNode() 
{
    NiNodePtr ptr = NiNew NiNode;
    NiCloningProcess p;
    NiNode::CopyMembers(ptr, p);
    return ptr;
}

void NiPickable::HideBoundingBox(NiNodePtr& BoundingBox) {

    this->DetachChild(BoundingBox);
    this->UpdateProperties();
    this->UpdateEffects();
    this->Update(0.0f);
    return;
}

void NiPickable::ShowBoundingBox(NiNodePtr& BoundingBox)
{
    BoundingBox = NiBoundingBox::CreateBoundingBox(this, BoundingBox);
    
    this->AttachChild(BoundingBox);
    this->CompactChildArray();
    this->UpdateProperties();
    this->UpdateEffects();
    this->Update(0.0f);
    return;
}

