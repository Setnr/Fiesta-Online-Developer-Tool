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

#ifndef NIPOINT2_H
#define NIPOINT2_H

#include "NiMath.h"
#include "NiStream.h"

class NIMAIN_ENTRY NiPoint2 : public NiMemObject
{
public:
    float x, y;

    NiPoint2 () { /**/ }
    NiPoint2 (float fX, float fY);

    float& operator[] (int i)
    {
        // warning:  safe for 4-byte aligned class member data
        const float* base = &x;
        return (float&) base[i];
    }

    const float& operator[] (int i) const
    {
        // warning:  safe for 4-byte aligned class member data
        const float* base = &x;
        return (float&) base[i];
    }

    bool operator== (const NiPoint2& pt) const;
    bool operator!= (const NiPoint2& pt) const;

    NiPoint2 operator+ (const NiPoint2& pt) const;
    NiPoint2 operator- (const NiPoint2& pt) const;
    float operator* (const NiPoint2& pt) const;
    NiPoint2 operator* (float fScalar) const;
    NiPoint2 operator/ (float fScalar) const;
    NiPoint2 operator- () const;
    NIMAIN_ENTRY friend NiPoint2 operator* (float fScalar,
        const NiPoint2& pt);
    static NiPoint2 ComponentProduct (const NiPoint2& p0, const NiPoint2& p1);

    NiPoint2& operator+= (const NiPoint2& pt);
    NiPoint2& operator-= (const NiPoint2& pt);
    NiPoint2& operator*= (float fScalar);
    NiPoint2& operator/= (float fScalar);

    float Length () const;
    float SqrLength () const;
    float Dot (const NiPoint2& pt) const;
    float Unitize ();

    static const NiPoint2 ZERO;
    static const NiPoint2 UNIT_X;
    static const NiPoint2 UNIT_Y;

    // *** begin Emergent internal use only ***

    // streaming
    void LoadBinary (NiStream& stream);
    void SaveBinary (NiStream& stream);

    // Utility methods for streaming arrays of NiPoint2's.
    // Note that the array's must be pre-allocated for Loading, 
    // meaning that the external caller is responsible for
    // streaming the size.
    static void LoadBinary (NiStream& stream, NiPoint2* pkValues, 
        unsigned int uiNumValues);
    static void SaveBinary (NiStream& stream, NiPoint2* pkValues, 
        unsigned int uiNumValues);
    char* GetViewerString (const char* pPrefix) const;

    // *** end Emergent internal use only ***
};

//---------------------------------------------------------------------------
#include "NiRTLib.h"

//---------------------------------------------------------------------------
//  Inline include
#include "NiPoint2.inl"

//---------------------------------------------------------------------------

#endif

