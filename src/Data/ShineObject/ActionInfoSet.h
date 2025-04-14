#pragma once
#include <map>
#include "PgUtil.h"

struct ActionInfo
{
	unsigned __int16 ID;
	char InxName[33];
	char ActionName[33];
	unsigned int dwWeaponDemandMask;
	char AniInx[33];
	unsigned int uiEventCode;
	unsigned int WeaponType;
	char EffFile[3][33];
	unsigned int EffPos[3];
	unsigned int EffNote[3];
	int EffKeep[3];
	int EffDir[3];
	char SndFile[2][33];
	unsigned int SndPos[2];
	unsigned int SndNote[2];
	int SndRandPlay;
};

NiSmartPointer(ActionInfoSet);
class ActionInfoSet : public NiRefObject 
{
public:
	ActionInfoSet(std::string MobInx) 
	{
		std::string Path = PgUtil::PathFromClientFolder("ressystem\\Action\\" + MobInx + ".dat");
		FILE* DatFile = fopen(Path.c_str(), "r");
		if (DatFile) 
		{
			fread(&total, 4, 1, DatFile);
			FirstPointer = NiAlloc(ActionInfo, total);
			if (FirstPointer) 
			{
				fread(FirstPointer, 352 * total, 1, DatFile);
			}
			fclose(DatFile);
		}
	}
	ActionInfo* GetActionByID(int ID) 
	{
		if (!FirstPointer)
			return nullptr;
		return &FirstPointer[ID];
	}
	~ActionInfoSet() 
	{
		NiFree(FirstPointer);
	}
	ActionInfo* FirstPointer = nullptr;
	int total;
};

NiSmartPointer(ActionInfoSet);
class ActionInfoSetManager : public NiRefObject 
{
public:
	static void Clear() {
		InxActionMap.clear();
	}
	static ActionInfoSetPtr GetActionInfoSet(std::string MobInx) {
		auto it = InxActionMap.find(MobInx);
		if (it == InxActionMap.end()) 
		{
			ActionInfoSetPtr set = NiNew ActionInfoSet(MobInx);
			InxActionMap.insert({ MobInx,set });
			return set;
		}
		else
		{
			return it->second;
		}
	}
private:
	static std::map<std::string, ActionInfoSetPtr> InxActionMap;

};