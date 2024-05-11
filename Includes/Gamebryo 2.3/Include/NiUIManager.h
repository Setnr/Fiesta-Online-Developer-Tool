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

#ifndef NIUIMANAGER_H
#define NIUIMANAGER_H

#include "NiUserInterfaceLibType.h"
#include <NiFont.h>
#include <NiTSet.h>
#include <NiInput.h>
#include <NiCursor.h>
#include <NiRenderer.h>
#include <NiRefObject.h>
#include <NiSmartPointer.h>

#include "NiUIRenderGroup.h"
#include "NiUIAtlasMap.h"
#include "NiUIGroup.h"
#include "NiUISignal.h"
#include "NiUILabel.h"

#define HOTKEYS

//---------------------------------------------------------------------------
class NIUSERINTERFACE_ENTRY NiUIManager : public NiRefObject
{
protected:
    NiUIManager();
public:
    virtual ~NiUIManager();
    static void Create();
    static NiUIManager* GetUIManager();
    static void Shutdown();
    bool Initialize(NiInputSystem* pkInputSystem,
        const char* pcAtlasFilename, NiCursor* pkCursor);
    void PrepareRenderer();
    void UpdateUI();
    void Draw(NiRenderer* pkRenderer);
    void SetVisible(bool bVisible);
    bool IsVisible();

    // These functions add/remove the specified group from the list
    // preserving "reading" order for all groups.
    // AddUIGroup ensures that the pointer is only added once
    void AddUIGroup(NiUIGroup* pkUIGroup);
    void RemoveUIGroup(NiUIGroup* pkUIGroup);
    NiUIGroup* GetUIGroupAt(unsigned int ui) const;
    unsigned int GetUIGroupCount() const;
    unsigned int GetSeletedUIGroup() const;
    NiUIGroup* SetSelectedUIGroup(unsigned int ui);

    bool ReserveMouse(NiUIBaseSlot1<unsigned char>& kLeftMouseSlot);
    void UnreserveMouse(NiUIBaseSlot1<unsigned char>& kLeftMouseSlot);

    enum ButtonState
    {
        ISPRESSED = 0x01,
        WASPRESSED = 0x02,
        WASRELEASED = 0x04
    };
    bool ReserveMouseButton(NiInputMouse::Button eHotkey, 
        NiUIBaseSlot1<unsigned char>* pkSlot, unsigned char ucButtonState, 
        NiInputMouse::Modifiers eModifier = NiInputMouse::MMOD_NONE);
    bool ReserveGamePadButton(NiInputGamePad::Button eHotkey, 
        NiUIBaseSlot1<unsigned char>* pkSlot, unsigned char ucButtonState,
        NiInputGamePad::ButtonMask eModifier = NiInputGamePad::NIGP_MASK_NONE);
    bool ReserveGamePadAxis(NiInputGamePad::DefaultAxis eAxis,
        NiUIBaseSlot1<unsigned char>* pkSlot);
    bool ReserveKeyboardButton(NiInputKeyboard::KeyCode eHotkey, 
        NiUIBaseSlot1<unsigned char>* pkSlot, unsigned char ucButtonState, 
        NiInputKeyboard::Modifiers eModifier = NiInputKeyboard::KMOD_NONE);
    void UnreserveMouseButton(NiInputMouse::Button eHotkey,
        NiUIBaseSlot1<unsigned char>* pkSlot,
        NiInputMouse::Modifiers eModifier = NiInputMouse::MMOD_NONE);
    void UnreserveGamePadButton(NiInputGamePad::Button eHotkey,
        NiUIBaseSlot1<unsigned char>* pkSlot,
        NiInputGamePad::ButtonMask eModifier = NiInputGamePad::NIGP_MASK_NONE);
    void UnreserveGamePadAxis(NiInputGamePad::DefaultAxis eAxis,
        NiUIBaseSlot1<unsigned char>* pkSlot);
    void UnreserveKeyboardButton(NiInputKeyboard::KeyCode eHotkey,
        NiUIBaseSlot1<unsigned char>* pkSlot,
        NiInputKeyboard::Modifiers eModifier = NiInputKeyboard::KMOD_NONE);
    bool KeyboardExists();
    bool MouseExists();
    bool GamePadExists();

    const NiUIAtlasMap* GetAtlasMap();
    NiFont* GetFont();
    float GetMaxCharWidthInNSC() const;
    float GetMaxCharHeightInNSC() const;

    void RegisterUIRenderGroup(NiUIRenderGroup* pkRenderGroup);

    NiInputKeyboard* GetKeyboard();
    NiInputMouse* GetMouse();
    NiInputSystem* GetInputSystem();
    NiCursor* GetCursor();

