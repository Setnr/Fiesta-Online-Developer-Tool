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

#ifndef NIACTORCOMPONENT_H
#define NIACTORCOMPONENT_H

#include <NiRefObject.h>
#include <NiSmartPointer.h>
#include "NiEntityComponentInterface.h"
#include <NiFlags.h>
#include <NiActorManager.h>

NiSmartPointer(NiActorComponent);

class NIENTITY_ENTRY NiActorComponent : public NiRefObject,
    public NiEntityComponentInterface
{
    NiDeclareFlags(unsigned char);

public:
    // Error strings.
    static NiFixedString ERR_TRANSLATION_NOT_FOUND;
    static NiFixedString ERR_ROTATION_NOT_FOUND;
    static NiFixedString ERR_SCALE_NOT_FOUND;
    static NiFixedString ERR_ACTOR_MANAGER_CREATION_FAILED;

    NiActorComponent();
    NiActorComponent(const NiFixedString& kKfmFilePath,
        unsigned int uiActiveSequenceID, bool bAccumulateTransforms);

    const NiFixedString& GetKfmFilePath() const;
    void SetKfmFilePath(const NiFixedString& kKfmFilePath);

    unsigned int GetActiveSequenceID() const;
    void SetActiveSequenceID(unsigned int uiActiveSequenceID);

    NiBool GetAccumulateTransforms() const;
    void SetAccumulateTransforms(bool bAccumulateTransforms);

    void ResetAnimation();

    NiActorManager* GetActorManager() const;

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    NiActorComponent(NiActorComponent* pkMasterComponent);

    NiBool ShouldReloadActor() const;
    NiBool ShouldChangeSequenceID() const;
    void ClearActor();

    // Flags.
    enum
    {
        LOAD_ERROR_HIT_MASK                 = 0x01,
        KFM_FILE_PATH_CHANGED_MASK          = 0x02,
        ACTIVE_SEQUENCE_ID_CHANGED_MASK     = 0x04,
        ACCUMULATE_TRANSFORMS_CHANGED_MASK  = 0x08
    };
    NiBool GetLoadErrorHit() const;
    void SetLoadErrorHit(bool bLoadErrorHit);
    NiBool GetKfmFilePathChanged() const;
    void SetKfmFilePathChanged(bool bKfmFilePathChanged);
    NiBool GetActiveSequenceIDChanged() const;
    void SetActiveSequenceIDChanged(bool bActiveSequenceIDChanged);
    NiBool GetAccumulateTransformsChanged() const;
    void SetAccumulateTransformsChanged(bool bAccumulateTransformsChanged);

    // Properties.
    NiFixedString m_kKfmFilePath;
    unsigned int m_uiActiveSequenceID;
    bool m_bAccumulateTransforms;

    NiActorManagerPtr m_spActor;
    NiActorComponentPtr m_spMasterComponent;

    // Class name.
    static NiFixedString ms_kClassName;

    // Component name.
    static NiFixedString ms_kComponentName;

    // Property names.
    static NiFixedString ms_kKfmFilePathName;
    static NiFixedString ms_kActiveSequenceIDName;
    static NiFixedString ms_kAccumulateTransformsName;
    static NiFixedString ms_kSequenceIDsName;
    static NiFixedString ms_kSequenceNamesName;
    static NiFixedString ms_kSceneRootPointerName;

    // Property descriptions.
    static NiFixedString ms_kKfmFilePathDescription;
    static NiFixedString ms_kActiveSequenceIDDescription;
    static NiFixedString ms_kAccumulateTransformsDescription;

    // Dependent property names.
    static NiFixedString ms_kTranslationName;
    static NiFixedString ms_kRotationName;
    static NiFixedString ms_kScaleName;

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
        NiFixedString& kData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        const NiFixedString& kData, unsigned int uiIndex);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        unsigned int& uiData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        unsigned int uiData, unsigned int uiIndex);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        bool& bData, unsigned int uiIndex) const;
    virtual NiBool SetPropertyData(const NiFixedString& kPropertyName,
        bool bData, unsigned int uiIndex);
    virtual NiBool GetPropertyData(const NiFixedString& kPropertyName,
        NiObject*& pkData, unsigned int uiIndex) const;
};

#include "NiActorComponent.inl"

#endif // NIACTORCOMPONENT_H
