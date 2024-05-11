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
inline unsigned int NiPhysXPropDesc::GetRBCompartmentID() const
{
    return m_uiRBCompartmentID;
}
//---------------------------------------------------------------------------
inline void NiPhysXPropDesc::SetRBCompartmentID(const unsigned int uiID)
{
    m_uiRBCompartmentID = uiID;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPropDesc::GetClothCompartmentID() const
{
    return m_uiClothCompartmentID;
}
//---------------------------------------------------------------------------
inline void NiPhysXPropDesc::SetClothCompartmentID(const unsigned int uiID)
{
    m_uiClothCompartmentID = uiID;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPropDesc::GetFluidCompartmentID() const
{
    return m_uiFluidCompartmentID;
}
//---------------------------------------------------------------------------
inline void NiPhysXPropDesc::SetFluidCompartmentID(const unsigned int uiID)
{
    m_uiFluidCompartmentID = uiID;
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPropDesc::GetActorCount() const
{
    return m_kActors.GetSize();
}
//---------------------------------------------------------------------------
inline NiPhysXActorDescPtr NiPhysXPropDesc::GetActorAt(unsigned int uiIndex)
{
    return m_kActors.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPropDesc::AddActor(NiPhysXActorDesc* pkActorDesc)
{
    return m_kActors.AddFirstEmpty(pkActorDesc);
}
//---------------------------------------------------------------------------
inline NiPhysXActorDescPtr NiPhysXPropDesc::RemoveActorAt(
    unsigned int uiIndex)
{
    return m_kActors.RemoveAtAndFill(uiIndex);
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPropDesc::GetJointCount() const
{
    return m_kJoints.GetSize();
}
//---------------------------------------------------------------------------
inline NiPhysXJointDescPtr NiPhysXPropDesc::GetJointAt(unsigned int uiIndex)
{
    return m_kJoints.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPropDesc::AddJoint(NiPhysXJointDesc* pkJointDesc)
{
    return m_kJoints.AddFirstEmpty(pkJointDesc);
}
//---------------------------------------------------------------------------
inline NiPhysXJointDescPtr NiPhysXPropDesc::RemoveJointAt(
    unsigned int uiIndex)
{
    return m_kJoints.RemoveAtAndFill(uiIndex);
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPropDesc::GetClothCount() const
{
    return m_kClothes.GetSize();
}
//---------------------------------------------------------------------------
inline NiPhysXClothDescPtr NiPhysXPropDesc::GetClothAt(unsigned int uiIndex)
{
    return m_kClothes.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPropDesc::AddCloth(NiPhysXClothDesc* pkClothDesc)
{
    return m_kClothes.AddFirstEmpty(pkClothDesc);
}
//---------------------------------------------------------------------------
inline NiPhysXClothDescPtr NiPhysXPropDesc::RemoveClothAt(
    unsigned int uiIndex)
{
    return m_kClothes.RemoveAtAndFill(uiIndex);
}
//---------------------------------------------------------------------------
inline NiPhysXMaterialDescPtr NiPhysXPropDesc::GetMaterial(
    const NxMaterialIndex usIndex)
{
    NiPhysXMaterialDescPtr pkMaterial;
    
    if (m_kMaterials.GetAt(usIndex, pkMaterial))
        return pkMaterial;

    return 0;
}
//---------------------------------------------------------------------------
inline NiTMapIterator NiPhysXPropDesc::GetFirstMaterial()
{
    return m_kMaterials.GetFirstPos();
}
//---------------------------------------------------------------------------
inline void NiPhysXPropDesc::GetNextMaterial(NiTMapIterator& kPos,
    NxMaterialIndex& usIndex, NiPhysXMaterialDescPtr& spMaterial)
{
    m_kMaterials.GetNext(kPos, usIndex, spMaterial);
}
//---------------------------------------------------------------------------
inline bool NiPhysXPropDesc::RemoveMaterial(const NxMaterialIndex usIndex)
{
    return m_kMaterials.RemoveAt(usIndex);
}
//---------------------------------------------------------------------------
inline unsigned int NiPhysXPropDesc::GetNumStates() const
{
    return m_uiNumStates;
}
//---------------------------------------------------------------------------
inline void NiPhysXPropDesc::SetNumStates(const unsigned int uiNumStates)
{
    m_uiNumStates = uiNumStates;
}
//---------------------------------------------------------------------------
inline bool NiPhysXPropDesc::StreamCanSkip()
{
    return true;
}
//---------------------------------------------------------------------------
