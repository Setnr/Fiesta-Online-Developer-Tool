#pragma once
#include <PgUtil.h>
#include "md5/md5.h"
#include <map>
#include <Logger.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "SHNStruct.h"
namespace SHN 
{
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
	class CDataReader
	{
		struct SHNRow
		{
			unsigned short ColLen;
		};
	public:
		CDataReader(std::string FileName)
		{
			_FileName = FileName;
			std::string Path = PgUtil::PathFromClientFolder(".\\ressystem\\" + FileName + ".shn");
			if (!std::filesystem::exists(Path))
			{
				LogError("Faild to Load " + FileName);
				return;
			}
			std::ifstream InputStream;
			InputStream.open(Path, std::ios::in | std::ios::binary | std::ios::ate);
			if (!InputStream)
			{
				LogError("Faild to open " + FileName + " " + std::strerror(errno));
				return;
			}
			InputStream.seekg(0, std::ios::beg);
			auto size = std::filesystem::file_size(Path);
			std::vector<char> FullData(size);
			InputStream.read(FullData.data(), size);
			auto CryptedSize = size - 0x24;
			memcpy_s(&Header, sizeof(HEAD), FullData.data(), sizeof(HEAD));
			if (Header.nDataMode == CDataReader::HEAD::DATA_MODE_ENCRYPTION) {
				Encription((FullData.data() + 0x24), CryptedSize);
			}
			memcpy_s(&Header, sizeof(HEAD), FullData.data(), sizeof(HEAD));
			auto SHNDataSize = size - 52 - (Header.nNumOfField * sizeof(HEAD::FIELD));
			SHNData = FullData;
			SHNStart = SHNData.data() + (size - SHNDataSize);
			MD5Hash = md5(SHNData.data(), Header.nFileSize);

			SelectedRow.resize(Header.nNumOfRecord);
		}
		unsigned int GetRows() { return this->Header.nNumOfRecord; }
		void* GetRow(unsigned int row)
		{
			SHNRow* srow = (SHNRow*)SHNStart;

			for (int j = 0; j < row; j++)
			{
				srow = (SHNRow*)(((size_t)srow) + srow->ColLen);
			}
			return (void*)srow;
		}
		unsigned short GetRowLen(unsigned int row)
		{
			SHNRow* srow = (SHNRow*)SHNStart;

			for (int j = 0; j < row; j++)
			{
				srow = (SHNRow*)((size_t)srow + srow->ColLen);
			}
			return srow->ColLen;
		}
		std::string GetFileName() { return _FileName; }
		bool DrawHeader(int AdditionalRows)
		{
			static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersInnerH;
			if (!ImGui::BeginTable(_FileName.c_str(), this->Header.nNumOfField + AdditionalRows, flags))
				return false;
			for (int i = 0; i < this->Header.nNumOfField; i++)
			{
				ImGui::TableSetupColumn(this->Header.Field[i].Name);
			}
			ImGui::TableHeadersRow();
			return true;
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
	};

	class SHNManager
	{
	public:
		static void Load()
		{
			std::map<SHNType, const char*> shnlist{ {MapInfoType, "MapInfo"} };
			for(auto shn :  shnlist)
			{
				auto reader = std::make_shared<CDataReader>(shn.second);
				SHNList.insert({ shn.first,reader });
			}
		}
		static bool GetSHN(SHN::SHNType type, std::shared_ptr<CDataReader>& reader)
		{
			auto pair = SHNList.find(type);
			if (pair != SHNList.end())
			{
				reader = pair->second;
				return true;
			}
			return false;
			
		}
	private:
		static std::map<SHNType, std::shared_ptr<CDataReader>> SHNList;
	};

	
}