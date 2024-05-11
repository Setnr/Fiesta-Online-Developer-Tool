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

#ifndef NIPLANE_H
#define NIPLANE_H

#include "NiPoint3.h"
#include "NiTArray.h"
#include "NiStream.h"

class NIMAIN_ENTRY NiPlane : public NiMemObject
{
public:
    enum {NO_SIDE, POSITIVE_SIDE, NEGATIVE_SIDE};

    NiPlane();
    NiPlane(const NiPoint3& kNormal, float fConstant);
    NiPlane(const NiPoint3& kNormal, const NiPoint3& kPoint);
    NiPlane(const NiPoint3& kP0, const NiPoint3& kP1, const NiPoint3& kP2);

    bool operator==(const NiPlane& kPlane);
    bool operator!=(const NiPlane& kPlane);

    void SetNormal(const NiPoint3& kNormal);
    const NiPoint3& GetNormal() const;
    void SetConstant(float fConstant);
    float GetConstant() const;

    float Distance(const NiPoint3& kPoint) const;
    int WhichSide(const NiPoint3& kPoint) const;

    // *** begin Emergent internal use only ***

    // streaming
    void LoadBinary(NiStream& kStream);
    void SaveBinary(NiStream& kStream);
    char* GetViewerString(const char* pcPrefix) const;

    // kPlane is m_kNormal*(x,y,z) = m_fConstant
    NiPoint3 m_kNormal;
    float m_fConstant;

    // *** end Emergent internal use only ***
} NI_DATA_ALIGNMENT(16);

typedef NiTPrimitiveArray<NiPlane*> NiPlaneArray;

//---------------------------------------------------------------------------
//  Inline include
#include "NiPlane.inl"

//---------------------------------------------------------------------------

#endif

