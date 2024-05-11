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
inline NiStreamProcessor* NiStreamProcessor::Get()
{
    return ms_pkInstance;
}
//---------------------------------------------------------------------------
inline NiStreamProcessor::Status NiStreamProcessor::GetStatus() const
{
    return m_eStatus;
}
//---------------------------------------------------------------------------
inline const NiThread* NiStreamProcessor::GetThread() const
{
    return m_pkSubmitThread;
}
//---------------------------------------------------------------------------
inline bool NiStreamProcessor::IsActive()
{
    return (ms_pkInstance && ms_pkInstance->m_bIsActive);
}
//---------------------------------------------------------------------------
inline bool NiStreamProcessor::IsRunning()
{
    return m_eStatus == RUNNING;
}
//---------------------------------------------------------------------------
inline NiUInt32 NiStreamProcessor::GetTotalWorkflowCount() const
{
    NiUInt32 uiWorkflowCount = 0;
    for (NiUInt32 uiPriority = 0; uiPriority < NUM_PRIORITIES; uiPriority++)
        uiWorkflowCount += m_akWorkflowQueues[uiPriority].GetSize();
    return uiWorkflowCount;
}
//---------------------------------------------------------------------------
inline NiUInt32 NiStreamProcessor::GetWorkflowCount(
    NiStreamProcessor::Priority ePriority) const
{
    return m_akWorkflowQueues[ePriority].GetSize();
}
//---------------------------------------------------------------------------
inline bool NiStreamProcessor::SetAffinity(
    const NiProcessorAffinity& kAffinity)
{
    if (m_pkSubmitThread)
        return m_pkSubmitThread->SetThreadAffinity(kAffinity);
    return false;
}
//---------------------------------------------------------------------------
inline bool NiStreamProcessor::SetPriority(NiThread::Priority ePriority)
{
    if (m_pkSubmitThread)
        return m_pkSubmitThread->SetPriority(ePriority);
    return false;
}
//---------------------------------------------------------------------------
inline NiSPThreadPool* NiStreamProcessor::GetThreadPool()
{
    return m_pkThreadPool;
}
//---------------------------------------------------------------------------

