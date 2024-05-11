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

#ifndef NICOLOR_H
#define NICOLOR_H

#include "NiMainLibType.h"
#include "NiStream.h"

class NiColorA;

class NIMAIN_ENTRY NiColor : public NiMemObject
{
public:
    float r, g, b;

    NiColor (float fR = 0.0f, float fG = 0.0f, float fB = 0.0f);

    NiColor& operator= (float fScalar);
    bool operator== (const NiColor& c) const;
    bool operator!= (const NiColor& c) const { return !(*this == c); }

    NiColor operator+ (const NiColor& c) const;
    NiColor operator- (const NiColor& c) const;
    NiColor operator* (float fScalar) const;
    NiColor operator* (const NiColor& c) const;
    NiColor operator/ (float fScalar) const;
    NiColor operator/ (const NiColor& c) const;
    NiColor operator- () const;
    NIMAIN_ENTRY friend NiColor operator* (float fScalar, const NiColor& c);

    NiColor& operator+= (const NiColor& c);
    NiColor& operator-= (const NiColor& c);
    NiColor& operator*= (float fScalar);
    NiColor& operator*= (const NiColor& c);
    NiColor& operator/= (float fScalar);
    NiColor& operator/= (const NiColor& c);

    // map to unit cube
    void Clamp ();
    void Scale ();

    static const NiColor WHITE; // (1,1,1) 
    static const NiColor BLACK; // (0,0,0) 

    // *** begin Emergent internal use only ***

    // streaming
    void LoadBinary (NiStream& stream);
    void SaveBinary (NiStream& stream);

    // Utility methods for streaming arrays of NiColor's.
    // Note that the array's must be pre-allocated for Loading, 
    // meaning that the external caller is responsible for
    // streaming the size.
    static void LoadBinary (NiStream& stream, NiColor* pkValues, 
        unsigned int uiNumValues);
    static void SaveBinary (NiStream& stream, NiColor* pkValues, 
        unsigned int uiNumValues);

    char* GetViewerString (const char* pPrefix) const;

    // *** end Emergent internal use only ***
};

class NIMAIN_ENTRY NiColorA : public NiMemObject
{
public:
    float r, g, b, a;

    NiColorA (float fR = 0.0f, float fG = 0.0f, 
              float fB = 0.0f, float fA = 0.0f);

    NiColorA& operator= (float fScalar);
    bool operator== (const NiColorA& c) const;
    bool operator!= (const NiColorA& c) const { return !(*this == c); }

    NiColorA operator+ (const NiColorA& c) const;
    NiColorA operator- (const NiColorA& c) const;
    NiColorA operator* (float fScalar) const;
    NiColorA operator* (const NiColorA& c) const;
    NiColorA operator/ (float fScalar) const;
    NiColorA operator/ (const NiColorA& c) const;
    NiColorA operator- () const;
    NIMAIN_ENTRY friend NiColorA operator* (float fScalar, const NiColorA& c);

    NiColorA& operator+= (const NiColorA& c);
    NiColorA& operator-= (const NiColorA& c);
    NiColorA& operator*= (float fScalar);
    NiColorA& operator*= (const NiColorA& c);
    NiColorA& operator/= (float fScalar);
    NiColorA& operator/= (const NiColorA& c);

    // map (r,g,b,a) to unit hypercube
    void Clamp ();
    void Scale ();

    static const NiColorA WHITE;  // (1,1,1,1) 
    static const NiColorA BLACK;  // (0,0,0,1) 

    // *** begin Emergent internal use only ***

    // streaming
    void LoadBinary (NiStream& stream);
    void SaveBinary (NiStream& stream);

    // Utility methods for streaming arrays of NiColorA's.
    // Note that the array's must be pre-allocated for Loading, 
    // meaning that the external caller is responsible for
    // streaming the size.
    static void LoadBinary (NiStream& stream, NiColorA* pkValues, 
        unsigned int uiNumValues);
    static void SaveBinary (NiStream& stream, NiColorA* pkValues, 
        unsigned int uiNumValues);

    char* GetViewerString (const char* pPrefix) const;

    // *** end Emergent internal use only ***
} NI_DATA_ALIGNMENT(16);

//---------------------------------------------------------------------------
//  Inline include
#include "NiColor.inl"

//---------------------------------------------------------------------------

#endif
