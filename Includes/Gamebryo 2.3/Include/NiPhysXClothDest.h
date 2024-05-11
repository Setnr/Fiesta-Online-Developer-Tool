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

#ifndef NIPHYSXCLOTHDEST_H
#define NIPHYSXCLOTHDEST_H

#include "NiPhysXLibType.h"

#include <NiPhysXDest.h>

class NIPHYSX_ENTRY NiPhysXClothDest : public NiPhysXDest
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareClone(NiPhysXClothDest);

public:
    NiPhysXClothDest(NiTriShape* pkTargetNode);
    ~NiPhysXClothDest();

    NiTriShape* GetTarget();

    NxCloth* GetCloth();
    void SetCloth(NxCloth* pkCloth, const unsigned short* pusVertexMap);
    
    NxMeshData& GetClothData();
    
    NxActor* GetParentActor();
    void SetParentActor(NxActor* pkActor);

    float GetMeshGrowFactor() const;
    void SetMeshGrowFactor(const float fGrowFactor);

    bool GetDoSleepXforms() const;
    void SetDoSleepXforms(const bool bDoSleepXforms);
    
    unsigned short GetNBTSet() const;
    void SetNBTSet(const unsigned short usSet);

    bool GetSleeping() const;
    
    // Update functions
    virtual void UpdateFromActors(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce);
    virtual void UpdateSceneGraph(const float fT,
        const NiTransform& kRootTransform, const float fScalePToW,
        const bool bForce);

    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // Used for mapping triangles
    static unsigned int KeyToHashIndex(unsigned int* key,
        unsigned int uiTableSize);
    static bool IsKeysEqual(unsigned int* key1, unsigned int* key2);
    
    // *** end Emergent internal use only ***
    
protected:
    NiPhysXClothDest(); // For cloning and streaming.

    void ProcessBasicTearing(NiTriShapeDynamicData* pkGeomData,
        unsigned short& usChangedFlags);
    void ProcessGeneralTearing(NiTriShapeDynamicData* pkGeomData,
        unsigned short& usChangedFlags);
    void CopyVertexData(unsigned int uiSrc, unsigned int uiDest,
        NiTriShapeDynamicData* pkGeomData, unsigned short& usChangedFlags);
    void RecomputeNormals(NiTriShapeDynamicData* pkGeomData);
    void RecomputeNBTs(NiTriShapeDynamicData* pkGeomData);
    void FindNBT(NiPoint3* pkNBTs, unsigned int uiNBTIndex, 
        unsigned int uiNumVertices, NiPoint3& kEC0, NiPoint3& kEC1, 
        NiPoint2& kET0, NiPoint2& kET1);

    void AllocateClothData(const unsigned short* pusVertexMap);
    void DeallocateClothData();

    // The node that has the geometry data that we are driving.
    // We need this to compute relative transformations, and from
    // it we can get the mesh data. We will only support NiTriShapes,
    // because we cannot stripify on the fly as mesh connectivity changes.
    NiTriShapePtr m_spTarget;

    // The cloth that we are working from.
    NxCloth* m_pkCloth;
    
    // The mesh data for the cloth. We have it here for memory management
    NxMeshData m_kClothData;

    // The amount by which the number of vertices is allowed to grow as
    // cloth tears.
    float m_fMeshGrowFactor;

    // The mapping from vertices in the shape to vertices in the PhysX mesh.
    unsigned short* m_pusVertexMap;
    
    // An optional pointer to a NxActor object that is driving a
    // Gamebryo node that is rigidly attached to the Gamebryo node
    // for the cloth mesh. This can be used to more accurately extract
    // transforms.
    NxActor* m_pkParentActor;

    // The world transform of the target node when the cloth goes to sleep,
    // expressed in PhysX world coordinates. If the cloth is sleeping, this
    // should remain constant, so we set the local transform on the
    // Gamebryo node to give this world pose.
    NiTransform m_kWorldXform;

    // Internal previous vertex count, needed to process tears.
    unsigned int m_uiPreviousVertexCount;

    // The original number of vertices in the target shape. Set at
    // construction and never changed after that.
    unsigned short m_usOriginalVertexCount;
    
    // The texture set to use for NBT generation, if required
    unsigned short m_usNBTSet;

    // Whether or not to update transforms when the cloth is sleeping.
    // If the m_bDoSleepXforms is false, we assume that the Gamebryo
    // node is static with respect to the PhysX coordinate system and
    // hence no update is necessary when the cloth is sleeping. True
    // by default.
    bool m_bDoSleepXforms;

    // Whether or not the cloth is sleeping _and_ has been updated to it's
    // sleeping state. Read only.
    bool m_bSleeping;
};

NiSmartPointer(NiPhysXClothDest);

#include "NiPhysXClothDest.inl"

#endif  // #ifndef NIPHYSXCLOTHDEST_H
