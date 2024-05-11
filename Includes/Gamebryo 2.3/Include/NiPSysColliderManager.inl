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
inline NiPSysCollider* NiPSysColliderManager::GetColliders()
{
    return m_spColliders;
}
//---------------------------------------------------------------------------
inline void NiPSysColliderManager::SetColliders(NiPSysCollider* pkColliders)
{
    m_spColliders = pkColliders;
}
//---------------------------------------------------------------------------
inline void NiPSysColliderManager::AddCollider(NiPSysCollider* pkCollider)
{
    pkCollider->SetNext(m_spColliders);
    m_spColliders = pkCollider;
    pkCollider->SetManager(this);
}
//---------------------------------------------------------------------------
