#pragma once
#include <SHN/SHNStruct.h>

#include <NiMain.h>
#include "PROTOStructs.h"

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
	enum AttachType : __int32
	{
		Origin = 0x0,
		Left = 0x1,
		Right = 0x2,
		both = 0x3,
	};
public: 
	CharShape(NiAVObject* node);
	void SetEquipment(NPCViewInfo* npcviewinfo);
	PROTO_EQUIPMENT GetEquipment() { return Equipment; }
	ItemInfo* GetWeapon();
private:
	NiNodePtr LinkNodes[27] = {nullptr};
	PROTO_EQUIPMENT Equipment;
	NiGeometryPtr m_apkLodGeom[9] = { nullptr };
	NiGeometryPtr m_apkGeom[5] = { nullptr };
	int m_nSetLodGeoCnt = 0;
	bool _Gender;
	BaseCharClass _Class;

	NiNodePtr m_pkRootNode;
	NiNodePtr EyeShapeNode;
	NiNodePtr m_pkBaseFaceShapeNode;
	NiNodePtr m_spFaceNode;
	unsigned char m_byFaceShapeType;
	unsigned __int8 m_byFace;
	unsigned __int8 m_byHairType;
	unsigned __int8 m_byHairColor;

	void CreateSetEquipment(ItemViewInfo* info);
	void CreateLinkEquipment(LinkIndex slot, ItemViewInfo* info);
	void SetBody(CharSet* pkSet);
	void SetLeg(CharSet* pkSet);
	void SetShoes(CharSet* pkSet);
	bool SetLodGeometry(int nLodLeve, const char* LodName);
	char ChangeGeom(NiGeometryPtr* pkCurGeom, NiGeometryPtr pkChgGeom, int nLodLevel);
	void ChangeBoneLODController(int _nLodLevel, NiGeometryPtr& _pkDestGeom, NiGeometryPtr& _pkSrcGeom, NiGeometryPtr& _pkNewGeom);
	void SetSubItem(ItemEquipEnum slot, DummyTypeEnum DummyType, std::string szNifName, std::string szSubItemTextureName, std::string szSubItemNode);
	void SetSubItem(LinkIndex enumIndex, std::string szNifName, std::string szSubItemTextureName, std::string szSubItemNode, AttachType eAttachType);
	void UpdateFaceShape();
	void UpdateHair();
	void HideAllHair();
	void SetHairParts(int PartsIndex, int id, bool show, std::string ModelName, std::string FrontText);
};