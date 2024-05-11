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

#ifndef NIUNIQUEID_H
#define NIUNIQUEID_H

#define NUM_BYTES_IN_UNIQUE_ID 16

class NIENTITY_ENTRY NiUniqueID 
{
public:
    NiUniqueID();
    NiUniqueID(const unsigned char* pucID);
    NiUniqueID(unsigned char ucA, unsigned char ucB,
        unsigned char ucC, unsigned char ucD, unsigned char ucE,
        unsigned char ucF, unsigned char ucG, unsigned char ucH,
        unsigned char ucI, unsigned char ucJ, unsigned char ucK,
        unsigned char ucL, unsigned char ucM, unsigned char ucN,
        unsigned char ucO, unsigned char ucP);
    bool operator==(const NiUniqueID& kUniqueID) const;
    bool operator!=(const NiUniqueID& kUniqueID) const;

    unsigned char m_aucValue[NUM_BYTES_IN_UNIQUE_ID];
};

#include "NiUniqueID.inl"

#endif // NIUNIQUEID_H
