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

#ifndef NIGEOMETRYDATA_H
#define NIGEOMETRYDATA_H

#include "NiBound.h"
#include "NiColor.h"
#include "NiObject.h"
#include "NiPoint3.h"
#include "NiPoint2.h"
#include "NiObjectGroup.h"
#include "NiShaderDeclaration.h"
class NiAdditionalGeometryData;
NiSmartPointer(NiAdditionalGeometryData);

class NIMAIN_ENTRY NiGeometryData : public NiObject
{
    NiDeclareRTTI;
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;

public:
    virtual ~NiGeometryData();

    // vertex access
    void CreateVertices(bool bInitialize);
    unsigned short GetVertexCount() const;
    NiPoint3* GetVertices();
    const NiPoint3* GetVertices() const;
    virtual void SetActiveVertexCount(unsigned short usActive);
    unsigned short GetActiveVertexCount() const;

    // normal access
    void CreateNormals(bool bInitialize);
    NiPoint3* GetNormals();
    const NiPoint3* GetNormals() const;

    enum DataFlags
    {
        NBT_METHOD_NONE         = 0x0000,
        NBT_METHOD_NDL          = 0x1000,
        NBT_METHOD_MAX          = 0x2000,
        NBT_METHOD_ATI          = 0x3000,
        NBT_METHOD_MASK         = 0xF000,
        TEXTURE_SET_MASK        = 0x003F
    };

    DataFlags GetNormalBinormalTangentMethod() const;
    void StoreNormalBinormalTangent(DataFlags eNBTMethod);

    // bound access
    void SetBound(const NiBound& kBound);
    NiBound& GetBound();
    const NiBound& GetBound() const;

    // color access
    void CreateColors(bool bInitialize);
    NiColorA* GetColors();
    const NiColorA* GetColors() const;

    // texture coordinate access
    void CreateTextures(bool bInitialize, unsigned short usSets);
    unsigned short GetTextureSets() const;
    NiPoint2* GetTextures();
    const NiPoint2* GetTextures() const;
    NiPoint2* GetTextureSet(unsigned short usSet);
    const NiPoint2* GetTextureSet(unsigned short usSet) const;
    void AppendTextureSet(NiPoint2* pkSet);

    // reallocation support
    void Replace(unsigned short usVertices, NiPoint3* pkVertex,
        NiPoint3* pkNormal, NiColorA* pkColor, NiPoint2* pkTexture,
        unsigned short usNumTextureSets, DataFlags eNBTMethod);

    enum Consistency
    {
        MUTABLE             = 0x0000,
        STATIC              = 0x4000,
        VOLATILE            = 0x8000,
        CONSISTENCY_MASK    = 0xF000
    };

    Consistency GetConsistency() const;
    void SetConsistency(Consistency eConsistency);

    // MarkAsChanged should be called if vertices, normals, colors, texture
    // coordinates, triangle indices, or the triangle count are changed.
    // The renderers need to know when any of that data has changed.
    // Values from the following enum should be OR'd together and passed to 
    // MarkAsChanged to indicate what values have changed. Note that the
    // enums for indicating that the triangle indices or the triangle count
    // have changed are enumerated in the NiGeometryData-derived class
    // NiTriBasedGeomData.
    enum
    {
        VERTEX_MASK     = 0x0001,
        NORMAL_MASK     = 0x0002,
        COLOR_MASK      = 0x0004,
        TEXTURE_MASK    = 0x0008,
        DIRTY_MASK      = 0x0FFF
    };

    void MarkAsChanged(unsigned short usFlags);

    // KEEP flags inform renderer that original data cannot be thrown away
    // when the object is precached. An app would use these flags to preserve
    // data so it can be used for purposes other than rendering.

    enum
    {
        KEEP_XYZ          = 0x01,
        KEEP_NORM         = 0x02,
        KEEP_COLOR        = 0x04,
        KEEP_UV           = 0x08,
        KEEP_INDICES      = 0x10,
        KEEP_BONEDATA     = 0x20,

        KEEP_ALL          = (KEEP_XYZ | KEEP_NORM | KEEP_COLOR | KEEP_UV |
                             KEEP_INDICES | KEEP_BONEDATA)
    };

    unsigned int GetKeepFlags() const;
    void SetKeepFlags(unsigned int uiKeepFlags);

    // COMPRESS flags inform renderer to certain compress packed parameters.

