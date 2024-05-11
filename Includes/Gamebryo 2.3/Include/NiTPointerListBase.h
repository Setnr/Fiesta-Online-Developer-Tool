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

#ifndef NITPOINTERLISTBASE_H
#define NITPOINTERLISTBASE_H

#include "NiTListBase.h"
#include "NiRTLib.h"

// NiTPointerListBase defines the interface for NiTList and NiTPointerList.
// The interface works best for pointer types T, however, other types
// will work when they satisfy the conditions below.  The template
// class assumes that type T has the following:
//   1.  A "zero" element (i.e., T var; var = 0; is supported)
//       which is considered to be a null array element.
//   2.  The default constructor for T must exist and create the
//       "zero" element.  The constructor must also handle all necessary
//       actions for constructing elements.  That is, the template array
//       class cannot make any post-construction member calls that are
//       specific to class T.
//   3.  Copy constructor T::T(const T&) must work properly.  The class T is
//       responsible for implementing this if need be.
//   4.  The destructor must handle all necessary actions for destroying
//       elements.  That is, the template array class cannot make any
//       pre-destruction member calls that are specific to class T.
//   5.  bool operator== (const T&);
//   6.  bool operator!= (const T&);
//   7.  T& operator= (const T&);
//
// NiTPointerList<T> requires sizeof(T) == sizeof(unsigned int), so that
// elements may be block allocated from a shared memory pool.
//
// For true objects, the NiTObjectList is usually a better choice.
//
// Example of iteration from head to tail:
//
//   NiTPointerList<T> kList;
//   NiTListIterator kPos = kList.GetHeadPos();
//   while (kPos)
//   {
//       T element = kList.GetNext(kPos);
//       <process element here>;
//   }

template <class TheAllocator, class T> class NiTPointerListBase : 
    public NiTListBase<TheAllocator,T>
{
public:
    virtual ~NiTPointerListBase();

    // Add/Insert functions 
    void AddHead(const T& element);
    void AddTail(const T& element);
    NiTListIterator InsertBefore(NiTListIterator kPos, const T& element);
    NiTListIterator InsertAfter(NiTListIterator kPos, const T& element);

    // Element access
    const T& GetHead() const;
    const T& GetTail() const;
    const T& Get(NiTListIterator kPos) const;

    // Element access with iteration support
    const T& GetNext(NiTListIterator& kPos) const;
    const T& GetPrev(NiTListIterator& kPos) const;

    // Remove functions
    T RemoveHead();
    T RemoveTail();
    T Remove(const T& element);
    T RemovePos(NiTListIterator& kPos);

    // Element search
    NiTListIterator FindPos(const T& element, 
        NiTListIterator kStart = 0) const;

protected:
    virtual NiTListItem<T>* NewItem();
    virtual void DeleteItem(NiTListItem<T>* pkItem);

};

