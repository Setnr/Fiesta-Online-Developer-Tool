#include "ShineNPC.h"
#include "Logger.h"
#include <SHN/MobLoader.h>
#include <NPCData/NPCData.h>
#include <string>
#include <algorithm>
#include <iostream>

NiImplementRTTI(ShineNPC, ShineMob);
NiImplementRTTI(GateSpawn, ShineMob);
NiImplementRootRTTI(NPCRole);
NiImplementRTTI(JobManager, NPCRole);
NiImplementRTTI(QuestNPC, NPCRole);
NiImplementRTTI(StoreManager, NPCRole);
NiImplementRTTI(ClientMenu, NPCRole);
NiImplementRTTI(Guard, NPCRole);
NiImplementRTTI(Merchant, NPCRole);
NiImplementRTTI(NPCMenu, NPCRole);
NiImplementRTTI(ShineGate, NPCRole);
NiImplementRTTI(IDGate, ShineGate);
NiImplementRTTI(ModeIDGate, ShineGate);
NiImplementRTTI(RandomGate, ShineGate);

auto RoleList = "JobManager\0QuestNPC\0StoreManager\0ClientMenu\0Guard\0Merchant\0NPCMenu\0ShineGate\0IDGate\0ModeGate\0RandomGate";
ShineNPC::ShineNPC(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator& end)
{
	int DataID = 0;
	++start;
	for (auto& iter = start; iter != end; iter++)
	{
		if (*iter == "" || iter->empty())
			continue;
		if (iter->contains(" "))
		{
			std::string& str = *iter;
			str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
		}
		switch (DataID) 
		{
		case 0:
			this->_MobInx = *iter;
			break;
		case 1:
			this->_MapInx = *iter;
			break;
		case 2:
			this->_Pos.x = std::stod(*iter);
			break;
		case 3:
			this->_Pos.y = std::stod(*iter);
			break;
		case 4:
			this->_Rotation = std::stod(*iter);
			break;
		case 5: 
			if (*iter == "0")
				this->_NPCMenu = false;
			else if (*iter == "1")
				this->_NPCMenu = true;
			else
				LogError(*iter + " is a wrong argument for NPCMENU");
			break;
		case 6:
			if (*iter == "Merchant") 
			{
				_Role = NiNew Merchant;
			}else if (*iter == "JobManager")
			{
				_Role = NiNew JobManager;
			}else if (*iter == "QuestNpc")
			{
				_Role = NiNew QuestNPC;
			}
			else if (*iter == "StoreManager")
			{
				_Role = NiNew StoreManager;
			}
			else if (*iter == "ClientMenu")
			{
				_Role = NiNew ClientMenu;
			}
			else if (*iter == "Guard")
			{
				_Role = NiNew Guard;
			}
			else if (*iter == "NPCMenu")
			{
				_Role = NiNew NPCMenu;
			}
			else if (*iter == "Gate")
			{
				_Role = NiNew ShineGate;
			}
			else if (*iter == "IDGate")
			{
				_Role = NiNew IDGate;
			}
			else if (*iter == "ModeIDGate")
			{
				_Role = NiNew ModeIDGate;
			}
			else if (*iter == "RandomGate")
			{
				_Role = NiNew RandomGate;
			}
			break;
		case 7:
			if (!_Role)
				LogError("Failed to Load NPC.txt");
			else
				_Role->SetArgument(*iter);
			break;
		default:
			continue;
		}

		DataID++;
	}
}

ShineNPC::ShineNPC(MapInfo* Info, NiPoint3 Pos)
{
	this->_MapInx = Info->MapName;
	this->UpdatePos(Pos);
	this->UpdateRotation(0.0f);
	_Role = NiNew Guard;
	unsigned short id = 0;
	MobInfo* info = nullptr;
	while (info == nullptr)
	{
		info = MobLoader::GetMobInfo(id);
		id++;
	}

	_MobInx = info->InxName;
}
void ShineNPC::AppendGate(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator& end)
{
	if (NiIsKindOf(ShineGate, _Role))
	{
		ShineGatePtr gate = NiSmartPointerCast(ShineGate, _Role);
		if(!gate->AppendGate(start,end))
			LogError("Gate: " + _MobInx + " has wrong order in NPC.txt");
	}
		
}

