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

#ifndef NIOFLIGHT_H
#define NIOFLIGHT_H

#define NUM_DEF_LIGHTS 2

#include <NiColor.h>
#include <NiTStringMap.h>

#include <mgapistd.h>

#include "NiOpenFlightLibType.h"

class NiAVObject;
class NiLight;
class NiNode;

class NIOPENFLIGHT_ENTRY NiOFLight
{
public:
    NiOFLight (mgrec* pkLtsrec);

    static void GetLightPalette (mgrec* pkDB);

    static NiNode* CreateLight(mgrec* pkRec);

    static void ProcessLights (mgrec* pkDB, NiNode* pkRotNode,
        NiNode* pkModel);

    static void SetAddDefaultLights(bool bAddDefaultLights) 
    { ms_bAddDefaultLights = bAddDefaultLights; }

    static bool GetAddDefaultLights() 
    { return ms_bAddDefaultLights; }

protected:
    enum { LR_INFINITE, LR_LOCAL, LR_SPOT };

    NiLight* GetLight ();
    unsigned int GetType () { return m_eType; }

    static NiOFLight* GetLightByName(const char* pcName)
    {
        NiOFLight* pkLight = NULL;
        ms_kLightCache.GetAt(pcName, pkLight); 
        return pkLight;
    }

    int m_iPaletteId;
    unsigned int m_eType;
    NiColor m_kAmbi, m_kDiff, m_kSpec;
    float m_fAttenConst, m_fAttenLinear, m_fAttenQuad;
    float m_fSpotExponent, m_fSpotAngle, m_fSpotYaw, m_fSpotPitch;
    bool m_bModelingLight;

    static NiTStringMap<NiOFLight*> ms_kLightCache;

    static bool ms_bAddDefaultLights;

    friend class NiOFLightInit;
};

#endif
