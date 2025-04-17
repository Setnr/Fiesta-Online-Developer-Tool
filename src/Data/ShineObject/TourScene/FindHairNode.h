#pragma once
#include "NS_TourScene.h"

class FindHairNode : public NS_TourScene
{
public:

    TS_RESULT ProcessData(NiAVObject* obj, void* _data_) override
    {
        if (!obj)
            return CONTINUE_TOUR;
        if (obj->GetName().Contains("#Hair") && NiIsKindOf(NiNode, obj))
        {
            HairNode.push_back((NiNode*)obj);
            return NEXT_TOUR;
        }
        else
            return CONTINUE_TOUR;

    }
    std::vector<NiNode*> HairNode;
};
