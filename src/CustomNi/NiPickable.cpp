#include "NiPickable.h"

NiImplementRTTI(NiPickable, NiNode);
NiImplementCreateClone(NiPickable);

//---------------------------------------------------------------------------
void NiPickable::CopyMembers(
    NiPickable* pDest, NiCloningProcess& kCloning)
{
    NiNode::CopyMembers(pDest, kCloning);
}