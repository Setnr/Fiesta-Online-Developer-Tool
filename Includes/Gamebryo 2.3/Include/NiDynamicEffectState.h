// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2007 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef NIDYNAMICEFFECTSTATE_H
#define NIDYNAMICEFFECTSTATE_H

#include "NiMainLibType.h"
#include "NiBool.h"
#include "NiRefObject.h"
#include "NiSmartPointer.h"
#include "NiRTLib.h"

class NiDynamicEffect;
class NiLight;

NiSmartPointer(NiDynamicEffectState);
NiSmartPointer(NiTextureEffect);

typedef void* NiDynEffectStateIter;

class NIMAIN_ENTRY NiDynamicEffectState : public NiRefObject
{
public:
    NiDynamicEffectState();

    NiDynamicEffectState* Copy() const;
    
    virtual ~NiDynamicEffectState();

    void AddEffect(NiDynamicEffect* pEffect);
    void RemoveEffect(NiDynamicEffect* pEffect);

    NiDynEffectStateIter GetLightHeadPos() const;
    NiLight* GetNextLight(NiDynEffectStateIter& iter) const;

    NiDynEffectStateIter GetProjLightHeadPos() const;
    NiTextureEffect* GetNextProjLight(
        NiDynEffectStateIter& iter) const;

    NiDynEffectStateIter GetProjShadowHeadPos() const;
    NiTextureEffect* GetNextProjShadow(
        NiDynEffectStateIter& iter) const;

    NiTextureEffect* GetEnvironmentMap() const;
    NiTextureEffect* GetFogMap() const;

    NiBool Equal(const NiDynamicEffectState* pState) const;

    bool IsMultiTexture() const;

protected:
    NiDynamicEffectState(const NiDynamicEffectState&) : NiRefObject() { /* */ }
    void SetMultiTexture();

    class NIMAIN_ENTRY ListItem : public NiMemObject
    {
    public:
        ListItem* m_pNext;
        NiDynamicEffect* m_pItem;
    };

    static bool ListsEqual(const ListItem* pL1, const ListItem* pL2);
    static void ListDelete(ListItem* &pL);
    static void ListCopy(const ListItem* pSrc, ListItem* &pDest);
    static void ListInsertSorted(ListItem* &pL, NiDynamicEffect* pItem);
    static void ListRemove(ListItem* &pL, NiDynamicEffect* pItem);

    bool m_bMultiTexture;
    ListItem* m_pLightList;
    ListItem* m_pProjLightList;
    ListItem* m_pProjShadowList;
    NiTextureEffect* m_pEnvMap;
    NiTextureEffect* m_pFogMap;

};

#include "NiDynamicEffectState.inl"

#endif // NIDYNAMICEFFECTSTATE_H
