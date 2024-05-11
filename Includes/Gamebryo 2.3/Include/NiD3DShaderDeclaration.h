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
#ifndef NID3DSHADERDECLARATION_H
#define NID3DSHADERDECLARATION_H

#include "NiD3DDefines.h"

#include <NiShaderDeclaration.h>
#include <NiSkinPartition.h>

class NiGeometryData;
class NiRenderer;
class NiSkinInstance;
class NiVBChip;

//---------------------------------------------------------------------------
class NID3D_ENTRY NiD3DShaderDeclaration : public NiShaderDeclaration
{
public:
    // See NiShaderDeclaration.h for the ShaderParameterType and
    // ShaderParameterUsage enumerations.

    enum ShaderParameterTesselator
    {
        SPTESS_DEFAULT,
        SPTESS_PARTIALU,
        SPTESS_PARTIALV,
        SPTESS_CROSSUV,
        SPTESS_UV,
        SPTESS_LOOKUP,
        SPTESS_LOOKUPPRESAMPLED,
        SPTESS_COUNT
    };

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
        ShaderParameterType m_ePackType;
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

    virtual ~NiD3DShaderDeclaration();

    static Packer RegisterPackingFunction(ShaderParameterType eType, 
        unsigned int uiREDType, Packer pfnPack);
    static unsigned int GetTypeSize(ShaderParameterType eType);

    //  Entry Access Functions
    virtual bool SetConstantData(unsigned int uiStartRegister, 
        unsigned int uiNumRegisters, float* pfData);
    bool ClearAllStreams();

    //  Shader related functions
    virtual unsigned int GetVertexStride(unsigned int uiStream) = 0;

    virtual void SetSoftwareVertexProcessing(bool bSoftwareVB);
    virtual bool GetSoftwareVertexProcessing() const;

    // *** begin Emergent internal use only ***

