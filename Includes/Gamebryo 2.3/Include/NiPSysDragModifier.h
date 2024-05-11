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

#ifndef NIPSYSDRAGMODIFIER_H
#define NIPSYSDRAGMODIFIER_H

#include "NiPSysModifier.h"
#include "NiPoint3.h"

class NiAVObject;

class NIPARTICLE_ENTRY NiPSysDragModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysDragModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysDragModifier(const char* pcName, NiAVObject* pkDragObj, 
        NiPoint3 kDragAxis = NiPoint3::UNIT_X, float fPercentage = 0.05f,
        float fRange = NI_INFINITY, float fFalloff = NI_INFINITY);

    virtual void Update(float fTime, NiPSysData* pkData);

    NiAVObject* GetDragObj() const;
    void SetDragObj(NiAVObject* pkDragObj);

    const NiPoint3& GetDragAxis() const;
    void SetDragAxis(const NiPoint3& kDragAxis);

    float GetPercentage() const;
    void SetPercentage(float fPercentage);
   
    float GetRange() const;
    void SetRange(float fDistance);

    float GetRangeFalloff() const;
    void SetRangeFalloff(float fDistance);

    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysDragModifier();

    NiAVObject* m_pkDragObj;
    NiPoint3 m_kDragAxis;

    float m_fPercentage;  
    float m_fRange;
    float m_fFalloff;
};

NiSmartPointer(NiPSysDragModifier);

#include "NiPSysDragModifier.inl"

#endif  // #ifndef NIPSYSDRAGMODIFIER_H
