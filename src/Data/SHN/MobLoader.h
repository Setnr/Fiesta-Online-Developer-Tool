#pragma once 
#include <NiMain.h>
#include <string>
#include <map>
#include "SHNStruct.h"
#include <NiActorManager.h>

class MobLoader 
{
public:
	static NiActorManager* GetNodeByName(std::string MobInx);
	static MobInfo* GetMobInfo(std::string MobInx);
	static MobInfo* GetMobInfo(unsigned short ID);
	static MobViewInfo* GetMobViewInfo(unsigned short ID);
	static void Clear();
private:
	static std::map<unsigned short, NiActorManagerPtr> IDNodeMap;
	static NiActorManager* LoadMob(unsigned short ID);
};