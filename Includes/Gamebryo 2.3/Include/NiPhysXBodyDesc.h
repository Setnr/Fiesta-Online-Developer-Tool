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


#ifndef NIPHYSXBODYDESC_H
#define NIPHYSXBODYDESC_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

class NIPHYSX_ENTRY NiPhysXBodyDesc : public NiObject
{
    NiDeclareClone(NiPhysXBodyDesc);
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareViewerStrings;

public:
    NiPhysXBodyDesc();
    virtual ~NiPhysXBodyDesc();
    
    virtual void ToBodyDesc(NxBodyDesc& kBodyDesc, const unsigned int uiIndex,
        const NxMat33& kXform) const;
    virtual void SetFromBodyDesc(const NxBodyDesc& kBodyDesc,
        const NxMat33& kXform);
    void SetVelocities(const NxVec3& kLinearVelocity,
        const NxVec3& kAngularVelocity, const unsigned int uiIndex,
        const NxMat33& kXform);

    virtual unsigned int AddState(NxActor* pkActor, const NxMat33& kXform);
    virtual void UpdateState(NxActor* pkActor, const unsigned int uiIndex,
        const NxMat33& kXform);
    virtual void RestoreState(NxActor* pkActor, const unsigned int uiIndex,
        const NxMat33& kXform);

    // *** begin Emergent internal use only ***

    void GetVelocities(NxVec3& kLinearVelocity, NxVec3& kAngularVelocity,
        const unsigned int uiIndex) const;

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    NxBodyDesc m_kBodyDesc;
    
    unsigned int m_uiNumVels;
    NxVec3* m_pkLinVels;
    NxVec3* m_pkAngVels;
};

NiSmartPointer(NiPhysXBodyDesc);

#include "NiPhysXBodyDesc.inl"

#endif // NIPHYSXBODYDESC_H

