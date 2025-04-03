#include "NiSHMDPickable.h"
#include <NiMain.h>
#include <vector>
NiSHMDPickable::NiSHMDPickable()
{
	Collision = NiNew NiNode;
}
NiSHMDPickable::~NiSHMDPickable()
{
	Collision = NULL;
}
void NiSHMDPickable::ExtractCollision()
{
	std::vector<NiAVObjectPtr> Removes;
	CompactChildArray();
	for (int i = 0; i < GetChildCount(); i++)
	{
		NiAVObjectPtr child = GetAt(i);
		if (!child)
			continue;
		auto Name = child->GetName();
		if ((Name.Contains("#CD") || Name.Contains("#M")) && NiIsKindOf(NiAVObject, child))
		{
			Removes.push_back(child);
		}
		else
		{
			if (NiIsKindOf(NiNode, child))
			{
				ExtractChildCollision(NiSmartPointerCast(NiNode, child));
			}
			
		}

	}
	for (auto rem : Removes)
	{
		if (NiIsKindOf(NiAVObject, rem))
		{
			DetachChild(rem);
			CompactChildArray();

			if (rem->GetName().Contains("#CD"))
				Collision->AttachChild(rem);
			

		}
	}
	Collision->UpdateProperties();
	Collision->UpdateEffects();
	Collision->Update(0.0f);
}

void NiSHMDPickable::ExtractChildCollision(NiNodePtr childParent)
{
	std::vector<NiAVObjectPtr> Removes;
	childParent->CompactChildArray();
	for (int i = 0; i < childParent->GetChildCount(); i++)
	{
		NiAVObjectPtr child = childParent->GetAt(i);
		if (!child)
			continue;
		auto Name = child->GetName();
		if ((Name.Contains("#CD") || Name.Contains("#M")) && NiIsKindOf(NiAVObject, child))
		{
			Removes.push_back(child);
		}
		else
		{
			if (NiIsKindOf(NiNode, child))
			{
				ExtractChildCollision(NiSmartPointerCast(NiNode, child));
			}
		}

	}
	for (auto rem : Removes)
	{
		if (NiIsKindOf(NiAVObject, rem))
		{
			childParent->DetachChild(rem);
			childParent->CompactChildArray();

			if (rem->GetName().Contains("#CD"))
				Collision->AttachChild(rem);
		}
	}
}

void NiSHMDPickable::UpdateCollisionTranslate(const NiPoint3& point)
{
	Collision->SetTranslate(point);
}

void NiSHMDPickable::UpdateCollisionRotate(const NiMatrix3& point)
{
	Collision->SetRotate(point);
}

void NiSHMDPickable::SetCollisionScale(float Scale)
{
	Collision->SetScale(Scale);
}

NiNodePtr NiSHMDPickable::ToNiNode()
{
	auto node = NiPickable::ToNiNode();
	node->AttachChild(Collision);
	return node;
}
