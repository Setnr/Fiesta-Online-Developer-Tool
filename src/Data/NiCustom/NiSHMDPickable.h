#pragma once
#include "NiPickable.h"
NiSmartPointer(NiSHMDPickable);
class NiSHMDPickable : public NiPickable 
{
	NiDeclareRTTI;
	NiDeclareClone(NiSHMDPickable);
public:
	NiSHMDPickable();
	~NiSHMDPickable();
	void ExtractCollision();
	void ExtractChildCollision(NiNodePtr childParent);
	NiNodePtr GetCollision() { return Collision; }
	void UpdateCollisionTranslate(const NiPoint3& point);
	void UpdateCollisionRotate(const NiMatrix3& point);
	void SetCollisionScale(float Scale);
	NiNodePtr ToNiNode();
private:
	NiNodePtr Collision;
};