#pragma once
#include "NS_TourScene.h"

class SetHairTexture : public NS_TourScene
{
public:
    SetHairTexture(NiSourceTexturePtr HairTexture, NiSourceTexturePtr DetailTexture) :
        _HairTexture(HairTexture),
        _DetailTexture(DetailTexture)
    {}
    TS_RESULT ProcessData(NiAVObject* obj, void* _data_) override
    {
        if (!obj)
            return CONTINUE_TOUR;
        if (obj->GetName().Equals("Scene Root"))
            return CONTINUE_TOUR;
        NiTexturingProperty* prop = NiSmartPointerCast(NiTexturingProperty, obj->GetProperty(NiProperty::TEXTURING));
        if (prop)
        {
            auto map = NiNew NiTexturingProperty::Map(_DetailTexture, 0);
            prop->SetDetailMap(map);
            prop->SetBaseTexture(_HairTexture);
        }
        return NEXT_TOUR;
    }
    NiSourceTexturePtr _HairTexture;
    NiSourceTexturePtr _DetailTexture;
};
