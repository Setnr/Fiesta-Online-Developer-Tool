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

#ifndef NID3DGEOMETRYGROUPMANAGER_H
#define NID3DGEOMETRYGROUPMANAGER_H

#include "NiD3DDefines.h"
#include <NiGeometryGroupManager.h>
#include <NiTSet.h>

class NiGeometryBufferManager;
class NiGeometryBufferData;

class NID3D_ENTRY NiD3DGeometryGroupManager : 
    public NiGeometryGroupManager
{
public:
    virtual ~NiD3DGeometryGroupManager();

    virtual NiGeometryGroup* CreateGroup(RepackingBehavior eBehavior);
    virtual bool DestroyGroup(NiGeometryGroup* pkGroup);
    
    virtual bool IsGroupEmpty(NiGeometryGroup* pkGroup) const;

    virtual bool AddObjectToGroup(
        NiGeometryGroup* pkGroup, 
        NiGeometryData* pkData, 
        NiSkinInstance* pkSkinInstance = NULL);
    virtual bool RemoveObjectFromGroup(NiGeometryData* pkData, 
        NiSkinInstance* pkSkinInstance = NULL);

    virtual bool AddObjectToGroup(
        NiGeometryGroup* pkGroup, 
        NiScreenTexture* pkScreenTexture);
    virtual bool RemoveObjectFromGroup(NiScreenTexture* pkScreenTexture);

    // *** begin Emergent internal use only ***

    static NiD3DGeometryGroupManager* Create(D3DDevicePtr pkD3DDevice,
        NiD3DVBManager* pkVBManager);

    // More useful D3D-specific AddObjectToGroup
    bool AddObjectToGroup(
        NiGeometryGroup* pkGroup, 
        NiGeometryData* pkData, 
        NiSkinInstance* pkSkinInstance, 
        bool bHWSkinning, 
        unsigned int uiBonesPerPartition, 
        unsigned int uiBonesPerVertex);

    bool RemoveSkinPartitionFromGroup(NiSkinPartition* pkSkinPartition);

    void SetHWSkinning(bool bHWSkinning);

    // *** end Emergent internal use only ***

protected:
    NiD3DGeometryGroupManager();

    NiTPrimitiveSet<NiGeometryGroup*> m_kGroups;

    D3DDevicePtr m_pkD3DDevice;
    NiD3DVBManager* m_pkVBManager;
    bool m_bHWSkinning;
};

#endif // NID3DGEOMETRYGROUPMANAGER_H