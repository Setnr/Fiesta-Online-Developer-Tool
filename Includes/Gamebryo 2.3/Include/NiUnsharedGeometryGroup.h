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

#ifndef NIUNSHAREDGEOMETRYGROUP_H
#define NIUNSHAREDGEOMETRYGROUP_H

#include "NiGeometryGroup.h"
#include <NiSkinPartition.h>
class NiGeometryData;

class NID3D_ENTRY NiUnsharedGeometryGroup : public NiGeometryGroup
{
public:
    virtual ~NiUnsharedGeometryGroup();

    virtual void Purge();

    // *** begin Emergent internal use only ***

    static NiGeometryGroup* Create();
    
    virtual void AddObject(
        NiGeometryData* pkData, 
        NiSkinInstance* pkSkinInstance, 
        NiSkinPartition::Partition* pkPartition);
    virtual void RemoveObject(NiGeometryData* pkData);
    virtual void RemoveObject(NiSkinPartition::Partition* pkPartition);

    virtual void AddObject(NiScreenTexture* pkScreenTexture);
    virtual void RemoveObject(NiScreenTexture* pkScreenTexture);

    virtual NiVBChip* CreateChip(NiGeometryBufferData* pkBuffData, 
        unsigned int uiStream);
    virtual void ReleaseChip(NiGeometryBufferData* pkBuffData, 
        unsigned int uiStream);

    virtual bool IsDynamic();

    // *** end Emergent internal use only ***

protected:
    static NiUnsharedGeometryGroup* ms_pkGroup;
};



#endif // NIUNSHAREDGEOMETRYGROUP_H