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
inline NiSPTask* NiSPWorkflow::AddNewTask()
{
    NiSPTask* pkTask = ms_pkTaskPool->GetFreeObject();
    Add(pkTask);
    return pkTask;
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPWorkflow::GetId() const
{
    return m_uiId;
}
//---------------------------------------------------------------------------
inline NiSPWorkflow::Status NiSPWorkflow::GetStatus() const
{
    return m_eStatus;
}
//---------------------------------------------------------------------------
inline void NiSPWorkflow::SetStatus(NiSPWorkflow::Status eStatus)
{
    m_eStatus = eStatus;
}
//---------------------------------------------------------------------------
inline NiInt32 NiSPWorkflow::GetCurrentStage() const
{
    return m_iCurrentStage;
}
//---------------------------------------------------------------------------
inline void NiSPWorkflow::SetCurrentStage(NiInt32 iStage)
{
     m_iCurrentStage = iStage;
}
//---------------------------------------------------------------------------
inline void NiSPWorkflow::AddPendingJobList(NiSPJobList* pkJobList)
{
    m_kPendingJobLists.Add(pkJobList);
}
//---------------------------------------------------------------------------
inline void NiSPWorkflow::RemovePendingJobList(NiSPJobList* pkJobList)
{
    int iFound = m_kPendingJobLists.Find(pkJobList);
    if (iFound > 0)
        m_kPendingJobLists.RemoveAt(iFound);
}
//---------------------------------------------------------------------------
inline void NiSPWorkflow::ClearPendingJobLists()
{
    m_kPendingJobLists.RemoveAll();
}
//---------------------------------------------------------------------------
inline NiUInt32 NiSPWorkflow::GetNumPendingJobLists() const
{
    return m_kPendingJobLists.GetSize();
}
//---------------------------------------------------------------------------
inline const NiSPJobList* NiSPWorkflow::GetPendingJobList(
    NiUInt32 uiIndex) const
{
    return m_kPendingJobLists.GetAt(uiIndex);
}
//---------------------------------------------------------------------------
inline void NiSPWorkflow::Execute()
{
    NIASSERT(m_pkWorkflowImpl && "m_pkWorkflowImpl was NULL!");
    m_pkWorkflowImpl->Execute(this);
}
//---------------------------------------------------------------------------
inline void NiSPWorkflow::ExecutionComplete()
{
    NIASSERT(m_pkWorkflowImpl && "m_pkWorkflowImpl was NULL!");
    m_pkWorkflowImpl->Terminate();
}
