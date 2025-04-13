#include "MobLoader.h"
#include <Logger.h>
#include <SHN/SHNManager.h>

std::map<unsigned short, NiActorManagerPtr> MobLoader::IDNodeMap;
std::map<std::string, NiActorManagerPtr> MobLoader::PlayerKFMMap;
std::mutex MobLoader::ActorLoadLock;

void MobLoader::Clear() {
    IDNodeMap.clear();
    PlayerKFMMap.clear();
}

NiActorManager* MobLoader::GetNodeByName(std::string MobInx)
{
    MobInfo* _MobInfo = MobLoader::GetMobInfo(MobInx);
    if (!_MobInfo)
        return nullptr;
    return GetNodeByMobInfo(_MobInfo);
}

NiActorManager* MobLoader::GetNodeByMobInfo(MobInfo* info)
{
    MobViewInfo* _MobViewInfo = MobLoader::GetMobViewInfo(info->ID);

    if (!_MobViewInfo)
        return nullptr;

    NiActorManager* NewNode;
    {
        std::lock_guard<std::mutex> lock(MobLoader::ActorLoadLock);
        auto it = IDNodeMap.find(info->ID);
        if (it == IDNodeMap.end())
        {
            NiActorManager* Node = LoadMob(info->ID);
            if (!Node) {
                LogError("Failed to Load Node for " + info->InxName);
                return nullptr;
            }
            IDNodeMap.insert({ info->ID, Node });
            NewNode = Node->Clone();
        }
        else
        {
            NewNode = it->second->Clone();
        }
    }
    
    if (NewNode)
        NewNode->GetNIFRoot()->SetScale(info->Size / 1000.f);

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

NiActorManager* MobLoader::GetPlayerKFM(std::string Combine)
{
    static std::mutex CloneMutex;
    std::lock_guard<std::mutex> lock(CloneMutex);
    auto it = PlayerKFMMap.find(Combine);
    if (it != PlayerKFMMap.end()) {
        return it->second->Clone();
    }
    auto FilePath = PgUtil::PathFromClientFolder("reschar\\" + Combine + "\\" + Combine + ".kfm");

    NiActorManagerPtr Actor = NiActorManager::Create(FilePath.c_str());
    PlayerKFMMap.insert({ Combine, Actor });
    return Actor->Clone();
}

NiActorManager* MobLoader::LoadMob(unsigned short ID)
{
    auto viewinfo = GetMobViewInfo(ID);
    if (viewinfo->NpcViewIndex != 0)
    {
        return PgUtil::CreatePlayerNPC(viewinfo->NpcViewIndex);
    }
    auto FilePath = PgUtil::PathFromClientFolder("reschar\\" + std::string(viewinfo->FileName) + "\\" + viewinfo->FileName + ".kfm");

    return NiActorManager::Create(FilePath.c_str());
}