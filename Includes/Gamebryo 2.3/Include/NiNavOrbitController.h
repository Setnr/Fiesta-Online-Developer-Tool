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

#ifndef NINAVORBITCONTROLLER_H
#define NINAVORBITCONTROLLER_H

#include "NiUserInterfaceLibType.h"
#include "NiNavBaseController.h"
#include "NiUISlot.h"
//---------------------------------------------------------------------------
class NIUSERINTERFACE_ENTRY NiNavOrbitController : public NiNavBaseController
{
public:
    NiNavOrbitController(NiAVObject* pkObject, const char* pcName, 
        float fRadius, NiPoint3 kUpVector = NiPoint3::ZERO);
    virtual ~NiNavOrbitController();

    enum Controls
    {
        RADIUS_DOLLY = 0,
        LATITUDE_ORBIT,
        LONGITUDE_ORBIT,
        PAN_RIGHT_LEFT,
        PAN_UP_DOWN,
        FLY,

        ORBIT_NUM_CONTROLS
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
    void SetObject(NiAVObject* pkObject);

    void SetUpDir(const NiPoint3& kNewUpDir);
    void SetControlActive(bool bActive, Controls eControlId);
    void SetRadius(float fRadius);

    virtual const char* GetControlName(unsigned int uiControlId) const;

protected:
    static const char* ms_pcElementName[ORBIT_NUM_CONTROLS];
    static const float ms_fRadiusChangeRate;
    static const float ms_fAngleChangeRate;
    static const float ms_fPanChangeRate;
    static const float ms_fMinRadius;

    virtual void MouseInput(unsigned int uiControlId, float fDelta);
    virtual void GamePadInput(unsigned int uiControlId, float fDelta);
    virtual void UpdatePose();

    void RadiusPosChange(unsigned char ucButtonState);
    void RadiusNegChange(unsigned char ucButtonState);
    void LatitudePosChange(unsigned char ucButtonState);
    void LatitudeNegChange(unsigned char ucButtonState);
    void LongitudePosChange(unsigned char ucButtonState);
    void LongitudeNegChange(unsigned char ucButtonState);
    // RL is right/left while UD is up/down
    void PanRLPosChange(unsigned char ucButtonState);
    void PanRLNegChange(unsigned char ucButtonState);
    void PanUDPosChange(unsigned char ucButtonState);
    void PanUDNegChange(unsigned char ucButtonState);
    void FlyPosChange(unsigned char ucButtonState);
    void FlyNegChange(unsigned char ucButtonState);

    //InputBinding m_akBindings[ORBIT_NUM_CONTROLS * 2];
    float m_afDeltaValues[ORBIT_NUM_CONTROLS];
    //bool m_abIsActive[ORBIT_NUM_CONTROLS];
    NiPoint3 m_kCenterPt;
    // If this is NiPoint3::ZERO, there are no constraints on motion
    NiPoint3 m_kUpVector;
    NiAVObjectPtr m_spControlledObject;
    float m_fRadius;
};
NiSmartPointer(NiNavOrbitController);
//---------------------------------------------------------------------------
#include "NiNavOrbitController.inl"
//---------------------------------------------------------------------------

#endif
