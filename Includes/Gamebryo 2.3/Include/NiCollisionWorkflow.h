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

#if defined (_PS3) && defined(ENABLE_PARALLEL_PICK)

#ifndef NICOLLISIONWORKFLOW_H
#define NICOLLISIONWORKFLOW_H

#include <NiSPStream.h>
#include <NiSPWorkflow.h>
#include <NiNopKernel.h>
#include <vectormath/cpp/vectormath_aos.h>
#include "NiIntersectTriangleKernel.h"
#include "NiPick.h"

class NiTriShape;
class NiSPTask;
class NiPick;
class NiCollisionWorkflow : public NiSPWorkflow
{
public:
    
    // Gets the next NiCollisionWorkflow
    static NiCollisionWorkflow& GetCurrentWorkflow();

    // Advances to next workflow
    static void NextWorkflow();

    // Wait for outstanding workflows to complete for the given pick
    static void FinishWorkflows(NiPick* pkPick);
    
    // Initialize
    static void InitializePools();
    static void ShutdownPools();

    // Constructor / Destructor
    NiCollisionWorkflow();
    ~NiCollisionWorkflow();

    // Update workflow 
    void Initialize(NiPoint3& kModelOrigin, 
        NiPoint3& kModelDir, bool bCull, bool bFirstHitOnly, 
        unsigned short* pIndicies, 
        unsigned short usTriCount, NiUInt64 uiVertStartAddress, 
        NiTriShape* pkTriShape, NiPick* pkPick);

    // Public variables
    NiTriShape*                 m_pkTriShape;
    Vectormath::Aos::Vector3    m_kModelDir_v;
    Vectormath::Aos::Vector3    m_kModelOrigin_v;
    bool                        m_kAllowBack_b;
    bool                        m_kFirstHitOnly_b;
    NiUInt64                    m_kVertStartAddress_i;
    NiUInt32                    m_usTriCount;
    NiPick*                     m_pkPick;
    
    void HandleSignal(NiUInt32 uiSignal);

protected:

    static const NiUInt32       MAX_WORKFLOWS = 32;
    static NiCollisionWorkflow* ms_kWorkflows;
    static NiUInt32             ms_uiCurrentWorkflow;

    NiSPTask*                   m_pkSyncTask;
    NiNopKernel                 m_kSignalKernel;
    NiIntersectTriangleKernel   m_kIntersectTriangleKernel;

    
    // Inputs, all fixed
    NiTSPStream<unsigned short>                 m_kIndicies;  
    NiTSPFixedInput<Vectormath::Aos::Vector3>   m_kModelOrigin;
    NiTSPFixedInput<Vectormath::Aos::Vector3>   m_kModelDir;
    NiTSPFixedInput<bool>                       m_kAllowBack;
    NiTSPFixedInput<bool>                       m_kFirstHitOnly;
    NiTSPFixedInput<NiUInt64>                   m_kVertStartAddress;
   
    
};

#endif // #ifndef NIPS3RENDERPOINTSWORKFLOW_H
#endif // #if defined (_PS3)
