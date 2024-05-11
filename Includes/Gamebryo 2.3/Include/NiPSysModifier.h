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

#ifndef NIPSYSMODIFIER_H
#define NIPSYSMODIFIER_H

#include "NiParticleLibType.h"
#include <NiObject.h>
#include <NiFixedString.h>

class NiParticleSystem;
class NiPSysData;

class NIPARTICLE_ENTRY NiPSysModifier : public NiObject
{
    NiDeclareRTTI;
    NiDeclareAbstractClone(NiPSysModifier);
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;

public:
    enum
    {
        ORDER_KILLOLDPARTICLES = 0,
        ORDER_EMITTER          = 1000,
        ORDER_SPAWN            = 2000,
        ORDER_GENERAL          = 3000,
        ORDER_FORCE            = 4000,
        ORDER_COLLIDER         = 5000,
        ORDER_POSUPDATE        = 6000,
        ORDER_POSTPOSUPDATE    = 6500,
        ORDER_BOUNDUPDATE      = 7000
    };

    // This function will call Update if m_bActive is true.
    void UpdateModifier(float fTime, NiPSysData* pkData);

    // Override these functions in derived classes, if desired. Update must
    // be overridden.
    virtual void Update(float fTime, NiPSysData* pkData) = 0;
    virtual void Initialize(NiPSysData* pkData, unsigned short usNewParticle);
    virtual void HandleReset();

    const NiFixedString& GetName() const;
    void SetName(const NiFixedString& kName);

    unsigned int GetOrder() const;

    NiParticleSystem* GetSystemPointer() const;

    bool GetActive() const;
    virtual void SetActive(bool bActive);

    // *** begin Emergent internal use only ***
    virtual void SetSystemPointer(NiParticleSystem* pkTarget);
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysModifier();

    NiPSysModifier(const NiFixedString& kName, unsigned int uiOrder);
    virtual ~NiPSysModifier();

    NiFixedString m_kName;
    unsigned int m_uiOrder;

    NiParticleSystem* m_pkTarget;

    bool m_bActive;

    static const float ms_fEpsilon;
};

NiSmartPointer(NiPSysModifier);

#include "NiPSysModifier.inl"

#endif  // #ifndef NIPSYSMODIFIER_H
