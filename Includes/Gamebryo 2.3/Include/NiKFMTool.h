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

#ifndef NIKFMTOOL_H
#define NIKFMTOOL_H

#include "NiAnimationLibType.h"
#include <NiFixedString.h>
#include <NiSmartPointer.h>
#include <NiRefObject.h>
#include <NiTArray.h>
#include <NiTSet.h>
#include <NiTPointerMap.h>

class NiFile;

class NIANIMATION_ENTRY NiKFMTool : public NiRefObject
{
public:
    // Public constants.
    enum KFM_RC   // Return code.
    {
        KFM_SUCCESS,
        KFM_ERROR,
        KFM_ERR_SEQUENCE,
        KFM_ERR_TRANSITION,
        KFM_ERR_TRANSITION_TYPE,
        KFM_ERR_BLEND_PAIR,
        KFM_ERR_NULL_TEXT_KEYS,
        KFM_ERR_BLEND_PAIR_INDEX,
        KFM_ERR_CHAIN_SEQUENCE,
        KFM_ERR_SEQUENCE_IN_CHAIN,
        KFM_ERR_INFINITE_CHAIN,
        KFM_ERR_SEQUENCE_GROUP,
        KFM_ERR_SEQUENCE_IN_GROUP,
        KFM_ERR_FILE_IO,
        KFM_ERR_FILE_FORMAT,
        KFM_ERR_FILE_VERSION,
        KFM_ERR_ENDIAN_MISMATCH,
        KFM_ERR_SYNC_TRANS_TYPE,
        KFM_ERR_NONSYNC_TRANS_TYPE
    };
    enum TransitionType
    {
        TYPE_BLEND,
        TYPE_MORPH,
        TYPE_CROSSFADE,
        TYPE_CHAIN,
        TYPE_DEFAULT_SYNC,
        TYPE_DEFAULT_NONSYNC,
        TYPE_DEFAULT_INVALID
    };
    static const float MAX_DURATION;
    static const unsigned int SYNC_SEQUENCE_ID_NONE;

public:
    // Public internal class declarations. Friend status is given to the
    // NiKFMTool class because the streaming functions need direct access to
    // the member variables in order to use the NiStreamSave* and
    // NiStreamLoad* functions to stream them to and from disk.
    class Transition : public NiMemObject
    {
    public:
        class BlendPair : public NiMemObject
        {
        public:
            BlendPair();
            BlendPair(const NiFixedString& kStartKey, 
                const NiFixedString& kTargetKey);
            ~BlendPair();

            const NiFixedString& GetStartKey() const;
            void SetStartKey(const NiFixedString& kStartKey);

            const NiFixedString& GetTargetKey() const;
            void SetTargetKey(const NiFixedString& kTargetKey);

        protected:
            NiFixedString m_kStartKey;
            NiFixedString m_kTargetKey;

            friend class NiKFMTool;
        };

        class ChainInfo : public NiMemObject
        {
        public:
            ChainInfo();
            ChainInfo(unsigned int uiSequenceID, float fDuration);

            unsigned int GetSequenceID() const;
            void SetSequenceID(unsigned int uiSequenceID);

            float GetDuration() const;
            void SetDuration(float fDuration);

        protected:
            unsigned int m_uiSequenceID;
            float m_fDuration;

            friend class NiKFMTool;
        };

        Transition();
        Transition(TransitionType eType, float fDuration);
        ~Transition();

        // Gets the translated type for the transition. Default types will
        // be resolved to their associated type.
        TransitionType GetType() const;

        // Gets and sets the stored type for the transition. Default types
        // will not be resolved.
        TransitionType GetStoredType() const;

        float GetDuration() const;
        void SetDuration(float fDuration);

        typedef NiTPrimitiveSet<BlendPair*> BlendPairSet;
        typedef NiTObjectSet<ChainInfo> ChainInfoSet;

        BlendPairSet& GetBlendPairs();
        ChainInfoSet& GetChainInfo();

        void ClearBlendPairs();
        void ClearChainInfo();

    protected:
        TransitionType m_eType;
        float m_fDuration;
        BlendPairSet m_aBlendPairs;
        ChainInfoSet m_aChainInfo;

        // This member is only used by default transition instances.
        TransitionType m_eDefaultType;

        friend class NiKFMTool;
    };

    class Sequence : public NiMemObject
    {
    public:
        Sequence();
        Sequence(unsigned int uiSequenceID, const NiFixedString& kFilename,
            int iAnimIndex);
        ~Sequence();

        unsigned int GetSequenceID() const;
        void SetSequenceID(unsigned int uiSequenceID);

        const NiFixedString& GetFilename() const;
        void SetFilename(const NiFixedString& kFilename);

        int GetAnimIndex() const;
        void SetAnimIndex(int iAnimIndex);

        NiTPointerMap<unsigned int, Transition*>& GetTransitions();

