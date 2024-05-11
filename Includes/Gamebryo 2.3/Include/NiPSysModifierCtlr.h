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

#ifndef NIPSYSMODIFIERCTLR_H
#define NIPSYSMODIFIERCTLR_H

#include <NiSingleInterpController.h>
#include "NiParticleLibType.h"
#include "NiParticleSystem.h"

class NiPSysModifier;

class NIPARTICLE_ENTRY NiPSysModifierCtlr : public NiSingleInterpController
{
    NiDeclareRTTI;
    NiDeclareAbstractClone(NiPSysModifierCtlr);
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;

public:
    virtual ~NiPSysModifierCtlr();

    virtual void SetTarget(NiObjectNET* pkTarget);

    // This function is called when the particle system is reset.
    virtual void HandleReset();

    // Gets controller's copy of the modifier name (used to find modifier
    // pointer).
    const NiFixedString& GetModifierName() const;

    // Gets a pointer to the modifier object that this controller animates.
    // If the controller has no target, NULL is returned.
    NiPSysModifier* GetModifierPointer() const;

    // *** begin Emergent internal use only ***
    // Cloning support.
    virtual const char* GetCtlrID();
    virtual void ProcessClone(NiCloningProcess& kCloning);
    void PostLinkObject(NiStream& kStream);
    // *** end Emergent internal use only ***

protected:
    NiPSysModifierCtlr(const NiFixedString& kModifierName);

    // For cloning and streaming only.
    NiPSysModifierCtlr();

    virtual bool InterpTargetIsCorrectType(NiObjectNET* pkTarget) const;

    NiFixedString m_kModifierName;
    NiPSysModifier* m_pkModifier;

private:
    // Sets m_pkModifier to point to the correct modifier, using the stored
    // name.
    void GetModifierPointerFromName();

    // Sets the m_pcModifierName pointer, making its own copy of the string.
    void SetModifierName(const NiFixedString& kName);
};

NiSmartPointer(NiPSysModifierCtlr);

#include "NiPSysModifierCtlr.inl"

#endif  // #ifndef NIPSYSMODIFIERCTLR_H
