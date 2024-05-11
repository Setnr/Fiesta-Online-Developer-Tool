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

#ifndef NIINHERITEDTRANSFORMATIONCOMPONENT_H
#define NIINHERITEDTRANSFORMATIONCOMPONENT_H

#include "NiEntityComponentInterface.h"
#include "NiEntityInterface.h"
#include <NiFlags.h>
#include <NiRefObject.h>
#include <NiSmartPointer.h>

class NIENTITY_ENTRY NiInheritedTransformationComponent :
    public NiRefObject, public NiEntityComponentInterface
{
    NiDeclareFlags(unsigned char);

public:
    static NiFixedString ERR_ATTACHMENT_POINT_NOT_FOUND;

    NiInheritedTransformationComponent();
    NiInheritedTransformationComponent(NiEntityInterface* pkSourceEntity,
        const NiFixedString& kAttachmentPointName);
    NiInheritedTransformationComponent(NiEntityInterface* pkSourceEntity,
        const NiFixedString& kAttachmentPointName, bool bInheritTranslation,
        bool bInheritRotation, bool bInheritScale);

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    NiInheritedTransformationComponent(NiEntityInterface* pkSourceEntity,
        const NiFixedString& kAttachmentPointName, bool bInheritTranslation,
        bool bInheritRotation, bool bInheritScale,
        const NiPoint3& kTranslation, const NiMatrix3& kRotation,
        float fScale);

    // Flags.
    enum
    {
        INHERIT_TRANSLATION_MASK                = 0x01,
        INHERIT_ROTATION_MASK                   = 0x02,
        INHERIT_SCALE_MASK                      = 0x04,
        ERROR_RESOLVING_ATTACHMENT_POINT_MASK   = 0x08
    };
    NiBool GetInheritTranslation() const;
    void SetInheritTranslation(bool bInheritTranslation);
    NiBool GetInheritRotation() const;
    void SetInheritRotation(bool bInheritRotation);
    NiBool GetInheritScale() const;
    void SetInheritScale(bool bInheritScale);
    NiBool GetErrorResolvingAttachmentPoint() const;
    void SetErrorResolvingAttachmentPoint(
        bool bErrorResolvingAttachmentPoint);

    // Properties.
    NiEntityInterface* m_pkSourceEntity;
    NiFixedString m_kAttachmentPointName;
    NiPoint3 m_kTranslation;
    NiMatrix3 m_kRotation;
    float m_fScale;

    NiAVObject* m_pkAttachmentPoint;
    mutable bool m_bRetrievingTranslation;
    mutable bool m_bRetrievingRotation;
    mutable bool m_bRetrievingScale;

    // Class name.
    static NiFixedString ms_kClassName;

    // Component name.
    static NiFixedString ms_kComponentName;

    // Property names.
    static NiFixedString ms_kTranslationName;
    static NiFixedString ms_kRotationName;
    static NiFixedString ms_kScaleName;
    static NiFixedString ms_kSourceEntityName;
    static NiFixedString ms_kAttachmentPointNameName;
    static NiFixedString ms_kInheritTranslationName;
    static NiFixedString ms_kInheritRotationName;
    static NiFixedString ms_kInheritScaleName;

    // Property descriptions.
    static NiFixedString ms_kTranslationDescription;
    static NiFixedString ms_kRotationDescription;
    static NiFixedString ms_kScaleDescription;
    static NiFixedString ms_kSourceEntityDescription;
    static NiFixedString ms_kAttachmentPointNameDescription;
    static NiFixedString ms_kInheritTranslationDescription;
    static NiFixedString ms_kInheritRotationDescription;
    static NiFixedString ms_kInheritScaleDescription;

    static NiFixedString ms_kSceneRootPointerName;

public:
    // NiEntityComponentInterface overrides.
    virtual NiEntityComponentInterface* Clone(bool bInheritProperties);
    virtual NiEntityComponentInterface* GetMasterComponent();
    virtual void SetMasterComponent(
        NiEntityComponentInterface* pkMasterComponent);
    virtual void GetDependentPropertyNames(
        NiTObjectSet<NiFixedString>& kDependentPropertyNames);

    // NiEntityPropertyInterface overrides.
    virtual NiBool SetTemplateID(const NiUniqueID& kID);
    virtual NiUniqueID GetTemplateID();
    virtual void AddReference();
    virtual void RemoveReference();
    virtual NiFixedString GetClassName() const;
    virtual NiFixedString GetName() const;
    virtual NiBool SetName(const NiFixedString& kName);
    virtual NiBool IsAnimated() const;
    virtual void Update(NiEntityPropertyInterface* pkParentEntity,
        float fTime, NiEntityErrorInterface* pkErrors,
        NiExternalAssetManager* pkAssetManager);
    virtual void BuildVisibleSet(NiEntityRenderingContext* pkRenderingContext,
        NiEntityErrorInterface* pkErrors);
    virtual void GetPropertyNames(
        NiTObjectSet<NiFixedString>& kPropertyNames) const;
    virtual NiBool CanResetProperty(const NiFixedString& kPropertyName,
        bool& bCanReset) const;
    virtual NiBool ResetProperty(const NiFixedString& kPropertyName);
    virtual NiBool MakePropertyUnique(const NiFixedString& kPropertyName,
        bool& bMadeUnique);
    virtual NiBool GetDisplayName(const NiFixedString& kPropertyName,
        NiFixedString& kDisplayName) const;
    virtual NiBool SetDisplayName(const NiFixedString& kPropertyName,
        const NiFixedString& kDisplayName);
    virtual NiBool GetPrimitiveType(const NiFixedString& kPropertyName,
        NiFixedString& kPrimitiveType) const;
    virtual NiBool SetPrimitiveType(const NiFixedString& kPropertyName,
        const NiFixedString& kPrimitiveType);
    virtual NiBool GetSemanticType(const NiFixedString& kPropertyName,
        NiFixedString& kSemanticType) const;
    virtual NiBool SetSemanticType(const NiFixedString& kPropertyName,
        const NiFixedString& kSemanticType);
    virtual NiBool GetDescription(const NiFixedString& kPropertyName,
        NiFixedString& kDescription) const;
    virtual NiBool SetDescription(const NiFixedString& kPropertyName,
        const NiFixedString& kDescription);
    virtual NiBool GetCategory(const NiFixedString& kPropertyName,
        NiFixedString& kCategory) const;
    virtual NiBool IsPropertyReadOnly(const NiFixedString& kPropertyName,
        bool& bIsReadOnly);
    virtual NiBool IsPropertyUnique(const NiFixedString& kPropertyName,
        bool& bIsUnique);
    virtual NiBool IsPropertySerializable(const NiFixedString& kPropertyName,
        bool& bIsSerializable);
    virtual NiBool IsPropertyInheritable(const NiFixedString& kPropertyName,
        bool& bIsInheritable);
    virtual NiBool IsExternalAssetPath(const NiFixedString& kPropertyName,
        unsigned int uiIndex, bool& bIsExternalAssetPath) const;
    virtual NiBool GetElementCount(const NiFixedString& kPropertyName,
        unsigned int& uiCount) const;
    virtual NiBool SetElementCount(const NiFixedString& kPropertyName,
        unsigned int uiCount, bool& bCountSet);
    virtual NiBool IsCollection(const NiFixedString& kPropertyName,
        bool& bIsCollection) const;
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        float& fData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        float fData, unsigned int uiIndex);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        bool& bData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        bool bData, unsigned int uiIndex);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        NiPoint3& kData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        const NiPoint3& kData, unsigned int uiIndex);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        NiMatrix3& kData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        const NiMatrix3& kData, unsigned int uiIndex);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        NiFixedString& kData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        const NiFixedString& kData, unsigned int uiIndex);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        NiEntityInterface*& pkData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        NiEntityInterface* pkData, unsigned int uiIndex);
};

NiSmartPointer(NiInheritedTransformationComponent);

#endif  // #ifndef NIINHERITEDTRANSFORMATIONCOMPONENT_H
