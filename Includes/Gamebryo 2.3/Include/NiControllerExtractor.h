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

#ifndef NICONTROLLEREXTRACTOR_H
#define NICONTROLLEREXTRACTOR_H

#include "NiControllerExtractorLibType.h"
#include <NiControllerSequence.h>

class NiCommand;
class NiTransformInterpolator;
class NiMultiTargetTransformController;

class NICONTROLLEREXTRACTOR_ENTRY NiControllerExtractor
{
public:
    enum AxisFlags
    {
        NO_FLAGS  = 0x00,
        X_FLAG    = 0x01,
        Y_FLAG    = 0x02,
        Z_FLAG    = 0x04,
        ALL_FLAGS = 0x07
    };

    class NICONTROLLEREXTRACTOR_ENTRY SequenceInfo : public NiRefObject
    {
    public:
        SequenceInfo();
        SequenceInfo(const char* pcName, float fStartTime, float fEndTime,
            bool bLoop, bool bSmoothRotation, AxisFlags eRotFlags,
            unsigned int uiTransFlags, const NiMatrix3& kAccumAxis,
            NiTextKeyExtraData* pkTextKeys, NiAVObject* pkCharacterRoot,
            NiAVObject* pkLayerRoot, NiAVObject* pkAccumRoot);
        ~SequenceInfo();

        void SetName(const char* pcName);
        NiAVObject* GetSequenceRoot() const;

        char* m_pcName;
        float m_fStartTime;
        float m_fEndTime;
        bool m_bLoop;
        bool m_bSmoothRotation;
        AxisFlags m_eRotFlags;
        unsigned int m_uiTransFlags;
        NiMatrix3 m_kAccumAxis;
        NiTextKeyExtraDataPtr m_spTextKeys;
        NiAVObjectPtr m_spCharacterRoot;
        NiAVObjectPtr m_spLayerRoot;
        NiAVObjectPtr m_spAccumRoot;
    };
    typedef NiPointer<SequenceInfo> SequenceInfoPtr;

    // Character Compress Animation Global Info
    class NICONTROLLEREXTRACTOR_ENTRY NiCCAGlobalInfo : public NiRefObject
    {
    public:
        NiCCAGlobalInfo();
        ~NiCCAGlobalInfo(){};

        NiAVObjectPtr m_spCharacterRoot;
        bool m_bUseBSplineCompression;
        float m_fBSplineCompressionRatio;
        bool m_bUseCompactControlPoints;
    };
    typedef NiPointer<NiCCAGlobalInfo> NiCCAGlobalInfoPtr;

    // Character Compress Animation Info
    class NICONTROLLEREXTRACTOR_ENTRY NiCCAInfo : public NiRefObject
    {
    public:
        NiCCAInfo();
        NiCCAInfo(NiCCAGlobalInfo* pkInfo);
        ~NiCCAInfo();

        void SetSequenceName(const char* pcName);

        NiAVObjectPtr m_spCharacterRoot;
        NiAVObjectPtr m_spObject;
        char* m_pcSequenceName;
        bool m_bUseBSplineCompression;
        float m_fBSplineCompressionRatio;
        bool m_bUseCompactControlPoints;
    };
    typedef NiPointer<NiCCAInfo> NiCCAInfoPtr;


    typedef NiTObjectArray<SequenceInfoPtr> InfoArray;
    typedef NiTObjectArray<NiControllerSequencePtr> SequenceArray;
    typedef NiTObjectArray<NiInterpolatorPtr> InterpolatorArray;
    typedef NiTPrimitiveArray<NiControllerSequence::IDTag*> IDArray;
    typedef NiTObjectArray<NiInterpControllerPtr> ControllerArray;
    typedef NiTPrimitiveArray<char*> NameArray;
    typedef NiTObjectArray<NiCCAGlobalInfoPtr> NiCCAGlobalInfoArray;
    typedef NiTObjectArray<NiCCAInfoPtr> NiCCAInfoArray;

