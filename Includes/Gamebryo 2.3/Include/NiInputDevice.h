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

#ifndef NIINPUTDEVICE_H
#define NIINPUTDEVICE_H

#if defined(_XENON)
    #include "NiXenonInputHeaders.h"
#endif  //#if defined(_XENON)
#include "NiInputLibType.h"
#include "NiInputErr.h"

#include <NiRefObject.h>
#include <NiRTTI.h>
#include <NiSmartPointer.h>
#include <NiTPointerList.h>
#include <NiSystem.h>

class NiInputSystem;
class NiAction;

class NIINPUT_ENTRY NiInputDevice : public NiRefObject
{
    NiDeclareRootRTTI(NiInputDevice);

public:
    // Type - the type of input device. Categorizes the devices.
    enum Type
    {
        NIID_NULL = 0,
        NIID_KEYBOARD,
        NIID_MOUSE,
        NIID_GAMEPAD
    };

    // Status - the current status of the device.
    enum Status
    {
        UNKNOWN = -1,
        READY,
        BUSY,
        REMOVED,
        LOST
    };

    // ControlDesc - describes a single 'control' on a device, such as a 
    // button or an axis on a gamepad stick.
    class NIINPUT_ENTRY ControlDesc : public NiMemObject
    {
    public:
        enum Type
        {
            UNKNOWN = -1,  // Unknown control type
            BUTTON = 0,    // Button - analog or digital
            AXIS,          // Axis - single axis of a stick or a slider
            POV,           // PointOfView hat or similar control
            MOTOR          // Rumble motor (output only)
        };

        enum Flags
        {
            DATA_DIGITAL    = 0x00000001,
            DATA_ANALOG     = 0x00000002,
            DATA_MASK       = 0x0000000f,
            TYPE_INPUT      = 0x00000010,
            TYPE_OUTPUT     = 0x00000020,
            TYPE_HYBRID     = 0x00000030,
            TYPE_MASK       = 0x000000f0,
            OUT_RUMBLE      = 0x00000100,
            OUT_FORCEFB     = 0x00000200,
            OUT_MASK        = 0x00000f00
        };

        ControlDesc();
        ControlDesc(unsigned int uiIndex, const char* pacName, Type eType, 
            unsigned int uiFlags);
        ~ControlDesc();

        const unsigned int GetIndex() const;
        const char* GetName() const;
        const Type GetType() const;
        const unsigned int GetFlags() const;

        const bool IsDigital() const;
        const bool IsAnalog() const;
        const bool IsInput() const;
        const bool IsOutput() const;
        const bool IsHybrid() const;
        const bool IsRumble() const;
        const bool IsForceFeedback() const;

        // *** begin Emergent internal use only ***
        void SetIndex(unsigned int uiIndex);
        void SetName(const char* pacName);
        void SetType(Type eType);
        void SetFlags(unsigned int uiFlags);

        void SetDigital(bool bEnabled);
        void SetAnalog(bool bEnabled);
        void SetInput(bool bEnabled);
        void SetOutput(bool bEnabled);
        void SetHybrid(bool bEnabled);
        void SetRumble(bool bEnabled);
        void SetForceFeedback(bool bEnabled);

        const unsigned int GetPlatformSpecificData() const;
        void SetPlatformSpecificData(unsigned int uiPlatformSpecificData);
        // *** end Emergent internal use only ***

    protected:
        unsigned int m_uiIndex;
        char* m_pacName;
        Type m_eType;
        unsigned int m_uiFlags;
        unsigned int m_uiPlatformSpecificData;
    };

    typedef NiTPointerList<ControlDesc*> NiIDControlList;

    // Description - describes the input device for enumeration methods.
    // The application can check the description of the device to determine
    // what it's capabilities are.
    class NIINPUT_ENTRY Description : public NiRefObject
    {
    public:
        Description();
        Description(Type eType, unsigned int uiPort, unsigned int uiSlot,
            const char* pacName);
        ~Description();

        const Type GetType() const;
        const unsigned int GetPort() const;
        const unsigned int GetSlot() const;
        const char* GetName() const;
        const char* GetDescriptionString() const;
        const void* GetDeviceSpecificData() const;

        // *** begin Emergent internal use only ***
        void SetType(Type eType);
        void SetPort(unsigned int uiPort);
        void SetSlot(unsigned int uiSlot);
        void SetName(const char* pacName);
        void SetDescriptionString(const char* pacDesc);
        void SetDeviceSpecificData(void* pvData);
        const bool GetDeviceOwned() const;
        void SetDeviceOwned(bool bOwned);

        unsigned int AddControlDesc(ControlDesc* pkDesc);
        // Used by the enumeration function for keyboards.
        void IncrementButtonCount();
        // *** end Emergent internal use only ***

        unsigned int GetControlCount();
        ControlDesc* GetFirstControlDesc();
        ControlDesc* GetNextControlDesc();
        ControlDesc* GetControlDescAtIndex(unsigned int uiIndex);
        ControlDesc* GetControlDescByName(const char* pacName);

        const unsigned int GetButtonCount() const;
        const unsigned int GetAxisCount() const;
        const unsigned int GetPOVCount() const;

    protected:
        Type m_eType;
        unsigned int m_uiPort;
        unsigned int m_uiSlot;
        char* m_pacName;
        char* m_pacDesc;
        bool m_bDeviceOwned;

        NiIDControlList m_kControls;
        NiTListIterator m_kControlsIter;

        unsigned int m_uiButtonCount;
        unsigned int m_uiAxisCount;
        unsigned int m_uiPOVCount;
    };

