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

#ifndef NIWEBMETRICSINSTANCEOUTPUT_H
#define NIWEBMETRICSINSTANCEOUTPUT_H

#include "NiWebMetricsOutputLibType.h"
#include <NiWebMetricsOutput.h>

// This class builds on NiWebMetricsOutput and provides some additional
// functionality.  It is designed to be the most convenient interface
// for Gamebryo users into the metrics probe library.  This should be used
// if it is desired that Gamebryo creates and sets the metrics probe
// instance as well as initializes and deinitializes the metrics probe.
// Therefore, only one instance of NiWebMetricsInstanceOutput should
// ever be created by an application.
//
// Because the metrics probe type (NiMetricsProbe) is known by this class,
// it can provide some extra functionality (such as setting FileMetricsLog
// parameters) that NiWebMetricsOutput cannot.

class NIWEBMETRICSOUTPUT_ENTRY NiWebMetricsInstanceOutput : 
    public NiWebMetricsOutput
{
public:
    NiWebMetricsInstanceOutput(const char* pcLogURI, const char* pcHostName,
        const char* pcAppName, const char* pcModuleName, 
        const char* pcReferenceName,
        int iFlushInterval, bool bStrictTime, 
        cIEmeMetricsProbeListener* pkListener);
    virtual ~NiWebMetricsInstanceOutput();

    // FileMetricsLog options getters and setters
    void SetUploaderPath(const char* pcUploaderPath);
    void SetUploaderHost(const char* pcUploaderHost);
    void SetUploaderPort(int iUploaderPort);
    void SetUploaderUri(const char* pcUploaderUri);
    void SetUploaderIni(const char* pcUploaderIni);
    void SetUploaderSrcId(const char* pcUploaderSrcId);
    void SetUploadOnClose(bool bUploadOnClose);

    const char* GetUploaderPath() const;
    const char* GetUploaderHost() const;
    int GetUploaderPort() const;
    const char* GetUploaderUri() const;
    const char* GetUploaderIni() const;
    const char* GetUploaderSrcId() const;
    bool GetUploadOnClose() const;

protected:
    static NiMetricsProbe* ms_pkInstance;
};


#endif //#ifndef NIWEBMETRICSINSTANCEOUTPUT_H
