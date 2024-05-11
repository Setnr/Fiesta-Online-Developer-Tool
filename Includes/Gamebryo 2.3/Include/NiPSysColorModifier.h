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

#ifndef NIPSYSCOLORMODIFIER_H
#define NIPSYSCOLORMODIFIER_H

#include "NiPSysModifier.h"
#include <NiColorData.h>

class NIPARTICLE_ENTRY NiPSysColorModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysColorModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysColorModifier(const char* pcName, NiColorData* pkColorData = NULL);

    virtual void Update(float fTime, NiPSysData* pkData);

    NiColorData* GetColorData() const;
    void SetColorData(NiColorData* pkColorData);

    // *** begin Emergent internal use only ***
    void ResetKeyTimeExtrema(); // Called automatically by SetColorData.
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysColorModifier();

    NiColorDataPtr m_spColorData;

    float m_fLoKeyTime;
    float m_fHiKeyTime;
};

NiSmartPointer(NiPSysColorModifier);

#include "NiPSysColorModifier.inl"

#endif  // #ifndef NIPSYSCOLORMODIFIER_H
