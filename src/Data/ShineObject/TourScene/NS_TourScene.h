#pragma once
#include <NiMain.h>
#include <Logger.h>

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
                if (NiIsKindOf(NiNode, child))
                    if (!TourScene<T>(NiSmartPointerCast(NiNode, child), _data_))
                        return TERMINATE_TOUR;
            }
        }

        m_nCurLevel--;
        return NEXT_TOUR;
    }
    TS_RESULT(__cdecl* m_pCallBack)(NiAVObject*, void*);
    int m_nCurLevel;
};