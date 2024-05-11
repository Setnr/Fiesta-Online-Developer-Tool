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

#ifndef NINAVFLYCONTROLLER_H
#define NINAVFLYCONTROLLER_H

#include "NiUserInterfaceLibType.h"
#include "NiNavBaseController.h"
//---------------------------------------------------------------------------
class NIUSERINTERFACE_ENTRY NiNavFlyController : public NiNavBaseController
{
public:
    NiNavFlyController(
        NiAVObject* pkReferenceObject, 
        NiAVObject* pkControlledObject, 
        const char* pcName, float fScale, 
        NiPoint3 kUpVector = NiPoint3::ZERO, 
        bool bFlipPitch = false, 
        bool bFlipYaw = false);
    virtual ~NiNavFlyController();

    enum Controls
    {
        SCALE_CHANGE = 0,
        YAW_LOOK,
        PITCH_LOOK,
        PAN_RIGHT_LEFT,
        PAN_UP_DOWN,
        LOOK_DOLLY,

        FLY_NUM_CONTROLS
    };

    virtual bool SetActive(bool bActive, NavState& kCurState);

    void ResetKeyboardHotkey(Controls eControlId, bool bPositiveDir, 
        NiInputKeyboard::KeyCode eKey, 
        NiInputKeyboard::Modifiers eModifier = NiInputKeyboard::KMOD_NONE);
    void ResetMouseHotkey(Controls eControlId, bool bPositiveDir, 
        NiInputMouse::Button eButton, 
        NiInputMouse::Modifiers eModifier = NiInputMouse::MMOD_NONE);
    void ResetMouseAxis(Controls eControlId, NiInputMouse::Axes eMouseAxis);
    void ResetGamePadHotkey(Controls eControlId, bool bPositiveDir, 
        NiInputGamePad::Button eButton, 
        NiInputGamePad::ButtonMask eModifier = NiInputGamePad::NIGP_MASK_NONE);
    void ResetGamePadAxis(Controls eControlId, 
        NiInputGamePad::DefaultAxis eAxis);
    void SetControlledObject(NiAVObject* pkControlledObject);
    void SetReferenceObject(NiAVObject* pkReferenceObject);

    void SetUpDir(const NiPoint3& kNewUpDir);
    void SetControlActive(bool bActive, Controls eControlId);
    void SetScale(float fScale);
    void SetFlipPitch(bool bFlipPitch);
    bool GetFlipPitch();
    void SetFlipYaw(bool bFlipYaw);
    bool GetFlipYaw();
    
    virtual const char* GetControlName(unsigned int uiControlId) const;

protected:
    static const char* ms_pcElementName[FLY_NUM_CONTROLS];
    static const float ms_fPosChangeScale;
    static const float ms_fScaleChangeRate;
    static const float ms_fMinScale;
    static const float ms_fAngleChangeRate;

    virtual void MouseInput(unsigned int uiControlId, float fDelta);
    virtual void GamePadInput(unsigned int uiControlId, float fDelta);
    virtual void UpdatePose();

    void CreateLookAtPt();

    void LookPosChange(unsigned char ucButtonState);
    void LookNegChange(unsigned char ucButtonState);
    // RL is right/left
    void PanRLPosChange(unsigned char ucButtonState);
    void PanRLNegChange(unsigned char ucButtonState);
    // UD is right/left
    void PanUDPosChange(unsigned char ucButtonState);
    void PanUDNegChange(unsigned char ucButtonState);
    void YawPosChange(unsigned char ucButtonState);
    void YawNegChange(unsigned char ucButtonState);
    void PitchPosChange(unsigned char ucButtonState);
    void PitchNegChange(unsigned char ucButtonState);
    void ScalePosChange(unsigned char ucButtonState);
    void ScaleNegChange(unsigned char ucButtonState);

    float m_afDeltaValues[FLY_NUM_CONTROLS];
    // If this is zero, the up direction may change
    NiPoint3 m_kUpVector;
    NiPoint3 m_kLookAtPt;
    NiAVObjectPtr m_spReferenceObject;
    NiAVObjectPtr m_spControlledObject;
    float m_fScale;

    float m_fFlipPitch;
    float m_fFlipYaw;
};
NiSmartPointer(NiNavFlyController);
//---------------------------------------------------------------------------
#include "NiNavFlyController.inl"
//---------------------------------------------------------------------------

#endif