    enum
    {
        COMPRESS_NORM     = 0x01,
        COMPRESS_COLOR    = 0x02,
        COMPRESS_UV       = 0x04,
        COMPRESS_WEIGHT   = 0x08,
        COMPRESS_POSITION = 0x10,
        
        COMPRESS_ALL      = (COMPRESS_NORM | COMPRESS_COLOR | COMPRESS_UV |
                             COMPRESS_WEIGHT | COMPRESS_POSITION)
    };
    
    // interface to renderer-specific geometry data
    class NIMAIN_ENTRY RendererData : public NiMemObject
    {
    public:
        virtual ~RendererData(){};
        virtual bool ContainsVertexData(NiShaderDeclaration::ShaderParameter 
            eParameter) const = 0;
    };
    RendererData* GetRendererData() const;
    void SetRendererData(RendererData* pkBuffData);

    unsigned int GetCompressFlags() const;
    void SetCompressFlags(unsigned int uiCompressFlags);

    NiAdditionalGeometryData* GetAdditionalGeometryData();
    void SetAdditionalGeometryData(NiAdditionalGeometryData* pkAddData);

    virtual bool ContainsVertexData(NiShaderDeclaration::ShaderParameter 
        eParameter) const;

    // *** begin Emergent internal use only ***

    // block allocation
    virtual unsigned int GetBlockAllocationSize() const;
    virtual NiObjectGroup* GetGroup() const;
    virtual void SetGroup(NiObjectGroup* pkGroup);

    // revision ID access. Used by renderers.
    unsigned short GetRevisionID() const;
    void ClearRevisionID();

    // unique ID access
    unsigned short GetUniqueID() const;

    // For temporarily using data only.  This call is like Replace(), 
    // but does not delete any pointers.  Be careful to call this only 
    // on objects created by the default (empty) constructor to prevent 
    // memory leaks.  These pointers will be owned (and deleted upon 
    // destruction) by this object so use this call carefully.
    void SetData(unsigned short usVertices, NiPoint3* pkVertex,
        NiPoint3* pkNormal, NiColorA* pkColor, NiPoint2* pkTexture,
        unsigned short usNumTextureSets, DataFlags eNBTMethod);
    
    // updates
    virtual void CalculateNormals() = 0;

    typedef unsigned char VertexMask;

    void IndexSort(unsigned short*& pusIndex0, unsigned short*& pusIndex1,
         VertexMask mask = ~0) const;
    int VertexCompare(unsigned short v0, unsigned short v1, 
        VertexMask mask = ~0) const;
    // *** end Emergent internal use only ***

protected:
    // The constructed object is given ownership of the input arrays and
    // has the responsibility for deleting them when finished with them.
    NiGeometryData (unsigned short usVertices, NiPoint3* pkVertex,
        NiPoint3* pkNormal, NiColorA* pkColor, NiPoint2* pkTexture,
        unsigned short usNumTextureSets, DataFlags eNBTMethod);

    // streaming support
    NiGeometryData();

    void IndexQSort(unsigned short *pusIndex, VertexMask mask, int l, int r)
        const;
    int ChoosePivot(unsigned short *pusIndex, VertexMask mask, int l, int r)
        const;

    void SetNumTextureSets(unsigned short usNumTextureSets);
    void SetNormalBinormalTangentMethod(DataFlags eNBTMethod);

    virtual void SetVertexCount( unsigned short usVertices);

    // block allocation
    NiObjectGroup* m_pkGroup;

    // model data (vertices and vertex attributes)
    unsigned short m_usVertices;
    unsigned short m_usID;

    NiBound m_kBound;
    NiPoint3* m_pkVertex;
    NiPoint3* m_pkNormal;
    NiColorA* m_pkColor;
    NiPoint2* m_pkTexture;

    unsigned short m_usActiveVertices;

    unsigned short m_usDataFlags;

    // Note that a revision ID of 0 implies that the object is static and can
    // be optimized in a renderer - it implies that the application does not
    // intend to morph the data
    unsigned short m_usDirtyFlags;

    unsigned char m_ucKeepFlags;
    unsigned char m_ucCompressFlags;

    
    NiAdditionalGeometryDataPtr m_spAdditionalGeomData;

    RendererData* m_pkBuffData;

    static unsigned short ms_usNextID;
    static NiCriticalSection ms_kCritSec;
};

typedef NiPointer<NiGeometryData> NiGeometryDataPtr;

#include "NiGeometryData.inl"

#endif // NIGEOMETRYDATA_H
