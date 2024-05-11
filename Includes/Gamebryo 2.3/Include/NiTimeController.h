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

#ifndef NITIMECONTROLLER_H
#define NITIMECONTROLLER_H

#include "NiFlags.h"
#include "NiMath.h"
#include "NiObject.h"
#include "NiStream.h"

class NiObjectNET;
class NiAVObject;
NiSmartPointer(NiTimeController);

class NIMAIN_ENTRY NiTimeController : public NiObject
{
    NiDeclareRTTI;
    NiDeclareAbstractClone(NiTimeController);
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;
    NiDeclareFlags(unsigned short);

public:
    NiTimeController();
    virtual ~NiTimeController();

    //
    // ANIMATION CORE TYPES:
    //
    // The APP_TIME animation type relies on the application
    // to supply time values for determining key frame 
    // interpolation.  
    //
    // The APP_INIT animation type provides facility for an
    // application to start or stop an animation node from
    // updating at any instant.  
    //
    // ANIMATION CYCLE TYPES:
    //
    // Cycle types allow for looping, reversing and clamping 
    // the animation sequence.
    //
    enum AnimType
    {
        APP_TIME,
        APP_INIT
    };

    enum CycleType
    {
        LOOP,
        REVERSE,
        CLAMP,

        MAX_CYCLE_TYPES
    };

    void SetAnimType(AnimType eType);
    AnimType GetAnimType() const;

    void SetCycleType(CycleType eType);
    CycleType GetCycleType() const;

    void SetPlayBackwards(bool bPlayBackwards);
    bool GetPlayBackwards() const;

    void SetFrequency(float fFreq);
    float GetFrequency() const;

    void SetPhase(float fPhase);
    float GetPhase() const;

    void SetBeginKeyTime(float fBegin);
    float GetBeginKeyTime() const;

    void SetEndKeyTime(float fEnd);
    float GetEndKeyTime() const;

    virtual void Start(float fTime = -NI_INFINITY);
    virtual void Stop();

    virtual void Update(float fTime) = 0;

    virtual void SetTarget(NiObjectNET* pkTarget);
    NiObjectNET* GetTarget() const;

    // 
    // Static functions that start and stop all animation objects
    // in a scene graph. They effect only animation nodes of type 
    // APP_INIT.
    //
    static void StartAnimations(NiObjectNET* pkRoot, float fTime);
    static void StartAnimations(NiObjectNET* pkRoot);
    static void StopAnimations(NiObjectNET* pkRoot);

    unsigned int ItemsInList() const;

    void SetNext(NiTimeController* pkNext);
    NiTimeController* GetNext() const; 

    virtual bool IsTransformController() const;
    virtual bool IsVertexController() const;
    
    float GetLastScaledTime() const;

    // *** begin Emergent internal use only ***
    bool DontDoUpdate(float fTime);

    float GetStartTime() const;
    float GetLastTime() const;

    virtual float ComputeScaledTime(float fTime);

    virtual void OnPreDisplay();
    static void OnPreDisplayIterate(NiTimeController* pkController);

    void SetActive(bool bActive);
    bool GetActive() const;

    static char* GetViewerString(const char* pcPrefix, AnimType eType);
    static char* GetViewerString(const char* pcPrefix, CycleType eType);

    virtual bool IsStreamable() const;

    // cloning
    virtual void ProcessClone(
        NiCloningProcess& kCloning);

    void ForceNextUpdate();
    // *** end Emergent internal use only ***

protected:
    // --- Begin NIF conversion code
    // Deprecated flags - use new flag code
    enum
    {
        MAX_POS = 5
    };
    // --- End NIF conversion code

    // flags
    enum
    {
        ANIMTYPE_MASK           = 0x0001,
        ANIMTYPE_POS            = 0,
        CYCLETYPE_MASK          = 0x0006,
        CYCLETYPE_POS           = 1,
        ACTIVE_MASK             = 0x0008,
        DIRECTION_MASK          = 0x0010,
        MANAGERCONTROLLED_MASK  = 0x0020,
        COMPUTESCALEDTIME_MASK  = 0x0040,
        FORCEUDPATE_MASK        = 0x0080
    };

    // These functions have been made protected because the ManagerUpdate bit
    // is no longer used to turn off a particular controller from being
    // updated by a manager. Instead, simply set the Active flag on the
    // controller to be false. These functions were not removed because they
    // are called by NiInterpController to store its ManagerControlled bit.
    void SetManagerControlledBit(bool bManagerUpdate);
    bool GetManagerControlledBit() const;

    void SetForceUpdate(bool bForceUpdate);
    bool GetForceUpdate() const;

    void SetComputeScaledTime(bool bComputeScaledTime);
    bool GetComputeScaledTime() const;

    float m_fFrequency;
    float m_fPhase;
    float m_fLoKeyTime;
    float m_fHiKeyTime;
    float m_fStartTime; // m_fStartTime is only used by APP_INIT animations
    float m_fLastTime;
    float m_fWeightedLastTime;
    float m_fScaledTime;

    NiObjectNET* m_pkTarget;

    // singly-linked list
    NiTimeControllerPtr m_spNext;

    // debugging
    virtual bool TargetIsRequiredType() const = 0;

    // There are cached values in some functions that need to be synched.
    static NiFastCriticalSection ms_kCachedValuesLock;
};

#include "NiTimeController.inl"

#endif

