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


#ifndef NIPHYSXMATERIALDESC_H
#define NIPHYSXMATERIALDESC_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

class NIPHYSX_ENTRY NiPhysXMaterialDesc : public NiObject
{
    NiDeclareStream;
    NiDeclareRTTI;
    NiDeclareClone(NiPhysXMaterialDesc);
    
public:
    NiPhysXMaterialDesc(const unsigned short usIndex = 0);
    virtual ~NiPhysXMaterialDesc();

    virtual void ToMaterialDesc(NxMaterialDesc& kMaterialDesc,
        const unsigned int uiIndex = 0) const;
    virtual void ToMaterial(NxMaterial* pkMaterial,
        const unsigned int uiIndex = 0) const;
    virtual void FromMaterialDesc(const NxMaterialDesc& kMaterialDesc,
        const unsigned int uiIndex = 0);
    virtual void FromMaterial(NxMaterial* pkMaterial,
        const unsigned int uiIndex = 0);
    
    // Get and set the material index associated with this material.
    unsigned short GetIndex() const;
    void SetIndex(const unsigned short);

    // Returns true if the material data is equal. Equivalent to an
    // operator==() on NxMaterialDesc.
    bool IsDataEqual(const NxMaterialDesc& kMaterialDesc,
        const unsigned int uiIndex = 0);
        
    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();
    
    // *** end Emergent internal use only ***

protected:
    unsigned short m_usIndex;
    unsigned int m_uiNumStates;
    NxMaterialDesc* m_pkMaterialDescs;
};

NiSmartPointer(NiPhysXMaterialDesc);

#include "NiPhysXMaterialDesc.inl"

#endif // #ifdef NIPHYSXMATERIALDESC_H

