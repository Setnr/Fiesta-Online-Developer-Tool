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
#ifndef NISPANALYZER_H
#define NISPANALYZER_H

#include <NiSystem.h>
#include <NiTPtrSet.h>

#include "NiFloodgateLibType.h"

// NiStreamProcessor
class NiSPWorkflow;
class NiSPTask;
class NIFLOODGATE_ENTRY NiSPAnalyzer : public NiMemObject
{
public:

    // Constructor / Destructor
    NiSPAnalyzer();
    ~NiSPAnalyzer();

    // Analyzes a workflow to determine when tasks should run
    void Analyze(NiSPWorkflow* pkWorkflow);

    static const unsigned short MAX_STAGE = 999;

    static const unsigned short MAX_RECURSIONS = 256;

protected:
    
    // Internal analyzer method
    void Analyze(NiSPTask* pTask);

    unsigned short m_usRecursionCount;
};

//---------------------------------------------------------------------------

#endif
//---------------------------------------------------------------------------
