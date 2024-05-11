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

#ifndef NISTRIPIFYINTERFACE_H
#define NISTRIPIFYINTERFACE_H

#include "NiStripifyLibType.h"
#include <NiMain.h>
#include <NiSystem.h>

#include "NiStripify.h"

class NISTRIPIFY_ENTRY NiStripifyInterface : public NiMemObject
{
public:
    enum Platform
    {
        PLATFORM_WIN32,  // PC
        PLATFORM_NGC,    // Nintendo GameCube
        PLATFORM_PS2,    // Sony PlayStation2
        PLATFORM_XBOX,   // Microsoft Xbox

        PLATFORM_COUNT
    };

    NiStripifyInterface(Platform ePlatform);
    void Stripify_Object(NiNode* pkNode);

    // Following public members are obsolete and will be removed from a
    // future release.

    enum Algorithm
    {
        ALGORITHM_NI,    //  Gamebryo
        ALGORITHM_NG,    //  NextGeneration
        ALGORITHM_COUNT
    };

    //  Pre-defined caches...
    enum CacheSize
    {
        CACHE_GEFORCE1_2,    //  GeForce1 or GeForce2
        CACHE_GEFORCE3,      //  GeForce3
        CACHE_COUNT
    };

    //  Stitching
    enum Stitch
    {
        STITCH_DISABLED,    //  Don't stitch strips
        STITCH_ENABLED,     //  Stitch strips
        STITCH_COUNT
    };

    void SetAlgorithm(Algorithm eAlgorithm);
    void SetInterfaceCacheSize(unsigned int uiCacheSize);
    void SetStitch(Stitch eStitch);

    Platform GetPlatform() const;
    Algorithm GetAlgorithm() const;
    unsigned int GetCacheSize() const;
    Stitch GetStitch() const;

protected:
    void Stripify_NextGeneration(NiNode* pkNode);

    class Platform_Settings
    {
    public:
        Platform m_ePlatform;
        Algorithm m_eAlgorithm;
        unsigned int m_uiCacheSize;
        Stitch m_eStitch;
        unsigned int m_uiStripMinSize;
        bool m_bListsOnly;
    };

    static Platform_Settings ms_akDefaultSettings[PLATFORM_COUNT];

    Platform_Settings m_kSettings;
};

#include "NiStripifyInterface.inl"

#endif  //#ifndef NISTRIPIFYINTERFACE_H
