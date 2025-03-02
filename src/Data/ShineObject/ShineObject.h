#pragma once 
#include <NiMainLibType.h>
#include <NiMain.h>
#include <NiRTTI.h>
#include <string>

NiSmartPointer(ShineObject);
class ShineObject : public NiRefObject
{
	NiDeclareRootRTTI(ShineObject);
public:
	ShineObject() = default;
	virtual std::string GetName() { return "ShineObject"; }
	std::string GetMapInx() { return _MapInx; }
	NiPoint3 GetPos() { return _Pos; }
	float GetRotation() { return _Rotation; }
	virtual void UpdatePos(NiPoint3 NewPos) 
	{ 
		_Pos = NewPos;
	}
	virtual void UpdateRotation(float NewRotation) 
	{ 
		_Rotation = NewRotation; 
	}
protected:
	std::string _MapInx;
	NiPoint3 _Pos;
	float _Rotation;
};