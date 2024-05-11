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

#ifndef NILOOKATINTERPOLATOR_H
#define NILOOKATINTERPOLATOR_H

#include "NiInterpolator.h"
#include <NiFlags.h>
#include <NiQuaternion.h>
#include "NiQuatTransform.h"

class NiAVObject;

class NIANIMATION_ENTRY NiLookAtInterpolator : public NiInterpolator
{
    NiDeclareRTTI;
    NiDeclareClone(NiLookAtInterpolator);
    NiDeclareStream;
    NiDeclareViewerStrings;
    NiDeclareFlags(unsigned short);

public:
    enum Axis
    {
        X,
        Y,
        Z
    };

    NiLookAtInterpolator(NiAVObject* pkLookAt = NULL, Axis eAxis = X,
        bool bFlip = false);
    ~NiLookAtInterpolator();
    
    // Virtual function overrides from base class.
    virtual bool Update(float fTime, NiObjectNET* pkInterpTarget,
        NiQuatTransform& kValue);
    virtual bool IsTransformValueSupported() const;
    
    NiAVObject* GetLookAt() const;
    void SetLookAt(NiAVObject* pkLookAt);

    bool GetFlip() const;
    void SetFlip(bool bFlip);

    Axis GetAxis() const;
    void SetAxis(Axis eAxis);

    NiInterpolator* GetTranslateInterpolator();
    void SetTranslateInterpolator(NiInterpolator* pkInterp);
    
    NiInterpolator* GetRollInterpolator();
    void SetRollInterpolator(NiInterpolator* pkInterp);
    
    NiInterpolator* GetScaleInterpolator();
    void SetScaleInterpolator(NiInterpolator* pkInterp);

    virtual void Collapse();

    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    
    // This method is used to resolve any dependencies that might
    // be broken by storing the interpolator in a sequence.
    // The default implementation returns true
    virtual bool ResolveDependencies(NiAVObjectPalette* pkPalette);
    virtual bool SetUpDependencies();
    virtual void GetActiveTimeRange(float& fBeginKeyTime, float& fEndKeyTime)
        const;
    virtual void GuaranteeTimeRange(float fStartTime, 
        float fEndTime);
    virtual NiInterpolator* GetSequenceInterpolator(float fStartTime,
        float fEndTime);
    virtual bool AlwaysUpdate() const;
    // *** end Emergent internal use only ***

protected:
    // Virtual functions overridden from base class.
   
    void SetLookAtName(const NiFixedString& kName);

    // Flags.
    enum
    {
        FLIP_MASK   = 0x0001,
        AXIS_MASK   = 0x0006,
        AXIS_POS    = 1
    };

    NiAVObject* m_pkLookAt;
    NiFixedString m_kLookAtName;

    NiQuatTransform m_kTransformValue;
    NiInterpolatorPtr m_aspInterpolators[3];
};

NiSmartPointer(NiLookAtInterpolator);

#include "NiLookAtInterpolator.inl"

#endif  // #ifndef NILOOKATINTERPOLATOR_H
