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

#ifndef NID3D10SELECT_H
#define NID3D10SELECT_H

#include "NiApplication.h"
#include "NiD3D10PixelFormat.h"
#include "NiD3D10Renderer.h"
#include "NiD3D10SystemDesc.h"

#include <NiOS.h>
#include <NiRenderer.h>
#include <NiUniversalTypes.h>

class NiD3D10AdapterDesc;
class NiD3D10DeviceDesc;
class NiD3D10OutputDesc;

//---------------------------------------------------------------------------
class NiD3D10Select 
{
public:
    static NiRendererPtr CreateRenderer(NiWindowRef pTopLevelWnd,
        bool bDialog, unsigned int uiWidth, unsigned int uiHeight,
        bool bMultiThread, bool bRefRast, bool& bFullscreen);

protected:
    typedef enum {
        RETURN_OK,
        RETURN_FAIL,
        RETURN_CANCEL
    } ReturnVal;

    enum { IDC_STATIC = 0xffff };
    enum { IDC_MODE_COMBO = 1000 };
    enum { IDC_ADAPTER_COMBO = 1001 };
    enum { IDC_DISPLAY_COMBO = 1002 };
    enum { IDC_RESOLUTION_COMBO = 1003 };
    enum { IDC_FORMAT_COMBO = 1004 };
    enum { IDC_MULTISAMPLECOUNT_COMBO = 1005 };
    enum { IDC_MULTISAMPLEQUALITY_COMBO = 1006 };

    static NiRendererPtr DialogCreateRenderer(NiWindowRef pTopLevelWnd,
        unsigned int uiDefaultWidth, unsigned int uiDefaultHeight,
        bool bRefRast, unsigned int uiCreateFlags, bool& bFullscreen);

    static ReturnVal Select(NiWindowRef pkWnd, unsigned int uiDefaultWidth,
        unsigned int uiDefaultHeight, bool bRefRast, bool& bFullscreen,
        const void*& pvAdapter, const void*& pvDevice,
        const void*& pvOutputDesc, const void*& pvDXGIModeDesc,
        DXGI_FORMAT& eDXGIFormat, unsigned int& uiMultisampleCount,
        unsigned int& uiMultisampleQuality);

    static void EnumerateAdapters(NiWindowRef pkDialog);
    static void EnumerateDisplays(NiWindowRef pkDialog);
    static void EnumerateResolutions(NiWindowRef pkDialog);
    static void EnumerateFormats(NiWindowRef pkDialog);
    static void EnumerateMultisampleCounts(NiWindowRef pkDialog);
    static void EnumerateMultisampleQuality(NiWindowRef pkDialog);
    static bool UpdateAdapter(NiWindowRef pkDialog);
    static bool UpdateDisplay(NiWindowRef pkDialog);
    static bool UpdateResolution(NiWindowRef pkDialog);
    static bool UpdateFormat(NiWindowRef pkDialog);
    static bool UpdateMultisampleCount(NiWindowRef pkDialog);
    static bool UpdateMultisampleQuality(NiWindowRef pkDialog);

    static BOOL CALLBACK Callback(NiWindowRef pkDialog, unsigned int uiMsg,
        WPARAM wParam, LPARAM lParam);
    static void CopyToWideChar(unsigned short** ppusStrOut, 
        const char* pcStrIn);
    static void AddDialogControl(unsigned short** ppusTemplate, 
        unsigned int uiStyle, short sSX, short sSY, short sCX, short sCY, 
        unsigned short usID, const char* pcStrClassName, 
        const char* pcStrTitle);
    static DLGTEMPLATE* BuildAdapterSelectTemplate();

    static bool ms_bDialogOpen;
    static bool ms_b32BitZBuffer;

    static bool ms_bFullscreen;
    static bool ms_bRefRast;
    static unsigned int ms_uiDefaultWidth;
    static unsigned int ms_uiDefaultHeight;
    static unsigned int ms_uiMultisampleCount;
    static unsigned int ms_uiMultisampleQuality;

    static const NiD3D10SystemDesc* ms_pkSysDesc;
    static const NiD3D10AdapterDesc* ms_pkAdapterDesc;
    static const NiD3D10DeviceDesc* ms_pkDeviceDesc;
    static const NiD3D10OutputDesc* ms_pkOutputDesc;
    static const DXGI_MODE_DESC* ms_pkDXGIModeDesc;
    static DXGI_FORMAT ms_eDXGIFormat;
};

#endif
