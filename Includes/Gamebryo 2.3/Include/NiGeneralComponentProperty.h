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

#ifndef NIGENERALCOMPONENTPROPERTY_H
#define NIGENERALCOMPONENTPROPERTY_H

#include "NiEntityLibType.h"
#include <NiRefObject.h>
#include <NiSmartPointer.h>
#include <NiFixedString.h>
#include <NiBool.h>

class NiGeneralComponent;

class NIENTITY_ENTRY NiGeneralComponentProperty : public NiRefObject
{
public:
    NiGeneralComponentProperty(const NiFixedString& kPropertyName,
        const NiFixedString& kDisplayName,
        const NiFixedString& kPrimitiveType,
        const NiFixedString& kSemanticType,
        const NiFixedString& kDescription, bool IsCollection = false);
    virtual ~NiGeneralComponentProperty();

    const NiFixedString& GetPropertyName() const;
    void SetPropertyName(const NiFixedString& kPropertyName);

    const NiFixedString& GetDisplayName() const;
    void SetDisplayName(const NiFixedString& kDisplayName);

    const NiFixedString& GetPrimitiveType() const;
    void SetPrimitiveType(const NiFixedString& kPrimitiveType);

    const NiFixedString& GetSemanticType() const;
    void SetSemanticType(const NiFixedString& kSemanticType);

    const NiFixedString& GetDescription() const;
    void SetDescription(const NiFixedString& kDescription);

    //functions for managing collections
    NiBool GetIsCollection() const;
    void SetIsCollection(bool bIsCollection);

    unsigned int GetCollectionSize() const;
    void SetCollectionSize(unsigned int uiSize);

    // Functions for setting data.
    template<class T> void SetData(T* pvDataArray, size_t stArrayCount,
        unsigned int uiIndex = 0);

    // Functions for getting data.
    void* GetData(unsigned int uiIndex = 0) const;
    size_t GetDataSizeInBytes(unsigned int uiIndex = 0) const;


    NiGeneralComponentProperty* Clone();

private:
    NiFixedString m_kPropertyName;
    NiFixedString m_kDisplayName;
    NiFixedString m_kPrimitiveType;
    NiFixedString m_kSemanticType;
    NiFixedString m_kDescription;
    bool m_bIsCollection;
    NiTPrimitiveArray<void*> m_kDataPointers;
    NiTPrimitiveArray<size_t> m_kDataSizesInBytes;
};

NiSmartPointer(NiGeneralComponentProperty);

#include "NiGeneralComponentProperty.inl"

#endif // NIGENERALCOMPONENTPROPERTY_H
