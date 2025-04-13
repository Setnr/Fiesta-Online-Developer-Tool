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

    NPCViewLoopDummy* LoopDummy = (NPCViewLoopDummy*)npcviewinfo;
    std::vector<LinkIndex> LinkIndex = {
        LINK_RIGHTHAND ,
        LINK_LEFTHAND,
        LINK_CHEST,
        LINK_NUM,
        LINK_NUM
    };
    for(int i = 0; i < LinkIndex.size(); i++)
    {
        std::string s = &LoopDummy->Equ_Dummy[i * 32];
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
                CreateLinkEquipment(LinkIndex[i], itemview);
                break;
            }
        }
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
    {
        SetBody(SetNode, &set);
        NiTexturingPropertyPtr tex = NiSmartPointerCast(NiTexturingProperty, m_apkLodGeom[0]->GetProperty(NiProperty::TEXTURING));
        tex->SetBaseTexture(NiSourceTexture::Create(PgUtil::PathFromClientFolder("reschar\\" + classname + "-" + gender + "\\" + info->TextureFile + ".dds").c_str()));
        break;
    }
    case ITEMEQUIP_LEG:
    {
        SetLeg(SetNode, &set);
        NiTexturingPropertyPtr tex = NiSmartPointerCast(NiTexturingProperty, m_apkLodGeom[3]->GetProperty(NiProperty::TEXTURING));
        tex->SetBaseTexture(NiSourceTexture::Create(PgUtil::PathFromClientFolder("reschar\\" + classname + "-" + gender + "\\" + info->TextureFile + ".dds").c_str()));
        break;
    }
    case ITEMEQUIP_SHOES:
    {
        SetShoes(SetNode, &set);
        NiTexturingPropertyPtr tex = NiSmartPointerCast(NiTexturingProperty, m_apkLodGeom[6]->GetProperty(NiProperty::TEXTURING));
        tex->SetBaseTexture(NiSourceTexture::Create(PgUtil::PathFromClientFolder("reschar\\" + classname + "-" + gender + "\\" + info->TextureFile + ".dds").c_str()));
        break;
    }
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
    NiGeometryPtr* v3; // ebx
    NiGeometryPtr v4;
    NiGeometryPtr v5;
    v3 = m_apkLodGeom;

    while (1) 
    {
        v4 = *v3;
        v5 = 0;
        if (v2 < pkSet->m_nSetGeoCnt)
            v5 = pkSet->m_pkBodyGeom[v2];
        if (!ChangeGeom(&m_apkLodGeom[v2], v5, v2))
            break;
        v2++;
        v3++;
        if (v2 >= 3)
        {
            m_apkGeom[2] = m_apkLodGeom[0];
            return;
        }
    }
}
void CharShape::SetLeg(NiNodePtr SetNode, CharSet* pkSet)
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
void CharShape::SetShoes(NiNodePtr SetNode, CharSet* pkSet)
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