    protected:
        unsigned int m_uiSequenceID;
        NiFixedString m_kFilename;
        int m_iAnimIndex;
        NiTPointerMap<unsigned int, Transition*> m_mapTransitions;

        friend class NiKFMTool;
    };

    class SequenceGroup : public NiMemObject
    {
    public:
        class SequenceInfo : public NiMemObject
        {
        public:
            SequenceInfo();
            SequenceInfo(unsigned int uiSequenceID, int iPriority,
                float fWeight, float fEaseInTime, float fEaseOutTime,
                unsigned int uiSynchronizeSequenceID = SYNC_SEQUENCE_ID_NONE);

            unsigned int GetSequenceID() const;
            void SetSequenceID(unsigned int uiSequenceID);

            int GetPriority() const;
            void SetPriority(int iPriority);

            float GetWeight() const;
            void SetWeight(float fWeight);
            
            float GetEaseInTime() const;
            void SetEaseInTime(float fEaseInTime);

            float GetEaseOutTime() const;
            void SetEaseOutTime(float fEaseOutTime);

            unsigned int GetSynchronizeSequenceID() const;
            void SetSynchronizeSequenceID(
                unsigned int uiSynchronizeSequenceID);

        protected:
            unsigned int m_uiSequenceID;
            int m_iPriority;
            float m_fWeight;
            float m_fEaseInTime;
            float m_fEaseOutTime;
            unsigned int m_uiSynchronizeSequenceID;

            friend class NiKFMTool;
        };

        SequenceGroup();
        SequenceGroup(unsigned int uiGroupID, const NiFixedString& kName);
        ~SequenceGroup();

        unsigned int GetGroupID() const;
        void SetGroupID(unsigned int uiGroupID);

        const NiFixedString& GetName() const;
        void SetName(const NiFixedString& kName);


        typedef NiTObjectSet<SequenceInfo> SequenceInfoSet;
        
        SequenceInfoSet& GetSequenceInfo();

    protected:
        unsigned int m_uiGroupID;
        NiFixedString m_kName;
        SequenceInfoSet m_aSequenceInfo;

        friend class NiKFMTool;
    };

    // Constructor and destructor.
    NiKFMTool(const NiFixedString& kBaseKFMPath = NULL);
    ~NiKFMTool();

    // Functions for adding components.
    KFM_RC AddSequence(unsigned int uiSequenceID,
        const NiFixedString& kFilename, int iAnimIndex);
    KFM_RC AddTransition(unsigned int uiSrcID, unsigned int uiDesID,
        TransitionType eType, float fDuration);
    KFM_RC AddBlendPair(unsigned int uiSrcID, unsigned int uiDesID,
        const NiFixedString& kStartKey, const NiFixedString& kTargetKey);
    KFM_RC AddSequenceToChain(unsigned int uiSrcID, unsigned int uiDesID,
        unsigned int uiSequenceID, float fDuration);
    KFM_RC AddSequenceGroup(unsigned int uiGroupID,
        const NiFixedString& kName);
    KFM_RC AddSequenceToGroup(unsigned int uiGroupID,
        unsigned int uiSequenceID, int iPriority, float fWeight,
        float fEaseInTime, float fEaseOutTime, 
        unsigned int uiSynchronizeToSequence = SYNC_SEQUENCE_ID_NONE);

    // Functions for updating components.
    KFM_RC UpdateSequence(unsigned int uiSequenceID,
        const char* pcFilename, int iAnimIndex);
    KFM_RC UpdateTransition(unsigned int uiSrcID, unsigned int uiDesID,
        TransitionType eType, float fDuration);
    KFM_RC UpdateSequenceID(unsigned int uiOldID, unsigned int uiNewID);
    KFM_RC UpdateGroupID(unsigned int uiOldID, unsigned int uiNewID);

    // Functions for removing components.
    KFM_RC RemoveSequence(unsigned int uiSequenceID);
    KFM_RC RemoveTransition(unsigned int uiSrcID, unsigned int uiDesID);
    KFM_RC RemoveBlendPair(unsigned int uiSrcID, unsigned int uiDesID,
        const NiFixedString& kStartKey, const NiFixedString& kTargetKey);
    KFM_RC RemoveAllBlendPairs(unsigned int uiSrcID, unsigned int uiDesID);
    KFM_RC RemoveSequenceFromChain(unsigned int uiSrcID, unsigned int uiDesID,
        unsigned int uiSequenceID);
    KFM_RC RemoveAllSequencesFromChain(unsigned int uiSrcID,
        unsigned int uiDesID);
    KFM_RC RemoveSequenceGroup(unsigned int uiGroupID);
    KFM_RC RemoveSequenceFromGroup(unsigned int uiGroupID,
        unsigned int uiSequenceID);
    KFM_RC RemoveAllSequencesFromGroup(unsigned int uiGroupID);

