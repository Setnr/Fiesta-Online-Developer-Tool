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

#ifndef NICONTROLLERSEQUENCE_H
#define NICONTROLLERSEQUENCE_H

#include <NiObject.h>
#include "NiAnimationLibType.h"
#include "NiInterpController.h"
#include "NiTextKeyExtraData.h"
#include "NiBlendInterpolator.h"
#include "NiStringPalette.h"
#include <NiMatrix3.h>
#include <NiPoint3.h>

NiSmartPointer(NiControllerSequence);
typedef NiTObjectArray<NiControllerSequencePtr>     
    NiControllerSequencePointerArray;
typedef NiTObjectSet<NiControllerSequencePtr> NiControllerSequencePointerSet;

class NiNode;
class NiControllerManager;
class NiSequence;
class NiObjectNET;
class NiAVObjectPalette;
class NiTextKeyMatch;

class NIANIMATION_ENTRY NiControllerSequence : public NiObject
{
public:
    NiDeclareRTTI;
    NiDeclareClone(NiControllerSequence);
    NiDeclareStream;

    enum AnimState
    {
        INACTIVE,
        ANIMATING,
        EASEIN,
        EASEOUT,
        TRANSSOURCE,
        TRANSDEST,
        MORPHSOURCE
    };
    
    static const float INVALID_TIME;
    static const unsigned int INVALID_INDEX;

    // Callback object. ActivationChanged is called whenever the state
    // of this sequence changes to or from INACTIVE. This is a pure virtual
    // function, so feel free to use it with multiple inheritance.
    class NIANIMATION_ENTRY CallbackObject
    {
    public:
        virtual ~CallbackObject();
        virtual void ActivationChanged(NiControllerSequence* pkSequence,
            AnimState eState) = 0;
    };

    // Constructor and destructor.
    NiControllerSequence(const NiFixedString& kName,
        unsigned int uiArraySize = 12,
        unsigned int uiArrayGrowBy = 12);
    ~NiControllerSequence();

    // Static functions for creating sequences from KF files.
    static NiControllerSequencePtr CreateSequenceFromFile(
        const char* pcFilename, unsigned int uiIndex);
    static NiControllerSequencePtr CreateSequenceFromFile(
        const NiStream& kStream, unsigned int uiIndex);
    static bool CreateAllSequencesFromFile(const char* pcFilename,
        NiControllerSequencePointerArray& kSequences);
    static bool CreateAllSequencesFromFile(const NiStream& kStream,
        NiControllerSequencePointerArray& kSequences);

    // Functions for getting and setting the sequence name.
    const NiFixedString& GetName() const;
    void SetName(const NiFixedString& kName);

    // Functions for getting and setting the text keys.
    NiTextKeyExtraData* GetTextKeys() const;
    void SetTextKeys(NiTextKeyExtraData* pkKeys);

    // Function to reset the sequence to the beginning.
    void ResetSequence();

    // Function that returns the animation state for this sequence.
    AnimState GetState() const;
    
    // Functions related to callback objects
    void AddActivationCallbackObject(CallbackObject* pkCallback);
    void RemoveActivationCallbackObject(CallbackObject* pkCallback);
    unsigned int GetActivationCallbackCount() const;
    CallbackObject* GetActivationCallbackObjectAt(unsigned int uiIndex);

    // Functions for getting and setting the offset for this sequence.
    float GetOffset() const;
    void SetOffset(float fOffset);

    // Functions for adding and removing interpolators.
    unsigned int AddInterpolator(NiInterpolator* pkInterpolator,
        const NiFixedString& kAVObjectName, const NiFixedString& kPropertyType,
        const NiFixedString& kCtlrType, const NiFixedString& kCtlrID,
        const NiFixedString& kInterpolatorID);
    NiInterpolatorPtr RemoveInterpolator(unsigned int uiIndex);

