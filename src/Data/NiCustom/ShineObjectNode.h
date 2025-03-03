#pragma once
#include "NiPickable.h"
#include <string>
#include "ShineObject/ShineObject.h"
#include <NiActorManager.h>
NiSmartPointer(ShineObjectNode);
class ShineObjectNode : public NiPickable
{
	NiDeclareRTTI;
	NiDeclareClone(ShineObjectNode);
public:
	ShineObjectNode(ShineObjectPtr BaseObject);
	virtual void UpdatePos(NiPoint3 NewPos)
	{
		this->SetTranslate(NewPos);
		_ShineObject->UpdatePos(NewPos);
	}
	virtual void UpdateRotation(float Rot) 
	{
		auto fAngle = (Rot * NI_PI) / 180.0;
		NiMatrix3 m;
		m.MakeZRotation(fAngle);
		this->SetRotate(m);
		
		_ShineObject->UpdateRotation(Rot);
	}
	NiPoint3 GetPos() {
		return _ShineObject->GetPos();
	}
	float GetRotation() {
		return _ShineObject->GetRotation();
	}
	virtual void DrawObjectMenu() {};
	ShineObjectPtr GetShineObject() { return _ShineObject; }
protected:
	virtual void UpdateActor(NiActorManagerPtr NewActor);
	ShineObjectPtr _ShineObject; 
	NiActorManagerPtr _Actor;
	ShineObjectNode();
};