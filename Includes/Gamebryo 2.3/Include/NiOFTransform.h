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

#ifndef NIOFTRANSFORM_H
#define NIOFTRANSFORM_H

#include <NiMatrix3.h>
#include <NiPoint3.h>

#include <mgapistd.h>

#include "NiOpenFlightLibType.h"

class NIOPENFLIGHT_ENTRY NiOFTransform
{
public:
    NiOFTransform (mgrec* pkXform);

    NiPoint3& GetTranslate () { return m_kTrn; }
    NiMatrix3& GetMatrix () { return m_kMat; }
    bool DeferApply () { return m_bDeferApply; }

protected:
    // these produce only translation/rotation
    void ConvertXllTranslate (mgrec* pkXform);
    void ConvertXllRotEdge (mgrec* pkXform);
    void ConvertXllRotPt (mgrec* pkXform);
    void ConvertXllPut (mgrec* pkXform);

    NiPoint3 m_kTrn;
    NiMatrix3 m_kMat;
    bool m_bDeferApply;
};

#endif
