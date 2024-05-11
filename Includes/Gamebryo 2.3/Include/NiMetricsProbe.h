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

#ifndef NIMETRICSPROBE_H
#define NIMETRICSPROBE_H

// Do not include this file directly in your project unless you have
// C++ exceptions enabled.  Include NiWebMetricsOutput.h instead.
// NiWebMetricsOutput wraps (and handles) all of the exceptions.

#include "NiWebMetricsOutputLibType.h"
#include <FileMetricsLog.h>
#include <MetricsProbe.h>

typedef cEmeMetricsProbeTemplate<cEmeFileMetricsLog, ClassLevelLockable> 
    tMetricsProbe;

class NiMetricsProbe : public tMetricsProbe, public NiMemObject {};

#endif //#ifndef NIMETRICSPROBE_H
