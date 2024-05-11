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
#ifndef NIDX9RESOURCE_H
#define NIDX9RESOURCE_H

//  NiDX9Resource is a base class for tracking resources under DX9 in the
//  ResourceManager, VBManager, IBManager, and TextureManager.
#include "NiDX9Headers.h"
#include "NiDX9RendererLibType.h"

#include <NiRTLib.h>
#include <NiSmartPointer.h>

class NIDX9RENDERER_ENTRY NiDX9Resource : public NiMemObject
{
public:
    // *** begin Emergent internal use only ***
    enum Type
    {
        TYPE_TEXTURE    = 0,
        TYPE_TEXTURE_CUBE,
        TYPE_TEXTURE_VOLUME,
        TYPE_VERTEXBUFFER,
        TYPE_INDEXBUFFER,
        TYPE_INVALID    = 0xFFFF,
        TYPE_COUNT
    };

    NiDX9Resource();
    ~NiDX9Resource();

    const unsigned int GetResourceID();
    const Type GetType();
    const D3DPOOL GetPool();
    const unsigned int GetUsage();
    const LPDIRECT3DRESOURCE9 GetResource();

    unsigned int m_uiResID;             //  Resource ID
    Type m_eType;                       //  Type of resource
    D3DPOOL m_ePool;                    //  Memory class for resource
    unsigned int m_uiUsage;             //  USage for resource
    LPDIRECT3DRESOURCE9 m_pkD3DResource; //  Ptr. to D3D resource
    // *** end Emergent internal use only ***
};

typedef NiPointer<NiDX9Resource> NiDX9ResourcePtr;

#include "NIDX9Resource.inl"

#endif  //#ifndef NiDX9RESOURCE_H
