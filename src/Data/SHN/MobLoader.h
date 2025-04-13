#pragma once 
#include <NiMain.h>
#include <string>
#include <map>
#include "SHNStruct.h"
#include <NiActorManager.h>
#include <mutex>

class MobLoader 
{
public:
	static NiActorManager* GetNodeByName(std::string MobInx);
	static NiActorManager* GetNodeByMobInfo(MobInfo* info);
	static MobInfo* GetMobInfo(std::string MobInx);
	static MobInfo* GetMobInfo(unsigned short ID);
	static MobViewInfo* GetMobViewInfo(unsigned short ID);
	static NiActorManager* GetPlayerKFM(std::string Combine = "Fighter-m");

	static void Clear();
private:
	static std::mutex ActorLoadLock;
	static std::map<unsigned short, NiActorManagerPtr> IDNodeMap;
	static std::map<std::string, NiActorManagerPtr> PlayerKFMMap;;
	static NiActorManager* LoadMob(unsigned short ID);
};