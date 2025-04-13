#pragma once 
#include <NiMainLibType.h>
#include <NiMain.h>
#include <NiRTTI.h>
#include <string>
#include "NiCustom/NiPickable.h"
#include <NiActorManager.h>

NiSmartPointer(ShineObject);
class ShineObject : public NiPickable
{
	NiDeclareRTTI;
public:
	ShineObject() = default;
	virtual std::string GetName() { return "ShineObject"; }
	std::string GetMapInx() { return _MapInx; }
	NiPoint3 GetPos() { return _Pos; }
	float GetRotation() { return _Rotation; }
	virtual void UpdatePos(NiPoint3 NewPos) 
	{ 
		_Pos = NewPos;
		NiNode::SetTranslate(NewPos);
	}
	virtual void UpdateRotation(float NewRotation) 
	{ 
		auto fAngle = (NewRotation * NI_PI) / 180.0;
		NiMatrix3 m;
		m.MakeZRotation(fAngle);
		NiNode::SetRotate(m);
		_Rotation = NewRotation; 
	}
	virtual void DrawObjectMenu(){}
	virtual void UpdateActor(NiActorManagerPtr NewActor);
	bool HasActor() { return _Actor; }
	virtual void LoadActor(){}
protected:
	std::string _MapInx;
	NiPoint3 _Pos;
	float _Rotation;
	NiActorManagerPtr _Actor;
	unsigned short _Handle;
};