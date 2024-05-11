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
template <class TheAllocator, class TVAL> inline
    NiTFixedStringMapBase<TheAllocator, TVAL>::NiTFixedStringMapBase(
    unsigned int uiHashSize)
{
    m_uiHashSize = uiHashSize;
    m_kAllocator.m_uiCount = 0;

    unsigned int uiSize = sizeof(NiTMapItem<NiFixedString,TVAL>*) * 
        m_uiHashSize;
    m_ppkHashTable = (NiTMapItem<NiFixedString,TVAL>**)NiMalloc(uiSize);
    NIASSERT(m_ppkHashTable);
    memset(m_ppkHashTable, 0, 
        m_uiHashSize * sizeof(NiTMapItem<NiFixedString, TVAL>*));
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    NiTFixedStringMapBase<TheAllocator, TVAL>::~NiTFixedStringMapBase()
{
    RemoveAll();
    NiFree(m_ppkHashTable);
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    unsigned int NiTFixedStringMapBase<TheAllocator, TVAL>::GetCount() const
{ 
    return m_kAllocator.m_uiCount;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    bool NiTFixedStringMapBase<TheAllocator, TVAL>::IsEmpty() const
{ 
    return m_kAllocator.m_uiCount == 0;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    void NiTFixedStringMapBase<TheAllocator, TVAL>::SetAt(NiFixedString& kKey, 
    TVAL val)
{
    // look up hash table location for key
    unsigned int uiIndex = KeyToHashIndex(kKey);
    NiTMapItem<NiFixedString, TVAL>* pkItem = m_ppkHashTable[uiIndex];

    unsigned int uiDepth = 0;

    // search list at hash table location for key
    while (pkItem)
    {
        uiDepth++;
        if (IsKeysEqual(kKey, pkItem->m_key))
        {
            // item already in hash table, set its new value
            ClearValue(pkItem);
            SetValue(pkItem, kKey, val);
            return;
        }
        pkItem = pkItem->m_pkNext;
    }

    // add object to beginning of list for this hash table index
    pkItem = (NiTMapItem<NiFixedString, TVAL>*)NewItem();

    NIASSERT(pkItem);
    SetValue(pkItem, kKey, val);
    pkItem->m_pkNext = m_ppkHashTable[uiIndex];
    m_ppkHashTable[uiIndex] = pkItem;
    m_kAllocator.m_uiCount++;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    bool NiTFixedStringMapBase<TheAllocator, TVAL>::RemoveAt(
    NiFixedString& kKey)
{
    // look up hash table location for key
    unsigned int uiIndex = KeyToHashIndex(kKey);
    NiTMapItem<NiFixedString, TVAL>* pkItem = m_ppkHashTable[uiIndex];

    // search list at hash table location for key
    if (pkItem)
    {
        if (IsKeysEqual(kKey, pkItem->m_key))
        {
            // item at front of list, remove it
            m_ppkHashTable[uiIndex] = pkItem->m_pkNext;
            ClearValue(pkItem);

            DeleteItem(pkItem);

            m_kAllocator.m_uiCount--;
            return true;
        }
        else
        {
            // search rest of list for item
            NiTMapItem<NiFixedString, TVAL>* pkPrev = pkItem;
            NiTMapItem<NiFixedString, TVAL>* pkCurr = pkPrev->m_pkNext;
            while (pkCurr && !IsKeysEqual(kKey, pkCurr->m_key))
            {
                pkPrev = pkCurr;
                pkCurr = pkCurr->m_pkNext;
            }
            if (pkCurr)
            {
                // found the item, remove it
                pkPrev->m_pkNext = pkCurr->m_pkNext;
                ClearValue(pkCurr);

                DeleteItem(pkCurr);

                m_kAllocator.m_uiCount--;
                return true;
            }
        }
    }

    return false;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    void NiTFixedStringMapBase<TheAllocator, TVAL>::RemoveAll()
{
    for (unsigned int i = 0; i < m_uiHashSize; i++) 
    {
        while (m_ppkHashTable[i])
        {
            NiTMapItem<NiFixedString, TVAL>* pkSave = m_ppkHashTable[i];
            m_ppkHashTable[i] = m_ppkHashTable[i]->m_pkNext;
            ClearValue(pkSave);

            DeleteItem(pkSave);
        }
    }

    m_kAllocator.m_uiCount = 0;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    bool NiTFixedStringMapBase<TheAllocator, TVAL>::GetAt(
    const NiFixedString& kKey, TVAL& val) const
{
    // look up hash table location for key
    unsigned int uiIndex = KeyToHashIndex(kKey);
    NiTMapItem<NiFixedString, TVAL>* pkItem = m_ppkHashTable[uiIndex];

    // search list at hash table location for key
    while (pkItem)
    {
        if (IsKeysEqual(kKey, pkItem->m_key))
        {
            // item found
            val = pkItem->m_val;
            return true;
        }
        pkItem = pkItem->m_pkNext;
    }

    return false;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    unsigned int NiTFixedStringMapBase<TheAllocator, TVAL>::KeyToHashIndex(
    const NiFixedString& kKey) const
{
    const char* pcKey = kKey;
    return (unsigned int) (((size_t) pcKey) % m_uiHashSize);
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    bool NiTFixedStringMapBase<TheAllocator, TVAL>::IsKeysEqual(
    const NiFixedString& kKey1, const NiFixedString& kKey2) const
{
    return kKey1 == kKey2;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    void NiTFixedStringMapBase<TheAllocator, TVAL>::SetValue(
    NiTMapItem<NiFixedString, TVAL>* pkItem, NiFixedString& kKey, TVAL val)
{
    pkItem->m_key = kKey;
    pkItem->m_val = val;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline 
    void NiTFixedStringMapBase<TheAllocator, TVAL>::ClearValue(
    NiTMapItem<NiFixedString, TVAL>* /* pkItem */)
{
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    NiTMapIterator NiTFixedStringMapBase<TheAllocator, TVAL>::GetFirstPos() 
    const
{
    for (unsigned int i = 0; i < m_uiHashSize; i++) 
    {
        if (m_ppkHashTable[i])
            return m_ppkHashTable[i];
    }
    return 0;
}
//---------------------------------------------------------------------------
template <class TheAllocator, class TVAL> inline
    void NiTFixedStringMapBase<TheAllocator, TVAL>::GetNext(
    NiTMapIterator& pos, NiFixedString& kKey, TVAL& val) const
{
    NiTMapItem<NiFixedString, TVAL>* pkItem = 
        (NiTMapItem<NiFixedString, TVAL>*) pos;

    kKey = pkItem->m_key;
    val = pkItem->m_val;

    if (pkItem->m_pkNext)
    {
        pos = pkItem->m_pkNext;
        return;
    }

    unsigned int i = KeyToHashIndex(pkItem->m_key);
    for (++i; i < m_uiHashSize; i++) 
    {
        pkItem = m_ppkHashTable[i];
        if (pkItem) 
        {
            pos = pkItem;
            return;
        }
    }

    pos = 0;
}
//---------------------------------------------------------------------------
template <class TVAL> inline NiTFixedStringMap<TVAL>::~NiTFixedStringMap()
{
    // RemoveAll is called from here because it depends on virtual functions
    // implemented in NiTAllocatorMap.  It will also be called in the 
    // parent destructor, but the map will already be empty.
    NiTFixedStringMap<TVAL>::RemoveAll();
}
//---------------------------------------------------------------------------
template <class TVAL> inline
    NiTMapItem<NiFixedString, TVAL>* NiTFixedStringMap<TVAL>::NewItem()
{
    NiTMapItem<NiFixedString, TVAL>* pkTMapItem = 
        (NiTMapItem<NiFixedString, TVAL>*)
        NiTFixedStringMapBase<NiTDefaultAllocator<TVAL>, TVAL>::m_kAllocator.
        Allocate();
    // Because the current NiTDefaultAllocator creates the key as an
    // unsigned int rather than as an NiFixedString, the initial value of 
    // the key must be set to 0.
    memset(&pkTMapItem->m_key, 0, sizeof(pkTMapItem->m_key));
    return pkTMapItem;
}
//---------------------------------------------------------------------------
template <class TVAL> inline
    void NiTFixedStringMap<TVAL>::DeleteItem(
    NiTMapItem<NiFixedString, TVAL>* pkItem)
{
    // set key and val to zero so that if they are smart pointers
    // their references will be decremented.
    pkItem->m_key = 0;
    pkItem->m_val = 0;
    NiTFixedStringMapBase<NiTDefaultAllocator<TVAL>, TVAL>::m_kAllocator.
        Deallocate(pkItem);
}
//---------------------------------------------------------------------------
