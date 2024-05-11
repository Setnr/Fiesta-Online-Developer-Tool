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

#ifndef NIPARAMSKF_H
#define NIPARAMSKF_H

#include "NiEntityLibType.h"
#include "NiExternalAssetParams.h"
#include <NiControllerSequence.h>
#include <NiRTTI.h>
#include <NiSystem.h>

class NIENTITY_ENTRY NiParamsKF : public NiExternalAssetParams
{
    NiDeclareRTTI;

public:
    NiParamsKF();
    virtual ~NiParamsKF();

    void SetAnimIndex(unsigned int uiAnimIndex);
    unsigned int GetAnimIndex();
    NiBool GetControllerSequence(NiControllerSequence*& pkCtrlSeq);   
   
    // *** begin Emergent internal use only ***
    NiBool SetControllerSequence(NiControllerSequence* pkCtrlSeq);
    // *** end Emergent internal use only ***

private:   
    // m_uiAnimIndex
    unsigned int m_uiAnimIndex;
    
    // Retrieved Sequence controller pointer
    NiControllerSequencePtr m_spCtrlSeq;
};

#endif // NIPARAMSKF_H