    // Functions for retrieving interpolators and other information.
    unsigned int GetArraySize() const;
    void SetArraySize(unsigned int uiNewArraySize);
    unsigned int GetArrayGrowBy() const;
    void SetArrayGrowBy(unsigned int uiArrayGrowBy);
    NiInterpolator* GetInterpolatorAt(unsigned int uiIndex) const;
    void GetInterpInfoAt(unsigned int uiIndex, const char*& pcAVObjectName,
        const char*& pcPropertyType, const char*& pcCtlrType,
        const char*& pcCtlrID, const char*& pcInterpolatorID);
    NiInterpController* GetControllerAt(unsigned int uiIndex) const;
    NiObjectNET* GetTargetAt(unsigned int uiIndex) const;
    NiBlendInterpolator* GetBlendInterpolatorAt(unsigned int uiIndex) const;
    unsigned char GetBlendIdxAt(unsigned int uiIndex);

    // Functions for getting and setting the sequence weight.
    float GetSequenceWeight() const;
    void SetSequenceWeight(float fWeight);

    // Function for looking up the time of a text key.
    float GetTimeAt(const NiFixedString& kTextKey, float fCurrentTime = 0.0f);
    float GetTimeAt(NiTextKeyMatch * pkMatchObject, float fCurrentTime = 0.0f);

    // Function for looking up the key time for a particular text key.
    float GetKeyTimeAt(const NiFixedString& kTextKey) const;
    float GetKeyTimeAt(NiTextKeyMatch * pkMatchObject) const;

    // Adjusted functions adjust by frequency of sequence.
    float GetKeyTimeAtDivFreq(const NiFixedString& kTextKey) const;
    float GetBeginKeyTimeDivFreq() const;
    float GetEndKeyTimeDivFreq() const;
    float TimeDivFreq(float fTime) const;
    float TimeMultFreq(float fTime) const;

    // Functions for getting and setting timing variables.
    NiTimeController::CycleType GetCycleType() const;
    bool SetCycleType(NiTimeController::CycleType eType);
    float GetFrequency() const;
    void SetFrequency(float fFrequency);
    float GetBeginKeyTime() const;
    void SetBeginKeyTime(float fBeginKeyTime);
    float GetEndKeyTime() const;
    void SetEndKeyTime(float fEndKeyTime);
    float GetLength();

    // Functions to provide easy access to the last stored time values.
    float GetLastTime() const;
    float GetLastScaledTime() const;

    // *** Begin deprecated functions ***
    // Deprecated function for getting the object name at an index. It
    // concatenates the AVObjectName and PropertyType from the IDTag at the
    // specified index and returns a newly allocated string in pcObjectName.
    // NOTE: pcObjectName must be deleted by the calling function to avoid
    //       a memory leak.
    void GetObjectNameAt(unsigned int uiIndex, char*& pcObjectName) const;

    // Deprecated functions for adding and removing name/controller pairs.
    // New code should use AddInterpolator and RemoveInterpolator instead.
    bool AddNameControllerPair(const char* pcObjectName,
        NiTimeController* pkController);
    void RemoveNameControllerPair(unsigned int uiIndex);
    // *** End deprecated functions ***

    // *** begin Emergent internal use only ***
    class NIANIMATION_ENTRY IDTag : public NiMemObject
    {
    public:
        IDTag(const NiFixedString& kAVObjectName = NULL,
            const NiFixedString& kPropertyType = NULL,
            const NiFixedString& kCtlrType = NULL,
            const NiFixedString& kCtlrID = NULL,
            const NiFixedString& kInterpolatorID = NULL);
        IDTag(const IDTag& kOther);

        const NiFixedString& GetAVObjectName() const;
        void SetAVObjectName(const NiFixedString& kAVObjectName);

        const NiFixedString& GetPropertyType() const;
        void SetPropertyType(const NiFixedString& kPropertyType);

        const NiFixedString& GetCtlrType() const;
        void SetCtlrType(const NiFixedString& kCtlrType);

        const NiFixedString& GetCtlrID() const;
        void SetCtlrID(const NiFixedString& kCtlrID);

        const NiFixedString& GetInterpolatorID() const;
        void SetInterpolatorID(const NiFixedString& kInterpolatorID);

