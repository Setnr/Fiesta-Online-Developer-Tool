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

#ifndef NIRTTI_H
#define NIRTTI_H

#include "NiMainLibType.h"
#include <NiMemObject.h>

// run-time type information

class NIMAIN_ENTRY NiRTTI : public NiMemObject
{
public:
    NiRTTI (const char* pcName, const NiRTTI* pkBaseRTTI);

    const char* GetName() const {return m_pcName;}
    const NiRTTI* GetBaseRTTI() const {return m_pkBaseRTTI;}

protected:
    const char* m_pcName;
    const NiRTTI* m_pkBaseRTTI;
};

// insert in root class declaration
#define NiDeclareRootRTTI(classname) \
    public: \
        static const NiRTTI ms_RTTI; \
        virtual const NiRTTI* GetRTTI() const {return &ms_RTTI;} \
        static bool IsExactKindOf(const NiRTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (pkObject) \
            { \
                return false; \
            } \
            return pkObject->IsExactKindOf(pkRTTI); \
        } \
        bool IsExactKindOf(const NiRTTI* pkRTTI) const \
        { \
            return (GetRTTI() == pkRTTI); \
        } \
        static bool IsKindOf(const NiRTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (!pkObject) \
            { \
                return false; \
            } \
            return pkObject->IsKindOf(pkRTTI); \
        } \
        bool IsKindOf(const NiRTTI* pkRTTI) const \
        { \
            const NiRTTI* pkTmp = GetRTTI(); \
            while (pkTmp) \
            { \
                if (pkTmp == pkRTTI) \
                { \
                    return true; \
                } \
                pkTmp = pkTmp->GetBaseRTTI(); \
            } \
            return false; \
        } \
        static classname* DynamicCast(const NiRTTI* pkRTTI, \
            const classname* pkObject) \
        { \
            if (!pkObject) \
            { \
                return (classname*)false; \
            } \
            return pkObject->DynamicCast(pkRTTI); \
        } \
        classname* DynamicCast(const NiRTTI* pkRTTI) const \
        { \
            return (IsKindOf(pkRTTI) ? (classname*) this : 0 ); \
        }

// insert in class declaration
#define NiDeclareRTTI \
    public: \
        static const NiRTTI ms_RTTI; \
        virtual const NiRTTI* GetRTTI() const {return &ms_RTTI;}

// insert in root class source file
#define NiImplementRootRTTI(rootclassname) \
    const NiRTTI rootclassname::ms_RTTI(#rootclassname, 0)

// insert in class source file
#define NiImplementRTTI(classname, baseclassname) \
    const NiRTTI classname::ms_RTTI(#classname, &baseclassname::ms_RTTI)

// macros for run-time type testing
#define NiIsExactKindOf(classname, pkObject) \
    classname::IsExactKindOf(&classname::ms_RTTI, pkObject)

#define NiIsKindOf(classname, pkObject) \
    classname::IsKindOf(&classname::ms_RTTI, pkObject)

// macro for run-time type casting
#define NiStaticCast(classname, pkObject) \
    ((classname*) pkObject)

#define NiDynamicCast(classname, pkObject) \
    ((classname*) classname::DynamicCast(&classname::ms_RTTI, pkObject))

#endif