    // This function provides for automatic retrieval of sequence info from
    // the NiTextKeyExtraData object that resides in the scene graph below
    // pkScene. This is primarily a convenience function for populating
    // NiControllerSequenceInfo objects appropriately. These
    // NiControllerSequenceInfo objects are then sent into ExtractControllers.
    static void ProcessTextKeys(NiAVObject* pkScene, InfoArray& kInfoArray,
        NiCCAGlobalInfoArray& kCCAGlobalInfoArray, 
        NiCCAInfoArray& kCCAInfoArray,
        NiAVObject* pkLastCharacterRoot = NULL, 
        NiCCAGlobalInfo* pkLastGlobalInfo = NULL);

    // Use this function to create a single sequence from the scene graph
    // below pkScene. If bRemoveInterpolators is true, all interpolators will
    // be removed from the scene graph. This should only be done if no
    // more sequences need to be extracted from the scene.
    static NiControllerSequence* BuildSequence(SequenceInfo& kInfo,
        bool bRemoveInterpolators = false, bool bStoreControllers = false,
        const char* pcExcludedControllerTypes = NULL);

    // Use this function to create multiple sequences at a time from the
    // scene graph below pkScene. If bRemoveInterpolators is true, all
    // interpolators will be removed from the scene graph. This should only be
    // done if no more sequences need to be extracted from the scene.
    static void BuildSequences(const InfoArray& kInfoArray,
        SequenceArray& kSequenceArray, bool bRemoveInterpolators = true,
        bool bStoreControllers = false,
        const char* pcExcludedControllerTypes = NULL);

    // This function removes all interpolators in the scene graph below
    // pkObject. It should only be called if no more sequences need to be
    // extracted from the scene.
    static void RemoveInterpolators(NiAVObject* pkObject,
        const char* pcExcludedControllerTypes = NULL);

protected:
    static NiAVObject* FindAccumRoot(NiAVObject* pkObject);
    static bool ParseAccumulationOptions(NiCommand& kCmd,
        AxisFlags& eRotFlags, unsigned int& uiTransFlags, bool& bLoop,
        bool& bSmoothRotation, NiMatrix3& kAccumAxis);
    static void EnsureUniqueNames(NiAVObject* pkObject);
    static void EnsureUniqueNameRecursive(NiAVObject* pkObject,
        NameArray& kNameArray, unsigned int& uiIndex);
    static void ExtractInterpolators(NiAVObject* pkObject,
        const SequenceInfo& kInfo, InterpolatorArray& kInterpolatorArray,
        IDArray& kIDArray, bool bStoreControllers,
        ControllerArray& kControllerArray, float& fFrequency,
        float& fPhase,
        NiTPointerMap<NiInterpolator*, bool>& kProcessedInterpolators,
        const char* pcExcludedControllerTypes);

    static void RearrangeTransforms(NiAVObject* pkRoot,
        NiControllerSequence* pkSequence, const SequenceInfo& kInfo,
        bool bStoreControllers, const char* pcExcludedControllerTypes);
    static NiNode* GetChildNode(NiNode* pkBaseNode);
    static NiTransformInterpolator* GetBaseInterpolator(
        NiControllerSequence* pkSequence, NiNode* pkBaseNode, bool bLoop,
        bool bStoreControllers);
    static NiTransformInterpolator* GetChildInterpolator(
        NiTransformInterpolator* pkBaseInterp,
        NiControllerSequence* pkSequence, NiNode* pkChildNode,
        bool bCreateKeys, bool bStoreControllers);
    static void FillDerivedValues(NiTransformInterpolator* pkInterp,
        NiAVObject* pkObject);

    static void FactorRot(const NiMatrix3& kFullRot, AxisFlags eRotFlags,
        NiMatrix3& kAccumRot, NiMatrix3& kNonAccumRot);
    static void FactorTrans(const NiPoint3& kFullTrans,
        unsigned int uiTransFlags, NiPoint3& kAccumTrans,
        NiPoint3& kNonAccumTrans);

    static void ConsolidateTransformControllers(NiAVObject* pkSequenceRoot,
        const NiAVObject* pkAccumRoot);
    static unsigned short FindNumTransformControllers(NiAVObject* pkObject,
        const NiAVObject* pkAccumRoot);
    static void ReplaceTransformControllers(NiAVObject* pkObject,
        const NiAVObject* pkAccumRoot,
        NiMultiTargetTransformController* pkMultiCtlr);
};

#endif  // #ifndef NICONTROLLEREXTRACTOR_H