        IDTag& operator=(const IDTag& kTag);

        void ClearValues();

        // *** begin Emergent internal use only ***

        // Streaming functions.
        void RegisterStreamables(NiStream& kStream);
        void SaveBinary(NiStream& kStream);
        void LoadBinary(NiStream& kStream,
             void*& pvDeprecatedExtraData);
        void HandleDeprecatedExtraData(NiStringPalette* pkPalette,
            NiStream& kStream);
        bool IsEqual(const IDTag& kDest);
        // *** begin Emergent internal use only ***

    protected:
        NiFixedString m_kAVObjectName;
        NiFixedString m_kPropertyType;
        NiFixedString m_kCtlrType;
        NiFixedString m_kCtlrID;
        NiFixedString m_kInterpolatorID;
    };
    unsigned int AddInterpolator(NiInterpolator* pkInterpolator,
        const IDTag& kIDTag);
    unsigned int GetIDTagCount();
    IDTag* GetIDTag(unsigned int uiTagIndex);

    void Update(float fTime, bool bUpdateInterpolators = true);

    NiControllerManager* GetOwner() const;
    bool SetOwner(NiControllerManager* pkOwner);

    bool StoreTargets(NiAVObject* pkRoot);
    void ClearTargets();

    bool Activate(char cPriority, bool bStartOver, float fWeight,
        float fEaseInTime, NiControllerSequence* pkTimeSyncSeq,
        bool bTransition);
    bool Deactivate(float fEaseOutTime, bool bTransition);

    bool StartBlend(NiControllerSequence* pkDestSequence, float fDuration,
        const char* pcDestTextKey, int iPriority, float fSourceWeight,
        float fDestWeight, NiControllerSequence* pkTimeSyncSeq);
    bool StartBlend(NiControllerSequence* pkDestSequence, float fDuration,
        float fDestFrame, int iPriority, float fSourceWeight,
        float fDestWeight, NiControllerSequence* pkTimeSyncSeq);

    bool StartMorph(NiControllerSequence* pkDestSequence, float fDuration,
        int iPriority, float fSourceWeight, float fDestWeight);

    void PopulatePoseSequence(NiControllerSequence* pkPoseSequence,
        unsigned int uiArraySize,
        NiControllerSequence* pkSequenceToSynchronize);

    // Accumulation functions
    void SetAccumRootName(const NiFixedString& kAccumName);
    const NiFixedString& GetAccumRootName() const;
    NiAVObject* GetAccumRoot() const;
    void SetAccumTranslation(
        const bool bAccumX, const bool bAccumY, const bool bAccumZ);
    void GetAccumTranslation(
        bool& bAccumX, bool& bAccumY, bool& bAccumZ) const;
    void SetAccumRotation(
        const bool bAccumX, const bool bAccumY, const bool bAccumZ);
    void GetAccumRotation(
        bool& bAccumX, bool& bAccumY, bool& bAccumZ) const;
    void SetAccumFrontFacing(const unsigned char ucAxis,
        const bool bIsNegative);
    void GetAccumFrontFacing(unsigned char& ucAxis, bool& bIsNegative) const;

    // This function is only called by NiControllerExtractor to support
    // storing controllers in NiControllerSequence objects for compatibility
    // reasons.
    void SetControllerAt(NiInterpController* pkInterpCtlr,
        unsigned int uiIndex);

    // This function is only called by the tools to post-process interpolators
    // once they've been attached to a sequence
    void ReplaceInterpolatorAt(NiInterpolator* pkInterp, 
        unsigned int uiIndex);

    bool CanSyncTo(NiControllerSequence* pkTargetSequence);

    float FindCorrespondingMorphFrame(NiControllerSequence* pkSourceSequence,
        float fSourceSequenceTime);
    float ComputeScaledTime(float fTime, bool bStoreLastTime = true);

    float GetEaseEndTime() const;
    float GetDestFrame() const;

