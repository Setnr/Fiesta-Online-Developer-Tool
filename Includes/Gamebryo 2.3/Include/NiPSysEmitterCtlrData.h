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

#ifndef NIPSYSEMITTERCTLRDATA_H
#define NIPSYSEMITTERCTLRDATA_H

#include "NiParticleLibType.h"
#include <NiObject.h>
#include <NiFloatKey.h>
#include <NiBoolKey.h>

// This class has been deprecated as of Gamebryo 1.2.
// NiPSysEmitterCtlr now holds pointers to an NiFloatInterpolator and an
// NiBoolInterpolator that store the same values that this class did.
// This class only exists to support streaming of old NIF assets.

class NIPARTICLE_ENTRY NiPSysEmitterCtlrData : public NiObject
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    virtual ~NiPSysEmitterCtlrData();

    unsigned int GetNumBirthRateKeys() const;
    NiFloatKey* GetBirthRateKeys(unsigned int& uiNumKeys,
        NiFloatKey::KeyType& eType, unsigned char& ucSize) const;
    void ReplaceBirthRateKeys(NiFloatKey* pkKeys, unsigned int uiNumKeys,
        NiFloatKey::KeyType eType);

    unsigned int GetNumEmitterActiveKeys() const;
    NiBoolKey* GetEmitterActiveKeys(unsigned int& uiNumKeys,
        NiBoolKey::KeyType& eType, unsigned char& ucSize);
    void ReplaceEmitterActiveKeys(NiBoolKey* pkKeys, unsigned int uiNumKeys,
        NiBoolKey::KeyType eType);

    // *** begin Emergent internal use only ***
    void SetBirthRateKeys(NiFloatKey* pkKeys, unsigned int uiNumKeys,
        NiFloatKey::KeyType eType);
    void SetEmitterActiveKeys(NiBoolKey* pkKeys, unsigned int uiNumKeys,
        NiBoolKey::KeyType eType);

    void GuaranteeKeysAtStartAndEnd(float fStartTime, float fEndTime);
    // *** end Emergent internal use only ***

protected:
    NiPSysEmitterCtlrData();

    unsigned int m_uiNumBirthRateKeys;
    NiFloatKey* m_pkBirthRateKeys;
    NiFloatKey::KeyType m_eBirthRateKeyType;
    unsigned char m_ucBirthRateSize;

    unsigned int m_uiNumEmitterActiveKeys;
    NiBoolKey* m_pkEmitterActiveKeys;
    NiBoolKey::KeyType m_eEmitterActiveKeyType;
    unsigned char m_ucEmitterActiveSize;
};

NiSmartPointer(NiPSysEmitterCtlrData);

#include "NiPSysEmitterCtlrData.inl"

#endif  // #ifndef NIPSYSEMITTERCTLRDATA_H
