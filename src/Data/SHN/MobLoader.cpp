#include "MobLoader.h"
#include <Logger.h>
#include <SHN/SHNManager.h>
#include <NiCustom/ShineMobNode.h>

std::map<unsigned short, NiActorManagerPtr> MobLoader::IDNodeMap;

void MobLoader::Clear() {
    IDNodeMap.clear();
}

NiActorManager* MobLoader::GetNodeByName(std::string MobInx)
{
    MobInfo* _MobInfo = MobLoader::GetMobInfo(MobInx);
    if (!_MobInfo)
        return nullptr;
    MobViewInfo* _MobViewInfo = MobLoader::GetMobViewInfo(_MobInfo->ID);

    if(!_MobViewInfo)
        return nullptr;

    auto it = IDNodeMap.find(_MobInfo->ID);
    if (it == IDNodeMap.end())
    {
        NiActorManager* Node = LoadMob(_MobInfo->ID);
        IDNodeMap.insert({ _MobInfo->ID, Node });
        return GetNodeByName(MobInx);
    }
    NiActorManager* NewNode = it->second->Clone();
    if (NewNode)
        NewNode->GetNIFRoot()->SetScale(_MobInfo->Size / 1000.f);
    return NewNode;
}

MobInfo* MobLoader::GetMobInfo(std::string MobInx) 
{
    std::shared_ptr<SHN::CDataReader> reader;
    SHN::SHNManager::GetSHN(SHN::MobInfo, reader);
    for (int i = 0; i < reader->GetRows(); i++)
    {
        MobInfo* info = (MobInfo*)reader->GetRow(i);
        if (info->InxName == MobInx)
            return info;
    }
    return nullptr;
}

MobInfo* MobLoader::GetMobInfo(unsigned short ID)
{
    std::shared_ptr<SHN::CDataReader> reader;
    SHN::SHNManager::GetSHN(SHN::MobInfo, reader);
    for (int i = 0; i < reader->GetRows(); i++)
    {
        MobInfo* info = (MobInfo*)reader->GetRow(i);
        if (info->ID == ID)
            return info;
    }
    return nullptr;
}

MobViewInfo* MobLoader::GetMobViewInfo(unsigned short ID)
{
    std::shared_ptr<SHN::CDataReader> reader;
    SHN::SHNManager::GetSHN(SHN::MobViewInfo, reader);
    for (int i = 0; i < reader->GetRows(); i++)
    {
        MobViewInfo* info = (MobViewInfo*)reader->GetRow(i);
        if (info->ID == ID)
            return info;
    }
    LogError("Tryed to Load Mob with ID : " + std::to_string(ID) + " which isnt part of the MobViewInfo.shn");
    return nullptr;
}

NiActorManager* MobLoader::LoadMob(unsigned short ID)
{
    auto viewinfo = GetMobViewInfo(ID);
    auto FilePath = PgUtil::PathFromClientFolder("reschar\\" + std::string(viewinfo->FileName) + "\\" + viewinfo->FileName + ".kfm");

    return NiActorManager::Create(FilePath.c_str());
}