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

#ifndef NIGLOBALCONSTANTENTRY_H
#define NIGLOBALCONSTANTENTRY_H

#include "NiShaderConstantMapEntry.h"
#include "NiFixedString.h"

class NIMAIN_ENTRY NiGlobalConstantEntry : public NiRefObject
{
public:
    NiGlobalConstantEntry();
    NiGlobalConstantEntry(const char* pcKey, 
        NiShaderAttributeDesc::AttributeType eType, 
        unsigned int uiDataSize, const void* pvDataSource);
    ~NiGlobalConstantEntry();

    //*** Access functions
    NiFixedString GetKey();
    void SetKey(const NiFixedString& kKey);

    const NiShaderAttributeDesc::AttributeType GetType() const;
    void SetType(NiShaderAttributeDesc::AttributeType eType);

    bool IsBool() const;
    bool IsString() const;
    bool IsUnsignedInt() const;
    bool IsFloat() const;
    bool IsPoint2() const;
    bool IsPoint3() const;
    bool IsPoint4() const;
    bool IsMatrix3() const;
    bool IsMatrix4() const;
    bool IsColor() const;
    bool IsTexture() const;

    unsigned int GetDataSize() const;
    void* GetDataSource() const;
    void GetData(unsigned int& uiDataSize, const void*& pvDataSource) const;
    void SetData(unsigned int uiDataSize, const void* pvDataSource);

    // *** begin Emergent internal use only
    // *** end Emergent internal use only

protected:
    // The name of the attribute this constant is mapped to,
    // or the name of the pre-defined/constant entry.
    NiFixedString m_kKey;

    // Type of data
    NiShaderAttributeDesc::AttributeType m_eType;

    // The data itself - only used in CONSTANT and some DEFINED mappings.
    // If the data is a constant, the entry will delete, otherwise, it is
    // left up to the one who set it.
    unsigned int m_uiDataSize;  // Size of the data element
    void* m_pvDataSource;       // Pointer to source of data
};

#include "NiGlobalConstantEntry.inl"

#endif  //#ifndef NIGLOBALCONSTANTENTRY_H
