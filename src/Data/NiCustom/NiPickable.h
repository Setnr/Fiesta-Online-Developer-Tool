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
	void SetFilePathOrName(std::string Path) { FilePathOrName = Path; }
	std::string GetSHMDPath() { return FilePathOrName; }
	NiNodePtr ToNiNode();
	void ShowBoundingBox();
	void HideBoundingBox();
	bool BoundingBoxIsVisible() { return BoundingBox != NULL; }
private:
	std::string FilePathOrName;
	NiNodePtr BoundingBox;
};