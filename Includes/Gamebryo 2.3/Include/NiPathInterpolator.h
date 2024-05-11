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

#ifndef NIPATHINTERPOLATOR_H
#define NIPATHINTERPOLATOR_H

#include <NiAVObject.h>
#include <NiKeyBasedInterpolator.h>
#include "NiPosData.h"
#include "NiFloatData.h"

NiSmartPointer(NiPathInterpolator);

class NIANIMATION_ENTRY NiPathInterpolator : public NiKeyBasedInterpolator
{
    NiDeclareRTTI;
    NiDeclareClone(NiPathInterpolator);
    NiDeclareStream;
    NiDeclareViewerStrings;
    NiDeclareFlags(unsigned short);

public:
    NiPathInterpolator(NiPosData* pkPathData = 0, NiFloatData* pkPctData = 0);
    virtual ~NiPathInterpolator();

    enum Channel
    {
        PATH = 0,
        PERCENTAGE
    };

    // Virtual function overrides from base class.
    virtual bool Update(float fTime, NiObjectNET* pkInterpTarget,
        NiQuatTransform& kValue);
    virtual bool IsTransformValueSupported() const;

    // shared data access
    void SetPathData(NiPosData* pkPathData);
    NiPosData* GetPathData();
    void SetPctData(NiFloatData* pkPctData);
    NiFloatData* GetPctData();

    // path data access
    NiPosKey* GetPathKeys(unsigned int& uiNumKeys, NiPosKey::KeyType& eType,
        unsigned char& ucSize) const;

    // For swapping animation sequences at runtime. The NiPosData object 
    //   assumes ownership of the data array being passed in, and has the 
    //   responsibility for deleting it when finished with it.
    void ReplacePathKeys(NiPosKey* pkPathKeys, unsigned int uiNumKeys,
        NiPosKey::KeyType ePathType);

    // percent data access
    NiFloatKey* GetPctKeys(unsigned int& uiNumKeys,
        NiFloatKey::KeyType& eType, unsigned char& ucSize) const;

    // For swapping animation sequences at runtime. The NiFloatData object 
    //   assumes ownership of the data array being passed in, and has the 
    //   responsibility for deleting it when finished with it.
    void ReplacePctKeys(NiFloatKey* pkPctKeys, unsigned int uiNumKeys,
        NiFloatKey::KeyType eType);

    // control option data member access methods
    void SetAllowFlip(bool bAllowFlip);
    bool GetAllowFlip() const;

    void SetBank(bool bBank);
    bool GetBank() const;

    enum BankDir { NEGATIVE = -1, POSITIVE = 1 };
    void SetBankDir(BankDir eDir);
    BankDir GetBankDir() const;

    void SetConstVelocity(bool bConstVelocity);
    bool GetConstVelocity() const;

    void SetFollow(bool bFollow);
    bool GetFollow() const;

    void SetMaxBankAngle(float fAngle);
    float GetMaxBankAngle() const;

    void SetSmoothing(float fSmooth);
    float GetSmoothing() const;

    void SetFollowAxis(short sAxis);
    short GetFollowAxis() const;

    void SetFlip(bool bFlip);
    bool GetFlip() const;

    bool GetCurveTypeOpen() const;

    float GetTotalPathLength() const;

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

    // *** begin Emergent internal use only ***
    void SetCurveTypeOpen(bool bOpen);
    virtual void GetActiveTimeRange(float& fBeginKeyTime, float& fEndKeyTime)
        const;
    virtual void GuaranteeTimeRange(float fStartTime, float fEndTime);
    virtual NiInterpolator* GetSequenceInterpolator(float fStartTime,
        float fEndTime);
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // --- Begin NIF conversion code
    // Deprecated flags - use new flag code
    enum
    {
        MAX_POS = 12
    };
    // --- End NIF conversion code

    // flags
    enum
    {
        CVDATAUPDATE_MASK   = 0x0001,
        OPENCURVE_MASK      = 0x0002,
        ALLOWFLIP_MASK      = 0x0004,
        BANK_MASK           = 0x0008,
        CONSTVELOCITY_MASK  = 0x0010,
        FOLLOW_MASK         = 0x0020,
        FLIP_MASK           = 0x0040
    };

    
    NiPoint3 GenPathInterp(int iI1, int iI2, float fNormTime);
    void CalcRefFrame(unsigned int uiI1, unsigned int uiI2, float fNormTime,
        NiMatrix3& refFrame);
    void GetPercentLoc(float fPercent, unsigned int& uiI1,
        unsigned int& uiI2, float& fNormTime);
    void GetLoc(float fTime, unsigned int& uiI1, unsigned int& uiI2,
        float& fNormTime);

    // constant velocity related routines
    void SetConstantVelocityData();
    float PathSpeed(int iI0, int iI1, float fTime) const;
    float PathLength(int iI0, int iI1, float fTime) const;

    void SetCVDataNeedsUpdate(bool bUpdate);
    bool GetCVDataNeedsUpdate() const;
 
    unsigned int m_uiLastPathIdx;
    unsigned int m_uiLastPctIdx;

    NiPosDataPtr m_spPathData;
    NiFloatDataPtr m_spPctData;

    // reparameterization by arc length
    mutable float* m_pfPartialLength;
    mutable float m_fTotalLength;

    float m_fMaxBankAngle;
    float m_fSmoothing;
    short m_sFollowAxis;
    float m_fMaxCurvature;
    BankDir m_eBankDir;

    NiQuaternion m_kRotation;
    NiPoint3 m_kPosition;
    float m_fLastTime;
};

#include "NiPathInterpolator.inl"

#endif
