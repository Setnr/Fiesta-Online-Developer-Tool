#pragma once
#include <NiNode.h>

NiSmartPointer(NiPickable);
class NiPickable : public NiNode
{
	NiDeclareRTTI;
	NiDeclareClone(NiPickable);
public:
	void DisableSorting() {
		this->SetSortObject(false);
	}
};