void ShineNPC::DrawRole()
{
	int idx;
	if (NiIsKindOf(JobManager, _Role))
		idx = 0;
	if (NiIsKindOf(QuestNPC, _Role))
		idx = 1;
	if (NiIsKindOf(StoreManager, _Role))
		idx = 2;
	if (NiIsKindOf(ClientMenu, _Role))
		idx = 3;
	if (NiIsKindOf(Guard, _Role))
		idx = 4;
	if (NiIsKindOf(Merchant, _Role))
		idx = 5;
	if (NiIsKindOf(NPCMenu, _Role))
		idx = 6;
	if (NiIsKindOf(ShineGate, _Role))
		idx = 7;
	if (NiIsKindOf(IDGate, _Role))
		idx = 8;
	if (NiIsKindOf(ModeIDGate, _Role))
		idx = 9;
	if (NiIsKindOf(RandomGate, _Role))
		idx = 10;
	if (ImGui::Combo("Role", &idx, RoleList)) 
	{
		switch (idx) 
		{
		case 0:
			_Role = NiNew JobManager;
			break;
		case 1:
			_Role = NiNew QuestNPC;
			break;
		case 2:
			_Role = NiNew StoreManager;
			break;
		case 3:
			_Role = NiNew ClientMenu;
			break;
		case 4:
			_Role = NiNew Guard;
			break;
		case 5:
			_Role = NiNew Merchant;
			break;
		case 6:
			_Role = NiNew NPCMenu;
			break;
		case 7:
			if(NiIsKindOf(ShineGate, _Role))
			{
				_Role = NiNew ShineGate(NiSmartPointerCast(ShineGate,_Role));
			}
			else
				_Role = NiNew ShineGate;
			break;
		case 8:
			if (NiIsKindOf(ShineGate, _Role))
			{
				_Role = NiNew IDGate(NiSmartPointerCast(ShineGate, _Role));
			}
			else
				_Role = NiNew IDGate;
			break;
		case 9:
			if (NiIsKindOf(ShineGate, _Role))
			{
				_Role = NiNew ModeIDGate(NiSmartPointerCast(ShineGate, _Role));
			}
			else
				_Role = NiNew ModeIDGate;
			break;;
		case 10:
			if (NiIsKindOf(ShineGate, _Role))
			{
				_Role = NiNew RandomGate(NiSmartPointerCast(ShineGate, _Role));
			}
			else
				_Role = NiNew RandomGate;
			break;
		}
	}
	_Role->Draw();
}

void NPCRole::Draw() 
{
	ImGui::BeginDisabled();
	ImGui::Text("RoleArg unused by Zone");
	ImGui::EndDisabled();
}

bool Merchant::SetArgument(std::string Argument)
{
	if (Argument == "SoulStone")
		_SellType = SoulStone;
	if (Argument == "Weapon")
		_SellType = Weapon;
	if (Argument == "Skill")
		_SellType = Skill;
	if (Argument == "Item")
		_SellType =Item;
	if (Argument == "WeaponTitle")
		_SellType = WeaponTitle;
	if (Argument == "ClassChange")
		_SellType = ClassChange;

	return _SellType;
}

bool NPCMenu::SetArgument(std::string Argument)
{
	if (Argument == "Guild")
		_SellType = Guild;
	if (Argument == "ExchangeCoin")
		_SellType = ExchangeCoin;
	if (Argument == "RandomOption")
		_SellType =RandomOption;
	if (Argument == "Ranking")
		_SellType = Ranking;
	if (Argument == "Auction")
		_SellType = Auction;

	return _SellType;
}

void NPCMenu::Draw()
{
	if(NPCData::isCN)
		ImGui::Combo("Argument", (int*)&_SellType, "Guild\0Exchange Coin\0Auction");
	else
		ImGui::Combo("Argument", (int*)&_SellType, "Guild\0Exchange Coin\0Random Option\0Ranking");
}
void Merchant::Draw()
{
	ImGui::Combo("Argument", (int*)&_SellType, "SoulStone\0Weapon\0Skill\0Item\0WeaponTitle\0ClassChange");
	switch (_SellType)
	{
	case SoulStone:
	case ClassChange:
		break;
	default:
		//Render ItemList;
		break;
	}
}

void ShineGate::Draw() 
{
	DrawSizeCheck();
	if (ImGui::Button("Add new LinkData"))
	{
		_LinkData.push_back( NiNew LinkData);
	}
	int ID = 0;
	if (!_LinkData.size())
		return;
	for (auto data = _LinkData.begin();data != _LinkData.end(); data++ ) 
	{
		LinkDataPtr LinkData = *data;
		if(ImGui::TreeNode(std::to_string(ID).c_str()))
		{
			char argument[33];
			strcpy_s(argument, _Argument.c_str());
			if (ImGui::InputText("Argument", argument, sizeof(argument)))
			{
				_Argument = argument;
			}
			char buffer[33];
			strcpy_s(buffer, LinkData->MapClient.c_str());
			if (ImGui::InputText("Map", buffer, sizeof(buffer)))
			{
				LinkData->MapClient = buffer;
				LinkData->MapServer = buffer;
			}
			ImGui::DragFloat2("Coord", &LinkData->x, 0.05);
			ImGui::DragFloat("Rotation", &LinkData->rot, 0.01f, 0.f, 360.f);
			ImGui::Checkbox("Party", &LinkData->Party);
			if (NPCData::isCN) 
			{
				ImGui::DragInt("Level From", &LinkData->from);
				ImGui::DragInt("Level To", &LinkData->to);
			}

			if (ImGui::Button("Delete"))
			{
				data = _LinkData.erase(data);
				data--;
			}
			ImGui::TreePop();
		}
		ID++;
	}

}

