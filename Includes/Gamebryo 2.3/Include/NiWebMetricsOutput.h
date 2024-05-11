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

#ifndef NIWEBMETRICSOUTPUT_H
#define NIWEBMETRICSOUTPUT_H

#include "NiWebMetricsOutputLibType.h"
#include <NiMetricsOutputModule.h>

// Forward declare so that Metrics headers don't have to be included directly
// by projects that use NiWebMetrics.
class cIEmeMetricsProbeListener;
class NiMetricsProbe;

// This class is an NiMetricsOutputModule for the metrics system which
// outputs instrumented metrics into the Emergent Metrics' metrics probe
// library so that it may be sent to a web server and analyzed.
//
// This class itself provides the most basic interface.  It does not
// create a metrics probe instance and assumes that the metrics
// probe library is created and set elsewhere.  Additionally, it does not
// initialize or deinitialize the metrics probe and assumes that it is
// done as well (though it provides a convenience function to do so).
//
// If integrating Gamebryo metrics with another system that uses the
// Emergent metrics probe library, then this is the class to use so that
// only a single metrics probe instance will be used.
class NIWEBMETRICSOUTPUT_ENTRY NiWebMetricsOutput : 
    public NiMetricsOutputModule
{
public:

    // Convenience wrappers for MetricsProbeTemplate functions.
    void Init(const char* pcLogURI, const char* pcHostName,
        const char* pcAppName, const char* pcModuleName, 
        const char* pcReferenceName,
        int iFlushInterval, bool bStrictTime, 
        cIEmeMetricsProbeListener* pkListener);
    void Deinit();

    virtual void AddValue(const char* pcName, float fValue);
    virtual void StartTimer(NiMetricsTimer& kTimer) {};
    virtual void EndTimer(NiMetricsTimer& kTimer);
    virtual void AddImmediateEvent(const char* pcName, float fValue);
    virtual void Update();

    void Flush();
};


#endif //#ifndef NIWEBMETRICSOUTPUT_H
