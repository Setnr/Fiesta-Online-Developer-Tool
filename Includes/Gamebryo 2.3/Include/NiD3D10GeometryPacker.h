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

#ifndef NID3D10GEOMETRYPACKER_H
#define NID3D10GEOMETRYPACKER_H

#include "NiD3D10RendererLibType.h"

#include <NiMemObject.h>

#include <NiShaderDeclaration.h>

class NID3D10RENDERER_ENTRY NiD3D10GeometryPacker : public NiMemObject
{
public:
    //  Packer function
    //  usCount     - number of elements to process
    //  pvInData    - ptr to data to use as input stream
    //  uiInSize    - size of input element
    //  uiInStride  - stride between input elements
    //  uiOutSize   - size of output element
    //  uiOutStride - stride between output elements
    //  pvOutData   - ptr to data to pack into
    //
    //  Had to put this in a class dues to the usage of NiTArray causing
    //  name overrun problems (limit of 256 characters) - which would hinder
    //  debugging.
    class PackingParameters : public NiMemObject
    {
    public:
        NiShaderDeclaration::ShaderParameterType m_ePackType;
        unsigned int m_uiDataType;
        unsigned short m_usCount;
        unsigned short* m_pusVertexMap;
        void* m_pvInData;
        unsigned int m_uiInSize;
        unsigned int m_uiInStride;
        unsigned int m_uiOutSize;
        unsigned int m_uiOutStride;
        void* m_pvOutData;
    };

    //  The Packer function returns the total bytes packed into the stream.
    typedef unsigned int (*Packer)(PackingParameters& kParams);

    NiD3D10GeometryPacker();
    virtual ~NiD3D10GeometryPacker();

    static Packer RegisterPackingFunction(
        NiShaderDeclaration::ShaderParameterType eType, 
        unsigned int uiAGDType, Packer pfnPack);

    // *** begin Emergent internal use only ***
    unsigned int PackEntry(unsigned short usCount, NiGeometryData* pkData, 
        const NiSkinInstance* pkSkin, 
        const NiShaderDeclaration::ShaderRegisterEntry* pkEntry,
        unsigned int uiStride, void* pvDest, unsigned int uiInputStride, 
        void* pvOverrideInput);

    //-----------------------------------------------------------------------
    //  Default data packing functions
    static unsigned int Pack_IllegalCombination(PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  Float output packers
    //  NOTE #-# packing uses DirectMapping functionn.
    //  In > Out uses DropInFields.
    //  In < Out uses ExtraOutsToZero or ExtraOutsToOne.
    static unsigned int Pack_Float32_DropIns(PackingParameters& kParams);
    static unsigned int Pack_Float32_ExtraOutsToZero(
        PackingParameters& kParams);
    static unsigned int Pack_Float32_ExtraOutsToOne(
        PackingParameters& kParams);
    static unsigned int Pack_Float32ToFloat32(PackingParameters& kParams);
    //  
    static unsigned int Pack_SNorm32ToFloat32(PackingParameters& kParams);
    static unsigned int Pack_UNorm32ToFloat32(PackingParameters& kParams);
    static unsigned int Pack_SNorm16ToFloat32(PackingParameters& kParams);
    static unsigned int Pack_UNorm16ToFloat32(PackingParameters& kParams);
    static unsigned int Pack_SNorm8ToFloat32(PackingParameters& kParams);
    static unsigned int Pack_UNorm8ToFloat32(PackingParameters& kParams);
    //-----------------------------------------------------------------------
    // These should probably be deprecated
    //  UByteColor output packers
    static unsigned int Pack_Float32ToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_SInt32ToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_UInt32ToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_SInt16ToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_UInt16ToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_SInt8ToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_UInt8ToUByteColor(PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  UByteColor output packers
    static unsigned int Pack_Float32ToUNorm8(PackingParameters& kParams);
    static unsigned int Pack_SInt32ToUNorm8(PackingParameters& kParams);
    static unsigned int Pack_UInt32ToUNorm8(PackingParameters& kParams);
    static unsigned int Pack_SInt16ToUNorm8(PackingParameters& kParams);
    static unsigned int Pack_UInt16ToUNorm8(PackingParameters& kParams);
    static unsigned int Pack_SInt8ToUNorm8(PackingParameters& kParams);
    static unsigned int Pack_UInt8ToUNorm8(PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  Short output packers
    static unsigned int Pack_Float32ToSInt16(PackingParameters& kParams);
    static unsigned int Pack_SInt32ToSInt16(PackingParameters& kParams);
    static unsigned int Pack_UInt32ToSInt16(PackingParameters& kParams);
    static unsigned int Pack_SInt16ToSInt16(PackingParameters& kParams);
    static unsigned int Pack_UInt16ToSInt16(PackingParameters& kParams);
    static unsigned int Pack_SInt8ToSInt16(PackingParameters& kParams);
    static unsigned int Pack_UInt8ToSInt16(PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  Unsigned byte output packers
    static unsigned int Pack_Float32ToUInt8(PackingParameters& kParams);
    static unsigned int Pack_SInt32ToUInt8(PackingParameters& kParams);
    static unsigned int Pack_UInt32ToUInt8(PackingParameters& kParams);
    static unsigned int Pack_SInt16ToUInt8(PackingParameters& kParams);
    static unsigned int Pack_UInt16ToUInt8(PackingParameters& kParams);
    static unsigned int Pack_SInt8ToUInt8(PackingParameters& kParams);
    static unsigned int Pack_UInt8ToUInt8(PackingParameters& kParams);
    //-----------------------------------------------------------------------
    static bool InitializePackingFunctions();

    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    static void SetDefaultPackingFunctions();
    static void SetDefaultPackingFunction(
        NiShaderDeclaration::ShaderParameterType eType, 
        unsigned int uiAGDType);

    class NiPackerEntry : public NiMemObject
    {
    public:
        unsigned int m_uiShaderParameterType;
        NiTPrimitiveArray<Packer> m_kPackFunctions;

        NiPackerEntry();
        NiPackerEntry(const NiPackerEntry& kSrc);
        ~NiPackerEntry();

        bool operator==(const NiPackerEntry& kSrc);
        bool operator!= (const NiPackerEntry& kSrc);
        NiPackerEntry& operator=(const NiPackerEntry& kSrc);
    };

    static NiTPrimitiveArray<NiPackerEntry*>* ms_pkPackFunctions;
};

//#include "NiD3D10GeometryPacker.inl"

#endif // NID3D10GEOMETRYPACKER_H