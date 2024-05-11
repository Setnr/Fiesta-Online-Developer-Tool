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

#ifndef NIPSYSBOMBMODIFIER_H
#define NIPSYSBOMBMODIFIER_H

#include "NiPSysModifier.h"
#include <NiPoint3.h>

class NIPARTICLE_ENTRY NiPSysBombModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysBombModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    enum DecayType
    {
        NONE,
        LINEAR,
        EXPONENTIAL
    };

    enum SymmType
    {
        SPHERICAL,
        CYLINDRICAL,
        PLANAR
    };

    NiPSysBombModifier(const char* pcName, NiAVObject* pkBombObj = NULL,
        NiPoint3 kBombAxis = NiPoint3::UNIT_X, float fDecay = 0.0f,
        float fDeltaV = 0.0f, DecayType eDecayType = NONE,
        SymmType eSymmType = SPHERICAL);

    virtual void Update(float fTime, NiPSysData* pkData);

    NiAVObject* GetBombObj() const;
    void SetBombObj(NiAVObject* pkBombObj);

    const NiPoint3& GetBombAxis() const;
    void SetBombAxis(const NiPoint3& kBombAxis);

    float GetDecay() const;
    void SetDecay(float fDecay);

    float GetDeltaV() const;
    void SetDeltaV(float fDeltaV);

    DecayType GetDecayType() const;
    void SetDecayType(DecayType eDecayType);

    SymmType GetSymmType() const;
    void SetSymmType(SymmType eSymmType);

    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***
    
protected:
    // For cloning and streaming only.
    NiPSysBombModifier();

    NiAVObject* m_pkBombObj;
    NiPoint3 m_kBombAxis;
    float m_fDecay;
    float m_fDeltaV;  // specified in units/second
    DecayType m_eDecayType;
    SymmType m_eSymmType;
};

NiSmartPointer(NiPSysBombModifier);

#include "NiPSysBombModifier.inl"

#endif  // #ifndef NIPSYSBOMBMODIFIER_H
