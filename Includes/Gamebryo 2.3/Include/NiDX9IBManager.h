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
//---------------------------------------------------------------------------
#ifndef NIDX9IBMANAGER_H
#define NIDX9IBMANAGER_H

//  The IndexBuffer manager is responsible for giving out and tracking index
//  buffers required by the application.
#include "NiDX9Headers.h"
#include "NiDX9RendererLibType.h"
#include "NiDX9Resource.h"
#include <NiBool.h>
#include <NiTPointerMap.h>
#include <NiRefObject.h>

class NiGeometryData;

class NIDX9RENDERER_ENTRY NiDX9IndexBufferManager : public NiRefObject
{
public:
    enum { DX9_MAX_IBS = 5 };

    class NiDX9IBResource : public NiDX9Resource
    {
    public:
        unsigned int m_uiLength;        //  Length of ib
        D3DFORMAT m_eFormat;            //  Format of ib
        unsigned int m_uiRevID;         //  Revision of ib
        LPDIRECT3DINDEXBUFFER9 m_pkIB;  //  Ptr to ib
        D3DINDEXBUFFER_DESC m_kIBDesc;  //  Description of ib
    };
    class NIDX9RENDERER_ENTRY NiDX9IBInfo : public NiMemObject
    {
    public:
        unsigned int m_uiIBID;                      //  ID w/in ib manager
        unsigned char m_ucIndex;                    //  Index next 'free'
        NiDX9IBResource* m_apkIBRes[DX9_MAX_IBS];   //  Ptr to ib
    };

    NiDX9IndexBufferManager(LPDIRECT3DDEVICE9 pkD3DDevice9);
    ~NiDX9IndexBufferManager();

    LPDIRECT3DINDEXBUFFER9 GetIndexBuffer(unsigned int uiLength, 
        unsigned int uiUsage, D3DFORMAT eFormat, D3DPOOL ePool);

    LPDIRECT3DINDEXBUFFER9 PackBuffer(const unsigned short* pusTriList, 
        unsigned int uiLength, unsigned int uiMaxLength, 
        LPDIRECT3DINDEXBUFFER9 pkOldIB, unsigned int& uiIBSize, 
        D3DPOOL ePool, unsigned int uiUsage);

    LPDIRECT3DINDEXBUFFER9 ResizeTempPointConnectivity(unsigned int uiPoints, 
        bool bRefill = false, unsigned int uiUsage = 0, 
        D3DPOOL ePool = D3DPOOL_MANAGED);
    LPDIRECT3DINDEXBUFFER9 ResizeTempLineConnectivity(unsigned int uiPoints, 
        NiBool* pbFlags, unsigned int& uiConn, unsigned int uiUsage = 0, 
        D3DPOOL ePool = D3DPOOL_MANAGED);

    void DestroyDefaultBuffers();

protected:
    NiDX9IBInfo* GetIBInfo(unsigned int uiUsage, D3DFORMAT eFormat,
        D3DPOOL ePool);
    NiDX9IBInfo* CreateIBInfo();
    LPDIRECT3DINDEXBUFFER9 CreateIndexBuffer(unsigned int uiLength, 
        unsigned int uiUsage, D3DFORMAT eFormat, D3DPOOL ePool, 
        D3DINDEXBUFFER_DESC* pkDesc = NULL);

    LPDIRECT3DDEVICE9 m_pkD3DDevice9;

    // Temporary array of pre-packed point connectivities
    LPDIRECT3DINDEXBUFFER9 m_pkTempPointConnIB;
    unsigned int m_uiTempPointConnLen;
    // Temporary array of pre-packed line connectivities
    LPDIRECT3DINDEXBUFFER9 m_pkTempLineConnIB;
    unsigned int m_uiTempLineConnLen;

    NiTPointerMap<unsigned int, NiDX9IBInfo*> m_kDefaultResources;
    NiTPointerMap<unsigned int, NiDX9IBInfo*> m_kManagedResources;
    NiTPointerMap<unsigned int, NiDX9IBInfo*> m_kSystemMemResources;

};

typedef NiPointer<NiDX9IndexBufferManager> NiDX9IndexBufferManagerPtr;

#endif  //#ifndef NIDX9IBMANAGER_H
