#include "NPCData.h"
#include "PgUtil.h"
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>


NPCDataPtr NPCData::Data = nullptr;
bool NPCData::isCN = false;
void NPCData::Init() 
{
	Data = NiNew NPCData;
	Data->Load();
}
void NPCData::Clear()
{
	Data = NULL;
}

void NPCData::Load() 
{
	auto Path = PgUtil::PathFromServerFolder(".\\Shine\\World\\NPC.txt");
	if (!std::filesystem::exists(Path)) 
	{
		NiMessageBox::DisplayMessage("Can´t find NPC.txt", "Error");
		exit(0);
	}
	std::ifstream npctxt(Path);
	if (!npctxt.is_open())
	{
		NiMessageBox::DisplayMessage("Failed to open NPC.txt", "Error");
		return;
	}
	std::string line;
	ShineNPCPtr LastNPC;
	while (getline(npctxt, line)) 
	{
		if (line.starts_with(";"))
			continue;
		if (line.contains("LevelFrom") && line.contains("LevelTo")) 
		{
			isCN = true;
		}
		if (line.contains("#recordin"))
		{
			std::regex del("\t");
			std::vector<std::string> list = { std::sregex_token_iterator(line.begin(), line.end(), del, -1), std::sregex_token_iterator() };
			std::vector<std::string>::iterator end = list.end();
			for(auto iter = list.begin(); iter != end; iter++)
			{
				if (*iter == "")
					continue;
				if (*iter != "#recordin")
				{
					if (*iter == "ShineNPC")
					{
						LastNPC = NiNew ShineNPC(iter, end);
						_NPCS.push_back(LastNPC);
						break;
					}
					else if (*iter == "LinkTable")
					{
						LastNPC->AppendGate(iter, end);
						break;
					}
				}
			}
		}
	}

}
void NPCData::Save()
{
	auto Path = PgUtil::PathFromServerFolder(".\\Shine\\World\\NPC.txt");
	if (std::filesystem::exists(Path + ".bak"))
		std::filesystem::remove(Path + ".bak");
	if (std::filesystem::exists(Path))
		std::filesystem::copy(Path, Path + ".bak");
	std::ofstream npctxt(Path);
	if (!npctxt.is_open())
	{
		LogError("Can´t open NPC.txt");
		return;
	}
	npctxt << "\t;Updated with Fiesta DeveloperTools by SetNr12" << std::endl;;
	npctxt << "\t;Shine NPC table(ShineNPC.txt)	" << std::endl;
	npctxt << "	#ignore	\\o042				; 인용부호 무시		" << std::endl;
	npctxt << "	#exchange	#	\\x20			; # => space		" << std::endl;
	npctxt << "	#delimiter	\\x20				; Space is delimiter	" << std::endl;
	npctxt << "\t#Table ShineNPC" << std::endl;
	npctxt << "\t#ColumnType\tSTRING[33]\tSTRING[20]\tDWRD\tDWRD\tWORD\tBYTE\tINDEX\tINDEX" << std::endl;
	npctxt << "\t#ColumnName\tMobName\tMap\tCoord-X\tCoord-Y\tDirect\tNPCMenu\tRole\tRoleArg0" << std::endl;
	npctxt << std::endl;
	npctxt << "\t#Table LinkTable" << std::endl;
	if (!NPCData::isCN)
	{
		npctxt << "\t#ColumnType\tIndex\tSTRING[33]\tSTRING[33]\tDWRD\tDWRD\tWORD\tBYTE" << std::endl;
		npctxt << "\t#ColumnName\targument\tMapServer\tMapClient\tCoord-X\tCoord-Y\tDirect\tParty" << std::endl;
	}
	else {
		npctxt << "\t#ColumnType\tIndex\tString[33]\tString[33]\tDWRD\tDWRD\tWORD\tWORD\tWORD\tBYTE" << std::endl;
		npctxt << "\t#ColumnName\targument\tMapServer\tMapClient\tCoord-X\tCoord-Y\tDirect\tLevelFrom\tLevelTo\tParty" << std::endl;
	}
	npctxt << std::endl;
	for (auto npc : _NPCS) 
	{
		npctxt << "\t"<< npc->toString() << std::endl;
	}
	npctxt << "\t#End" << std::endl;
}
void NPCData::SaveNPCs()
{
	Data->Save();
}
std::vector<ShineNPCPtr>NPCData::GetNPCsByMap(std::string MapName) 
{
	std::vector<ShineNPCPtr> data;
	if (!Data)
	{
		LogError("No NPC.txt was laoded");
		return data;
	}
	for (auto NPC : Data->_NPCS) 
	{
		if (MapName == NPC->GetMapInx()) 
		{
			data.push_back(NPC);
		}
	}
	return data;
}

ShineNPCPtr NPCData::CreateNewNPC(MapInfo* Info, NiPoint3 Pos)
{
	ShineNPC* npc = NiNew ShineNPC(Info, Pos);
	Data->_NPCS.push_back(npc);
	return npc;
}

std::vector<std::pair<std::string,ShineGate::LinkDataPtr>> NPCData::GetSpawnPointsByMap(std::string MapName)
{
	std::vector<std::pair<std::string, ShineGate::LinkDataPtr>> list;
	for (auto data : Data->_NPCS) 
	{
		if (NiIsKindOf(ShineGate, data->GetRole())) 
		{
			ShineGatePtr gate = NiSmartPointerCast(ShineGate, data->GetRole());
			for (auto link : gate->GetLinkData()) 
			{
				if (link->MapClient == MapName)
					list.push_back({ gate->GetArgument(),link});
			}
		}
	}
	return list;
}

void NPCData::RemoveNPC(ShineNPCPtr npc)
{
	auto it = std::find(Data->_NPCS.begin(), Data->_NPCS.end(), npc);
	if (it != Data->_NPCS.end())
		Data->_NPCS.erase(it);
}

