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

#ifndef NIACTIONDATA_H
#define NIACTIONDATA_H

#include "NiInputLibType.h"
#include <NiMemObject.h>

class NIINPUT_ENTRY NiActionData : public NiMemObject
{
public:
    NiActionData();
    virtual ~NiActionData();

    //
    const unsigned int GetAppData() const;
    const unsigned int GetDeviceID() const;
    const void GetDeviceID(unsigned int& uiPort, unsigned int& uiSlot) const;
    const unsigned int GetDataValue() const;
    const void* GetContext() const;
    const unsigned int GetTimeStamp() const;
    const unsigned int GetSequence() const;

    // *** begin Emergent internal use only ***
    void SetAppData(unsigned int uiValue);
    void SetDeviceID(unsigned int uiValue);
    void SetDeviceID(unsigned int uiPort, unsigned int uiSlot);
    void SetDataValue(unsigned int uiValue);
    void SetContext(void* pvContext);
    void SetTimeStamp(unsigned int uiValue);
    void SetSequence(unsigned int uiValue);
    // *** end Emergent internal use only ***

protected:
    unsigned int m_uiAppData;   // App assigned data, can be a pointer
    unsigned int m_uiDeviceID;  // ID of the device that generated the action
                                // The id will be 0xSSSSPPPP (slot/port)
    unsigned int m_uiDataValue; // Data value for the device
    void* m_pvContext;          // Context from action
    unsigned int m_uiTimeStamp; // Time stamp for action generation
    unsigned int m_uiSequence;  // Sequence number for the action
};

#include "NiActionData.inl"

#endif  //#ifndef NIACTIONDATA_H
