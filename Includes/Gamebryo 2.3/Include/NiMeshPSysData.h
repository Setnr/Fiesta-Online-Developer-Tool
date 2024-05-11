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

#ifndef NIMESHPSYSDATA_H
#define NIMESHPSYSDATA_H

#include "NiPSysData.h"
#include <NiNode.h>


class NIPARTICLE_ENTRY NiMeshPSysData : public NiPSysData
{
    NiDeclareRTTI;
    NiDeclareClone(NiMeshPSysData);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:

    enum 
    {
        DEFAULT_POOL_SIZE = -1
    };


    NiMeshPSysData(unsigned short usMaxNumParticles, bool bHasColors,
        bool bHasRotations, unsigned int uiPoolSize = DEFAULT_POOL_SIZE,
        unsigned int uiNumGenerations = 1, bool bFillPoolsOnLoad = false);

    virtual ~NiMeshPSysData();

    virtual void RemoveParticle(unsigned short usParticle);
    virtual void ResolveAddedParticles();

    NiAVObject* GetMeshAt(unsigned short usIndex) const;

    bool GetFillPoolsOnLoad();
    void SetFillPoolsOnLoad(bool bFill);

    unsigned int GetDefaultPoolsSize();
    void SetDefaultPoolsSize(unsigned int uiSize);

    // *** begin Emergent internal use only ***
    NiNode* GetContainerNode() const;
    void SetMeshAt(unsigned short usIndex, NiAVObject* pkMesh);

    virtual void SetActiveVertexCount(unsigned short usActive);

    unsigned int GetNumGenerations();
    void SetNumGenerations(unsigned int uiNumGenerations);

    int GetPoolSize(unsigned int uiGeneration);
    void SetPoolSize(unsigned int uiGeneration, unsigned int uiSize);

    int GetPoolCount(unsigned int uiGeneration);
    bool IsPoolEmpty(unsigned int uiGeneration);
    void EmptyPool(unsigned int uiGeneration);
    void PurgeRendererDataForPools(NiRenderer* pkRenderer);
    void FillPool(unsigned int uiGeneration, NiAVObject* pkOriginal);

    NiAVObjectPtr RemoveFromPool(unsigned int uiGeneration);
    void AddToPool(unsigned int uiGeneration, NiAVObjectPtr spObject);

    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:


    // For cloning and streaming only.
    NiMeshPSysData();

    NiNodePtr m_spParticleMeshes;

    bool m_bFillPoolsOnLoad;
    unsigned int m_uiDefaultPoolSize;
    typedef NiTObjectArray<NiAVObjectPtr> NiAVObjectArray;
    NiTPrimitiveArray< NiAVObjectArray*>  m_kPools;
};

NiSmartPointer(NiMeshPSysData);

#include "NiMeshPSysData.inl"

#endif  // #ifndef NIMESHPSYSDATA_H
