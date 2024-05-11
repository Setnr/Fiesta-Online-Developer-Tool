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

#ifndef NITFIXEDSTRINGMAP_H
#define NITFIXEDSTRINGMAP_H

#include "NiTMap.h"
#include "NiTPointerMap.h"
#include "NiFixedString.h"

template <class TheAllocator, class TVAL> class NiTFixedStringMapBase :
    public NiMemObject
{
public:
    // construction and destruction
    NiTFixedStringMapBase(unsigned int uiHashSize = 37);
    virtual ~NiTFixedStringMapBase();

    // counting elements in map
    unsigned int GetCount() const;
    bool IsEmpty() const;

    // add or remove elements
    void SetAt(NiFixedString& kString, TVAL val);
    bool RemoveAt(NiFixedString& kString);
    void RemoveAll();

    // element access
    bool GetAt(const NiFixedString& kString, TVAL& val) const;

    // map traversal
    NiTMapIterator GetFirstPos() const;
    void GetNext(NiTMapIterator& pos, NiFixedString& kString, TVAL& val) const;

protected:
    // hash table stored as array of doubly-linked lists
    unsigned int KeyToHashIndex(const NiFixedString& kString) const;
    bool IsKeysEqual(const NiFixedString& kString1, 
        const NiFixedString& kString2) const;
    NiTMapItem<NiFixedString,TVAL>* GetPtrToAssoc(
        const NiFixedString& kString) const;
    void SetValue(NiTMapItem<NiFixedString, TVAL>* pkItem, 
        NiFixedString& kString, TVAL val);
    void ClearValue(NiTMapItem<NiFixedString, TVAL>* pkItem);

    virtual NiTMapItem<NiFixedString, TVAL>* NewItem() = 0;
    virtual void DeleteItem(NiTMapItem<NiFixedString, TVAL>* pkItem) = 0;

    unsigned int m_uiHashSize;             // maximum slots in hash table
    NiTMapItem<NiFixedString, TVAL>** m_ppkHashTable;// hash table storage

    struct AntiBloatAllocator : public TheAllocator
    {
        // We reduce TheAllocator by 4 bytes by deriving
        // See http://www.cantrip.org/emptyopt.html
        unsigned int m_uiCount;      // number of elements in list
    };

    AntiBloatAllocator m_kAllocator;  

private:
    // To prevent an application from inadvertently causing the compiler to
    // generate the default copy constructor or default assignment operator,
    // these methods are declared as private. They are not defined anywhere,
    // so code that attempts to use them will not link.
    NiTFixedStringMapBase(const NiTFixedStringMapBase&);
    NiTFixedStringMapBase& operator=(const NiTFixedStringMapBase&);
};

template <class TVAL> class NiTFixedStringMap :
    public NiTFixedStringMapBase<NiTDefaultAllocator<TVAL>, TVAL>
{
public:
    NiTFixedStringMap(unsigned int uiHashSize = 37) :
        NiTFixedStringMapBase<NiTDefaultAllocator<TVAL>, TVAL>(uiHashSize) {}
    virtual ~NiTFixedStringMap();

    virtual NiTMapItem<NiFixedString, TVAL>* NewItem();
    virtual void DeleteItem(NiTMapItem<NiFixedString, TVAL>* pkItem);
};

#include "NiTFixedStringMap.inl"

#endif // NITFIXEDSTRINGMAP_H
