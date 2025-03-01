#pragma once

#include "ShineObject.h"

NiSmartPointer(ShineMob);
class ShineMob : public ShineObject
{
	NiDeclareRTTI;
public:
	ShineMob() = default;

	virtual std::string GetName() { return _MobInx; }
	std::string GetMobIndex() { return _MobInx; }
	void SetMobIndex(std::string MobInx) { _MobInx = MobInx; }
protected:
	std::string _MobInx;

};