    typedef NiPointer<Description> DescriptionPtr;
    typedef NiTPointerList<DescriptionPtr> DeviceDescList;

protected:
    NiInputDevice(Description* pkDescription);

public:
    virtual ~NiInputDevice();

    // member access
    const Type GetType() const;
    const Status GetStatus() const;
    const unsigned int GetPort() const;
    const unsigned int GetSlot() const;

    const unsigned int GetModifiers() const;

    //
    virtual NiInputErr UpdateDevice();
    virtual NiInputErr UpdateActionMappedDevice(
        NiInputSystem* pkInputSystem);
    virtual NiInputErr HandleRemoval();
    virtual NiInputErr HandleInsertion();

    //
    NiInputErr GetLastError();

    // *** begin Emergent internal use only ***
    void SetStatus(Status eStatus);
    void SetPort(unsigned int uiPort);
    void SetSlot(unsigned int uiSlot);

    const bool GetActionMapped() const;
    void SetActionMapped(bool bMapped);

    virtual void TouchAllInputValues();

    void AddModifiers(unsigned int uiMod);
    void RemoveModifiers(unsigned int uiMod);
    void ClearModifiers();
    bool ModifersAreDown(unsigned int uiModifiers) const;
    bool ModifiersAreUp(unsigned int uiModifiers) const;

    virtual bool IsControlMapped(unsigned int uiControl, 
        unsigned int uiSemantic, bool bUseModifiers, 
        unsigned int uiModifiers, bool bUseRange = false,
        int iRangeLow = 0, int iRangeHigh = 0);
    virtual NiInputErr AddMappedAction(NiAction* pkAction);
    NiInputErr RemoveMappedAction(NiAction* pkAction);
    NiInputErr ClearMappedActions();

    void SetLastError(NiInputErr eErr);
    // *** end Emergent internal use only ***

    // Button and Axes are defined here, as they are used in both 
    // NiInputGamePad and NiInputMouse.
    //
    // We are going to assume EVERY button as analog.
    // If a button is digital, then the platform-specific code dealing
    // with it should set it's value to 255 when it is pressed.
    // W.r.t. modifiers, if a button is >= 128 (half-pressed) it will
    // be considered pressed from a modifier standpoint.
    // Since we need to have the state as well as the modifiers that were 
    // active when it was pressed, we will use a small class per button.
    class NIINPUT_ENTRY Button : public NiMemObject
    {
    public:
        Button();

        //
        const unsigned int GetModifiers() const;
        bool RemoveModifiers(unsigned int uiModifiers);
        const bool IsPressed(unsigned int& uiModifiers) const;
        const bool WasPressed(unsigned int& uiModifiers) const;
        const bool WasReleased(unsigned int& uiModifiers) const;

        const unsigned char GetValue() const;
        const unsigned char GetValue(unsigned int& uiModifiers) const;
        const unsigned char GetLastValue() const;
        const unsigned char GetLastValue(unsigned int& uiModifiers) const;
        const bool ValueChanged() const;
        const bool ValueChanged(bool& bModifiersChanged) const;

        void SetButtonPress(unsigned int uiModifiers, bool bPressed = true,
            unsigned char ucValue = 0xff);
        void SetButtonRelease();
    
        void UpdatePressedLastUpdate();

        const bool GetLastPressed() const;
        const unsigned int GetLastModifiers() const;

        // *** begin Emergent internal use only ***
        void TouchValue();
        // *** end Emergent internal use only ***

    protected:
        unsigned char m_ucValue;        // Analog
        bool m_bPressed;                // Digital (>= 128 on analog ones)

        unsigned char m_ucLastValue;    // Last Analog
        bool m_bLastPressed;            // Last Digital (>= 128 on analog)

        bool m_bPressedLastUpdate;

        unsigned int m_uiModifiers;     // Mod present when pressed
        unsigned int m_uiLastModifiers; // Mod present when last pressed
    };

    // Axis
    class NIINPUT_ENTRY Axis : public NiMemObject
    {
    public:
        Axis();
        Axis(int iLow, int iHigh);
        ~Axis();
        
        //
        void GetRange(int& iLow, int& iHigh) const;
        const int GetRawValue() const;
        const int GetValue() const;

        const int GetLastRawValue() const;
        const int GetLastValue() const;

        // *** begin Emergent internal use only ***
        void SetRawRange(int iLow, int iHigh);
        void SetRange(int iLow, int iHigh);

        void SetRawValue(int iValue, bool bInvert, float fDeadZone);
        void SetValue(int iValue);

        int MapRawValue(int iValue);

        void TouchValue();
        // *** end Emergent internal use only ***

    protected:
        int m_iRawValue;
        int m_iValue;

        int m_iLastRawValue;
        int m_iLastValue;

        int m_iRawLow;      // Used to specify the range
        int m_iRawHigh;
        int m_iLow;         // Used to specify the range
        int m_iHigh;
    };

protected:
    bool CheckActionDeviceIDAndModifiers(NiAction* pkAction,
        unsigned int uiModifiers);

    DescriptionPtr m_spDescription;

    Status m_eStatus;

    // Modifiers - all devices use them to some degree...
    unsigned int m_uiModifiers;

    bool m_bActionMapped;
    NiTPointerList<NiAction*>* m_pkMappedActions;

    NiInputErr m_eLastError;
};

NiSmartPointer(NiInputDevice);

#include "NiInputDevice.inl"

#endif  //#ifndef NIINPUTDEVICE_H
