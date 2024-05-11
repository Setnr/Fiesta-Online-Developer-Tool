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

#ifndef NIACTIONMAP_H
#define NIACTIONMAP_H

#include "NiAction.h"
#include "NiInputErr.h"

#include <NiTArray.h>
#include <NiTPool.h>

class NIINPUT_ENTRY NiActionMap : public NiRefObject
{
public:
    NiActionMap();
    NiActionMap(const char* pcName, void* pvPlatformSpecificData = 0);

    ~NiActionMap();

    const char* GetName() const;
    const void* GetPlatformSpecificData() const;
    const unsigned int GetActionCount() const;
    const NiAction* GetAction(unsigned int uiIndex) const;

    NiAction* AddAction(const char* pcName, unsigned int uiAppData, 
        unsigned int uiSemantic, unsigned int uiFlags, 
        unsigned int uiDeviceID = 0, int iRangeLow = 0, int iRangeHigh = 0,
        void* pvContext = 0, unsigned int uiKeyModifiers = 0, 
        unsigned int uiMouseModifiers = 0, unsigned int uiPadModifiers = 0);

    // *** begin Emergent internal use only ***
    void SetName(const char* pcName);

    void SetPlatformSpecificData(void* pvData);

    // Used to fill in the actions
    NiAction* GetAction(unsigned int uiIndex);

    static void CreateActionPool();
    static void ReleaseActionPool();
    static NiAction* GetActionFromPool();
    static void ReturnActionToPool(NiAction* pkAction);
    // *** end Emergent internal use only ***

protected:

    char* m_pcName;
    void* m_pvPlatformSpecificData;

    NiInputErr m_eLastError;

    NiTPrimitiveArray<NiAction*> m_akActions;
    static NiTPool<NiAction, NiTNewInterface<NiAction> >* ms_pakActionPool;
};

NiSmartPointer(NiActionMap);

#include "NiActionMap.inl"

#endif  //#ifndef NIACTIONMAP_H
