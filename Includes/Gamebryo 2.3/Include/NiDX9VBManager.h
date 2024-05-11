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

#ifndef NIDX9VBMANAGER_H
#define NIDX9VBMANAGER_H

//  The Resource manager is the 'lower-level' manager which doles out 
//  resources such as Vertex and Index buffers and textures, which can be
//  Textures, CubeTextures, and VolumeTextures.
//  This manager is resonsible for the overall control of memory allocations
//  for these types of resources.
//  On top of this layer, there will exist managers which contain the specfic
//  knowledge of the individual resource types.
#include "NiDX9Headers.h"
#include "NiDX9RendererLibType.h"
#include "NiDX9Resource.h"
#include <NiRefObject.h>
//  Have to include this to allow for the sub-class Partition
#include <NiSkinPartition.h>

class NiColorA;
class NiPoint3;
class NiPoint2;
class NiGeometryData;
class NiSkinInstance;
class NiDX9D3DDevice9;
class NiVBChip;

class NIDX9RENDERER_ENTRY NiDX9VertexBufferManager : public NiRefObject
{
public:
    enum { DX9_MAX_VBS = 5 };

    class NIDX9RENDERER_ENTRY NiDX9VBResource : public NiDX9Resource
    {
    public:
        unsigned int m_uiLength;        //  Length of vb
        unsigned int m_uiFVF;           //  FVF of vb
        unsigned int m_uiRevID;         //  Revision of vb
        LPDIRECT3DVERTEXBUFFER9 m_pkVB;  //  Ptr to vb
        D3DVERTEXBUFFER_DESC m_kVBDesc;  //  Description of vb
    };
    class NIDX9RENDERER_ENTRY NiDX9VBInfo : public NiMemObject
    {
    public:
        unsigned int m_uiVBID;                      //  ID w/in vb manager
        unsigned char m_ucIndex;                    //  Index next 'free'
        NiDX9VBResource* m_pkVBRes[DX9_MAX_VBS]; //  Ptr to vb
    };

    NiDX9VertexBufferManager(LPDIRECT3DDEVICE9 pkD3DDevice9);
    ~NiDX9VertexBufferManager();

    NiVBChip* PackUntransformedVB(NiGeometryData* pkData, 
        NiSkinInstance* pkSkin, unsigned short usDirtyFlags, 
        NiVBChip* pkOldVBChip, void* pvLockedBuff = NULL);

    NiVBChip* PackTransformedVB(NiGeometryBufferData* pkBuffData, 
        unsigned short usVerts, const NiPoint2* pkScreenSpaceVerts, 
        const NiColorA* pkColors, NiPoint2* pkTex, 
        unsigned short usDirtyFlags, NiVBChip* pkOldVBChip, 
        void* pvLockedBuff = NULL);

    NiVBChip* PackSkinnedVB(NiGeometryData* pkData, 
        const NiSkinInstance* pkSkin, NiSkinPartition::Partition* pkPartition, 
        unsigned short usDirtyFlags, NiVBChip* pkOldVBChip, 
        void* pvLockedBuff = NULL);

    bool AllocateBufferSpace(NiGeometryBufferData* pkBuffData, 
        unsigned int uiStream);

    void GenerateVertexParameters(NiGeometryData* pkData,
        NiSkinPartition::Partition* pkPartition, 
        unsigned int& uiVertexTypeDesc, unsigned int& uiVertexSize, 
        unsigned int& uiPosOffset, unsigned int& uiWeightOffset, 
        unsigned int& uiNormOffset, unsigned int& uiColorOffset, 
        unsigned int& uiTexOffset);
    void* LockVB(LPDIRECT3DVERTEXBUFFER9 pkVB, unsigned int uiOffset, 
        unsigned int uiSize, unsigned int uiLockFlags);
    void PackVB(unsigned char* pucDestBuffer, unsigned char* pucSrcBuffer, 
        unsigned int uiVertexSize, unsigned int uiDataSize, 
        unsigned int uiNumVerts);
    bool UnlockVB(LPDIRECT3DVERTEXBUFFER9 pkVB);

    void EndFrame();

    void DestroyDefaultBuffers();

    unsigned int GetFrameID() const;

protected:
    NiDX9VBInfo* GetVBInfo(unsigned int uiUsage, unsigned int uiFVF,
        D3DPOOL ePool);
    NiDX9VBInfo* CreateVBInfo();
    LPDIRECT3DVERTEXBUFFER9 CreateVertexBuffer(unsigned int uiLength, 
        unsigned int uiUsage, unsigned int uiFVF, D3DPOOL ePool, 
        D3DVERTEXBUFFER_DESC* pkDesc = NULL);

    LPDIRECT3DDEVICE9 m_pkD3DDevice9;

    NiTPointerMap<unsigned int, NiDX9VBInfo*> m_kDefaultResources;
    NiTPointerMap<unsigned int, NiDX9VBInfo*> m_kManagedResources;
    NiTPointerMap<unsigned int, NiDX9VBInfo*> m_kSystemMemResources;

    unsigned int m_uiFrameID;

    void* m_pvTempBuffer;
    unsigned int m_uiTempBufferSize;
    void* m_pvLockedBuffer;
    unsigned int m_uiLockedBufferSize;

    NiCriticalSection m_kVBCriticalSection;
};

typedef NiPointer<NiDX9VertexBufferManager> NiDX9VertexBufferManagerPtr;

#endif  //#ifndef NIDX9VBMANAGER_H
