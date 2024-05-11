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

//---------------------------------------------------------------------------
inline NiSkinInstance::NiSkinInstance(NiSkinData* pkSkinData,
    NiAVObject* pkRootParent, NiAVObject** ppkBones) :
    m_spSkinData(pkSkinData), 
    m_pkRootParent(pkRootParent), 
    m_ppkBones(ppkBones), 
    m_uiFrameID(UINT_MAX),
    m_uiNumMatrices(0),
    m_uiNumRegisters(0),
    m_uiAllocatedSize(0),
    m_pvBoneMatrices(NULL),
    m_pvSkinToWorldWorldToSkinMatrix(NULL)
{
    NIASSERT(pkSkinData != NULL);
    NIASSERT(pkRootParent != NULL);
    NIASSERT(ppkBones != NULL);
}
//---------------------------------------------------------------------------
inline NiSkinInstance::NiSkinInstance() :
    m_spSkinData(0), 
    m_pkRootParent(0), 
    m_ppkBones(0), 
    m_uiFrameID(UINT_MAX),
    m_uiNumMatrices(0),
    m_uiNumRegisters(0),
    m_uiAllocatedSize(0),
    m_pvBoneMatrices(NULL),
    m_pvSkinToWorldWorldToSkinMatrix(NULL)
{
    // Used for streaming: All members will be initialized by LoadBinary.
    // Used for cloning: All members will be initialized by CopyMembers and
    // ProcessClone.
}
//---------------------------------------------------------------------------
inline NiSkinData* NiSkinInstance::GetSkinData() const
{
    return m_spSkinData;
}
//---------------------------------------------------------------------------
inline NiAVObject* NiSkinInstance::GetRootParent() const
{
    return m_pkRootParent;
}
//---------------------------------------------------------------------------
inline NiAVObject*const* NiSkinInstance::GetBones() const
{
    return m_ppkBones;
}
//---------------------------------------------------------------------------
inline NiSkinPartition* NiSkinInstance::GetSkinPartition() const
{
    return m_spSkinPartition;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::Deform(const NiGeometryData* pkModelData,
    NiPoint3* pkDstVertex, NiPoint3* pkDstNormal, NiPoint3* pkDstBinormal, 
    NiPoint3* pkDstTangent, unsigned int uiStride) const
{
    Deform(pkModelData->GetVertices(), pkModelData->GetNormals(),
        pkModelData->GetVertexCount(), pkDstVertex, pkDstNormal, 
        pkDstBinormal, pkDstTangent, uiStride);
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::SetRootParent(NiAVObject* pkRootParent)
{
    m_pkRootParent = pkRootParent;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::SetSkinData(NiSkinData* pkSkinData)
{
    m_spSkinData = pkSkinData;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::SetBone(unsigned int uiIndex, NiAVObject* pkBone)
{

    m_ppkBones[uiIndex] = pkBone;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::SetSkinPartition(
    NiSkinPartition* pkSkinPartition)
{
    m_spSkinPartition = pkSkinPartition;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::GetWorldToSkinTransform(
    NiTransform& kWorldToSkin) const
{
    NiTransform kWorldToRootParent;

    m_pkRootParent->GetWorldTransform().Invert(kWorldToRootParent);
    kWorldToSkin = m_spSkinData->GetRootParentToSkin() * kWorldToRootParent;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::ConcatWorldToSkinTransform(
    const NiTransform& kSourceMat, NiTransform& kDestMat) const
{
    NiTransform kWorldToRootParent;

    m_pkRootParent->GetWorldTransform().Invert(kWorldToRootParent);
    kDestMat = kSourceMat * m_spSkinData->GetRootParentToSkin() 
        * kWorldToRootParent;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::GetBoneWorldTransform(unsigned int uiBone, 
    NiTransform& kBoneWorld) const
{
    kBoneWorld = m_ppkBones[uiBone]->GetWorldTransform();
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::ConcatBoneWorldTransform(unsigned int uiBone, 
    const NiTransform& kSourceMat, NiTransform& kDestMat) const
{
    kDestMat = kSourceMat * m_ppkBones[uiBone]->GetWorldTransform();
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::GetSkinToBoneTransform(unsigned int uiBone, 
    NiTransform& kSkinToBone) const
{
    const NiSkinData::BoneData* pkBoneData = m_spSkinData->GetBoneData();
    kSkinToBone = pkBoneData[uiBone].m_kSkinToBone;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::ConcatSkinToBoneTransform(unsigned int uiBone, 
    const NiTransform& kSourceMat, NiTransform& kDestMat) const
{
    const NiSkinData::BoneData* pkBoneData = m_spSkinData->GetBoneData();
    kDestMat = kSourceMat * pkBoneData[uiBone].m_kSkinToBone;
}
//---------------------------------------------------------------------------
inline unsigned int NiSkinInstance::GetFrameID() const
{
    return m_uiFrameID;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::SetFrameID(unsigned int uiID)
{
    m_uiFrameID = uiID;
}

//---------------------------------------------------------------------------
inline void NiSkinInstance::SetBoneMatrices(void* pvMatrices, 
    unsigned int uiAllocatedSize)
{
    m_pvBoneMatrices = pvMatrices;
    m_uiAllocatedSize = uiAllocatedSize;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::SetBoneMatrixInfo(unsigned int uiNumMatrices, 
    unsigned int uiNumRegisters)
{
    m_uiNumMatrices = uiNumMatrices;
    m_uiNumRegisters = uiNumRegisters;
}
//---------------------------------------------------------------------------
inline unsigned int NiSkinInstance::GetNumBoneMatrices() const
{
    return m_uiNumMatrices;
}
//---------------------------------------------------------------------------
inline unsigned int NiSkinInstance::GetBoneMatrixRegisters() const
{
    return m_uiNumRegisters;
}
//---------------------------------------------------------------------------
inline void* NiSkinInstance::GetBoneMatrices() const
{
    return m_pvBoneMatrices;
}
//---------------------------------------------------------------------------
inline unsigned int NiSkinInstance::GetBoneMatrixAllocatedSize() const
{
    return m_uiAllocatedSize;
}
//---------------------------------------------------------------------------
inline void NiSkinInstance::SetSkinToWorldWorldToSkinMatrix(void* pvMatrix)
{
    m_pvSkinToWorldWorldToSkinMatrix = pvMatrix;
}
//---------------------------------------------------------------------------
inline void* NiSkinInstance::GetSkinToWorldWorldToSkinMatrix() const
{
    return m_pvSkinToWorldWorldToSkinMatrix;
}
//---------------------------------------------------------------------------