void ShineGate::DrawSizeCheck() 
{
	return;
	if (_LinkData.size() > 1)
	{
		ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "Too many LinkData!");
	}
}

ShineGate::ShineGate(ShineGatePtr kCopy)
{
	this->_LinkData = kCopy->_LinkData;
	this->_Argument = kCopy->_Argument;
}

std::string ShineGate::GetArgumentString()
{
	std::string msg = _Argument;
	for (auto data = _LinkData.begin(); data != _LinkData.end(); data++)
	{
		LinkDataPtr LinkData = *data;
		const char* IsParty = (LinkData->Party) ? "1" : "0";
		msg += "\n\t#recordin\tLinkTable\t" + _Argument + "\t" + LinkData->MapClient + "\t" +
			LinkData->MapServer + "\t" + std::to_string((int)LinkData->x) + "\t" + std::to_string((int)LinkData->y) +
			"\t";
		if (NPCData::isCN)
			msg += std::to_string((int)LinkData->from) + "\t" + std::to_string((int)LinkData->to) + "\t";

		msg += std::to_string((int)LinkData->rot) + "\t" + IsParty;

	}
	return msg;
};

bool ShineGate::AppendGate(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator& end)
{
	if (*start != "LinkTable")
		return false;
	int id = 0;
	LinkDataPtr data = NiNew LinkData;
	
	for (auto iter = start; iter != end; iter++)
	{
		
		if (*iter == "" || *iter == "LinkTable" || iter->empty() || iter->contains(" "))
			continue;
		if (!NPCData::isCN)
		{
			switch (id)
			{
			case 0:
				if (_Argument != *iter)
					return false;
				break;
			case 1:
				data->MapServer = *iter;
				break;
			case 2:
				data->MapClient = *iter;
				break;
			case 3:
				data->x = std::stof(*iter);
				break;
			case 4:
				data->y = std::stof(*iter);
				break;
			case 5:
				data->rot = std::stof(*iter);
				break;
			case 6:
				data->Party = std::stoi(*iter);
				break;

			}
		}
		else 
		{
			switch (id)
			{
			case 0:
				if (_Argument != *iter)
					return false;
				break;
			case 1:
				data->MapServer = *iter;
				break;
			case 2:
				data->MapClient = *iter;
				break;
			case 3:
				data->x = std::stof(*iter);
				break;
			case 4:
				data->y = std::stof(*iter);
				break;
			case 5:
				data->rot = std::stof(*iter);
				break;
			case 6:
				data->from = std::stoi(*iter);
				break;
			case 7:
				data->to = std::stoi(*iter);
				break;
			case 8:
				data->Party = std::stoi(*iter);
				break;

			}
		}
		id++;
	}
	_LinkData.push_back(data);
	return true;
}

std::string ShineNPC::toString()
{
	std::stringstream os;
	NiPoint3 pos = GetPos();
	int rot = GetRotation();
	const char* IsParty = (*HasMenu()) ? "1" : "0";
	os << "#recordin\tShineNPC\t" << GetMobIndex() << "\t" << GetMapInx() << "\t"
		<< std::to_string((int)pos.x) << "\t" << std::to_string((int)pos.y) << "\t" << std::to_string(rot)
		<< "\t" << IsParty << "\t" << GetRole()->toString();
	return os.str();
}
std::string NPCRole::toString()
{
	std::stringstream os;
	os << GetRoleString() << "\t" << GetArgumentString();
	return os.str();
}

GateSpawn::GateSpawn(ShineGate::LinkDataPtr data)
{
	MobInfo* info = nullptr;
	int id = 0;
	while (info == nullptr) {
		info = MobLoader::GetMobInfo(id);
		id++;
	}
	_Data = data;
	_Pos = NiPoint3(_Data->x, _Data->y, 0.f);
	_Rotation = _Data->rot;
	_MobInx = info->InxName;
	_MapInx = data->MapClient;
}
