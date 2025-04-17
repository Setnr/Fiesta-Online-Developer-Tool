#pragma once
#include "NS_TourScene.h"
#include <set>
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
                m_setLinkNode.insert(NiSmartPointerCast(NiAVObject, ppNode[i]));
        }
    }
    TS_RESULT ProcessData(NiAVObject* obj, void* _data_) override
    {
        if (!obj->GetName())
            return NEXT_TOUR;
        auto it = m_setLinkNode.find(obj);
        if (it != m_setLinkNode.end())
            return CONTINUE_TOUR;
        if (strcmp(obj->GetName(), m_szName))
            return NEXT_TOUR;
        m_pkBone = obj;
        return TERMINATE_TOUR;
    }
    const char* m_szName;
    NiAVObject* m_pkBone;
    std::set<NiAVObjectPtr> m_setLinkNode;
};