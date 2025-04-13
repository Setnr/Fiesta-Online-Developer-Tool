#pragma once

#include "ShineObject.h"
#include "CharShape.h"
#include <SHN/SHNManager.h>
#include <SHN/MobLoader.h>

NiSmartPointer(ShineMob);
class ShineMob : public ShineObject
{
	NiDeclareRTTI;
public:
	ShineMob();

	virtual std::string GetName() { return _MobInx; }
	std::string GetMobIndex() { return _MobInx; }
	void SetMobIndex(std::string MobInx) { 
		_MobInx = MobInx;
		_MobInfo = MobLoader::GetMobInfo(_MobInx);
		_MobViewInfo = MobLoader::GetMobViewInfo(_MobInfo->ID);
	}
	virtual void DrawObjectMenu() override;
	void LoadActor() override;
	void UpdateActor(NiActorManagerPtr NewActor)override;
protected:
	std::string _MobInx;
	MobInfo* _MobInfo;
	MobViewInfo* _MobViewInfo;
	NPCViewInfo* _NPCViewInfo;
	CharShapePtr _Shape;
};