#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "Logger.h"
#include "SHNStruct.h"
class CDataReader
{
public:
	bool Load(std::string& FileName);
	bool DrawHeader();
	void DrawRow(unsigned int row);
	unsigned int GetRows();
	template<class T>
	T* GetRow(unsigned int row)
	{
		SHNRow* srow = (SHNRow*)SHNStart;

		for (int j = 0; j < row; j++)
		{
			srow = (SHNRow*)((int)srow + srow->ColLen);
		}
		return (T*)srow;
	}
	unsigned short GetRowLen(unsigned int row) 
	{
		SHNRow* srow = (SHNRow*)SHNStart;

		for (int j = 0; j < row; j++)
		{
			srow = (SHNRow*)((int)srow + srow->ColLen);
		}
		return srow->ColLen;
	}
private:
	struct HEAD
	{
		enum DATA_MODE : __int32
		{
			DATA_MODE_NORMAL = 0x0,
			DATA_MODE_ENCRYPTION = 0x1,
		};
		struct FIELD
		{
			enum TYPE_LIST : __int32
			{
				TYPE_LIST_END = 0x0,
				TYPE_LIST_BYTE = 0x1,
				TYPE_LIST_WORD = 0x2,
				TYPE_LIST_DWORD = 0x3,
				TYPE_LIST_QWORD = 0x4,
				TYPE_LIST_FLOAT = 0x5,
				TYPE_LIST_FILENAME = 0x6,
				TYPE_LIST_FILEAUTO = 0x7,
				TYPE_LIST_REMARK = 0x8,
				TYPE_LIST_STR = 0x9,
				TYPE_LIST_STRAUTO = 0xA,
				TYPE_LIST_INX = 0xB,
				TYPE_LIST_INXBYTE = 0xC,
				TYPE_LIST_INXWORD = 0xD,
				TYPE_LIST_INXDWORD = 0xE,
				TYPE_LIST_INXQWORD = 0xF,
				TYPE_LIST_BYTE_BIT = 0x10,
				TYPE_LIST_WORD_BIT = 0x11,
				TYPE_LIST_DWORD_BIT = 0x12,
				TYPE_LIST_QWORD_BIT = 0x13,
				TYPE_LIST_BYTE_ARRAY = 0x14,
				TYPE_LIST_WORD_ARRAY = 0x15,
				TYPE_LIST_DWORD_ARRAY = 0x16,
				TYPE_LIST_QWORD_ARRAY = 0x17,
				TYPE_LIST_STR_ARRAY = 0x18,
				TYPE_LIST_STRAUTO_ARRAY = 0x19,
				TYPE_LIST_VARSTR = 0x1A,
				TYPE_LIST_INXSTR = 0x1B,
				TYPE_LIST_UNKNOWNED = 0x1C,
				TYPE_LIST_TWO_INX = 0x1D,
			};
			char Name[48];
			TYPE_LIST Type;
			unsigned int Size;
		};
		unsigned int nVersionKey;
		char sVersion[20];
		unsigned int nReserved;
		DATA_MODE nDataMode;
		unsigned int nFileSize;

		unsigned int nDataSize;
		unsigned int nNumOfRecord;
		unsigned int nFieldSize;
		unsigned int nNumOfField;
		FIELD Field[25];
	};
	HEAD Header;
	std::vector<char> SHNData;
	char* SHNStart;
	std::string MD5Hash;
	std::string _FileName;

	std::vector<char> SelectedRow;;
	char* Encription(char* buf, const int32_t len)
	{
		auto num = (uint8_t)len;

		for (auto i = len - 1; i >= 0; i--)
		{
			buf[i] = (uint8_t)(buf[i] ^ num);
			auto num3 = (uint8_t)(i);
			num3 = (uint8_t)(num3 & 0xF);
			num3 = (uint8_t)(num3 + 85);
			num3 = (uint8_t)(num3 ^ (uint8_t)((uint8_t)i * 11));
			num3 = (uint8_t)(num3 ^ num);
			num3 = (uint8_t)(num3 ^ 0xAA);
			num = num3;
		}
		return buf;
	}
	
	void DumpStructure();
	std::string FIELD_TYPELISTtoString(HEAD::FIELD::TYPE_LIST type);
	
};
class SHNManager
{
public:
	enum SHNType : char
	{
		Abstate,
		AbStateView,
		ActionEffectInfo,
		ActionViewInfo,
		ActiveSkill,
		ActiveSkillGroup,
		ActiveSkillInfoServer,
		ActiveSkillView,
		BadNameFilter,
		CharacterTitleData,
		ChargedEffect,
		ClassName,
		ColorInfo,
		DamageEffect,
		DamageSoundInfo,
		DiceDividind,
		DiceGame,
		DiceRate,
		EffectViewInfo,
		EmotionFilter,
		ErrorCodeTable,
		FaceInfo,
		Gather,
		GradeItemOption,
		GTIView,
		GuildGradeData,
		GuildTournamentRequire,
		GuildTournamentSkill,
		GuildTournamentSkillDesc,
		HairColorInfo,
		HairInfo,
		HolyPromiseReward,
		ItemDismantle,
		ItemInfo,
		ItemShopView,
		ItemViewInfo,
		KingdomQuestDesc,
		MapInfoType,
		MapViewInfo,
		MHEmotion,
		MiniHouse,
		MiniHouseDummy,
		MiniHouseEndure,
		MiniHouseFurniture,
		MiniHouseFurnitureObjEffect,
		MiniHouseObjAni,
		MobCoordinate,
		MobInfo,
		MobKillAnnounce,
		MobKillAnnounceText,
		MobSpecies,
		MobViewInfo,
		MobWeapon,
		NpcDialogData,
		NPCViewInfo,
		PassiveSkill,
		PassiveSkillView,
		Produce,
		ProduceView,
		QuestDialog,
		RaceNameInfo,
		Riding,
		SetItem,
		SetItemEffect,
		SetItemView,
		SlanderFilter,
		SubAbState,
		TextData,
		TextData2,
		TextData3,
		TownPortal,
		UpEffect,
		UpgradeInfo,
		WeaponAttrib,
		WeaponTitleData,
		WorldMapAvatarInfo,
		MAX
	};
	static void Init();
	static SHNManager Manager;
	void Add(SHNType type, std::shared_ptr<CDataReader> ptr) 
	{
		SHNList.insert({ type,ptr });
	}
	static std::shared_ptr<CDataReader> Get(SHNType type);
	std::shared_ptr<CDataReader> InternalGet(SHNType type);

	template<class T>
	void CheckSHN(std::shared_ptr<CDataReader> reader)
	{
		if (sizeof(T) != reader->GetRowLen(0) + 2) 
		{
#if SHNV2
			LogError("SHN Size Missmatch, this build is for V2");
#endif
#if SHN2k16
			LogError("SHN Size Missmatch, this build is for 2k16");
#endif
		}
	}
private:
	std::map<SHNType, std::shared_ptr<CDataReader>> SHNList;
};
