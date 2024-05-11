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

#ifndef NID3D10SHADERCONSTANTMAP_H
#define NID3D10SHADERCONSTANTMAP_H

#include <NiShaderConstantMap.h>

#include "NiD3D10RendererLibType.h"
#include "NiD3D10ShaderConstantData.h"

#include <NiFixedString.h>
#include <NiShaderConstantMapEntry.h>
#include <NiSkinPartition.h>
#include <NiTArray.h>

class NiBound;
class NiD3D10GeometryData;
class NiD3D10ShaderProgram;
class NiDynamicEffectState;
class NiGeometry;
class NiPropertyState;
class NiTransform;
class NiShaderConstantMapEntry;

NiSmartPointer(NiD3D10ShaderConstantData);

class NID3D10RENDERER_ENTRY NiD3D10ShaderConstantMap : 
    public NiShaderConstantMap
{
public:
    NiD3D10ShaderConstantMap(NiGPUProgram::ProgramType eType);
    virtual ~NiD3D10ShaderConstantMap();

    void SetName(const NiFixedString& kName);
    const NiFixedString& GetName() const;

    virtual NiShaderError AddEntry(const char* pcKey, unsigned int uiFlags,
        unsigned int uiExtra, unsigned int uiShaderRegister, 
        unsigned int uiRegisterCount, const char* pcVariableName = NULL, 
        unsigned int uiDataSize = 0, unsigned int uiDataStride = 0, 
        const void* pvDataSource = NULL, bool bCopyData = false);
    virtual NiShaderError AddPredefinedEntry(const char* pcKey, 
        unsigned int uiExtra, unsigned int uiShaderRegister, 
        const char* pcVariableName);
    virtual NiShaderError AddAttributeEntry(const char* pcKey, 
        unsigned int uiFlags, unsigned int uiExtra, 
        unsigned int uiShaderRegister, unsigned int uiRegisterCount, 
        const char* pcVariableName, unsigned int uiDataSize, 
        unsigned int uiDataStride, const void* pvDataSource, 
        bool bCopyData = true);
    virtual NiShaderError AddConstantEntry(const char* pcKey, 
        unsigned int uiFlags, unsigned int uiExtra, 
        unsigned int uiShaderRegister, unsigned int uiRegisterCount, 
        const char* pcVariableName, unsigned int uiDataSize, 
        unsigned int uiDataStride, const void* pvDataSource, 
        bool bCopyData = true);
    virtual NiShaderError AddGlobalEntry(const char* pcKey, 
        unsigned int uiFlags, unsigned int uiExtra, 
        unsigned int uiShaderRegister, unsigned int uiRegisterCount, 
        const char* pcVariableName, unsigned int uiDataSize, 
        unsigned int uiDataStride, const void* pvDataSource, 
        bool bCopyData = false);
    virtual NiShaderError AddOperatorEntry(const char* pcKey, 
        unsigned int uiFlags, unsigned int uiExtra, 
        unsigned int uiShaderRegister, unsigned int uiRegisterCount, 
        const char* pcVariableName);
    virtual NiShaderError AddObjectEntry(const char* pcKey,
        unsigned int uiShaderRegister, const char* pcVariableName,
        unsigned int uiObjectIndex,
        NiShaderAttributeDesc::ObjectType eObjectType);

    virtual NiShaderError RemoveEntry(const char* pcKey);
    virtual NiShaderConstantMapEntry* GetEntry(const char* pcKey);
    virtual NiShaderConstantMapEntry* GetEntryAtIndex(unsigned int uiIndex);
    virtual unsigned int GetEntryCount();

    NiShaderError UpdateShaderConstants(
        NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, 
        const NiSkinPartition::Partition* pkPartition, 
        NiD3D10GeometryData* pkGeomData, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects, const NiTransform& kWorld, 
        const NiBound& kWorldBound, unsigned int uiPass = 0, 
        bool bGlobal = false);

    bool LinkShaderConstantBuffer(ID3D10ShaderReflection* pkShaderReflection);

    const NiD3D10ShaderConstantData* GetShaderConstantData() const;
    bool CreateShaderConstantData(
        ID3D10ShaderReflectionConstantBuffer* pkBufferReflection = NULL, 
        unsigned int uiBufferIndex = 
        NiD3D10ShaderConstantData::BUFFER_INDEX_UNASSIGNED);
    void ReleaseShaderConstantData();

    // *** begin Emergent internal use only
    static void SetupTextureTransformMatrix(D3DXMATRIXA16& kResult, 
        const NiMatrix3* pkTexMatrix, bool bTrans = false);

    // UpdateConstantBufferPacking will update the register and register
    // count of all the NiShaderConstantMapEntry objects in the map 
    // according to the packed location of each entry in the final constant
    // buffer.
    // The register and register counts will be encoded with the register and
    // the register element; such values will be marked with the
    // SCM_REGISTER_ENCODING flag to distinguish them from unencoded values.
    // Register values of -1 (0xFFFFFFFF) will remain intact to indicate that 
    // the entry should not be packed in the constant buffer.
    unsigned int UpdateConstantBufferPacking(
        ID3D10ShaderReflectionConstantBuffer* pkBufferReflection = NULL);

    enum 
    {
        SCM_REGISTER_NONE             = 0xFFFFFFFF,
        SCM_REGISTER_ENCODING         = 0x10000000,
        SCM_REGISTER_ENCODING_MASK    = 0xF0000000,
        SCM_REGISTER_ELEMENT_MASK     = 0x0F000000,
        SCM_REGISTER_ELEMENT_SHIFT    = 24,
        SCM_REGISTER_PACKED_BIT       = 0x00800000,
        SCM_REGISTER_MASK             = 0x0000FFFF,
        SCM_REGISTER_SHIFT            = 0
    };

    static bool EncodePackedRegisterAndElement(unsigned int& uiEncodedValue, 
        unsigned int uiRegister, unsigned int uiElement, 
        bool bPackedRegisters);
    static bool DecodePackedRegisterAndElement(unsigned int uiEncodedValue, 
        unsigned int& uiRegister, unsigned int& uiElement,
        bool& bPackedRegisters);

    bool ValidateShaderConstantMapViaReflection(
        ID3D10ShaderReflectionConstantBuffer* pkReflectionBuffer);

    bool IsConstantBufferCurrent() const;
    void SetConstantBufferObsolete();
    // *** end Emergent internal use only

protected:
    enum 
    {
        INVALID_ENTRY_INDEX = UINT_MAX
    };
    unsigned int GetEntryIndex(const char* pcKey);

    NiShaderError InsertEntry(NiShaderConstantMapEntry* pkEntry);

    static NiShaderError SetupPredefinedEntry(
        NiShaderConstantMapEntry* pkEntry);
    static NiShaderError SetupObjectEntry(NiShaderConstantMapEntry* pkEntry);

    // Update all shader constants from the shader constant map entries
    NiShaderError UpdateShaderConstantValues(
        NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, 
        const NiSkinPartition::Partition* pkPartition, 
        NiD3D10GeometryData* pkGeomData, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects, const NiTransform& kWorld, 
        const NiBound& kWorldBound, unsigned int uiPass, bool bGlobal);

    // Copy shader constants for individual shader constant map entries
    // to the shader constant buffer
    NiShaderError UpdateDefinedConstantValue(void* pvShaderConstantBuffer, 
        NiShaderConstantMapEntry* pkEntry, NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        const NiSkinPartition::Partition* pkPartition, 
        NiD3D10GeometryData* pkBuffData, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects, const NiTransform& kWorld, 
        const NiBound& kWorldBound, unsigned int uiPass);
    NiShaderError UpdateConstantConstantValue(void* pvShaderConstantBuffer, 
        NiShaderConstantMapEntry* pkEntry, unsigned int uiPass);
    NiShaderError UpdateAttributeConstantValue(void* pvShaderConstantBuffer, 
        NiShaderConstantMapEntry* pkEntry, NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        const NiSkinPartition::Partition* pkPartition, 
        NiD3D10GeometryData* pkBuffData, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects, const NiTransform& kWorld, 
        const NiBound& kWorldBound, unsigned int uiPass, 
        bool bGlobal = false, NiExtraData* pkExtraData = 0);
    NiShaderError UpdateGlobalConstantValue(void* pvShaderConstantBuffer, 
        NiShaderConstantMapEntry* pkEntry, unsigned int uiPass);
    NiShaderError UpdateOperatorConstantValue(void* pvShaderConstantBuffer, 
        NiShaderConstantMapEntry* pkEntry, NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects, const NiTransform& kWorld, 
        const NiBound& kWorldBound, unsigned int uiPass, bool bGlobal = false, 
        NiExtraData* pkExtraData = 0);
    NiShaderError UpdateObjectConstantValue(void* pvShaderConstantBuffer, 
        NiShaderConstantMapEntry* pkEntry, NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, 
        const NiSkinPartition::Partition* pkPartition,
        NiD3D10GeometryData* pkBuffData, const NiPropertyState* pkState,
        const NiDynamicEffectState* pkEffects, const NiTransform& kWorld,
        const NiBound& kWorldBound, unsigned int uiPass);

    // Return the data that will be used to fill in the shader constant
    const void* ObtainDefinedConstantValue(NiShaderConstantMapEntry* pkEntry, 
        NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld, const NiBound& kWorldBound, 
        unsigned int uiPass, D3DXMATRIXA16& kTempMatrix);
    const void* ObtainConstantConstantValue(NiShaderConstantMapEntry* pkEntry, 
        unsigned int uiPass);
    const void* ObtainAttributeConstantValue(NiShaderConstantMapEntry* pkEntry,
        NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld, const NiBound& kWorldBound, 
        unsigned int uiPass, bool bGlobal, NiExtraData* pkExtraData, 
        D3DXMATRIXA16& kTempMatrix);
    const void* ObtainGlobalConstantValue(NiShaderConstantMapEntry* pkEntry, 
        unsigned int uiPass);
    const void* ObtainOperatorConstantValue(NiShaderConstantMapEntry* pkEntry, 
        NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld, const NiBound& kWorldBound, 
        unsigned int uiPass, bool bGlobal, NiExtraData* pkExtraData, 
        D3DXMATRIXA16& kTempMatrix);
    const void* ObtainObjectConstantValue(NiShaderConstantMapEntry* pkEntry, 
        NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld, const NiBound& kWorldBound, 
        unsigned int uiPass, D3DXMATRIXA16& kResult);

    // Return the requested data from a specific dynamic effect
    static bool ObtainDataFromDynamicEffect(NiShaderConstantMapEntry* pkEntry, 
        ObjectMappings eMapping, NiDynamicEffect* pkDynEffect, 
        NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, 
        const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, 
        const NiTransform& kWorld,const NiBound& kWorldBound, 
        unsigned int uiPass, D3DXMATRIXA16& kResult);

    // Perform the Operator operations
    static const void* PerformOperatorMultiply(
        const void* pvOperand1, NiShaderAttributeDesc::AttributeType eType1, 
        const void* pvOperand2, NiShaderAttributeDesc::AttributeType eType2, 
        bool bInverse, bool bTranspose, D3DXMATRIXA16& kTempMatrix);
    static const void* PerformOperatorDivide(
        const void* pvOperand1, NiShaderAttributeDesc::AttributeType eType1, 
        const void* pvOperand2, NiShaderAttributeDesc::AttributeType eType2, 
        bool bInverse, bool bTranspose, D3DXMATRIXA16& kTempMatrix);
    static const void* PerformOperatorAdd(
        const void* pvOperand1, NiShaderAttributeDesc::AttributeType eType1, 
        const void* pvOperand2, NiShaderAttributeDesc::AttributeType eType2, 
        bool bInverse, bool bTranspose, D3DXMATRIXA16& kTempMatrix);
    static const void* PerformOperatorSubtract(
        const void* pvOperand1, NiShaderAttributeDesc::AttributeType eType1, 
        const void* pvOperand2, NiShaderAttributeDesc::AttributeType eType2, 
        bool bInverse, bool bTranspose, D3DXMATRIXA16& kTempMatrix);

    // Copy the shader constant data into the shader constant buffer
    static NiShaderError FillShaderConstantBuffer(void* pvShaderConstantBuffer,
        const void* pvSourceData, unsigned int uiStartRegister, 
        unsigned int uiStartElement, unsigned int uiRegisterCount, 
        unsigned int uiElementCount, unsigned int uiArrayCount, 
        bool bPackRegisters, const unsigned short* pusReorderArray);

    // Encode start register/register count for a given shader constant entry
    // based on the current buffer write location
    static void CalculatePackingEntry(unsigned int& uiCurrentRegister, 
        unsigned int& uiCurrentElement, unsigned int uiRegisterCount, 
        unsigned int uiElementCount, unsigned int& uiEncodedStartRegister, 
        unsigned int& uiEncodedRegisterCount, bool bPackRegisters);
    static void CalculatePackingEntryArray(unsigned int uiNumArrayElements, 
        unsigned int& uiCurrentRegister, unsigned int& uiCurrentElement, 
        const unsigned int uiRegisterCount, const unsigned int uiElementCount, 
        unsigned int& uiEncodedStartRegister, 
        unsigned int& uiEncodedRegisterCount, bool bPackRegisters);

    NiTObjectArray<NiShaderConstantMapEntryPtr> m_aspEntries;
    NiD3D10ShaderConstantDataPtr m_spShaderConstantData;

    NiFixedString m_kName;

    bool m_bConstantBufferCurrent;

    static const float ms_fDirLightDistance;
};

NiSmartPointer(NiD3D10ShaderConstantMap);

#endif // NID3D10SHADERCONSTANTMAP_H
