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

#ifndef NITRANSFORMINTERPOLATOR_H
#define NITRANSFORMINTERPOLATOR_H

#include "NiKeyBasedInterpolator.h"
#include "NiQuatTransform.h"
#include "NiTransformData.h"

class NIANIMATION_ENTRY NiTransformInterpolator :
    public NiKeyBasedInterpolator
{
    NiDeclareRTTI;
    NiDeclareClone(NiTransformInterpolator);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiTransformInterpolator(NiTransformData* pkData = NULL);
    NiTransformInterpolator(NiQuatTransform kPoseValue);

    enum Channel
    {
        POSITION = 0,
        ROTATION,
        SCALE
    };

    // Virtual function overrides from base class.
    virtual bool Update(float fTime, NiObjectNET* pkInterpTarget,
        NiQuatTransform& kValue);
    virtual bool IsTransformValueSupported() const;

    // Data access.
    NiTransformData* GetTransformData() const;
    void SetTransformData(NiTransformData* pkData);

    // Translation keys.
    unsigned int GetNumPosKeys() const;
    NiPosKey* GetPosData(unsigned int& uiNumKeys, NiPosKey::KeyType& eType, 
        unsigned char& ucSize) const;
    void ReplacePosData(NiPosKey* pkKeys, unsigned int uiNumKeys,
        NiPosKey::KeyType eType);
    void SetPoseTranslate(NiPoint3 kTranslate);

    // Rotation keys.
    unsigned int GetNumRotKeys() const;
    NiRotKey* GetRotData(unsigned int& uiNumKeys, NiRotKey::KeyType& eType,
        unsigned char& ucSize) const;
    void ReplaceRotData(NiRotKey* pkKeys, unsigned int uiNumKeys,
        NiRotKey::KeyType eType);
    void SetPoseRotate(NiQuaternion kRotate);

    // Scale keys.
    unsigned int GetNumScaleKeys() const;
    NiFloatKey* GetScaleData(unsigned int& uiNumKeys,
        NiFloatKey::KeyType& eType, unsigned char& ucSize) const;
    void ReplaceScaleData(NiFloatKey* pkKeys, unsigned int uiNumKeys,
        NiFloatKey::KeyType eType);
    void SetPoseScale(float fScale);

    void SetPoseValue(NiQuatTransform kPoseValue);

    // Implemented from NiKeyBasedInterpolator
    virtual unsigned short GetKeyChannelCount() const;
    virtual unsigned int GetKeyCount(unsigned short usChannel) const;
    virtual NiAnimationKey::KeyType GetKeyType(unsigned short usChannel)
        const;
    virtual NiAnimationKey::KeyContent GetKeyContent(
        unsigned short usChannel) const;
    virtual NiAnimationKey* GetKeyArray(unsigned short usChannel) const;
    virtual unsigned char GetKeyStride(unsigned short usChannel) const;

    virtual bool GetChannelPosed(unsigned short usChannel) const;
    virtual void Collapse();

    // *** begin Emergent internal use only ***
    virtual void GetActiveTimeRange(float& fBeginKeyTime, float& fEndKeyTime)
        const;
    virtual void GuaranteeTimeRange(float fStartTime, 
        float fEndTime);
    virtual NiInterpolator* GetSequenceInterpolator(float fStartTime,
        float fEndTime);
    // *** end Emergent internal use only ***

protected:
    // Virtual function overrides from base class.
    
    NiQuatTransform m_kTransformValue;

    NiTransformDataPtr m_spData;
    unsigned int m_uiLastTransIdx;
    unsigned int m_uiLastRotIdx;
    unsigned int m_uiLastScaleIdx;
};

NiSmartPointer(NiTransformInterpolator);

#include "NiTransformInterpolator.inl"

#endif  // #ifndef NITRANSFORMINTERPOLATOR_H
