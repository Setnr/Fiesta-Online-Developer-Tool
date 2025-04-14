#include "ShineMob.h"
#include <ImGui/imgui.h>
#include <Logger.h>

NiImplementRTTI(ShineMob, ShineObject);

ShineMob::ShineMob()
{
}

void ShineMob::DrawObjectMenu()
{
	ShineObject::DrawObjectMenu();
    char buffer[33];
    strcpy_s(buffer, GetMobIndex().c_str());
    if (ImGui::InputText("MobInx", buffer, sizeof(buffer)))
    {
        NiActorManagerPtr NewNode = MobLoader::GetNodeByName(buffer);
        if (NewNode)
        {
            SetMobIndex(buffer);
            UpdateActor(NewNode);
        }
    }
}

void ShineMob::LoadActor()
{
    _MobInfo = MobLoader::GetMobInfo(_MobInx);
    _MobViewInfo = MobLoader::GetMobViewInfo(_MobInfo->ID);

    UpdateActor( MobLoader::GetNodeByName(_MobInx));
}

void ShineMob::UpdateActor(NiActorManagerPtr NewActor)
{
    ShineObject::UpdateActor(NewActor);
    unsigned int ActionCode = 0;
    if (_MobViewInfo->NpcViewIndex)
    {
        std::shared_ptr<SHN::CDataReader> reader;
        if (!SHN::SHNManager::GetSHN(SHN::SHNType::NPCViewInfo, reader))
        {
            LogError("NPCViewInfo wasnt able to be recieved");
            return;
        }
        NPCViewInfo* info = nullptr;
        for (int i = 0; i < reader->GetRows(); i++)
        {
            info = (NPCViewInfo*)reader->GetRow(i);
            if (info->TypeIndex == _MobViewInfo->NpcViewIndex)
                break;
        }
        _NPCViewInfo = info;
        _ActionInfoSet = ActionInfoSetManager::GetActionInfoSet(PgUtil::GetBaseClassName(_NPCViewInfo->Class) + "-" + PgUtil::GetGenderString(_NPCViewInfo->Gender));
        _Shape = NiNew CharShape(_Actor->GetNIFRoot());
        _Shape->SetEquipment(_NPCViewInfo);

        auto _Info = _Shape->GetWeapon();
        ActionCode = AnimationEventCodes::Code1::STAND +
            AnimationEventCodes::Code2::NORMALSTAND +
            AnimationEventCodes::GetWeaponAnimationType(_Info);
    }
    else
        _ActionInfoSet = ActionInfoSetManager::GetActionInfoSet(_MobViewInfo->FileName);
    if (!ActionCode)
    {
        if (_MobInfo)
            if (_MobInfo->IsNPC)
                ActionCode = AnimationEventCodes::Code1::STAND +
                    AnimationEventCodes::Code2::NORMALSTAND;
            else
                ActionCode = AnimationEventCodes::Code1::STAND +
                    AnimationEventCodes::Code2::COMBATSTAND;
        else
            ActionCode = AnimationEventCodes::Code1::STAND +
                AnimationEventCodes::Code2::COMBATSTAND;
    }
    _Actor->ActivateSequence(ActionCode);
    
}
