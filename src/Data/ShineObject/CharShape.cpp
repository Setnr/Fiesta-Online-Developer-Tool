#include "CharShape.h"
#include <SHN/SHNManager.h>
#include <vector>
#include <Logger.h>
#include "PgUtil.h"
#include "Data/NiCustom/NiPickable.h"
#include <NiMultiTargetTransformController.h>
#include <NiBoneLODController.h>
#include <set>

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
            }
            if (NiIsKindOf(NiNode, child))
                if (!TourScene<T>(NiSmartPointerCast(NiNode, child), _data_))
                    return TERMINATE_TOUR;

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
    FindBoneNode(NiNode** ppNode, int nArry, const char* szName) 
    {
        m_nCurLevel = -1;
        m_szName = szName;
        m_pkBone = 0;
        for (int i = 0; i < nArry; i++)
        {
            if (ppNode[i])
                m_setLinkNode.insert(ppNode[i]);
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
    std::set<NiAVObject*> m_setLinkNode;
};

CharShape::CharShape(NiAVObject* node)
{
    m_pkRootNode = NiSmartPointerCast(NiNode,node);
    
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

    EyeShapeNode = NiSmartPointerCast(NiNode, node->GetObjectByName("EyeShape"));
}

void CharShape::SetEquipment(NPCViewInfo* npcviewinfo)
{
    _Gender = npcviewinfo->Gender;
    _Class = npcviewinfo->Class;

    std::string s = npcviewinfo->Equ_RightHand;
    if (s!= "-") 
    {
        auto itemview = SHN::SHNManager::GetViewInfoByInx(s);
        switch (itemview->EquipType) 
        {
        case EquipTypeEnum::ICON:
            break;
        case EquipTypeEnum::SET:
            CreateSetEquipment(itemview);
            break;
        case EquipTypeEnum::LINK:
            CreateLinkEquipment(LinkIndex::LINK_RIGHTHAND ,itemview);
            break;
        }
    } 
    s = npcviewinfo->Equ_LeftHand;
    if (s != "-")
    {
        auto itemview = SHN::SHNManager::GetViewInfoByInx(s);
        switch (itemview->EquipType)
        {
        case EquipTypeEnum::ICON:
            break;
        case EquipTypeEnum::SET:
            CreateSetEquipment(itemview);
            break;
        case EquipTypeEnum::LINK:
            CreateLinkEquipment(LinkIndex::LINK_LEFTHAND, itemview);
            break;
        }
    }
    s = npcviewinfo->Equ_Body;
    if (s != "-")
    {
        auto itemview = SHN::SHNManager::GetViewInfoByInx(s);
        switch (itemview->EquipType)
        {
        case EquipTypeEnum::ICON:
            break;
        case EquipTypeEnum::SET:
            CreateSetEquipment(itemview);
            break;
        case EquipTypeEnum::LINK:
            CreateLinkEquipment(LinkIndex::LINK_CHEST, itemview);
            break;
        }
    }
    if (NiIsKindOf(NiPickable, m_pkRootNode))
    {
        NiPickablePtr pickable = NiSmartPointerCast(NiPickable, m_pkRootNode);
        NiStream s;
        s.InsertObject(pickable->ToNiNode());
        s.Save("E:\\VM\\Gamebryo\\Nebula v2 Rewind\\ResChar\\Joker-f\\test.nif");
    }
    else {
        NiStream s;
        s.InsertObject(m_pkRootNode);
        s.Save("E:\\VM\\Gamebryo\\Nebula v2 Rewind\\ResChar\\Joker-f\\test.nif");
    }
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
    switch (_ItemInfo->Equip) 
    {
    case ITEMEQUIP_BODY:
        SetBody(SetNode, &set);
        break;
    case ITEMEQUIP_LEG:
        SetLeg(SetNode);
        break;
    case ITEMEQUIP_SHOES:
        SetShoes(SetNode);
        break;
    }

    m_pkRootNode->UpdateProperties();
    m_pkRootNode->UpdateEffects();
    m_pkRootNode->Update(0.0f);

    
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
void CharShape::SetBody(NiNodePtr SetNode, CharSet* pkSet)
{
    std::vector<const char*> LodLevels = { "LOD0","LOD1","LOD2" };
    int v2 = 0;
    NiGeometry** v3; // ebx
    NiGeometryPtr v4;
    NiGeometryPtr v5;
    v3 = m_apkLodGeom;

    while (1) 
    {
        v4 = *v3;
        v5 = 0;
        if (v2 < pkSet->m_nSetGeoCnt)
            v5 = pkSet->m_pkBodyGeom[v2];
        if (!ChangeGeom(v3, v5, v2))
            break;
        v2++;
        v3++;
        if (v2 >= 3)
        {
            m_apkGeom[2] = m_apkLodGeom[0];
            return;
        }
    }


    NiNodePtr BIPNode = NiSmartPointerCast(NiNode, m_pkRootNode->GetObjectByName("Bip01"));
    NiBoneLODController* controller = (NiBoneLODController*)NiGetController(NiBoneLODController, BIPNode);

}
void CharShape::SetLeg(NiNodePtr SetNode)
{
    std::vector<const char*> LodLevels = { "LOD0","LOD1","LOD2" };
    for (int i = 0; i < LodLevels.size(); i++)
    {
        auto newnode = NiSmartPointerCast(NiNode, SetNode->GetObjectByName(LodLevels[i])->GetObjectByName("Leg"));
        auto parent = m_apkLodGeom[i + 3]->GetParent();
        parent->DetachChild(m_apkLodGeom[i + 3]);
        parent->AttachChild(newnode);
        PgUtil::CatchGeomentry(newnode, &m_apkLodGeom[i + 3]);
    }
}
void CharShape::SetShoes(NiNodePtr SetNode)
{
    std::vector<const char*> LodLevels = { "LOD0","LOD1","LOD2" };
    for (int i = 0; i < LodLevels.size(); i++)
    {
        auto newnode = NiSmartPointerCast(NiNode, SetNode->GetObjectByName(LodLevels[i])->GetObjectByName("Shoes"));
        auto parent = m_apkLodGeom[i + 6]->GetParent();
        parent->DetachChild(m_apkLodGeom[i + 6]);
        parent->AttachChild(newnode);
        PgUtil::CatchGeomentry(newnode, &m_apkLodGeom[i + 6]);
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
char CharShape::ChangeGeom(NiGeometry** pkCurGeom, NiGeometry* pkChgGeom, int nLodLevel)
{
    NiGeometry* pkChgNodeBL;
    if (!m_pkRootNode || !*pkCurGeom || !pkChgGeom)
        return false;
    NiObjectNET::SetDefaultCopyType(NiObjectNET::COPY_EXACT);
    pkChgNodeBL = pkChgGeom;
    NiAVObject* v5 = (NiAVObject*) pkChgGeom->GetParent()->Clone();
    PgUtil::CatchGeomentry(v5, &pkChgGeom);
    pkChgGeom->SetTranslate((*pkCurGeom)->GetTranslate());
    pkChgGeom->SetRotate((*pkCurGeom)->GetRotate());
    if (nLodLevel >= 0)
        ChangeBoneLODController(nLodLevel, *pkCurGeom, pkChgNodeBL, pkChgGeom);
    NiSkinPartition* partition = pkChgGeom->GetSkinInstance()->GetSkinPartition();
    NiAVObject** objarray = NiAlloc(NiAVObject*, (*pkCurGeom)->GetSkinInstance()->GetSkinData()->GetBoneCount());
    NiSkinInstance* skin = NiNew NiSkinInstance(pkChgGeom->GetSkinInstance()->GetSkinData(), (*pkCurGeom)->GetSkinInstance()->GetRootParent(), objarray);
    pkChgGeom->SetSkinInstance(skin);
    skin->SetSkinPartition(partition);;

    NiAVObject* const* bones = pkChgGeom->GetSkinInstance()->GetBones();

    int BoneSetCt = 0;
    int OldBoneCt = (*pkCurGeom)->GetSkinInstance()->GetSkinData()->GetBoneCount();
    int NewBoneCt = pkChgGeom->GetSkinInstance()->GetSkinData()->GetBoneCount();
    for (int i = 0; i < NewBoneCt; i++)
    {
        NiNode* bone = NULL;
        auto name = bones[i]->GetName();
        FindBoneNode tour(LinkNodes, 27, name);
        tour.TourScene<NiNode>((NiNode*)(*pkCurGeom)->GetSkinInstance()->GetRootParent(), 0);
        if (tour.m_pkBone)
        {
            skin->SetBone(i, tour.m_pkBone);
            BoneSetCt++;
        }
    }
    auto curname = (*pkCurGeom)->GetName();
    (*pkCurGeom)->SetName("Dead Mesh");
    auto parent = (*pkCurGeom)->GetParent()->GetParent();
    parent->DetachChild((*pkCurGeom)->GetParent());
    parent->AttachChild(v5);
    *pkCurGeom = pkChgGeom;
    pkChgGeom->UpdateProperties();
    pkChgGeom->UpdateEffects();
    pkChgGeom->Update(0.0f);
    return true;
}

void CharShape::ChangeBoneLODController(int _nLodLevel, NiGeometry* _pkDestGeom, NiGeometry* _pkSrcGeom, NiGeometry* _pkNewGeom)
{
    auto v5 = _pkDestGeom->GetSkinInstance();
    auto v6 = _pkSrcGeom->GetSkinInstance();
    auto v7 = (NiNode*)v5->GetRootParent();
    auto v8 = (NiNode*)v6->GetRootParent();
    auto v9 = PgUtil::FindBoneRootNodes(v7);
    auto v10 = PgUtil::FindBoneRootNodes(v8);
    auto v12 = NiGetController(NiBoneLODController, v9);
    auto v13 = NiGetController(NiBoneLODController, v10);
    NiBoneLODController::NiTriBasedGeomSet geomset;
    NiBoneLODController::NiSkinInstanceSet skinset;
    v13->GetSkinData(geomset, skinset);
    int i;
    for (i = 0; i < geomset.GetSize(); i++)
    {
        if (geomset.GetAt(i) == _pkSrcGeom)
            break;
    }
    int oldBoneCt = _pkSrcGeom->GetSkinInstance()->GetSkinData()->GetBoneCount();
    NiAVObject** objarray = NiAlloc(NiAVObject*, oldBoneCt);
    NiSkinInstance* skin = NiNew NiSkinInstance(skinset.GetAt(i)->GetSkinData(), v7, objarray);
    auto skinpartition = _pkSrcGeom->GetSkinInstance()->GetSkinPartition();
    skin->SetSkinPartition(skinpartition);
    NiAVObject*const* bones = _pkSrcGeom->GetSkinInstance()->GetBones();
    int newBoneCt = skin->GetSkinData()->GetBoneCount();
    int BoneSetCt = 0;
    for (i = 0; i < skin->GetSkinData()->GetBoneCount(); i++) 
    {
        auto name = bones[i]->GetName();
        FindBoneNode tour(LinkNodes, 27, name);
        tour.TourScene<NiNode>((NiNode*)v7, 0);
        if (tour.m_pkBone)
        {
            skin->SetBone(i, tour.m_pkBone);
            BoneSetCt++;
        }

       
    }
    v12->ReplaceSkin((NiTriBasedGeom*)_pkDestGeom, (NiTriBasedGeom*)_pkNewGeom);
}
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
    NiNode* LodNode =(NiNode*) m_spRootNode->GetObjectByName(_pLodNodeName);
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