    // Cloning support.
    virtual void ProcessClone(NiCloningProcess& kCloning);
    virtual void PostLinkObject(NiStream& kStream);
    // *** end Emergent internal use only ***

protected:
    // For use with cloning and streaming only.
    NiControllerSequence();

    void SortByBlendInterpolator();

    // Internal storage structures.
    class InterpArrayItem : public NiMemObject
    {
    public:
        InterpArrayItem();

        void ClearValues();

        // Streaming functions.
        void RegisterStreamables(NiStream& kStream);
        void SaveBinary(NiStream& kStream);
        void LoadBinary(NiStream& kStream);
        bool IsEqual(const InterpArrayItem& kDest);

        NiInterpolatorPtr m_spInterpolator;
        NiInterpControllerPtr m_spInterpCtlr;
        NiBlendInterpolator* m_pkBlendInterp;
        unsigned char m_ucBlendIdx;
    };

    void AttachInterpolators(char cPriority = 0);
    void DetachInterpolators();

    bool ConvertOldNameControllerPair(const char* pcObjectName,
        NiInterpController* pkController);

    void SetInterpsWeightAndTime(float fWeight, float fEaseSpinner,
        float fTime);

    bool VerifyDependencies(NiControllerSequence* pkSequence);
    bool VerifyMatchingMorphKeys(NiControllerSequence* pkTimeSyncSeq);

    bool ReallocateArrays();
    bool ResolveTransformInterpolators(NiAVObject* pkRoot,
        NiAVObjectPalette* pkObjectPalette);

    // The name of this sequence.
    NiFixedString m_kName;

    // Storage arrays for interpolators and ID tags.
    unsigned int m_uiArraySize;
    unsigned int m_uiArrayGrowBy;
    InterpArrayItem* m_pkInterpArray;
    IDTag* m_pkIDTagArray;

    // The weight of the sequence.
    float m_fSeqWeight;

    // Text keys which point to times in the animation
    NiTextKeyExtraDataPtr m_spTextKeys;

    // Timing variables.
    NiTimeController::CycleType m_eCycleType;
    float m_fFrequency;
    float m_fBeginKeyTime;
    float m_fEndKeyTime;
    float m_fLastTime;
    float m_fWeightedLastTime;
    float m_fLastScaledTime;

    // The NiControllerManager that owns this sequence.
    NiControllerManager* m_pkOwner;

    // The current state of the sequence.
    AnimState m_eState;

    // Callback objects
    NiTPrimitiveArray<CallbackObject*> m_kActivationCallbacks;

    // Variables used for time measurement.
    float m_fOffset;
    float m_fStartTime;
    float m_fEndTime;
    float m_fDestFrame;

    // Variables used for time synchronization.
    NiControllerSequence* m_pkPartnerSequence;

    // NiStringPalettes were used to allocate strings
    // for prior versions of the engine. This member
    // is here purely for successful streaming of old sequences
    // and will only be non-NULL if in streaming code.
    // Version begin use: 10, 1, 0, 113
    // Version end use:   20, 1, 0, 1
    NiStringPalettePtr m_spDeprecatedStringPalette;

    // Variables used for animation accumulation.
    NiFixedString m_kAccumRootName;
    NiAVObject* m_pkAccumRoot;
    enum {
        // Translations that are accumulated
        ACCUM_X_TRANS   = 0x0001,
        ACCUM_Y_TRANS   = 0x0002,
        ACCUM_Z_TRANS   = 0x0004,
        
        // Rotation that is accumulated. Either one, none or all
        // of these may be specified.
        ACCUM_X_ROT     = 0x0008,
        ACCUM_Y_ROT     = 0x0010,
        ACCUM_Z_ROT     = 0x0020,

        // Front facing axis. Default is positive X.
        ACCUM_X_FRONT   = 0x0040,
        ACCUM_Y_FRONT   = 0x0080,
        ACCUM_Z_FRONT   = 0x0100,
        ACCUM_NEG_FRONT = 0x0200
    };
    NiDeclareFlags(unsigned int);
};

#include "NiControllerSequence.inl"

#endif  // #ifndef NICONTROLLERSEQUENCE_H
