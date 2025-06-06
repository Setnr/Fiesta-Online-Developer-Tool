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

#ifndef NICRITICALSECTION_H
#define NICRITICALSECTION_H

#include "NiSystemLibType.h"
#include "NiRTLib.h"

#define NI_MULTITHREADED

class NISYSTEM_ENTRY NiCriticalSection
{
public:
    /// Default constructor.
    NiCriticalSection();

#if defined(_PS3)
    // Special constructor needed to allow NiTAbstractPoolAllocator to contain
    // a static NiCriticalSection object.
    NiCriticalSection(unsigned int uiDummyParam);
#endif    
    /// Destructor.
    ~NiCriticalSection();

    /// Locks the critical section.
    void Lock();

    /// Unlocks the critical section.
    void Unlock();

    /// Gets the ID of the thread that last locked the critical section.
    /// @return The ID of the last thread to lock the critical section or
    ///         0 if the critical section is not locked.
    unsigned int GetOwningThreadID() const;

    /// Gets the current count of calls to Lock by the same thread. The
    /// critical section will not be unlocked until Unlock has been called
    /// this many times.
    /// @return The number of times the critical section has been locked by
    ///         the same thread.
    unsigned int GetCurrentLockCount() const;

protected:
#if defined (NI_MULTITHREADED)
#if defined (WIN32) || defined(_XENON)
    CRITICAL_SECTION m_kCriticalSection;
    /// The ID of the last thread to lock the critical section.
    unsigned long m_ulThreadOwner;
#elif defined (_PS3)
    sys_mutex_t m_kCriticalSection;
    sys_ppu_thread_t m_ulThreadOwner;
#endif // #ifdef for platforms

    /// The count of how many times the critical section has been locked by
    /// the same thread.
    unsigned int m_uiLockCount;
#endif // #if defined (NI_MULTITHREADED)
};

class NISYSTEM_ENTRY NiFastCriticalSection
{
public:
    /// Default constructor.
    NiFastCriticalSection();
    /// Destructor.
    ~NiFastCriticalSection();

    /// Locks the critical section.
    void Lock();

    /// Unlocks the critical section.
    void Unlock();
    
protected:
#if defined (NI_MULTITHREADED)
#if defined (WIN32) || defined(_XENON)
    CRITICAL_SECTION m_kCriticalSection;
#elif defined (_PS3)
    sys_mutex_t m_kCriticalSection;
#endif // #ifdef for platforms

#if defined (_DEBUG)
    bool m_bLocked;
#endif // #if defined (_DEBUG)

#endif // #if defined (NI_MULTITHREADED)
};

#include "NiCriticalSection.inl"

#endif // #ifndef NICRITICALSECTION_H
