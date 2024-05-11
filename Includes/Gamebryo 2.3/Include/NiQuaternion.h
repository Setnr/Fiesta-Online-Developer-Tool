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

#ifndef NIQUATERNION_H
#define NIQUATERNION_H

#include "NiMainLibType.h"
#include "NiStream.h"
#include "NiMatrix3.h"

class NiPoint3;

class NIMAIN_ENTRY NiQuaternion : public NiMemObject
{
public:
    NiQuaternion () {   };
    NiQuaternion (float w, float x, float y, float z);
    NiQuaternion (float fAngle, const NiPoint3& axis);
    
    void SetW (float w);
    void SetX (float x);
    void SetY (float y);
    void SetZ (float z);
    float GetW () const;
    float GetX () const;
    float GetY () const;
    float GetZ () const;

    void SetValues(float w, float x, float y, float z);
    void GetValues(float& w, float& x, float& y, float& z) const;
    
    NiQuaternion operator+ (const NiQuaternion& q) const;
    NiQuaternion operator- (const NiQuaternion& q) const;
    NiQuaternion operator- () const;
    NiQuaternion operator* (const NiQuaternion& q) const;
    NiQuaternion operator* (float c) const;
    NIMAIN_ENTRY friend NiQuaternion operator* (float c,
        const NiQuaternion& q);
    
    bool operator== (const NiQuaternion& q) const;
    bool operator!= (const NiQuaternion& q) const;
    
    static float Dot (const NiQuaternion& p, const NiQuaternion& q);
    static NiQuaternion UnitInverse (const NiQuaternion& q);
    static NiQuaternion Exp (const NiQuaternion& q);
    static NiQuaternion Log (const NiQuaternion& q);

    static NiQuaternion Slerp (float t, const NiQuaternion& p,
        const NiQuaternion& q);

    static NiQuaternion Intermediate (const NiQuaternion& q0,
        const NiQuaternion& q1, const NiQuaternion& q2);

    static NiQuaternion Squad (float t, const NiQuaternion& p,
        const NiQuaternion& a, const NiQuaternion& b,
        const NiQuaternion& q);

    // convert between quaternion and angle-axis representation
    void ToAngleAxis (float& angle, NiPoint3& axis) const;
    void FromAngleAxis (float angle, const NiPoint3& axis);

    // Fast-path angle axis functions for basis vectors.
    void FromAngleAxisX(float fAngle);
    void FromAngleAxisY(float fAngle);
    void FromAngleAxisZ(float fAngle);

    // convert between quaternion and rotation matrix
    void ToRotation (NiMatrix3& rot) const;
    void FromRotation (const NiMatrix3& rot);

    // quaternion corresponding to the identity matrix
    static const NiQuaternion IDENTITY;

    // *** begin Emergent internal use only ***

    // cutoff for sin(angle) near zero
    static const float ms_fEpsilon;
    
    // Snap all denormalized floats to zero
    void Snap();

    // streaming
    void LoadBinary (NiStream& stream);
    void SaveBinary (NiStream& stream);
    
    // Utility methods for streaming arrays of NiPoint3's.
    // Note that the array's must be pre-allocated for Loading, 
    // meaning that the external caller is responsible for
    // streaming the size.
    static void LoadBinary (NiStream& stream, NiQuaternion* pkValues, 
        unsigned int uiNumValues);
    static void SaveBinary (NiStream& stream, NiQuaternion* pkValues, 
        unsigned int uiNumValues);

    char* GetViewerString (const char* pPrefix) const;

    // used by slerp and nlerp
    void Normalize();
    void FastNormalize();
    // *** end Emergent internal use only ***
    
protected:
    // used by Slerp
    
    static float ISqrt_approx_in_neighborhood(float s);
    static float Lerp(float v0, float v1, float fPerc);
    static float CounterWarp(float t, float fCos);

public:
    float m_fW;
    float m_fX;
    float m_fY;
    float m_fZ;
} NI_DATA_ALIGNMENT(16);

//---------------------------------------------------------------------------
//  Inline include
#include "NiQuaternion.inl"

//---------------------------------------------------------------------------

#endif


