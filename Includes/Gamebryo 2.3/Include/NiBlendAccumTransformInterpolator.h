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

#ifndef NIBLENDACCUMTRANSFORMINTERPOLATOR_H
#define NIBLENDACCUMTRANSFORMINTERPOLATOR_H

#include "NiBlendInterpolator.h"
#include "NiQuatTransform.h"

class NIANIMATION_ENTRY NiBlendAccumTransformInterpolator :
    public NiBlendInterpolator
{
    NiDeclareRTTI;
    NiDeclareClone(NiBlendAccumTransformInterpolator);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiBlendAccumTransformInterpolator(bool bManagerControlled,
        float fWeightThreshold, unsigned char ucArraySize);
    virtual ~NiBlendAccumTransformInterpolator();

    // Virtual function overrides from base class.
    virtual bool Update(float fTime, NiObjectNET* pkInterpTarget,
        NiQuatTransform& kValue);
    virtual bool GetValue(NiQuatTransform& kValue) const;
    virtual bool IsTransformValueSupported() const;
    virtual unsigned char AddInterpInfo(NiInterpolator* pkInterpolator,
        float fWeight, char cPriority = 0, float fEaseSpinner = 1.0f);
    virtual NiInterpolatorPtr RemoveInterpInfo(unsigned char ucIndex);
    
    void ClearAccumulatedTransform();
    void SetAccumulatedTransform(const NiQuatTransform& kTransform);
    const NiQuatTransform& GetAccumulatedTransformValue() const;

protected:
    // For cloning and streaming only.
    NiBlendAccumTransformInterpolator();

    // Virtual function overrides from base class.
    virtual bool InterpolatorIsCorrectType(NiInterpolator* pkInterpolator)
        const;

    bool StoreSingleValue(float fTime, NiObjectNET* pkInterpTarget,
        NiQuatTransform& kValue);
    bool BlendValues(float fTime, NiObjectNET* pkInterpTarget,
        NiQuatTransform& kValue);

    virtual bool ReallocateArrays();
    
    void CalculateSingleValue(float fTime, NiObjectNET* pkInterpTarget);
    void CalculateValue(unsigned char ucIndex, float fTime, 
        NiObjectNET* pkInterpTarget);
    
    class AccumArrayItem : public NiMemObject
    {
    public:
        AccumArrayItem();

        void ClearValues();

        void LoadBinary(NiStream& kStream);
        void SaveBinary(NiStream& kStream);

        // Last time that this interpolator has been updated for
        float m_fLastTime;
        
        // m_kLastValue stores the most recent _raw_ transformation out of
        // the interpolator (the transform at time m_fLastTime).
        NiQuatTransform m_kLastValue;
        
        // When blending two or more sequences, this holds the change over
        // a single frame.
        NiQuatTransform m_kDeltaValue;
        
        // This is used to convert into the global coordinates so that
        // everyone agrees on which directions are x,y,z.
        NiMatrix3 m_kRefFrame;
    };

    NiQuatTransform m_kAccumulatedTransformValue;
    AccumArrayItem* m_pkAccumArray;
    bool m_bReset;
};

NiSmartPointer(NiBlendAccumTransformInterpolator);

#include "NiBlendAccumTransformInterpolator.inl"

#endif  // #ifndef NIBLENDTRANSFORMINTERPOLATOR_H