    //-----------------------------------------------------------------------
    //  Default data packing functions
    static unsigned int Pack_IllegalCombination(PackingParameters& kParams);
    static unsigned int Pack_DirectMapping(PackingParameters& kParams);
    static unsigned int Pack_DropInFields(PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  Float output packers
    //  NOTE #-# packing uses DirectMapping fcn.
    //  In > Out uses DropInFields.
    //  In < Out uses ExtraOutsToZero or ExtraOutsToOne.
    static unsigned int Pack_Float_DropIns(PackingParameters& kParams);
    static unsigned int Pack_Float_ExtraOutsToZero(
        PackingParameters& kParams);
    static unsigned int Pack_Float_ExtraOutsToOne(PackingParameters& kParams);
    static unsigned int Pack_FloatToFloat(PackingParameters& kParams);
    //  
    static unsigned int Pack_LongToFloat(PackingParameters& kParams);
    static unsigned int Pack_UnsignedLongToFloat(PackingParameters& kParams);
    static unsigned int Pack_ShortToFloat(PackingParameters& kParams);
    static unsigned int Pack_UnsignedShortToFloat(PackingParameters& kParams);
    static unsigned int Pack_ByteToFloat(PackingParameters& kParams);
    static unsigned int Pack_UnsignedByteToFloat(PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  UByteColor output packers
    static unsigned int Pack_FloatToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_LongToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_UnsignedLongToUByteColor(
        PackingParameters& kParams);
    static unsigned int Pack_ShortToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_UnsignedShortToUByteColor(
        PackingParameters& kParams);
    static unsigned int Pack_ByteToUByteColor(PackingParameters& kParams);
    static unsigned int Pack_UnsignedByteToUByteColor(
        PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  Short output packers
    static unsigned int Pack_FloatToShort(PackingParameters& kParams);
    static unsigned int Pack_LongToShort(PackingParameters& kParams);
    static unsigned int Pack_UnsignedLongToShort(PackingParameters& kParams);
    static unsigned int Pack_ShortToShort(PackingParameters& kParams);
    static unsigned int Pack_UnsignedShortToShort(PackingParameters& kParams);
    static unsigned int Pack_ByteToShort(PackingParameters& kParams);
    static unsigned int Pack_UnsignedByteToShort(PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  Normalized short output packers
    static unsigned int Pack_FloatToNormalizeShort(
        PackingParameters& kParams);
    static unsigned int Pack_LongToNormalizeShort(PackingParameters& kParams);
    static unsigned int Pack_UnsignedLongToNormalizeShort(
        PackingParameters& kParams);
    static unsigned int Pack_ShortToNormalizeShort(PackingParameters& kParams);
    static unsigned int Pack_UnsignedShortToNormalizeShort(
        PackingParameters& kParams);
    static unsigned int Pack_ByteToNormalizeShort(PackingParameters& kParams);
    static unsigned int Pack_UnsignedByteToNormalizeShort(
        PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  Normalize packed output packers
    static unsigned int Pack_FloatToNormalizedPacked(
        PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  Packed byte output packers
    static unsigned int Pack_FloatToPackedByte(PackingParameters& kParams);
    static unsigned int Pack_LongToPackedByte(PackingParameters& kParams);
    static unsigned int Pack_UnsignedLongToPackedByte(
        PackingParameters& kParams);
    static unsigned int Pack_ShortToPackedByte(PackingParameters& kParams);
    static unsigned int Pack_UnsignedShortToPackedByte(
        PackingParameters& kParams);
    static unsigned int Pack_ByteToPackedByte(PackingParameters& kParams);
    static unsigned int Pack_UnsignedByteToPackedByte(
        PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  Unsigned byte output packers
    static unsigned int Pack_FloatToUnsignedByte(PackingParameters& kParams);
    static unsigned int Pack_LongToUnsignedByte(PackingParameters& kParams);
    static unsigned int Pack_UnsignedLongToUnsignedByte(
        PackingParameters& kParams);
    static unsigned int Pack_ShortToUnsignedByte(PackingParameters& kParams);
    static unsigned int Pack_UnsignedShortToUnsignedByte(
        PackingParameters& kParams);
    static unsigned int Pack_ByteToUnsignedByte(PackingParameters& kParams);
    static unsigned int Pack_UnsignedByteToUnsignedByte(
        PackingParameters& kParams);
    //-----------------------------------------------------------------------
    //  2D Homogeneous float output packers
    static unsigned int Pack_FloatTo2DHomogenous(PackingParameters& kParams);

    static bool InitializePackingFunctions();

    virtual NiVBChip* PackUntransformedVB(NiGeometryData* pkData, 
        NiSkinInstance* pkSkin, NiSkinPartition::Partition* pkPartition, 
        unsigned short usDirtyFlags, NiVBChip* pkOldVBChip,
        unsigned int uiStream, void* pvLockedBuff = NULL) = 0;

    virtual bool GenerateVertexParameters(
        NiD3DVertexDeclaration& kDeclaration, 
        unsigned int& uiNumStreams) = 0;

    virtual NiD3DVertexDeclaration GetD3DDeclaration();

    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    NiD3DShaderDeclaration();

    bool Initialize(NiRenderer* pkRenderer, 
        unsigned int uiMaxStreamEntryCount, unsigned int uiStreamCount);

    unsigned int PackEntry(unsigned short usCount, NiGeometryData* pkData, 
        const NiSkinInstance* pkSkin, 
        NiSkinPartition::Partition* pkPartition, 
        ShaderRegisterEntry* pkEntry,
        unsigned int uiStride, unsigned char* pucData);

    static void SetDefaultPackingFunctions();
    static void SetDefaultPackingFunction(ShaderParameterType eType, 
        unsigned int uiREDType);

    NiD3DRenderer* m_pkRenderer;
    NiD3DVBManager* m_pkVBManager;
    D3DDevicePtr m_pkD3DDevice;

    unsigned int m_uiDeclSize;
    unsigned int m_uiCurrDeclIndex;
    
    static unsigned int ms_auiTypeSizes[SPTYPE_COUNT];
    static NiTPrimitiveArray<NiPackerEntry*>* ms_pkPackFunctions;
};

NiSmartPointer(NiD3DShaderDeclaration);

#include "NiD3DShaderDeclaration.inl"

#endif  //#ifndef NID3DSHADERDECLARATION_H
