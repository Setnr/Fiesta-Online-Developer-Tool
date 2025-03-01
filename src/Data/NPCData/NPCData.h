#pragma once
#include <NiMain.h>
#include "ShineObject/ShineNPC.h"
#include <vector>
#include <SHN/SHNStruct.h>

NiSmartPointer(NPCData);
class NPCData : public NiRefObject
{
public:
	static void Init();
	static void Clear();
	static std::vector<ShineNPCPtr>GetNPCsByMap(std::string MapName);
	static ShineNPCPtr CreateNewNPC(MapInfo* Info, NiPoint3 Pos);
	void Load();
	void Save();
	static void SaveNPCs();

	static bool isCN;
private:
	static NPCDataPtr Data;
	std::vector<ShineNPCPtr> _NPCS;
};
