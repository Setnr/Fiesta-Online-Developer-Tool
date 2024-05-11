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

#ifndef NID3D10GEOMETRYDATA_H
#define NID3D10GEOMETRYDATA_H

#include <NiGeometryData.h>

#include "NiD3D10Headers.h"
#include "NiD3D10RendererLibType.h"
#include "NiD3D10DataStream.h"

#include <NiTMap.h>

//  Geometry buffer tracking class
class NID3D10RENDERER_ENTRY NiD3D10GeometryData : 
    public NiGeometryData::RendererData
{
public:
    NiD3D10GeometryData(unsigned int uiPrimitiveCount = 1);
    NiD3D10GeometryData(NiSkinInstance* pkSkinInstance, 
        unsigned int uiPartitions, unsigned int* puiPrimitiveCount);

    virtual ~NiD3D10GeometryData();

    void UpdateInputLayout(void* pvInputSignature, 
        unsigned int uiInputSignatureSize);
    
    bool ContainsVertexData(NiShaderDeclaration::ShaderParameter eParam) const;

    unsigned int GetPartitionCount() const;
    unsigned int GetPrimitiveStart(unsigned int uiPartition) const;
    unsigned int GetPrimitiveCount(unsigned int uiPartition) const;
    unsigned int GetTotalPrimitiveCount() const;
    D3D10_INPUT_ELEMENT_DESC* GetInputElementDescArray() const;
    unsigned int GetInputElementDescCount() const;

    D3D10_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;

    ID3D10InputLayout* GetInputLayout() const;

    unsigned int GetVBCount() const;
    NiD3D10DataStream* GetVBDataStream(unsigned int uiIndex) const;
    ID3D10Buffer*const* GetVBArray() const;
    const unsigned int* GetVBStrideArray() const;

    NiD3D10DataStream* GetIBDataStream(unsigned int uiPrimitive) const;
    ID3D10Buffer*const GetIB(unsigned int uiPrimitive) const;
    DXGI_FORMAT GetIBFormat(unsigned int uiPrimitive) const;
    unsigned int GetIBOffset(unsigned int uiPrimitive) const;

    const unsigned int* GetVBOffsetArray(unsigned int uiPrimitive) const;

    unsigned int GetIndexCount(unsigned int uiPrimitive) const;
    unsigned int GetStartIndexLocation(unsigned int uiPrimitive) const;
    int GetBaseVertexLocation(unsigned int uiPrimitive) const;

    unsigned int GetFlags() const;

    enum
    {
        VERTEX_FLAG_TEXTUREMASK              = 0xFF000000,
        VERTEX_FLAG_TEXTURESHIFT             = 24,
        VERTEX_FLAG_HASNORMALS               = 0x00800000,
        VERTEX_FLAG_HASCOLORS                = 0x00400000,
        VERTEX_FLAG_HASBINORMALSTANGENTS     = 0x00200000
    };

    static unsigned int CreateVertexFlags(bool bHasColors, bool bHasNormals, 
        bool bHasBinormalsTangents, unsigned int uiUVCount);
    static bool GetHasColorsFromFlags(unsigned int uiFlags);
    static bool GetHasNormalsFromFlags(unsigned int uiFlags);
    static bool GetHasBinormalsTangentsFromFlags(unsigned int uiFlags);
    static unsigned int GetNumUVsFromFlags(unsigned int uiFlags);

    // Primitive data

    // *** begin Emergent internal use only ***
    void SetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY eTopology);
    void SetInputElementDescArray(D3D10_INPUT_ELEMENT_DESC* pkElementDescArray,
        unsigned int uiElementCount);
    void SetInputLayout(ID3D10InputLayout* pkInputLayout,
        void* pvInputSignature);

    void SetVBCount(unsigned int uiVBCount);
    // Setting NiD3D10DataStream objects is preferable, because all the
    // information on locking the VB is stored there.
    void SetVBDataStream(unsigned int uiIndex, 
        NiD3D10DataStream* pkVBDataStream);
    void SetVB(unsigned int uiIndex, ID3D10Buffer*const pkVB);
    void SetVBArray(ID3D10Buffer*const* ppkVBArray, unsigned int uiVBCount);
    void SetVBStrideArray(const unsigned int* puiVBStrideArray, 
        unsigned int uiVBCount);

    // Setting NiD3D10DataStream objects is preferable, because all the
    // information on locking the IB is stored there.
    void SetIBDataStream(unsigned int uiPrimitive, 
        NiD3D10DataStream* pkIBDataStream);
    void SetIB(unsigned int uiPrimitive, ID3D10Buffer* pkIndexBuffer);
    void SetIBFormat(unsigned int uiPrimitive, DXGI_FORMAT eIBFormat);
    void SetIBOffset(unsigned int uiPrimitive, unsigned int uiIBOffset);

    void SetVBOffsetArray(unsigned int uiPrimitive, 
        const unsigned int* puiVBOffsetArray, unsigned int uiVBCount);

    void SetIndexCount(unsigned int uiPrimitive, unsigned int uiIndexCount);
    void SetStartIndexLocation(unsigned int uiPrimitive, 
        unsigned int uiStartIndexLocation);
    void SetBaseVertexLocation(unsigned int uiPrimitive, 
        int iBaseVertexLocation);

    void SetFlags(unsigned int uiFlags);

    // Force geometry data to re-calculate the buffers from the 
    // NiD3D10DataStream objects - this is because an IMMUTABLE buffer may
    // have been created as a temporary STAGING buffer at first.
    void UpdateVBs();
    void UpdateIBs();

    void ReleaseCachedInputLayouts();
    void ReleaseElementArray();

    NiSkinInstance* GetSkinInstance() const;

    NiD3D10GeometryData* GetNext() const;
    void SetNext(NiD3D10GeometryData* pkNext);
    // *** end Emergent internal use only ***

protected:
    const unsigned int m_uiPartitionCount;
    unsigned int* m_puiPartitionStartPrimitive;
    D3D10_PRIMITIVE_TOPOLOGY m_ePrimitiveTopology;

    D3D10_INPUT_ELEMENT_DESC* m_pkElementDescArray;
    unsigned int m_uiElementCount;

    ID3D10InputLayout* m_pkCurrentInputLayout;
    NiTMap<void*, ID3D10InputLayout*> m_kInputLayouts;

    unsigned int m_uiVBCount;
    NiD3D10DataStreamPtr* m_pspVBDataStreamArray;
    ID3D10Buffer** m_ppkVBArray;
    unsigned int* m_puiVBStrideArray;

    class PrimitiveDesc : public NiMemObject
    {
    public:
        PrimitiveDesc();
        virtual ~PrimitiveDesc();

        NiD3D10DataStreamPtr m_spIBDataStream;
        ID3D10Buffer* m_pkIndexBuffer;
        DXGI_FORMAT m_eIBFormat;
        unsigned int m_uiIBOffset;

        unsigned int* m_puiVBOffsetArray;

        unsigned int m_uiIndexCount;
        unsigned int m_uiStartIndexLocation;
        int m_iBaseVertexLocation;
    };

    PrimitiveDesc* m_pkPrimitives;

    NiSkinInstance* m_pkSkinInstance;

    NiD3D10GeometryData* m_pkNext;

    unsigned int m_uiFlags;
};

//#include "NiD3D10GeometryData.inl"

#endif  //#ifndef NID3D10GEOMETRYDATA_H