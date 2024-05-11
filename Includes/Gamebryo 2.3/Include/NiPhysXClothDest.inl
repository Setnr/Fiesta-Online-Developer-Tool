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
inline NiTriShape* NiPhysXClothDest::GetTarget()
{
    return m_spTarget;
}
//---------------------------------------------------------------------------
inline NxCloth* NiPhysXClothDest::GetCloth()
{
    return m_pkCloth;
}
inline NxMeshData& NiPhysXClothDest::GetClothData()
{
    return m_kClothData;
}
//---------------------------------------------------------------------------
inline NxActor* NiPhysXClothDest::GetParentActor()
{
    return m_pkParentActor;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDest::SetParentActor(NxActor* pkActor)
{
    m_pkParentActor = pkActor;
}
//---------------------------------------------------------------------------
inline float NiPhysXClothDest::GetMeshGrowFactor() const
{
    return m_fMeshGrowFactor;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDest::SetMeshGrowFactor(const float fGrowFactor)
{
    m_fMeshGrowFactor = fGrowFactor;
}
//---------------------------------------------------------------------------
inline bool NiPhysXClothDest::GetDoSleepXforms() const
{
    return m_bDoSleepXforms;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDest::SetDoSleepXforms(const bool bDoSleepXforms)
{
    m_bDoSleepXforms = bDoSleepXforms;
}
//---------------------------------------------------------------------------
inline unsigned short NiPhysXClothDest::GetNBTSet() const
{
    return m_usNBTSet;
}
//---------------------------------------------------------------------------
inline void NiPhysXClothDest::SetNBTSet(const unsigned short usSet)
{
    m_usNBTSet = usSet;
}
//---------------------------------------------------------------------------
inline bool NiPhysXClothDest::GetSleeping() const
{
    return m_bSleeping;
}
//---------------------------------------------------------------------------
inline bool NiPhysXClothDest::StreamCanSkip()
{
    return true;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXClothDest::KeyToHashIndex(unsigned int* key,
        unsigned int uiTableSize)
{
    return ((key[0] << 16) | (key[1] << 8) | key[2]) % uiTableSize;
}
//---------------------------------------------------------------------------
inline bool NiPhysXClothDest::IsKeysEqual(
    unsigned int* key1, unsigned int* key2)
{
    return (key1[0] == key2[0] && key1[1] == key2[1] && key1[2] == key2[2])
        || (key1[1] == key2[0] && key1[2] == key2[1] && key1[0] == key2[2])
        || (key1[2] == key2[0] && key1[0] == key2[1] && key1[1] == key2[2]);
}
//---------------------------------------------------------------------------
