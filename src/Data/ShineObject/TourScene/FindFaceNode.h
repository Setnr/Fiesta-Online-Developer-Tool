#pragma once
#include "NS_TourScene.h"
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
