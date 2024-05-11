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

#ifndef NISKINPARTITION_H
#define NISKINPARTITION_H

#include "NiGeometryData.h"
#include "NiMainLibType.h"
#include "NiObject.h"
#include "NiSmartPointer.h"
#include "NiStream.h"
#include "NiTSet.h"

class NiTriBasedGeomData;
class NiSkinData;

class NIMAIN_ENTRY NiSkinPartition : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;

protected:
    class Interaction;
    class VertexInteractions;
    class BoneSet;
    class BoneSetMerger;
    class TriangleSet;

public:
    class NIMAIN_ENTRY Partition : public NiMemObject
    {
    public:
        Partition();
        virtual ~Partition();

        unsigned short* m_pusBones;
        float* m_pfWeights;
        unsigned short* m_pusVertexMap;
        unsigned char* m_pucBonePalette;
        unsigned short* m_pusTriList;
        unsigned short* m_pusStripLengths;

        unsigned short m_usVertices;
        unsigned short m_usTriangles;
        unsigned short m_usBones;
        unsigned short m_usStrips;
        unsigned short m_usBonesPerVertex;
        NiGeometryData::RendererData* m_pkBuffData;

        // *** begin Emergent internal use only ***
        void Create(const TriangleSet& kTriangles,
            const BoneSet& kBones, const NiTriBasedGeomData* pkTriData,
            const VertexInteractions* pkVertexInteraction,
            bool bNeedPalette, bool bForcePalette,
            unsigned char ucBonesPerVertex, unsigned int uiMaxBones);
        void LoadBinary(NiStream& kStream);
        void SaveBinary(NiStream& kStream);
        bool IsEqual(const Partition& kPartition);
        void PostPrecache();
        unsigned int GetStripLengthSum() const;
        // *** end Emergent internal use only ***

    protected:
        unsigned short* CreateVertexMap(const TriangleSet& kTriangles,
            const NiTriBasedGeomData* pkTriData);
        void CreateTriangleList(const TriangleSet& kTriangles,
            const NiTriBasedGeomData* pkTriData,
            const unsigned short* pusVertexPartitionIndex);
    };

    NiSkinPartition();
    ~NiSkinPartition();

    bool MakePartitions(const NiTriBasedGeomData* pkTriData,
        const NiSkinData* pkSkinData, unsigned char ucBonesPerPartition,
        unsigned char ucBonesPerVertex, bool bForcePalette);

    void ReplacePartitions(Partition* pkPartitions, 
        unsigned int uiPartitions);

    bool ResizePartitions(unsigned char ucBonesPerPartition,
        unsigned char ucBonesPerVertex);

    unsigned int GetPartitionCount() const;
    Partition* GetPartitions();
    const Partition* GetPartitions() const;

protected:
    typedef NiTPrimitiveSet<BoneSet*> NiBoneSetCollection;

    void MakeBoneSets(const NiTriBasedGeomData* pkTriData, 
        const NiSkinData* pkSkinData, unsigned char ucBonesPerPartition,
        VertexInteractions* pkVertexInteraction, 
        NiBoneSetCollection& kBoneSets,
        unsigned int*& puiTriangleToPartition);
    VertexInteractions* CreateVertexInteractions(unsigned int uiVertices, 
        const NiSkinData* pSkinData);
    bool RemoveExcessInteractions(const NiTriBasedGeomData* pkTriData,
        unsigned char ucBonesPerPartition, unsigned char ucBonesPerVertex,
        VertexInteractions* pkVertexInteraction);
    void CreateInitialBoneSets(const NiTriBasedGeomData* pkTriData,
        VertexInteractions* pkVertexInteraction,
        NiBoneSetCollection& kBoneSets);
    void MergeBoneSets(unsigned char ucBonesPerPartition,
        NiBoneSetCollection& kBoneSets);
    void MakeBoneMap(NiTPointerMap<unsigned int, float>& kMap,
        const unsigned short* pusTriangle,
        const VertexInteractions* pkVertexInteraction);
    static int AssignTriangle(NiBoneSetCollection& kBoneSets,
        const unsigned short* pusTriangle,
        VertexInteractions* pkVertexInteraction, bool* pbAssigned,
        bool bAssign);

    void MergeIntoPartition(Partition& kDest, Partition* pkSrc, 
         unsigned int uiSrcStartIdx, unsigned int uiSrcEndIdx,
         unsigned char ucBonesPerPartition, unsigned char ucBonesPerVertex,
         unsigned short* puiBoneIndices, unsigned int uiBoneCount);

    unsigned int m_uiPartitions;
    Partition* m_pkPartitions;

};

NiSmartPointer(NiSkinPartition);

#include "NiSkinPartition.inl"

#endif // NISKINPARTITION_H
