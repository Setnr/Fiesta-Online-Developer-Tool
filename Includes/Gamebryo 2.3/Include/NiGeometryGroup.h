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

#ifndef NIGEOMETRYGROUP_H
#define NIGEOMETRYGROUP_H

class NiGeometryBufferData;
class NiScreenTexture;
class NiVBChip;
class NiGeometryData;
class NiSkinInstance;

#include "NiD3DDefines.h"
#include <NiSkinPartition.h>

class NID3D_ENTRY NiGeometryGroup : public NiMemObject
{
public:
    virtual void Purge() = 0;

    // *** begin Emergent internal use only ***

    bool IsEmpty() const;

    virtual void AddObject(
        NiGeometryData* pkData, 
        NiSkinInstance* pkSkinInstance, 
        NiSkinPartition::Partition* pkPartition)
        = 0;
    virtual void RemoveObject(NiGeometryData* pkData) = 0;
    virtual void RemoveObject(NiSkinPartition::Partition* pkPartition) = 0;

    virtual void AddObject(NiScreenTexture* pkScreenTexture) = 0;
    virtual void RemoveObject(NiScreenTexture* pkScreenTexture) = 0;

    void SetVertexBufferSize(unsigned int uiVBSize);

    void AddObject(NiGeometryBufferData* pkBuffData);
    void RemoveObject(NiGeometryBufferData* pkBuffData);

    virtual NiVBChip* CreateChip(NiGeometryBufferData* pkBuffData, 
        unsigned int uiStream) = 0;
    virtual void ReleaseChip(NiGeometryBufferData* pkBuffData, 
        unsigned int uiStream) = 0;

    virtual bool IsDynamic() = 0;

    // *** end Emergent internal use only ***

protected:
    NiGeometryGroup();
    virtual ~NiGeometryGroup();

    unsigned int m_uiRefCount;

    D3DDevicePtr m_pkD3DDevice;

    friend class NiD3DGeometryGroupManager;

#if !defined(WIN32) && !!defined(_XENON)
    static NiCriticalSection ms_kCritSec;
#endif //#if !defined(WIN32) && !!defined(_XENON)
};

#include "NiGeometryGroup.inl"

#endif // NIGEOMETRYGROUP_H