//---------------------------------------------------------------------------
// Inline include
#if false
#include "NiTPointerListBase.inl"
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
template <class TheAllocator, class T> inline
NiTPointerListBase<TheAllocator, T>::~NiTPointerListBase()
{
    // RemoveAll is called from here because it depends on virtual functions
    // implemented in the derived class.
    // It will also be called in the parent destructor, 
    // but the map will already be empty.
    NiTPointerListBase<TheAllocator, T>::RemoveAll();
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListItem<T>* NiTPointerListBase<TheAllocator, T>::NewItem()
{
    return (NiTListItem<T>*)NiTListBase<TheAllocator,
        T>::m_kAllocator.Allocate();
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
void NiTPointerListBase<TheAllocator, T>::DeleteItem(NiTListItem<T>* pkItem)
{
    pkItem->m_element = 0;
    NiTListBase<TheAllocator, T>::m_kAllocator.Deallocate(pkItem);
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
const T& NiTPointerListBase<TheAllocator, T>::GetHead() const
{
    return NiTListBase<TheAllocator, T>::m_pkHead->m_element;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
const T& NiTPointerListBase<TheAllocator, T>::GetTail() const
{
    return NiTListBase<TheAllocator, T>::m_pkTail->m_element;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
const T& NiTPointerListBase<TheAllocator, T>::GetNext(NiTListIterator& kPos)
const
{
    NIASSERT(kPos != 0);

    const T& element = ((NiTListItem<T>*) kPos)->m_element;

    kPos = ((NiTListItem<T>*) kPos)->m_pkNext;
    return element;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
const T& NiTPointerListBase<TheAllocator, T>::GetPrev(NiTListIterator& kPos)
const
{
    NIASSERT(kPos != 0);

    const T& element = ((NiTListItem<T>*) kPos)->m_element;
    kPos = ((NiTListItem<T>*) kPos)->m_pkPrev;

    return element;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
const T& NiTPointerListBase<TheAllocator, T>::Get(NiTListIterator kPos) const
{
    NIASSERT(kPos != 0);
    return ((NiTListItem<T>*) kPos)->m_element;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
T NiTPointerListBase<TheAllocator, T>::RemoveHead()
{
    NIASSERT((NiTListBase<TheAllocator, T>::m_pkHead));

    NiTListItem<T>* pkNode = NiTListBase<TheAllocator, T>::m_pkHead;

    NiTListBase<TheAllocator, T>::m_pkHead = NiTListBase<TheAllocator,
        T>::m_pkHead->m_pkNext;

    if (NiTListBase<TheAllocator, T>::m_pkHead)
        NiTListBase<TheAllocator, T>::m_pkHead->m_pkPrev = 0;
    else
        NiTListBase<TheAllocator, T>::m_pkTail = 0;

    T element = pkNode->m_element;

    DeleteItem(pkNode);

    NiTListBase<TheAllocator, T>::m_kAllocator.m_uiCount--;

    return element;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
T NiTPointerListBase<TheAllocator, T>::RemoveTail()
{
    NIASSERT((NiTListBase<TheAllocator, T>::m_pkTail));

    NiTListItem<T>* pkNode = NiTListBase<TheAllocator, T>::m_pkTail;

    NiTListBase<TheAllocator, T>::m_pkTail = NiTListBase<TheAllocator,
        T>::m_pkTail->m_pkPrev;

    if (NiTListBase<TheAllocator, T>::m_pkTail)
        NiTListBase<TheAllocator, T>::m_pkTail->m_pkNext = 0;
    else
        NiTListBase<TheAllocator, T>::m_pkHead = 0;

    T element = pkNode->m_element;

    DeleteItem(pkNode);

    NiTListBase<TheAllocator, T>::m_kAllocator.m_uiCount--;

    return element;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
T NiTPointerListBase<TheAllocator, T>::RemovePos(NiTListIterator& kPos)
{
    NiTListItem<T>* pkNode = (NiTListItem<T>*) kPos;

    NIASSERT(pkNode);

    if (pkNode == NiTListBase<TheAllocator, T>::m_pkHead)
    {
        kPos = pkNode->m_pkNext; // kPos points to new head
        return RemoveHead();
    }
    if (pkNode == NiTListBase<TheAllocator, T>::m_pkTail)
    {
        kPos = 0; // kPos has walked off end of list
        return RemoveTail();
    }

    NiTListItem<T>* pkPrev = pkNode->m_pkPrev;
    NiTListItem<T>* pkNext = pkNode->m_pkNext;

    kPos = pkNext;

    if (pkPrev)
        pkPrev->m_pkNext = pkNext;
    if (pkNext)
        pkNext->m_pkPrev = pkPrev;

    T element = pkNode->m_element;

    DeleteItem(pkNode);

    NiTListBase<TheAllocator, T>::m_kAllocator.m_uiCount--;

    return element;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListIterator NiTPointerListBase<TheAllocator, T>::
InsertAfter(NiTListIterator kPos, const T& element)
{
    NIASSERT(kPos);

    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    pkNode->m_element = element;

    return AddNodeAfter(kPos, pkNode);
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListIterator NiTPointerListBase<TheAllocator, T>::
InsertBefore(NiTListIterator kPos, const T& element)
{
    NIASSERT(kPos);

    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    pkNode->m_element = element;

    return this->AddNodeBefore(kPos, pkNode);
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
void NiTPointerListBase<TheAllocator, T>::AddHead(const T& element)
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    pkNode->m_element = element;

    this->AddNodeHead(pkNode);
    //AddNodeHead(pkNode);
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
void NiTPointerListBase<TheAllocator, T>::AddTail(const T& element)
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    pkNode->m_element = element;

    this->AddNodeTail(pkNode);
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
T NiTPointerListBase<TheAllocator, T>::Remove(const T& element)
{
    NiTListIterator kPos = NiTPointerListBase<TheAllocator, T>::FindPos(
        element);

    return kPos ? RemovePos(kPos) : element;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class T> inline
NiTListIterator NiTPointerListBase<TheAllocator, T>::
FindPos(const T& element, NiTListIterator kStart) const
{
    if (kStart == 0)
        kStart = NiTPointerListBase<TheAllocator, T>::GetHeadPos();

    while (kStart)
    {
        NiTListIterator kPos = kStart;
        if (element == GetNext(kStart))
            return kPos;
    }
    return 0;
}
//---------------------------------------------------------------------------

#endif
//---------------------------------------------------------------------------

#endif // NITPOINTERLISTBASE_H
