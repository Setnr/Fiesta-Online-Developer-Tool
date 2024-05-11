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
//  NiSwitchNode inline functions

//---------------------------------------------------------------------------
inline NiAVObject* NiSwitchNode::GetActiveChild() const
{
    if (0 <= m_iIndex && m_iIndex < int(m_kChildren.GetSize()))
        return m_kChildren.GetAt(m_iIndex);
    else
        return 0;
}
//---------------------------------------------------------------------------
inline void NiSwitchNode::SetIndex(int iIndex)
{
    NIASSERT(iIndex >= -1 && iIndex < int(m_kChildren.GetSize()));
    if (iIndex >= -1 && iIndex < int(m_kChildren.GetSize()))
        m_iIndex = iIndex;
}
//---------------------------------------------------------------------------
inline int NiSwitchNode::GetIndex() const
{
    return m_iIndex;
}
//---------------------------------------------------------------------------
inline void NiSwitchNode::UpdateOnlyActiveChild(bool bUpdateOnlyActive)
{
    SetBit(bUpdateOnlyActive, UPDATE_ONLY_ACTIVE_CHILD_MASK);
}
//---------------------------------------------------------------------------
inline bool NiSwitchNode::GetUpdateOnlyActiveChild() const
{
    return GetBit(UPDATE_ONLY_ACTIVE_CHILD_MASK);
}
//---------------------------------------------------------------------------
inline void NiSwitchNode::SetUpdateControllers(bool bUpdateControllers)
{
    SetBit(bUpdateControllers, UPDATECONTROLLERS_MASK);
}
//---------------------------------------------------------------------------
inline bool NiSwitchNode::GetUpdateControllers() const
{
    return GetBit(UPDATECONTROLLERS_MASK);
}
//---------------------------------------------------------------------------
