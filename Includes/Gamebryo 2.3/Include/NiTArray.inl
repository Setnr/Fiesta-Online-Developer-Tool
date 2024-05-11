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
//  NiTArray  inline functions

//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
NiTArray<T, TAlloc>::NiTArray(unsigned int uiMaxSize, unsigned int uiGrowBy)
{
    NIASSERT(uiMaxSize <= USHRT_MAX);
    NIASSERT(uiGrowBy <= USHRT_MAX);

    m_usMaxSize = (unsigned short)uiMaxSize;
    m_usGrowBy = (unsigned short)uiGrowBy;
    m_usSize = 0;
    m_usESize = 0;

    if (m_usMaxSize > 0)
    {
        m_pBase = TAlloc::Allocate(m_usMaxSize);
        NIASSERT(m_pBase);
    }
    else
    {
        m_pBase = 0;
    }
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
NiTArray<T, TAlloc>::~NiTArray()
{
    TAlloc::Deallocate(m_pBase);
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
T* NiTArray<T, TAlloc>::GetBase()
{
    return m_pBase;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
unsigned int NiTArray<T, TAlloc>::GetSize() const
{
    return m_usSize;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
unsigned int NiTArray<T, TAlloc>::GetEffectiveSize() const
{
    return m_usESize;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
unsigned int NiTArray<T, TAlloc>::GetAllocatedSize() const
{
    return m_usMaxSize;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
unsigned int NiTArray<T, TAlloc>::GetGrowBy() const
{
    return m_usGrowBy;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
void NiTArray<T, TAlloc>::SetSize(unsigned int uiMaxSize)
{
    NIASSERT(uiMaxSize <= USHRT_MAX);

    if (uiMaxSize == m_usMaxSize)
    {
        return;
    }

    // If the number of slots gets smaller, the elements in the unwanted
    // slots must be zeroed in case class T has side effects that must occur.
    // For example, if T is a smart pointer class, then decrementing the ref
    // count must occur.
    unsigned short i;
    if (uiMaxSize < m_usSize)
    {
        for (i = (unsigned short)uiMaxSize; i < m_usSize; i++)
        {
            if (m_pBase[i] != T(0))
            {
                m_pBase[i] = T(0);
                m_usESize--;
            }
        }

        m_usSize = uiMaxSize;
    }

    T* pSaveBase = m_pBase;
    m_usMaxSize = uiMaxSize;
    if (uiMaxSize > 0)
    {
        // allocate a new array
        m_pBase = TAlloc::Allocate(m_usMaxSize);
        NIASSERT(m_pBase);

        // copy old array to new array
        for (i = 0; i < m_usSize; i++)
        {
            m_pBase[i] = pSaveBase[i];
        }
        // initialize new memory
        for (i = m_usSize; i < m_usMaxSize; i++)
        {
            m_pBase[i] = T(0);  
        }

        
    }
    else
    {
        m_pBase = 0;
    }

    // delete old array
    TAlloc::Deallocate(pSaveBase);
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
void NiTArray<T, TAlloc>::SetGrowBy(unsigned int uiGrowBy)
{
    NIASSERT(uiGrowBy <= USHRT_MAX);
    m_usGrowBy = uiGrowBy;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
const T& NiTArray<T, TAlloc>::GetAt(unsigned int uiIndex) const
{
    NIASSERT(uiIndex < m_usMaxSize);
    return m_pBase[uiIndex];
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
void NiTArray<T, TAlloc>::SetAt(unsigned int uiIndex, const T& element)
{
    NIASSERT(uiIndex < m_usMaxSize);

    if (uiIndex >= m_usSize)
    {
        m_usSize = uiIndex+1;
        if (element != T(0))
        {
            m_usESize++;
        }
    }
    else 
    {
        if (element != T(0))
        {
            if (m_pBase[uiIndex] == T(0))
            {
                m_usESize++;
            }
        }
        else
        {
            if (m_pBase[uiIndex] != T(0))
            {
                m_usESize--;
            }
        }
    }

    m_pBase[uiIndex] = element;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
unsigned int NiTArray<T, TAlloc>::SetAtGrow(unsigned int uiIndex, 
    const T& element)
{
    NIASSERT(uiIndex <= USHRT_MAX);

    if (uiIndex >= m_usMaxSize)
    {
        SetSize(uiIndex + m_usGrowBy);
    }

    SetAt(uiIndex, element);
    return uiIndex;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
unsigned int NiTArray<T, TAlloc>::Add(const T& element)
{
    return SetAtGrow(m_usSize, element);
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
unsigned int NiTArray<T, TAlloc>::AddFirstEmpty(const T& element)
{
    if (element == T(0))
    {
        return 0xffffffff;
    }

    for (unsigned short i = 0; i < m_usSize; i++)
    {
        if (m_pBase[i] == T(0))
        {
            // empty slot - add here
            m_pBase[i] = element;
            m_usESize++;
            return i;
        }
    }

    // no empty slots - add at end
    return SetAtGrow(m_usSize, element);
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
T NiTArray<T, TAlloc>::RemoveAt(unsigned int uiIndex)
{
    if (uiIndex >= m_usSize)
    {
        return T(0);
    }
    
    T element = m_pBase[uiIndex];
    m_pBase[uiIndex] = T(0);

    if (element != T(0))
    {
        m_usESize--;
    }

    if (uiIndex == (unsigned short)(m_usSize - 1))
    {
        m_usSize--;
    }

    return element;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
T NiTArray<T, TAlloc>::RemoveAtAndFill(unsigned int uiIndex)
{
    if (uiIndex >= m_usSize)
    {
        return T(0);
    }
    
    m_usSize--;
    T element = m_pBase[uiIndex];

    m_pBase[uiIndex] = m_pBase[m_usSize];
    m_pBase[m_usSize] = T(0);

    if (element != T(0))
    {
        m_usESize--;
    }

    return element;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
T NiTArray<T, TAlloc>::RemoveEnd()
{
    if (m_usSize == 0)
        return T(0);

    m_usSize--;
    T element = m_pBase[m_usSize];
    m_pBase[m_usSize] = T(0);

    if (element != T(0))
    {
        m_usESize--;
    }

    return element;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
void NiTArray<T, TAlloc>::RemoveAll()
{
    // The elements in the to-be-removed slots must be zeroed in case class
    // T has side effects that must occur.  For example, if T is a smart
    // pointer class, then decrementing the ref count must occur.
    for (unsigned short i = 0; i < m_usSize; i++)
    {
        m_pBase[i] = T(0);
    }

    m_usSize = 0;
    m_usESize = 0;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
unsigned int NiTArray<T, TAlloc>::Remove(const T& element)
{
    if (element != T(0))
    {
        for (unsigned short i = 0; i < m_usSize; i++)
        {
            if (m_pBase[i] == element)
            {
                m_pBase[i] = T(0);

                m_usESize--;
                if (i == m_usSize - 1)
                    m_usSize--;

                return i;
            }
        }
    }

    return (unsigned int)~0;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
unsigned int NiTArray<T, TAlloc>::Find(const T& element, 
    unsigned int uiStartIdx)
{
    if (element != T(0))
    {
        for (unsigned short i = (unsigned short)uiStartIdx; i < m_usSize; i++)
        {
            if (m_pBase[i] == element)
            {
                return i;
            }
        }
    }

    return (unsigned int)~0;
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
void NiTArray<T, TAlloc>::Compact()
{
    if (m_usESize == m_usSize)
    {
        return;
    }

    // move elements to contiguous memory at beginning of array
    if (m_usESize)
    {
        for (unsigned short i = 0, j = 0; i < m_usSize; i++)
        {
            if (m_pBase[i] != T(0))
            {
                if (m_pBase[j] != m_pBase[i])
                {
                    m_pBase[j] = m_pBase[i];
                }
                j++;
            }
        }
    }

    // downsize storage
    T* pSaveBase = m_pBase;
    m_usSize = m_usESize;
    m_usMaxSize = m_usSize;
    if (m_usMaxSize > 0)
    {
        m_pBase = TAlloc::Allocate(m_usMaxSize);
        NIASSERT(m_pBase);

        // copy old array to new array
        for (unsigned short i = 0; i < m_usSize; i++)
        {
            m_pBase[i] = pSaveBase[i];
        }
    }
    else
    {
        m_pBase = 0;
    }

    // delete old array
    TAlloc::Deallocate(pSaveBase);
}
//---------------------------------------------------------------------------
template <class T, class TAlloc> inline
void NiTArray<T, TAlloc>::UpdateSize()
{
    while (m_usSize > 0)
    {
        if (m_pBase[m_usSize - 1] != T(0))
        {
            break;
        }

        m_usSize--;
    }
}
//---------------------------------------------------------------------------
template <class T> inline
NiTObjectArray<T>::NiTObjectArray(unsigned int uiMaxSize, 
    unsigned int uiGrowBy) : NiTArray<T, NiTNewInterface<T> >(uiMaxSize, 
    uiGrowBy)
{  
}
//---------------------------------------------------------------------------
template <class T> inline
NiTPrimitiveArray<T>::NiTPrimitiveArray(unsigned int uiMaxSize, 
    unsigned int uiGrowBy) : NiTArray<T, NiTMallocInterface<T> >(
    uiMaxSize, uiGrowBy)
{  
}
//---------------------------------------------------------------------------
