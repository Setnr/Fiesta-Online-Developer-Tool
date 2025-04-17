#pragma once
#include "NS_TourScene.h"

class DisableLight : public NS_TourScene
{
public:

    TS_RESULT ProcessData(NiAVObject* obj, void* _data_) override
    {
        if (!obj)
            return CONTINUE_TOUR;
        if (NiIsKindOf(NiLight, obj))
        {
            NiLightPtr light = (NiLight*)obj;
            auto Parent = light->GetParent();
            Parent->DetachChild(light);
            DeletedLights.push_back(light);
        }
        return NEXT_TOUR;
    }
    std::vector<NiLightPtr> DeletedLights;
};
