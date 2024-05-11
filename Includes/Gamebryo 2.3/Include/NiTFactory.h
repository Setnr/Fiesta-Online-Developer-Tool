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

#ifndef NITFACTORY_H
#define NITFACTORY_H

#include <NiFixedString.h>
#include <NiTStringMap.h>

// Use NiFactoryDeclare in your .h to create a factory create class.
// The parameters are as follows:
//      createclass - the class that needs a creator
//      factorytype - the base type that the factory returns
//      registerstring - the identifier used to link with this creator
#define NiFactoryDeclareCreator(createclass, factorytype, registerstring) \
    class createclass##Creator \
    {   \
    public: \
        static void Register(NiTFactory<factorytype*>* pkFactory) \
        {   \
            pkFactory->Register( \
                NiFixedString(registerstring), createclass##Creator::Create);\
        }   \
        static void RegisterPersistent(NiTFactory<factorytype*>* pkFactory) \
        {   \
            pkFactory->RegisterPersistent( \
                NiFixedString(registerstring), \
                createclass##Creator::Create, \
                createclass##Creator::Destroy); \
        }   \
    private:    \
        static factorytype* Create(const char* pcParams = NULL) \
        {   \
            return NiNew createclass;   \
        }   \
        static void Destroy(factorytype* pkDelete) \
        {   \
            NiDelete ((createclass*)pkDelete);  \
        }   \
   }; 

#define NiFactoryDeclareCustomCreator(createclass, factorytype, \
    registerstring, pfnCreate, pfnDestroy) \
class createclass##Creator \
    {   \
    public: \
        static void Register() \
        {   \
            NiTFactory<factorytype*>::Register( \
            NiFixedString(registerstring), pfnCreate); \
        }   \
        static void RegisterPersistent() \
        {   \
            NiTFactory<factorytype*>::RegisterPersistent( \
                NiFixedString(registerstring), pfnCreate, pfnDestroy); \
        }   \
    }; 

// NiFactoryRegister(Persistent) takes the classname of the type that is 
// returned for convenience. It calls the register function of the Creator 
// class.
#define NiFactoryRegisterPersistent(classname, pkFactory) \
    classname##Creator::RegisterPersistent(pkFactory); 

#define NiFactoryRegister(classname, pkFactory) \
    classname##Creator::Register(pkFactory); 

template <class T> class NiTFactory : public NiRefObject 
{
public:
    NiTFactory()
    {
        m_pkCreators = NiNew NiTStringMap<CreateFunction>;
        m_pkPersistents = NiNew NiTStringMap<T>;
        m_pkDestroyers = NiNew NiTStringMap<DestroyFunction>;
    };

    ~NiTFactory()
    {
        if (m_pkCreators)
        {
            m_pkCreators->RemoveAll();
            NiDelete m_pkCreators;
            m_pkCreators = 0;
        }

        if (m_pkPersistents)
        {
            // For objects stored in this template, they need to be deleted.
            // For this purpose, a DestroyFunction is necessary to get 
            // around multiple-inhert. issues in conj. with NiMemObject. These
            // issues are avoided by calling a deletion function that has
            // the appropriate cast and NiDelete, delete, free, etc...
            NiTMapIterator kIter = m_pkPersistents->GetFirstPos();
            while(kIter)
            {
                T pkObj = NULL;
                const char* pcKey = NULL;
                m_pkPersistents->GetNext(kIter, pcKey, pkObj);

                DestroyFunction pfnFunc = NULL;
                m_pkDestroyers->GetAt(pcKey, pfnFunc);
                pfnFunc(pkObj);
            }

            m_pkPersistents->RemoveAll();
            NiDelete m_pkPersistents;
            m_pkPersistents = 0;
        }

        if (m_pkDestroyers)
        {
            m_pkDestroyers->RemoveAll();
            NiDelete m_pkDestroyers;
            m_pkDestroyers = 0;
        }
    };

    typedef T (*CreateFunction)(const char*);
    typedef void (*DestroyFunction)(T);
    typedef T Type_T;

    void GetKeys(NiTPrimitiveArray<const char*>& kKeys)
    {
        kKeys.RemoveAll();

        NIASSERT(m_pkCreators);
        NiTMapIterator kIter = m_pkCreators->GetFirstPos();
        while(kIter)
        {
            CreateFunction pfnFunc = NULL;
            const char* pcKey = NULL;
            m_pkCreators->GetNext(kIter, pcKey, pfnFunc);
            kKeys.Add(pcKey);
        }
    }

    T Create(const char* pcName, const char* pcParams = NULL)
    {
        CreateFunction pfnFunc;
        NIASSERT(m_pkCreators);
        if (!m_pkCreators->GetAt(pcName, pfnFunc))
        {
            // Creator not registered
            return NULL;
        }

        return pfnFunc(pcParams);
    }

    T GetPersistent(const NiFixedString& kName)
    {
        T pkT;
        if (!m_pkPersistents->GetAt(kName, pkT))
            return NULL;

        return pkT;
    }

    void Register(NiFixedString& kName, CreateFunction pfnFunc)
    {
        NIASSERT(m_pkCreators);
        m_pkCreators->SetAt(kName, pfnFunc); 
    }

    void RegisterPersistent(NiFixedString& kName, 
        CreateFunction pfnCreateFunc, DestroyFunction pfnDestroyFunc,
        const char* pcParams = NULL)
    {
        NIASSERT(pfnCreateFunc);
        if (pfnCreateFunc == NULL)
            return;

        Register(kName, pfnCreateFunc);
        RegisterDestroy(kName, pfnDestroyFunc);

        // Register the persistent
        m_pkPersistents->SetAt(kName, pfnCreateFunc(pcParams));
    }

    unsigned int GetCount()
    {
        NIASSERT(m_pkCreators);
        return m_pkCreators->GetCount();
    }

    NiTMapIterator GetFirstPos()
    {
        NIASSERT(m_pkCreators);
        return m_pkCreators->GetFirstPos();
    }

    void GetNext(NiTMapIterator& kIter, const char*& pcKey)
    {
        pcKey = NULL;
        CreateFunction pfnFunc = NULL;
        m_pkCreators->GetNext(kIter, pcKey, pfnFunc);
    }

protected:   
    void RegisterDestroy(NiFixedString& kName, DestroyFunction pfnFunc)
    {
        NIASSERT(m_pkDestroyers);
        m_pkDestroyers->SetAt(kName, pfnFunc); 
    }

    NiTStringMap<CreateFunction>* m_pkCreators;
    NiTStringMap<T>* m_pkPersistents;
    NiTStringMap<DestroyFunction>* m_pkDestroyers;
};

#endif // NITFACTORY_H
