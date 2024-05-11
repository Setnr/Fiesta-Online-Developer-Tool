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

#ifndef NIPSYSUPDATETASK_H
#define NIPSYSUPDATETASK_H

#include <NiTask.h>
#include <NiTPool.h>

#include "NiParticleLibType.h"

class NiParticleSystem;
NiSmartPointer(NiParticleSystem);

// This class contains a pointer to an NiParticleSystem. When it is executed
// by a TaskManager, it calls Do_UpdateSystem on the particle
// system and then returns itself to the pool of tasks.
class NIPARTICLE_ENTRY NiPSysUpdateTask : public NiTask
{
    NiDeclareRTTI;

    // Pool managing interface
public:
    static NiPSysUpdateTask* GetFreeObject();
    static void ReleaseObject(NiPSysUpdateTask* pkTask);

    void Init(NiParticleSystem* pkSystem, float fTime);

    virtual void DoTask();
    virtual bool Clear();

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:

    // These objects are pool allocated, must use GetFree and Release
    NiPSysUpdateTask();
    ~NiPSysUpdateTask();

    static NiCriticalSection ms_kPoolCriticalSection;
    static NiTObjectPool<NiPSysUpdateTask>* ms_pkPool;
    // allow this class to be constructed by pool:
    friend class NiTNewInterface<NiPSysUpdateTask>;

protected:
    NiParticleSystemPtr m_spSystem;
    float m_fTime;
};

#endif // NIPSYSUPDATETASK_H
