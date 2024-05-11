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

#ifndef NISKININSTANCE_H
#define NISKININSTANCE_H

#include "NiAVObject.h"
#include "NiGeometryData.h"
#include "NiTransform.h"
#include "NiSkinData.h"
#include "NiSkinPartition.h"

// NiSkinInstance contains data that cannot shared by multiple
// instances of the same skinned object.

class NIMAIN_ENTRY NiSkinInstance : public NiObject
{
    NiDeclareRTTI;
    NiDeclareClone(NiSkinInstance);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    // The NiSkinInstance class assumes control of the ppkBones array passed
    // to its constructor. (i.e. Its destructor deletes it.)

    NiSkinInstance(NiSkinData* pkSkinData, NiAVObject* pkRootParent,
        NiAVObject** ppkBones);
    virtual ~NiSkinInstance();

    NiSkinData* GetSkinData() const;
    NiAVObject* GetRootParent() const;
    NiAVObject*const* GetBones() const;
    NiSkinPartition* GetSkinPartition() const;
    void SetBone(unsigned int uiIndex, NiAVObject* pkBone);

    void Deform(const NiGeometryData* pkModelData, NiPoint3* pkDstVertex,
        NiPoint3* pkDstNormal, NiPoint3* pkDstBinormal, 
        NiPoint3* pkDstTangent, unsigned int uiStride) const;

    void Deform(const NiPoint3* pkSrcVertex, const NiPoint3* pkSrcNormal,
        unsigned int uiVertices, NiPoint3* pkDstVertex, NiPoint3* pkDstNormal,
        NiPoint3* pkDstBinormal, NiPoint3* pkDstTangent, 
        unsigned int uiStride) const;

    void UpdateModelBound(NiBound& kBound);

    // Besides being used internally, these functions provide an interface
    // which eases the development of custom skinning code.
    void GetWorldToSkinTransform(NiTransform& kWorldToSkin) const;
    void ConcatWorldToSkinTransform(const NiTransform& kSourceMat, 
        NiTransform& kDestMat) const;
    void GetBoneWorldTransform(unsigned int uiBone, 
        NiTransform& kBoneWorld) const;
    void ConcatBoneWorldTransform(unsigned int uiBone, 
        const NiTransform& kSourceMat, NiTransform& kDestMat) const;
    void GetSkinToBoneTransform(unsigned int uiBone, 
        NiTransform& kSkinToBone) const;
    void ConcatSkinToBoneTransform(unsigned int uiBone, 
        const NiTransform& kSourceMat, NiTransform& kDestMat) const;
    void ConcatBoneWorldTransformAndSkinToBoneTransform(unsigned int uiBone, 
        NiTransform& kDestMat) const;

    bool ContainsVertexData(NiShaderDeclaration::ShaderParameter 
        eParameter) const;

    // *** begin Emergent internal use only ***

    NiSkinInstance();
    void SetSkinData(NiSkinData* pkSkinData);
    void SetRootParent(NiAVObject* pkRootParent);
    void SetSkinPartition(NiSkinPartition* pkSkinPartition);

    unsigned int GetFrameID() const;
    void SetFrameID(unsigned int uiID);

    // Bone matrix cache
    void SetBoneMatrices(void* pvMatrices, unsigned int uiAllocatedSize);
    void SetBoneMatrixInfo(unsigned int uiNumMatrices, 
        unsigned int uiNumRegisters);
    unsigned int GetNumBoneMatrices() const;
    unsigned int GetBoneMatrixRegisters() const;
    void* GetBoneMatrices() const;
    unsigned int GetBoneMatrixAllocatedSize() const;

    void SetSkinToWorldWorldToSkinMatrix(void* pvMatrix);
    void* GetSkinToWorldWorldToSkinMatrix() const;

    // streaming support
    virtual void PostLinkObject(NiStream& kStream);

    // *** end Emergent internal use only ***

protected:
    NiSkinDataPtr m_spSkinData;
    NiSkinPartitionPtr m_spSkinPartition;

    // Bone pointers are regular pointers rather than smart pointers to avoid
    // reference cycles that arise when a skin is a descendant of the root 
    // bone.
    NiAVObject* m_pkRootParent;
    NiAVObject** m_ppkBones;

    unsigned int m_uiFrameID;

    // Bone matrix cace
    unsigned int m_uiNumMatrices;
    unsigned int m_uiNumRegisters;
    unsigned int m_uiAllocatedSize;
    void* m_pvBoneMatrices;

    // SkinToWorld-WorldToSkin matrix. Used to remove the overhead of the
    // additional matrix concatenation per-bone.
    void* m_pvSkinToWorldWorldToSkinMatrix;
};

typedef NiPointer<NiSkinInstance> NiSkinInstancePtr;

#include "NiSkinInstance.inl"

#endif // NISKININSTANCE_H
