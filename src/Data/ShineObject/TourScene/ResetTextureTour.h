#pragma once
#include "NS_TourScene.h"

class ResetTexture : public NS_TourScene
{
public:

    TS_RESULT ProcessData(NiAVObject* obj, void* _data_) override
    {
        if (!obj)
            return CONTINUE_TOUR;
        if (obj->GetName().Equals("Scene Root"))
            return CONTINUE_TOUR;
        NiTexturingProperty* prop = NiSmartPointerCast(NiTexturingProperty, obj->GetProperty(NiProperty::TEXTURING));
        if (prop)
        {
            prop->SetBaseTexture((NiTexture*)_data_);
        }
        if (NiIsKindOf(NiGeometry, obj))
        {
            NiGeometry* geo = (NiGeometry*)obj;
            geo->SetConsistency(NiGeometryData::MUTABLE);
            geo->CalculateNormals();
            geo->GetModelData()->MarkAsChanged(NiGeometryData::NORMAL_MASK);

        }
        return NEXT_TOUR;
    }
};