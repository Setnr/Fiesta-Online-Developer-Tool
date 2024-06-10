#pragma once
#include <NiNode.h>
#include <string>
NiSmartPointer(NiPickable);
class NiPickable : public NiNode
{
	NiDeclareRTTI;
	NiDeclareClone(NiPickable);
public:
	void DisableSorting() {
		this->SetSortObject(false);
	}
	void SetSHMDPath(std::string Path) { SHMDPath = Path; }
	std::string GetSHMDPath() { return SHMDPath; }
	NiNodePtr ToNiNode();
	void ShowBoundingBox(NiNodePtr& BoundingBox);
	void HideBoundingBox(NiNodePtr& BoundingBox);
private:
	std::string SHMDPath;
};