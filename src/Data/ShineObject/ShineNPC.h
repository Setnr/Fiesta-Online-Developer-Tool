#pragma once 
#include <regex>
#include "ShineMob.h"
#include <SHN/SHNStruct.h>
#include <Logger.h>



NiSmartPointer(NPCRole);
class NPCRole : public NiRefObject
{
	NiDeclareRootRTTI(NPCRole);
public:
	virtual bool SetArgument(std::string Argument) { return true; };
	virtual void Draw();
	virtual std::string GetRoleString() = 0;
	virtual std::string GetArgumentString() = 0;
	std::string toString();
protected:
	NPCRole() {};
};

NiSmartPointer(JobManager);
class JobManager : public NPCRole {
	NiDeclareRTTI;
public:
	JobManager() : NPCRole() {};
	virtual std::string GetRoleString() { return "JobManager"; };
	virtual std::string GetArgumentString() { return "-"; };
};
NiSmartPointer(QuestNPC);
class QuestNPC : public NPCRole {
	NiDeclareRTTI;
public:
	QuestNPC() : NPCRole() {};
	virtual std::string GetRoleString() { return "QuestNpc"; };
	virtual std::string GetArgumentString() { return "-"; };

};
NiSmartPointer(StoreManager);
class StoreManager : public NPCRole {
	NiDeclareRTTI;
public:
	StoreManager() : NPCRole() {};
	virtual std::string GetRoleString() { return "StoreManager"; };
	virtual std::string GetArgumentString() { return "-"; };

};
NiSmartPointer(ClientMenu);
class ClientMenu : public NPCRole {
	NiDeclareRTTI;
public:
	ClientMenu() : NPCRole() {};
	virtual std::string GetRoleString() { return "ClientMenu"; };
	virtual std::string GetArgumentString() { return "-"; };

};
NiSmartPointer(Guard);
class Guard : public NPCRole {
	NiDeclareRTTI;
public:
	Guard() : NPCRole() {};
	virtual std::string GetRoleString() { return "Guard"; };
	virtual std::string GetArgumentString() { return "-"; };

};

NiSmartPointer(Merchant);
class Merchant : public NPCRole{
	NiDeclareRTTI;
public:
	Merchant() : NPCRole() {};
	virtual bool SetArgument(std::string Argument);
	virtual void Draw();
	virtual std::string GetRoleString() { return "Merchant"; };
	virtual std::string GetArgumentString() 
	{
		switch (_SellType) {
		case SoulStone: return "SoulStone";
		case Weapon: return "Weapon";
		case Skill: return "Skill";
		case Item: return "Item";
		case WeaponTitle: return "WeaponTitle";
		case ClassChange: return "ClassChange";
		default: return "-";
		}
	};
private: 
	enum SellType : int
	{
		SoulStone,
		Weapon,
		Skill,
		Item, 
		WeaponTitle,
		ClassChange
	}_SellType;
};

NiSmartPointer(NPCMenu);
class NPCMenu : public NPCRole {
	NiDeclareRTTI;
public:
	NPCMenu() : NPCRole() {};
	virtual bool SetArgument(std::string Argument);
	virtual void Draw();
	virtual std::string GetRoleString() { return "NPCMenu"; };
	virtual std::string GetArgumentString()
	{
		switch (_SellType) {
		case Guild: return "Guild";
		case ExchangeCoin: return "ExchangeCoin";
		case RandomOption: return "RandomOption";
		case Ranking: return "Ranking";
		default: return "-";
		}
	};
private:
	enum SellType : int
	{
		Guild,
		ExchangeCoin,
		RandomOption,
		Ranking,
		Auction
	}_SellType;
};



NiSmartPointer(ShineGate);
class ShineGate : public NPCRole{
	NiDeclareRTTI;
public:
	ShineGate(){};
	bool AppendGate(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator& end);
	virtual bool SetArgument(std::string Argument) { _Argument = Argument;  return true; };
	virtual void Draw();

	virtual void DrawSizeCheck();
	ShineGate(ShineGatePtr kCopy);
	virtual std::string GetRoleString() { return "Gate"; };
	virtual std::string GetArgumentString();
	
	
protected:
	struct LinkData
	{
		std::string MapServer;
		std::string MapClient;
		float x;
		float y;
		float rot;
		int from;
		int to;
		bool Party;
	};
	std::vector<LinkData> _LinkData;
	std::string _Argument;
};

NiSmartPointer(IDGate);
class IDGate : public ShineGate {
	NiDeclareRTTI;
	IDGate() : ShineGate() {};
	IDGate(ShineGatePtr kCopy) : ShineGate(kCopy) {};
	virtual std::string GetRoleString() { return "IDGate"; };
};

NiSmartPointer(ModeIDGate);
class ModeIDGate : public ShineGate {
	NiDeclareRTTI;
	ModeIDGate() : ShineGate() {};
	ModeIDGate(ShineGatePtr kCopy) : ShineGate(kCopy) {};
	virtual std::string GetRoleString() { return "ModeIDGate"; };
};

NiSmartPointer(RandomGate);
class RandomGate : public ShineGate {
	NiDeclareRTTI;
	RandomGate() : ShineGate() {};
	RandomGate(ShineGatePtr kCopy) : ShineGate(kCopy) {};
	virtual std::string GetRoleString() { return "RandomGate"; };
	
public:
	virtual void DrawSizeCheck() {};
};



NiSmartPointer(ShineNPC);
class ShineNPC : public ShineMob
{
	NiDeclareRTTI;
public:
	ShineNPC(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator& end);
	ShineNPC(MapInfo* Info, NiPoint3 Pos);
	void AppendGate(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator& end);
	bool* HasMenu() { return &_NPCMenu; }
	void DrawRole();
	NPCRolePtr GetRole() { return _Role; }
	std::string toString();
private:
	bool _NPCMenu;
	NPCRolePtr _Role; 
};
