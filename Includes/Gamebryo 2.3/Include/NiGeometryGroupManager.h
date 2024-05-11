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

#ifndef NIGEOMETRYGROUPMANAGER_H
#define NIGEOMETRYGROUPMANAGER_H

#include "NiMainLibType.h"
#include "NiRenderer.h"
#include "NiTSet.h"

class NiGeometryGroup;
class NiGeometryData;
class NiScreenTexture;
class NiSkinInstance;

class NIMAIN_ENTRY NiGeometryGroupManager : public NiMemObject
{
public:
    virtual ~NiGeometryGroupManager();

    enum RepackingBehavior
    {
        STATIC,
        UNSHARED,
        DYNAMIC
    };

    // Flags - none yet

    virtual NiGeometryGroup* CreateGroup(RepackingBehavior eBehavior);
    virtual bool DestroyGroup(NiGeometryGroup* pkGroup);
    
    static NiGeometryGroupManager* GetManager();

    virtual bool IsGroupEmpty(NiGeometryGroup* pkGroup) const;

    virtual bool AddObjectToGroup(
        NiGeometryGroup* pkGroup, 
        NiGeometryData* pkData, 
        NiSkinInstance* pkSkinInstance = NULL);
    virtual bool RemoveObjectFromGroup(NiGeometryData* pkData, 
        NiSkinInstance* pkSkinInstance);

    virtual bool AddObjectToGroup(
        NiGeometryGroup* pkGroup, 
        NiScreenTexture* pkScreenTexture);
    virtual bool RemoveObjectFromGroup(NiScreenTexture* pkScreenTexture);

    // *** begin Emergent internal use only ***

    // *** end Emergent internal use only ***

protected:
    NiGeometryGroupManager();

    static NiGeometryGroupManager* ms_pkManager;
};

#include "NiGeometryGroupManager.inl"

#endif // NIGEOMETRYGROUPMANAGER_H