    NiRect<float> ConvertNSCToNDC(const NiRect<float>& kNSCRect);
    NiRect<unsigned int> ConvertNSCToPixels(const NiRect<float>& kNSCRect);
    NiPoint2 ConvertPixelDimensionsToNSCDimensions(unsigned int uiWidth, 
        unsigned int uiHeight);
    NiPoint2 ConvertNDCDimensionsToNSCDimensions(float fWidth, float fHeight);
    
protected:
    static NiUIManager* ms_pkUIManager;

    // Returns -1 if not in the list, otherwise, it's the index
    int MouseButtonReservedAt(NiInputMouse::Button eHotkey,
        NiInputMouse::Modifiers eModifier = NiInputMouse::MMOD_NONE);
    int GamePadButtonReservedAt(NiInputGamePad::Button eHotkey, 
        NiInputGamePad::ButtonMask eModifier = NiInputGamePad::NIGP_MASK_NONE);
    int GamePadAxisReservedAt(NiInputGamePad::DefaultAxis eAxis);
    int KeyboardButtonReservedAt(NiInputKeyboard::KeyCode eHotkey,
        NiInputKeyboard::Modifiers eModifier = NiInputKeyboard::KMOD_NONE);

    // These functions aid AddUIGroup(NiUIGroupPtr) to ensure that UI groups
    // are always stored in the correct order
    void OrderGroups();
    bool IsLower(NiPoint2& kP1, NiPoint2& kP2) const;
    // This move the "has focus" to the next available element
    void MoveDown(unsigned char ucValue);
    void MoveUp(unsigned char ucValue);
    void EnterPressed(unsigned char ucValue);
    void LeftMouseButton(unsigned char ucValue);
    NiUIMemberSlot1<NiUIManager, unsigned char> m_kMoveDown;
    NiUIMemberSlot1<NiUIManager, unsigned char> m_kMoveUp;
    NiUIMemberSlot1<NiUIManager, unsigned char> m_kEnterPressed;
    NiUIMemberSlot1<NiUIManager, unsigned char> m_kLeftMouseBtn;
    NiUISignal1<unsigned char> m_kLeftMouseSignal;
    int m_iCurUIGroup;

    class NiMouseHotkeySignal : public NiRefObject
    {
    public:
        NiInputMouse::Button m_eHotkey;
        NiInputMouse::Modifiers m_eModifier;
        NiUISignal1<unsigned char> m_kSignal;
        unsigned char m_ucState;
    };
    typedef NiPointer<NiMouseHotkeySignal> NiMouseHotkeySignalPtr;

    class NiKeyboardHotkeySignal : public NiRefObject
    {
    public:
        NiInputKeyboard::KeyCode m_eHotkey;
        NiInputKeyboard::Modifiers m_eModifier;
        NiUISignal1<unsigned char> m_kSignal;
        unsigned char m_ucState;
    };
    typedef NiPointer<NiKeyboardHotkeySignal> NiKeyboardHotkeySignalPtr;

    class NiGamePadHotkeySignal : public NiRefObject
    {
    public:
        NiInputGamePad::Button m_eHotkey;
        unsigned int m_uiModifier;
        NiInputGamePad::DefaultAxis m_eAxis;
        NiUISignal1<unsigned char> m_kSignal;
        unsigned char m_ucState;
        bool m_bIsHotkey;
    };
    typedef NiPointer<NiGamePadHotkeySignal> NiGamePadHotkeySignalPtr;

    NiUIAtlasMap m_kAtlasMap;
    NiFontPtr m_spFont;
    NiInputSystemPtr m_spInputSystem;
    NiCursorPtr m_spCursor;

    NiTObjectPtrSet <NiMouseHotkeySignalPtr> m_kMouseActionSet;
    NiTObjectPtrSet <NiGamePadHotkeySignalPtr> m_kGamePadActionSet;
    NiTObjectPtrSet <NiKeyboardHotkeySignalPtr> m_kKeyboardActionSet;

    NiTObjectArray <NiUIGroupPtr> m_kUIGroups;
    NiTObjectArray <NiUIRenderGroupPtr> m_kUIRenderGroups;

    static const unsigned int ms_auiResolutionLevels[2];

    NiRect<float> m_kSafeFrameInNDC;
    float m_fDisplayWidthInPixels;
    float m_fDisplayHeightInPixels;
    NiPoint2 m_kMaxCharDimensionsInNSC;

    bool m_bMouseExists;
    bool m_bKeyboardExists;
    bool m_bGamePadExists;
    bool m_bMouseReserved;
    bool m_bSearchWithMouse;
    bool m_bMouseOverButton;
    bool m_bEmitMouseSignal;
    bool m_bVisible;
};
NiSmartPointer(NiUIManager);
//---------------------------------------------------------------------------
#include "NiUIManager.inl"
//---------------------------------------------------------------------------

#endif
