#pragma once
#include <SHN/SHNStruct.h>

#include <NiMain.h>

class CharSet 
{
public:
	CharSet(NiNodePtr Node);
	bool Init();
	bool SetLodGeometry(int _nLodLevel, const char* _pLodNodeName);
	bool SetNormalGeometry();
	int m_nSetGeoCnt;
	NiNodePtr m_spRootNode;
	NiGeometryPtr m_pkBodyGeom[3];
	NiGeometryPtr m_pkLegGeom[3];
	NiGeometryPtr m_pkShoesGeom[3];
};

NiSmartPointer(CharShape);
class CharShape : public NiRefObject
{
#pragma pack(push,1 )
	enum LinkIndex : __int32
	{
		LINK_HATACC = 0x0,
		LINK_HAIR = 0x1,
		LINK_FACE = 0x2,
		LINK_EYE = 0x3,
		LINK_MOUTH = 0x4,
		LINK_CHEST = 0x5,
		LINK_CHEST_R = 0x6,
		LINK_BACK = 0x7,
		LINK_TOPLINK = 0x8,
		LINK_LEFTSHOULDER = 0x9,
		LINK_RIGHTSHOULDER = 0xA,
		LINK_LEFTHAND = 0xB,
		LINK_RIGHTHAND = 0xC,
		LINK_LEFTBRACELET = 0xD,
		LINK_RIGHTBRACELET = 0xE,
		LINK_BELT = 0xF,
		LINK_TAIL = 0x10,
		LINK_LEFTSHOESACC = 0x11,
		LINK_RIGHTSHOESACC = 0x12,
		LINK_LEFTUPPERARM = 0x13,
		LINK_RIGHTUPPERARM = 0x14,
		LINK_LEFTTHIGH = 0x15,
		LINK_RIGHTTHIGH = 0x16,
		LINK_LEFTCALF = 0x17,
		LINK_RIGHTCALF = 0x18,
		LINK_WORLD = 0x19,
		LINK_WORLD_COS = 0x1A,
		LINK_NUM = 0x1B,
	};
	struct PROTO_EQUIPMENT
	{
		struct type_upgrade
		{
			unsigned __int8 lefthand : 4;
			unsigned __int8 righthand : 4;
			unsigned __int8 body : 4;
			unsigned __int8 leg : 4;
			unsigned __int8 shoes : 4;
		};
		unsigned __int16 Equ_Head;
		unsigned __int16 Equ_Mouth;
		unsigned __int16 Equ_RightHand;
		unsigned __int16 Equ_Body;
		unsigned __int16 Equ_LeftHand;
		unsigned __int16 Equ_Pant;
		unsigned __int16 Equ_Boot;
		unsigned __int16 Equ_AccBoot;
		unsigned __int16 Equ_AccPant;
		unsigned __int16 Equ_AccBody;
		unsigned __int16 Equ_AccHeadA;
		unsigned __int16 Equ_MiniMon_R;
		unsigned __int16 Equ_Eye;
		unsigned __int16 Equ_AccLeftHand;
		unsigned __int16 Equ_AccRightHand;
		unsigned __int16 Equ_AccBack;
		unsigned __int16 Equ_CosEff;
		unsigned __int16 Equ_AccHip;
		unsigned __int16 Equ_Minimon;
		unsigned __int16 Equ_AccShield;
		type_upgrade upgrade;
	};
#pragma pack(pop)
public: 
	CharShape(NiAVObject* node);
	void SetEquipment(NPCViewInfo* npcviewinfo);
private:
	NiNodePtr LinkNodes[27] = {nullptr};
	NiNodePtr EyeShapeNode;
	PROTO_EQUIPMENT Equip;
	NiGeometryPtr m_apkLodGeom[9] = { nullptr };
	NiGeometryPtr m_apkGeom[5] = { nullptr };
	int m_nSetLodGeoCnt = 0;
	bool _Gender;
	BaseCharClass _Class;

	NiNodePtr m_pkRootNode;

	void CreateSetEquipment(ItemViewInfo* info);
	void CreateLinkEquipment(LinkIndex slot, ItemViewInfo* info);
	void SetBody(NiNodePtr SetNode, CharSet* pkSet);
	void SetLeg(NiNodePtr SetNode, CharSet* pkSet);
	void SetShoes(NiNodePtr SetNode, CharSet* pkSet);
	bool SetLodGeometry(int nLodLeve, const char* LodName);
	char ChangeGeom(NiGeometryPtr* pkCurGeom, NiGeometryPtr pkChgGeom, int nLodLevel);
	void ChangeBoneLODController(int _nLodLevel, NiGeometryPtr& _pkDestGeom, NiGeometryPtr& _pkSrcGeom, NiGeometryPtr& _pkNewGeom);
};