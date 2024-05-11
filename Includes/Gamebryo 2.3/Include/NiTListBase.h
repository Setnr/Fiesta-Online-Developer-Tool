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

#ifndef NITLISTBASE_H
#define NITLISTBASE_H

#include "NiRTLib.h"
#include "NiTCollection.h"

typedef void* NiTListIterator;
//---------------------------------------------------------------------------
template <class T> class NiTListItem : public NiMemObject
{
public:
    NiTListItem* m_pkNext;
    NiTListItem* m_pkPrev;
    T m_element;
};
//---------------------------------------------------------------------------
template <class TheAllocator, class T> class NiTListBase : public NiMemObject
{
public:
    // counting elements in list
    unsigned int GetSize() const;
    bool IsEmpty() const;
   
    // clear list
    void RemoveAll();

    // iteration support
    NiTListIterator GetHeadPos() const;
    NiTListIterator GetTailPos() const;  
    
    NiTListIterator GetNextPos(NiTListIterator kPos) const;
    NiTListIterator GetPrevPos(NiTListIterator kPos) const;

protected:
    // construction and destruction
    NiTListBase();
    virtual ~NiTListBase();

    // add or remove elements
    virtual NiTListItem<T>* NewItem() = 0;
    virtual void DeleteItem(NiTListItem<T>* pkItem) = 0;

    void AddNodeHead(NiTListItem<T>* pkNode);
    void AddNodeTail(NiTListItem<T>* pkNode);

    NiTListIterator AddNodeAfter(NiTListIterator pos,
        NiTListItem<T>* pkNode);
    NiTListIterator AddNodeBefore(NiTListIterator pos,
        NiTListItem<T>* pkNode);

    // list is doubly-linked
    NiTListItem<T>* m_pkHead;              // points to head of list
    NiTListItem<T>* m_pkTail;              // points to tail of list

    struct AntiBloatAllocator : public TheAllocator
    {
        // We reduce TheAllocator by 4 bytes by deriving
        // See http://www.cantrip.org/emptyopt.html
        unsigned int m_uiCount;      // number of elements in list
    };

    AntiBloatAllocator m_kAllocator;  // points to the Allocator
private:
    // To prevent an application from inadvertently causing the compiler to
    // generate the default copy constructor or default assignment operator,
    // these methods are declared as private. They are not defined anywhere,
    // so code that attempts to use them will not link.
    NiTListBase(const NiTListBase&);
    NiTListBase& operator=(const NiTListBase&);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inline include
#if false
#include "NiTListBase.inl"
#else
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
//---------------------------------------------------------------------------
//  NiTListBase inline functions

//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListBase<TheAllocator, T>::NiTListBase()
{
    m_kAllocator.m_uiCount = 0;
    m_pkHead = 0;
    m_pkTail = 0;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListBase<TheAllocator, T>::~NiTListBase()
{
    // RemoveAll() must be called from derived classes as it relies on
    // the pure virtual DeleteItem(). If RemoveAll is called here without
    // being called in the derived class, R6025 runtime error will be
    // generated (pure virtual function called).
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
unsigned int NiTListBase<TheAllocator, T>::GetSize() const
{
    return m_kAllocator.m_uiCount;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
bool NiTListBase<TheAllocator, T>::IsEmpty() const
{
    return m_kAllocator.m_uiCount == 0;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
void NiTListBase<TheAllocator, T>::RemoveAll()
{
    NiTListItem<T>* pkCurr = m_pkHead;
    while (pkCurr)
    {
        NiTListItem<T>* pkDel = pkCurr;
        pkCurr = pkCurr->m_pkNext;
        DeleteItem(pkDel);
    }

    m_kAllocator.m_uiCount = 0;
    m_pkHead = 0;
    m_pkTail = 0;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListIterator NiTListBase<TheAllocator, T>::GetHeadPos() const
{
    return m_pkHead;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListIterator NiTListBase<TheAllocator, T>::GetTailPos() const
{
    return m_pkTail;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListIterator NiTListBase<TheAllocator, T>::
GetNextPos(NiTListIterator kPos) const
{
    return kPos ? ((NiTListItem<T>*) kPos)->m_pkNext : 0;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListIterator NiTListBase<TheAllocator, T>::
GetPrevPos(NiTListIterator kPos) const
{
    return kPos ? ((NiTListItem<T>*) kPos)->m_pkPrev : 0;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
void NiTListBase<TheAllocator, T>::AddNodeHead(NiTListItem<T>* pkNode)
{
    pkNode->m_pkPrev = 0;
    pkNode->m_pkNext = m_pkHead;

    if (m_pkHead)
        m_pkHead->m_pkPrev = pkNode;
    else
        m_pkTail = pkNode;

    m_pkHead = pkNode;
    m_kAllocator.m_uiCount++;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
void NiTListBase<TheAllocator, T>::AddNodeTail(NiTListItem<T>* pkNode)
{
    pkNode->m_pkNext = 0;
    pkNode->m_pkPrev = m_pkTail;

    if (m_pkTail)
        m_pkTail->m_pkNext = pkNode;
    else
        m_pkHead = pkNode;

    m_pkTail = pkNode;
    m_kAllocator.m_uiCount++;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListIterator NiTListBase<TheAllocator, T>::
AddNodeBefore(NiTListIterator kPos, NiTListItem<T>* pkNode)
{
    NiTListItem<T>* pkNext = (NiTListItem<T>*) kPos;

    pkNode->m_pkNext = pkNext;
    pkNode->m_pkPrev = pkNext->m_pkPrev;

    if (pkNext->m_pkPrev)
        pkNext->m_pkPrev->m_pkNext = pkNode;
    else
        m_pkHead = pkNode;

    pkNext->m_pkPrev = pkNode;

    m_kAllocator.m_uiCount++;
    return pkNode;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListIterator NiTListBase<TheAllocator, T>::
AddNodeAfter(NiTListIterator kPos, NiTListItem<T>* pkNode)
{
    NiTListItem<T>* pkPrev = (NiTListItem<T>*) kPos;

    pkNode->m_pkPrev = pkPrev;
    pkNode->m_pkNext = pkPrev->m_pkNext;

    if (pkPrev->m_pkNext)
        pkPrev->m_pkNext->m_pkPrev = pkNode;
    else
        m_pkTail = pkNode;

    pkPrev->m_pkNext = pkNode;

    m_kAllocator.m_uiCount++;
    return pkNode;
}
//---------------------------------------------------------------------------

#endif
//---------------------------------------------------------------------------

#endif // NITLISTBASE_H