    // Functions for retrieving components.
    Sequence* GetSequence(unsigned int uiSequenceID) const;
    Transition* GetTransition(unsigned int uiSrcID, unsigned int uiDesID)
        const;
    SequenceGroup* GetSequenceGroup(unsigned int uiGroupID) const;

    // Functions for retrieving identifier codes.
    void GetSequenceIDs(unsigned int*& puiSequenceIDs,
        unsigned int& uiNumIDs) const;
    void GetGroupIDs(unsigned int*& puiGroupIDs, unsigned int& uiNumIDs)
        const;
    unsigned int FindUnusedSequenceID() const;
    unsigned int FindUnusedGroupID() const;

    // Functions for accessing model data.
    const NiFixedString& GetModelPath() const;
    void SetModelPath(const NiFixedString& kModelPath);
    const NiFixedString& GetModelRoot() const;
    void SetModelRoot(const NiFixedString& kModelRoot);

    // Functions for accessing default transition information.
    TransitionType GetDefaultSyncTransType() const;
    KFM_RC SetDefaultSyncTransType(TransitionType eType);
    TransitionType GetDefaultNonSyncTransType() const;
    KFM_RC SetDefaultNonSyncTransType(TransitionType eType);
    float GetDefaultSyncTransDuration() const;
    void SetDefaultSyncTransDuration(float fDuration);
    float GetDefaultNonSyncTransDuration() const;
    void SetDefaultNonSyncTransDuration(float fDuration);

    // Functions for performing lookups on components or component data.
    KFM_RC IsTransitionAllowed(unsigned int uiSrcID, unsigned int uiDesID,
        bool& bAllowed) const;
    static const char* LookupReturnCode(KFM_RC eReturnCode);
    bool IsValidChainTransition(unsigned int uiSrcID, unsigned int uiDesID,
        Transition* pkTransition);

    // Functions for getting fully qualified paths.
    const NiFixedString& GetBaseKFMPath() const;
    void SetBaseKFMPath(const NiFixedString& kBaseKFMPath);
    const NiFixedString& GetFullModelPath();
    NiFixedString GetFullKFFilename(unsigned int uiSequenceID);

    // Functions for streaming data to a file.
    KFM_RC LoadFile(const char* pcFilename);
    KFM_RC SaveFile(const char* pcFilename, bool bUseBinary = true,
        bool bLittleEndian = true);

protected:
    void UpdateTransitionsContainingSequence(unsigned int uiOldID,
        unsigned int uiNewID);
    void UpdateSequenceGroupsContainingSequence(unsigned int uiOldID,
        unsigned int uiNewID);
    void RemoveTransitionsContainingSequence(unsigned int uiSequenceID);
    void RemoveSequenceFromSequenceGroups(unsigned int uiSequenceID);

    Sequence* GetSequenceFromID(unsigned int uiSequenceID) const;
    Transition* GetTransitionFromID(unsigned int uiSequenceID,
        Sequence* pkSequence) const;
    SequenceGroup* GetSequenceGroupFromID(unsigned int uiGroupID) const;

    void GatherChainIDs(unsigned int uiSrcID, unsigned int uiDesID,
        Transition* pkTransition, NiUnsignedIntSet& kChainIDs);
    void HandleDelayedBlendsInChains();

    void ConvertRelativePaths(const char* pcNewBaseKFMPath);

    // Streaming functions.
    KFM_RC WriteBinary(NiFile& kFile);
    KFM_RC WriteAscii(NiFile& kFile);
    KFM_RC ReadBinary(NiFile& kFile, unsigned int uiVersion);
    KFM_RC ReadAscii(NiFile& kFile, unsigned int uiVersion);
    KFM_RC ReadOldVersionAscii(NiFile& kFile, unsigned int uiVersion);

    // Streaming helper functions.
    void SaveCString(NiFile& kFile, const char* pcString);
    void LoadCString(NiFile& kFile, char*& pcString);
    void SaveFixedString(NiFile& kFile, const NiFixedString& kString);
    void LoadFixedString(NiFile& kFile, NiFixedString& kString);
    void LoadCStringAsFixedString(NiFile& kFile, NiFixedString& kString);

    // Protected variables.
    NiFixedString m_kBaseKFMPath;
    NiFixedString m_kFullPathBuffer;
    NiFixedString m_kModelPath;
    NiFixedString m_kModelRoot;
    NiTPointerMap<unsigned int, Sequence*> m_mapSequences;
    NiTPointerMap<unsigned int, SequenceGroup*> m_mapSequenceGroups;

    // Default transition settings.
    Transition* m_pkDefaultSyncTrans;
    Transition* m_pkDefaultNonSyncTrans;
};

NiSmartPointer(NiKFMTool);

#include "NiKFMTool.inl"

#endif // #ifndef NIKFMTOOL_H
