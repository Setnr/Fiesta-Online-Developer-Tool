#include "CharShape.h"
#include <SHN/SHNManager.h>
#include <vector>
#include <Logger.h>
#include "PgUtil.h"
#include "Data/NiCustom/NiPickable.h"
#include <NiMultiTargetTransformController.h>
#include <NiBoneLODController.h>
#include <set>

NiSmartPointer(NiCustomSkinData);
class NiCustomSkinData : public NiSkinData 
{
public:
    void UpdateRootTransform(NiTransform transform) 
    {
        m_kRootParentToSkin = transform;
    }
};
NiSmartPointer(CustomNiBoneLODController);
class CustomNiBoneLODController : public NiBoneLODController 
{
public:
    void UpdateSkin(NiTriBasedGeom* pkOldSkin, NiTriBasedGeom* pkNewSkin) 
    {
        unsigned int ui;

        for (ui = 0; ui < m_kSkinArray.GetSize(); ui++)
        {
            SkinInfoSet* pkSet = m_kSkinArray.GetAt(ui);
            if (pkSet)
            {
                for (unsigned int uj = 0; uj < pkSet->GetSize(); uj++)
                {
                    SkinInfo* pkInfo = pkSet->GetAt(uj);
                    if (pkInfo->m_pkSkinGeom == pkOldSkin)
                    {
                        pkInfo->m_pkSkinGeom = pkNewSkin;
                        pkInfo->m_spSkinInst = pkNewSkin->GetSkinInstance();
                    }
                }
            }
        }

        for (ui = 0; ui < m_kSkinSet.GetSize(); ui++)
        {
            if (m_kSkinSet.GetAt(ui) == pkOldSkin)
            {
                m_kSkinSet.ReplaceAt(ui, pkNewSkin);
            }
        }
    }
};

class NS_TourScene 
{
public:
    enum TS_RESULT : __int32
    {
        CONTINUE_TOUR = 0xFFFFFFFF,
        TERMINATE_TOUR = 0x0,
        NEXT_TOUR = 0x1,
    };
    virtual TS_RESULT ProcessData(NiAVObject* obj, void* _data_) 
    {
        if (m_pCallBack)
            return m_pCallBack(obj, _data_);
        LogError("TourScene ProcessData doesnt have a CallBack");
        return TERMINATE_TOUR;
    }
    template <class T>
    TS_RESULT TourScene(NiNode* pNiNode, void* _data_)
    {
        if (!pNiNode)
            return TERMINATE_TOUR;
        int nChildCnt = pNiNode->GetArrayCount();
        m_nCurLevel++;
        for (int i = 0; i < nChildCnt; i++) 
        {
            auto child = pNiNode->GetAt(i);
            if (!child)
                continue;
            if (NiIsKindOf(T, child)) 
            {
                TS_RESULT data = ProcessData(child, _data_);
                if (data == TERMINATE_TOUR)
                    return TERMINATE_TOUR;
                if (NiIsKindOf(NiNode, child))
                    if (!TourScene<T>(NiSmartPointerCast(NiNode, child), _data_))
                        return TERMINATE_TOUR;
            }
        }

        m_nCurLevel--;
        return NEXT_TOUR;
    }
    TS_RESULT(__cdecl* m_pCallBack)(NiAVObject*, void*);
    int m_nCurLevel;
};

class FindBoneNode : public NS_TourScene
{
public:
    FindBoneNode(NiNodePtr* ppNode, int nArry, const char* szName) 
    {
        m_nCurLevel = -1;
        m_szName = szName;
        m_pkBone = 0;
        for (int i = 0; i < nArry; i++)
        {
            if (ppNode[i])
                m_setLinkNode.insert(NiSmartPointerCast(NiAVObject,ppNode[i]));
        }
    }
    TS_RESULT ProcessData(NiAVObject* obj, void* _data_) override
    {
        if (!obj->GetName())
            return NEXT_TOUR;
        auto it = m_setLinkNode.find(obj);
        if (it != m_setLinkNode.end())
            return CONTINUE_TOUR;
        if(strcmp(obj->GetName(), m_szName))
            return NEXT_TOUR;
        m_pkBone = obj;
        return TERMINATE_TOUR;
    }
    const char* m_szName;
    NiAVObject* m_pkBone;
    std::set<NiAVObjectPtr> m_setLinkNode;
};
class ResetTexture : public NS_TourScene
{
public:

    TS_RESULT ProcessData(NiAVObject* obj, void* _data_) override
    {
        if (!obj)
            return CONTINUE_TOUR;
        if(obj->GetName().Equals("Scene Root"))
            return CONTINUE_TOUR;
        NiTexturingProperty* prop = NiSmartPointerCast(NiTexturingProperty,obj->GetProperty(NiProperty::TEXTURING));
        if (prop)
            prop->SetBaseTexture((NiTexture*)_data_);
        return NEXT_TOUR;
    }
};
class FindFaceNode : public NS_TourScene
{
public:

    TS_RESULT ProcessData(NiAVObject* obj, void* _data_) override
    {
        if (!obj)
            return CONTINUE_TOUR;
        if (obj->GetName().Equals("#Face") && NiIsKindOf(NiNode, obj))
        {
            FaceNode = (NiNode*)obj;
            return TERMINATE_TOUR;
        }
        else
            return CONTINUE_TOUR;
        
    }
    NiNode* FaceNode;
};
CharSet::CharSet(NiNodePtr Node)
{
    m_spRootNode = Node;
}
bool CharSet::Init()
{
    m_pkBodyGeom[0] = 0;
    m_pkBodyGeom[1] = 0;
    m_pkBodyGeom[2] = 0;
    m_pkLegGeom[0] = 0;
    m_pkLegGeom[1] = 0;
    m_pkLegGeom[2] = 0;
    m_pkShoesGeom[0] = 0;
    m_pkShoesGeom[1] = 0;
    m_pkShoesGeom[2] = 0;
    return m_spRootNode && (SetLodGeometry(0, "LOD0")
        && SetLodGeometry(1, "LOD1")
        && SetLodGeometry(2, "LOD2")
        || SetNormalGeometry());
}
bool CharSet::SetLodGeometry(int _nLodLevel, const char* _pLodNodeName)
{
    NiNode* LodNode = (NiNode*)m_spRootNode->GetObjectByName(_pLodNodeName);
    if (!LodNode)
        return false;
    auto h = LodNode->GetObjectByName("Body");
    if (!PgUtil::CatchGeomentry(h, &m_pkBodyGeom[_nLodLevel]))
        return false;

    h = LodNode->GetObjectByName("Leg");
    if (!PgUtil::CatchGeomentry(h, &m_pkLegGeom[_nLodLevel]))
        return false;
    h = LodNode->GetObjectByName("Shoes");
    if (!PgUtil::CatchGeomentry(h, &m_pkShoesGeom[_nLodLevel]))
        return false;
    m_nSetGeoCnt++;
    return true;
}
bool CharSet::SetNormalGeometry()
{

    auto h = m_spRootNode->GetObjectByName("Body");
    if (!PgUtil::CatchGeomentry(h, m_pkBodyGeom))
        return false;

    h = m_spRootNode->GetObjectByName("Leg");
    if (!PgUtil::CatchGeomentry(h, m_pkLegGeom))
        return false;
    h = m_spRootNode->GetObjectByName("Shoes");
    if (!PgUtil::CatchGeomentry(h, m_pkShoesGeom))
        return false;
    m_nSetGeoCnt++;
    return true;
}
CharShape::CharShape(NiAVObject* node)
{
    m_pkRootNode = NiSmartPointerCast(NiNode,node);
    
    memset(&Equipment, 0, sizeof(Equipment));

    std::vector<const char*> ModelPartName = {
    "HatAcc",           // 0x00
    "HairLink",         // 0x01
    "FaceLink",         // 0x02
    "Eye",              // 0x03
    "Mouth",            // 0x04
    "Chest",            // 0x05
    "Chest_R",          // 0x06
    "Body",             // 0x07
    "Back",             // 0x08
    "TopLink",          // 0x09
    "LeftShoulder",     // 0x0A
    "RightShoulder",    // 0x0B
    "Shield",           // 0x0C
    "Weapon",           // 0x0D
    "LeftBracelet",     // 0x0E
    "RightBracelet",    // 0x0F
    "Belt",             // 0x10
    "Tail",             // 0x11
    "Leg",              // 0x12
    "Shoes",            // 0x13
    "LeftShoesAcc",     // 0x14
    "RightShoesAcc",    // 0x15
    "LOD0",             // 0x16
    "LOD1",             // 0x17
    "LOD2",             // 0x18
    "LeftUpperArm",     // 0x19
    "RightUpperArm",    // 0x1A
    "LeftThigh",        // 0x1B
    "RightThigh",       // 0x1C
    "LeftCalf",         // 0x1D
    "RightCalf",        // 0x1E
    "World",            // 0x1F
    "World_Cos"         // 0x20
    };
    int LinkCt = 0;
    for (int i = 0; i < ModelPartName.size(); i++) 
    { 
        if (i == 0x12 || i == 0x13 || i == 0x16 || i == 0x17 || i == 0x18 || i == 0x7)
            continue;
        auto obj = node->GetObjectByName(ModelPartName[i]);
        if (obj)
        {
            LinkNodes[LinkCt] = NiSmartPointerCast(NiNode, obj);
        }
        else
            LogInfo(ModelPartName[i] + " not found");
        LinkCt++;
    }

    for (int i = 0x16; i <= 0x18; i++)
    {
        auto obj = node->GetObjectByName(ModelPartName[i]);

        auto bodyobj = NiSmartPointerCast(NiNode,obj->GetObjectByName(ModelPartName[0x7]));
        auto legbj = NiSmartPointerCast(NiNode, obj->GetObjectByName(ModelPartName[0x12]));
        auto shoeobj = NiSmartPointerCast(NiNode, obj->GetObjectByName(ModelPartName[0x13]));

       

    }
    if(SetLodGeometry(0, ModelPartName[0x16]))
    {
        if (SetLodGeometry(1, ModelPartName[0x17]))
        {
            if (SetLodGeometry(2, ModelPartName[0x18]))
            {
                m_apkGeom[2] = m_apkLodGeom[0];
                m_apkGeom[3] = m_apkLodGeom[3];
                m_apkGeom[4] = m_apkLodGeom[6];
            }
        }
    }
    FindFaceNode FaceFinder;
    FaceFinder.TourScene<NiNode>(m_pkRootNode, 0);
    if (!FaceFinder.FaceNode)
        LogError("Failed to Find Face Node");
    m_pkBaseFaceShapeNode = FaceFinder.FaceNode;
    if(!m_pkBaseFaceShapeNode)
        LogError("Failed to Find Face Node");
    EyeShapeNode = NiSmartPointerCast(NiNode, node->GetObjectByName("EyeShape"));
}
void CharShape::SetEquipment(NPCViewInfo* npcviewinfo)
{
    _Gender = npcviewinfo->Gender;
    _Class = npcviewinfo->Class;

    NPCViewLoopDummy* LoopDummy = (NPCViewLoopDummy*)npcviewinfo;
    std::vector<LinkIndex> LinkIndex = {
        LINK_RIGHTHAND ,
        LINK_LEFTHAND,
        LINK_CHEST,
        LINK_NUM,
        LINK_NUM
    };
    std::vector<int> PROTO_EQUIPMENT_INDEX = {
        2,4,3,5,6
    };
    for(int i = 0; i < LinkIndex.size(); i++)
    {
        std::string s = &LoopDummy->Equ_Dummy[i * 32];
        if (s != "-")
        {
            auto itemview = SHN::SHNManager::GetViewInfoByInx(s);
            unsigned __int16* EquPtr = (unsigned __int16*) &Equipment;
            EquPtr[PROTO_EQUIPMENT_INDEX[i]] = itemview->ID;
            switch (itemview->EquipType)
            {
            case EquipTypeEnum::ICON:
                break;
            case EquipTypeEnum::SET:
                CreateSetEquipment(itemview);
                break;
            case EquipTypeEnum::LINK:
                CreateLinkEquipment(LinkIndex[i], itemview);
                break;
            }
        }
    }
    m_byFaceShapeType = 0;
    m_byFace = npcviewinfo->FaceShape;
    UpdateFaceShape();
    m_pkRootNode->UpdateProperties();
    m_pkRootNode->UpdateEffects();
    m_pkRootNode->Update(0.0f);

}
ItemInfo* CharShape::GetWeapon()
{
    if (Equipment.Equ_RightHand)
        return SHN::SHNManager::GetItemInfoByID(Equipment.Equ_RightHand);
    if (Equipment.Equ_LeftHand)
        return SHN::SHNManager::GetItemInfoByID(Equipment.Equ_LeftHand);
    return nullptr;
}
void CharShape::CreateSetEquipment(ItemViewInfo* info)
{
    std::string gender = "f";
    unsigned int setno = info->FSetNo;
    if (_Gender)
    {
        gender = "m";
        setno = info->MSetNo;
    }
    std::string classname = PgUtil::GetBaseClassName(_Class);
    char buffer[256];
    sprintf(buffer, "reschar\\%s-%s\\set%03d.nif", classname.c_str(), gender.c_str(), setno);
    auto SetNode = PgUtil::LoadNifFile<NiNode>(PgUtil::PathFromClientFolder(buffer).c_str());
    CharSet set(SetNode);
    set.Init();
    ItemInfo* _ItemInfo = SHN::SHNManager::GetItemInfoByID(info->ID);
    std::vector<ItemViewDummy*> dummylist = SHN::SHNManager::GetItemViewDummys(info->InxName);
    std::string TexturePath = PgUtil::PathFromClientFolder("reschar\\" + classname + "-" + gender + "\\" + info->TextureFile + ".dds");
    switch (_ItemInfo->Equip) 
    {
    case ITEMEQUIP_BODY:
    {
        SetBody(&set);
        NiTexturingPropertyPtr tex = NiSmartPointerCast(NiTexturingProperty, m_apkLodGeom[0]->GetProperty(NiProperty::TEXTURING));
        tex->SetBaseTexture(PgUtil::LoadTextureWithPixelData(TexturePath));
        break;
    }
    case ITEMEQUIP_LEG:
    {
        SetLeg(&set);
        NiTexturingPropertyPtr tex = NiSmartPointerCast(NiTexturingProperty, m_apkLodGeom[3]->GetProperty(NiProperty::TEXTURING));
        tex->SetBaseTexture(PgUtil::LoadTextureWithPixelData(TexturePath));
        break;
    }
    case ITEMEQUIP_SHOES:
    {
        SetShoes(&set);
        NiTexturingPropertyPtr tex = NiSmartPointerCast(NiTexturingProperty, m_apkLodGeom[6]->GetProperty(NiProperty::TEXTURING));
        tex->SetBaseTexture(PgUtil::LoadTextureWithPixelData(TexturePath));
        break;
    }
    }

    auto Equip = _ItemInfo->Equip;
    if (Equip == ITEMEQUIP_HAT) 
    {
        Equip = ITEMEQUIP_HATACC;
    }
    char NodeName[256];
    sprintf(buffer, "_SubItem%02d", Equip);
    for (auto Dummy : dummylist) 
    {
        if(Dummy->IsMale == _Gender)
            SetSubItem(Equip, Dummy->DummyType, Dummy->Nif,Dummy->ArmorTexture, buffer);
    }
   

    
}
void CharShape::CreateLinkEquipment(LinkIndex slot, ItemViewInfo* info)
{
    if (!LinkNodes[slot])
        return;
    NiNodePtr EquipNode = PgUtil::LoadNifFile<NiNode>(PgUtil::PathFromClientFolder("resitem//" + std::string(info->LinkFile) + ".nif").c_str());
    EquipNode->SetName("EquipedNode");

    auto DetachNode = LinkNodes[slot]->GetObjectByName("EquipedNode");
    if (DetachNode)
        LinkNodes[slot]->DetachChild(DetachNode);
    LinkNodes[slot]->AttachChild(EquipNode);
    LinkNodes[slot]->UpdateProperties();
    LinkNodes[slot]->UpdateEffects();
    LinkNodes[slot]->Update(0.0f);
}
void CharShape::SetBody(CharSet* pkSet)
{
    std::vector<const char*> LodLevels = { "LOD0","LOD1","LOD2" };
    int LodLevel = 0;
    NiGeometryPtr v5;
    while (1) 
    {
        v5 = 0;
        if (LodLevel < pkSet->m_nSetGeoCnt)
            v5 = pkSet->m_pkBodyGeom[LodLevel];
        if (!ChangeGeom(&m_apkLodGeom[LodLevel], v5, LodLevel))
            break;
        LodLevel++;
        if (LodLevel >= 3)
        {
            m_apkGeom[2] = m_apkLodGeom[0];
            return;
        }
    }
}
void CharShape::SetLeg(CharSet* pkSet)
{
    std::vector<const char*> LodLevels = { "LOD0","LOD1","LOD2" };
    int v2 = 0;
    NiGeometryPtr* v3; // ebx
    NiGeometryPtr v4;
    NiGeometryPtr v5;
    v3 = &m_apkLodGeom[3];

    while (1)
    {
        v4 = *v3;
        v5 = 0;
        if (v2 < pkSet->m_nSetGeoCnt)
            v5 = pkSet->m_pkLegGeom[v2];
        if (!ChangeGeom(&m_apkLodGeom[v2 + 3], v5, v2))
            break;
        v2++;
        v3++;
        if (v2 >= 3)
        {
            m_apkGeom[3] = m_apkLodGeom[3];
            return;
        }
    }
}
void CharShape::SetShoes(CharSet* pkSet)
{
    std::vector<const char*> LodLevels = { "LOD0","LOD1","LOD2" };
    int v2 = 0;
    NiGeometryPtr* v3; // ebx
    NiGeometryPtr v4;
    NiGeometryPtr v5;
    v3 = &m_apkLodGeom[6];

    while (1)
    {
        v4 = *v3;
        v5 = 0;
        if (v2 < pkSet->m_nSetGeoCnt)
            v5 = pkSet->m_pkShoesGeom[v2];
        if (!ChangeGeom(&m_apkLodGeom[v2 + 6], v5, v2))
            break;
        v2++;
        v3++;
        if (v2 >= 3)
        {
            m_apkGeom[4] = m_apkLodGeom[6];
            return;
        }
    }
}
bool CharShape::SetLodGeometry(int nLodLeve, const char* LodName)
{
    if (!m_pkRootNode)
        return false;
    if (nLodLeve >= 3)
        return false;
    auto node = NiSmartPointerCast(NiNode,m_pkRootNode->GetObjectByName(LodName));
    if (!node)
        return false;

    if (!PgUtil::CatchGeomentry(node->GetObjectByName("Body"), &m_apkLodGeom[nLodLeve]))
        return false;
   
    if (!PgUtil::CatchGeomentry(node->GetObjectByName("Leg"), &m_apkLodGeom[nLodLeve + 3]))
        return false;
    if (!PgUtil::CatchGeomentry(node->GetObjectByName("Shoes"), &m_apkLodGeom[nLodLeve + 6]))
        return false;
    m_nSetLodGeoCnt++;
    return true;
    
}
char CharShape::ChangeGeom(NiGeometryPtr* pkCurGeom, NiGeometryPtr pkChgGeom, int nLodLevel)
{
    NiGeometryPtr pkChgNodeBL;
    if (!m_pkRootNode || !*pkCurGeom || !pkChgGeom)
        return false;
    NiObjectNET::SetDefaultCopyType(NiObjectNET::COPY_EXACT);

    NiGeometryPtr UnchangedGeometry = pkChgGeom;
    NiNodePtr NewNode = (NiNode*)pkChgGeom->GetParent()->Clone();
    PgUtil::CatchGeomentry(NewNode, &pkChgGeom);
    NiGeometryPtr OldGeometry = *pkCurGeom;
    pkChgGeom->SetTranslate(OldGeometry->GetTranslate());
    pkChgGeom->SetRotate(OldGeometry->GetRotate());
    pkChgNodeBL = pkChgGeom;
    if (nLodLevel >= 0)
        ChangeBoneLODController(nLodLevel, OldGeometry, pkChgNodeBL, UnchangedGeometry);
    NiSkinInstancePtr NewSkinInstance = pkChgGeom->GetSkinInstance();
    NiSkinDataPtr NewSkinData = NewSkinInstance->GetSkinData();
    int NewBoneCount = NewSkinData->GetBoneCount();
    NiNodePtr OldRootParent = NiSmartPointerCast(NiNode,OldGeometry->GetSkinInstance()->GetRootParent());
    NiAVObject** objarray = NiAlloc(NiAVObject*, NewBoneCount);
    NiSkinInstance* skin = NiNew NiSkinInstance(NewSkinData, OldRootParent, objarray);
    skin->SetSkinPartition(pkChgGeom->GetSkinInstance()->GetSkinPartition());
    pkChgGeom->SetSkinInstance(skin);
    
    OldGeometry->SetSkinInstance(skin);

    NiAVObject* const* NewBoneList = NewSkinInstance->GetBones();
    for (int i = 0; i < NewBoneCount; i++) 
    {
        auto name = NewBoneList[i]->GetName();
        FindBoneNode tour(LinkNodes, 27, name);
        tour.TourScene<NiNode>(OldRootParent, 0);
        if (tour.m_pkBone)
        {
            skin->SetBone(i, tour.m_pkBone);
        }
    }
    NiNodePtr OldNode = NiSmartPointerCast(NiNode,OldGeometry->GetParent());
    NiNodePtr Parent = NiSmartPointerCast(NiNode, OldNode->GetParent());
    pkChgGeom->SetTranslate(OldGeometry->GetTranslate());
    pkChgGeom->SetRotate(OldGeometry->GetRotate());

    *pkCurGeom = pkChgGeom;

    Parent->DetachChild(OldNode);
    Parent->AttachChild(NewNode);
    NewNode->SetTranslate(OldNode->GetTranslate());
    NewNode->SetWorldTranslate(OldNode->GetTranslate());
    NewNode->SetWorldBound(OldNode->GetWorldBound());
    NewNode->SetScale(OldNode->GetScale());

    return true;
}
void CharShape::ChangeBoneLODController(int _nLodLevel, NiGeometryPtr& _pkDestGeom, NiGeometryPtr& _pkSrcGeom, NiGeometryPtr& _pkNewGeom)
{
    NiSkinInstancePtr OldSkinInstance = _pkDestGeom->GetSkinInstance();
    NiSkinInstancePtr NewSkinInstance = _pkSrcGeom->GetSkinInstance();
    NiNodePtr OldParentRoot = (NiNode*)OldSkinInstance->GetRootParent();
    NiNodePtr NewParentRoot = (NiNode*)NewSkinInstance->GetRootParent();
    NiNodePtr OldRootBone = PgUtil::FindBoneRootNodes(OldParentRoot);
    NiNodePtr NewRootBone = PgUtil::FindBoneRootNodes(NewParentRoot);
    CustomNiBoneLODControllerPtr OldLODController = NiGetController(CustomNiBoneLODController, OldRootBone);
    CustomNiBoneLODControllerPtr NewLODController = NiGetController(CustomNiBoneLODController, NewRootBone);

    NiBoneLODController::NiTriBasedGeomSet OldGeomSet;
    NiBoneLODController::NiSkinInstanceSet OldSkinSet;
    OldLODController->GetSkinData(OldGeomSet, OldSkinSet);


    NiBoneLODController::NiTriBasedGeomSet NewGeomSet;
    NiBoneLODController::NiSkinInstanceSet NewSkinSet;
    NewLODController->GetSkinData(NewGeomSet, NewSkinSet);

    NiTriBasedGeomPtr GeometryInOldLOD = NiSmartPointerCast(NiTriBasedGeom, _pkDestGeom);
    if (!OldLODController->FindGeom(GeometryInOldLOD))
    {   
        LogError("OLDLODController doesnt have SrcGeometry");
        return;
    }

    NiTriBasedGeomPtr GeometryInNewLOD = NiSmartPointerCast(NiTriBasedGeom, _pkNewGeom);
    if (!NewLODController->FindGeom(GeometryInNewLOD))
    {
        LogError("NewLODController doesnt have SrcGeometry");
        return;
    }
    int LODOffset;
    for (LODOffset = 0; LODOffset < NewGeomSet.GetSize(); LODOffset++)
    {
        NiTriBasedGeom*& geom = NewGeomSet.GetAt(LODOffset);
        if (GeometryInNewLOD == geom)
            break;
    }

    NiSkinDataPtr NewSkinData = NewSkinSet.GetAt(LODOffset)->GetSkinData();
    NiCustomSkinDataPtr Helper = NiSmartPointerCast(NiCustomSkinData, NewSkinData);
    Helper->UpdateRootTransform(OldSkinInstance->GetSkinData()->GetRootParentToSkin());

    int OldBoneCt = OldSkinInstance->GetSkinData()->GetBoneCount();
    int NewBoneCt = NewSkinInstance->GetSkinData()->GetBoneCount();

    NiAVObject** objarray = NiAlloc(NiAVObject*, NewBoneCt);
    NiSkinInstance* skin = NiNew NiSkinInstance(NewSkinData, OldParentRoot, objarray);
    skin->SetSkinPartition(NewSkinInstance->GetSkinPartition());
    _pkSrcGeom->SetSkinInstance(skin);

    NiAVObject* const* NewBoneList = NewSkinInstance->GetBones();
    for (int i = 0; i < NewBoneCt; i++)
    {
        auto name = NewBoneList[i]->GetName();
        FindBoneNode tour(LinkNodes, 27, name);
        tour.TourScene<NiNode>((NiNode*)OldParentRoot, 0);
        if (tour.m_pkBone)
        {
            skin->SetBone(i, tour.m_pkBone);
        }
    }
    OldLODController->UpdateSkin(NiSmartPointerCast(NiTriBasedGeom,_pkDestGeom), NiSmartPointerCast(NiTriBasedGeom, _pkSrcGeom));
}
void CharShape::SetSubItem(ItemEquipEnum slot, DummyTypeEnum DummyType, std::string szNifName, std::string szSubItemTextureName, std::string szSubItemNode)
{
    if (DummyType == DummyTypeEnum::None)
        return;

    switch (DummyType) 
    {
    case 1:
        SetSubItem(LINK_HATACC, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 2:
        SetSubItem(LINK_HAIR, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 3:
        SetSubItem(LINK_FACE, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 4:
        SetSubItem(LINK_EYE, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 5:
        SetSubItem(LINK_MOUTH, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 6:
        SetSubItem(LINK_CHEST, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 7:
        SetSubItem(LINK_BACK, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 9:
        SetSubItem(LINK_BELT, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 10:
        SetSubItem(LINK_TAIL, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 11:
        SetSubItem(LINK_LEFTSHOULDER, szNifName, szSubItemTextureName, szSubItemNode, Left);
        SetSubItem(LINK_RIGHTSHOULDER, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 12:
        SetSubItem(LINK_LEFTSHOULDER, szNifName, szSubItemTextureName, szSubItemNode, Left);
        break;
    case 13:
        SetSubItem(LINK_RIGHTSHOULDER, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 14:
        SetSubItem(LINK_LEFTUPPERARM, szNifName, szSubItemTextureName, szSubItemNode, Left);
        SetSubItem(LINK_RIGHTUPPERARM, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 15:
        SetSubItem(LINK_LEFTUPPERARM, szNifName, szSubItemTextureName, szSubItemNode, Left);
        break;
    case 16:
        SetSubItem(LINK_RIGHTUPPERARM, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 17:
        SetSubItem(LINK_RIGHTBRACELET, szNifName, szSubItemTextureName, szSubItemNode, Right);
        SetSubItem(LINK_LEFTBRACELET, szNifName, szSubItemTextureName, szSubItemNode, Left);
        break;
    case 18:
        SetSubItem(LINK_LEFTBRACELET, szNifName, szSubItemTextureName, szSubItemNode, Left);
        break;
    case 19:
        SetSubItem(LINK_RIGHTBRACELET, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 20:
        SetSubItem(LINK_LEFTTHIGH, szNifName, szSubItemTextureName, szSubItemNode, Left);
        SetSubItem(LINK_RIGHTTHIGH, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 21:
        SetSubItem(LINK_LEFTTHIGH, szNifName, szSubItemTextureName, szSubItemNode, Left);
        break;
    case 22:
        SetSubItem(LINK_RIGHTTHIGH, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 23:
        SetSubItem(LINK_LEFTCALF, szNifName, szSubItemTextureName, szSubItemNode, Left);
        SetSubItem(LINK_RIGHTCALF, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 24:
        SetSubItem(LINK_LEFTCALF, szNifName, szSubItemTextureName, szSubItemNode, Left);
        break;
    case 25:
        SetSubItem(LINK_RIGHTCALF, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 26:
        SetSubItem(LINK_LEFTSHOESACC, szNifName, szSubItemTextureName, szSubItemNode, Left);
        SetSubItem(LINK_RIGHTSHOESACC, szNifName, szSubItemTextureName, szSubItemNode, Right);
        break;
    case 27:
        SetSubItem(LINK_LEFTSHOESACC, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 28:
        SetSubItem(LINK_RIGHTSHOESACC, szNifName, szSubItemTextureName, szSubItemNode, Origin);
        break;
    case 29:
        SetSubItem(LINK_WORLD_COS, szNifName, szSubItemTextureName, szSubItemNode, Origin);
    }
}
void CharShape::SetSubItem(LinkIndex enumIndex, std::string szNifName, std::string szSubItemTextureName, std::string szSubItemNode, AttachType eAttachType)
{
    auto LinkNode = LinkNodes[enumIndex];
    if (!LinkNode)
        return;
    std::pair<NiNodePtr, NiSourceTexturePtr> pair = PgUtil::LoadSubItem(szNifName, _Class, _Gender, szSubItemTextureName);
    NiNodePtr ObjectNode = pair.first;
    NiSourceTexturePtr Texture = pair.second;
    NiNodePtr AttachedNode = NULL;
    switch (eAttachType) 
    {
    case Origin:
        LinkNode->AttachChild(ObjectNode);
        ObjectNode->SetName(szSubItemNode.c_str());
        AttachedNode = ObjectNode;
        break;
    case Right:
    {
        NiAVObjectPtr RightNode = ObjectNode->GetObjectByName("Right");
        RightNode->SetTranslate(NiPoint3::ZERO);
        RightNode->SetRotate(0.f, 0.f, 0.f, 0.f);
        RightNode->SetName(szSubItemNode.c_str());
        if (NiIsKindOf(NiNode, RightNode))
        {
            LinkNode->AttachChild(RightNode);
            AttachedNode = NiSmartPointerCast(NiNode,RightNode);
        }
        break;
    }
    case Left:
    {
        NiAVObjectPtr RightNode = ObjectNode->GetObjectByName("Left");
        RightNode->SetTranslate(NiPoint3::ZERO);
        RightNode->SetRotate(0.f, 0.f, 0.f, 0.f);
        RightNode->SetName(szSubItemNode.c_str());
        if (NiIsKindOf(NiNode, RightNode))
        {
            LinkNode->AttachChild(RightNode);
            AttachedNode = NiSmartPointerCast(NiNode, RightNode);
        }
        break;
    }
    }
    NiGeometryPtr geometry;
    if (PgUtil::CatchGeomentry(AttachedNode, &geometry)) {
        NiTexturingPropertyPtr ptr = NiSmartPointerCast(NiTexturingProperty,geometry->GetProperty(NiProperty::TEXTURING));
        //if (ptr)
         //   ptr->SetBaseTexture(Texture);
    }

    ResetTexture _ResTex;
    _ResTex.TourScene<NiNode>(AttachedNode, (void*)Texture);

    LinkNode->UpdateEffects();
    LinkNode->UpdateProperties();
    LinkNode->Update(0.0);
}

void CharShape::UpdateFaceShape()
{
    char buffer[512];
    sprintf(buffer, "reschar\\%s-%s\\%s-%s-face%03d.dds", PgUtil::GetBaseClassName(_Class).c_str(), PgUtil::GetGenderString(_Gender).c_str(), PgUtil::GetBaseClassName(_Class).c_str(), PgUtil::GetGenderString(_Gender).c_str(), m_byFaceShapeType);

    NiSourceTexturePtr tex = PgUtil::LoadTextureWithPixelData(PgUtil::PathFromClientFolder(buffer));
    if (m_byFaceShapeType)
    {
        //Unk
    }
    else 
    {
        m_spFaceNode = m_pkBaseFaceShapeNode;
        ResetTexture _ResTex;
        _ResTex.TourScene<NiAVObject>(m_spFaceNode, (void*)tex);
    }
}
