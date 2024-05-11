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

#ifndef NILOOKATCONTROLLER_H
#define NILOOKATCONTROLLER_H

#include <NiAVObject.h>
#include <NiTimeController.h>
#include "NiAnimationLibType.h"

NiSmartPointer(NiLookAtController);

// This class has been deprecated as of Gamebryo 1.2. 
// Equivalent functionality can be found with an NiTransformController
// containing an NiLookAtInterpolator.

class NIANIMATION_ENTRY NiLookAtController : public NiTimeController
{
    NiDeclareRTTI;
    NiDeclareClone(NiLookAtController);
    NiDeclareStream;
    NiDeclareViewerStrings;
    NiDeclareFlags(unsigned short);

public:
    // updates
    virtual void Update(float fTime);
    
    void SetLookAt(NiAVObject* pkLookAt);
    NiAVObject* GetLookAt() const;

    void SetFlip(bool bFlip);
    bool GetFlip() const;

    enum Axis { X, Y, Z };
    void SetAxis(Axis eAxis);
    Axis GetAxis() const;

    virtual bool IsTransformController() const;

protected:
    // Protected constructor to prevent instantiation of this class outside
    // of stream loading.
    NiLookAtController();


    // cloning
    virtual void ProcessClone(
        NiCloningProcess& kCloning);

    // --- Begin NIF conversion code
    // Deprecated flags - use new flag code
    enum
    {
        MAX_POS = 8
    };
    // --- End NIF conversion code

    // flags
    enum
    {
        FLIP_MASK   = 0x0001,
        AXIS_MASK   = 0x0006,
        AXIS_POS    = 1
    };

    NiAVObject* m_pkLookAt;

    // debugging
    virtual bool TargetIsRequiredType() const;
};

#include "NiLookAtController.inl"

#endif

