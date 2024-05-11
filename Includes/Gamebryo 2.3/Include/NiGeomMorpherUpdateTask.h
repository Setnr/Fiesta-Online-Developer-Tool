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

#ifndef NIGEOMMORPHERUPDATETASK_H
#define NIGEOMMORPHERUPDATETASK_H

#include <NiTask.h>
#include <NiTPool.h>
#include "NiGeomMorpherController.h"

#include "NiAnimationLibType.h"

class NiGeomMorpherController;

class NIANIMATION_ENTRY NiGeomMorpherUpdateTask : public NiTask
{
    NiDeclareRTTI;

    // Pool managing interface
public:
    static NiGeomMorpherUpdateTask* GetFreeObject();
    static void ReleaseObject(NiGeomMorpherUpdateTask* pkTask);

    virtual void DoTask();
    virtual bool Clear();

    void SetController(NiGeomMorpherController* pkController);

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    NiGeomMorpherUpdateTask();

    NiGeomMorpherControllerPtr m_spMorphController;
    NiObjectNETPtr m_spTarget;

    static NiCriticalSection ms_kPoolCriticalSection;
    static NiTObjectPool<NiGeomMorpherUpdateTask>* ms_pkPool;
    // allow this class to be constructed by pool:
    friend class NiTNewInterface<NiGeomMorpherUpdateTask>; 
};

#endif // NIGEOMMORPHERUPDATETASK_H

