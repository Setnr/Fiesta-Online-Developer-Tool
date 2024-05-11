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

#ifndef NICOMPOSITEVALIDATOR_H
#define NICOMPOSITEVALIDATOR_H

#include "NiRenderClickValidator.h"
#include "NiTPointerList.h"

class NIMAIN_ENTRY NiCompositeValidator : public NiRenderClickValidator
{
    NiDeclareRTTI;

public:
    // Functions for accessing the list of validators.
    void AppendValidator(NiRenderClickValidator* pkValidator);
    void PrependValidator(NiRenderClickValidator* pkValidator);
    void RemoveValidator(NiRenderClickValidator* pkValidator);
    void RemoveAllValidators();
    NiTPointerList<NiRenderClickValidatorPtr>& GetValidators();
    const NiTPointerList<NiRenderClickValidatorPtr>& GetValidators() const;

protected:
    // A list of validators to execute in the ValidateClick function.
    NiTPointerList<NiRenderClickValidatorPtr> m_kValidators;
};

NiSmartPointer(NiCompositeValidator);

#include "NiCompositeValidator.inl"

#endif  // #ifndef NICOMPOSITEVALIDATOR_H
