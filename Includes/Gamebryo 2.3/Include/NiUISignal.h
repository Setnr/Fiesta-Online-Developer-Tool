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

#ifndef NIUISIGNAL_H
#define NIUISIGNAL_H

#include "NiUserInterfaceLibType.h"
#include <NiTSet.h>
//---------------------------------------------------------------------------
// These classes/templates may be found in NiSlot.h.  They are forward 
// declared here in order to avoid circular dependancy.
class NiUIBaseSlot0;
template <typename Arg1Type>
class NiUIBaseSlot1;
template <typename Arg1Type, typename Arg2Type>
class NiUIBaseSlot2;
//---------------------------------------------------------------------------
// These three classes are identical in all ways except in the parameters 
// which their "EmitSignal" functions require and they type of NiUIBaseSlot*
// they can refer to.
class NiUISignal0 : public NiMemObject
{
public:
    NiUISignal0();
    virtual ~NiUISignal0();
    void EmitSignal() const;
    void Subscribe(NiUIBaseSlot0* pkSlot);
    void Unsubscribe(NiUIBaseSlot0* pkSlot);
    bool IsSubscribed(NiUIBaseSlot0* pkSlot) const;
    unsigned int NumSubscribed() const;
protected:
    NiTPrimitiveSet <NiUIBaseSlot0*> m_pkSlots;
};
//---------------------------------------------------------------------------
template <typename Arg1Type>
class NiUISignal1 : public NiMemObject
{
public:
    NiUISignal1();
    virtual ~NiUISignal1();
    void EmitSignal(Arg1Type Arg1) const;
    void Subscribe(NiUIBaseSlot1<Arg1Type>* pkSlot);
    void Unsubscribe(NiUIBaseSlot1<Arg1Type>* pkSlot);
    bool IsSubscribed(NiUIBaseSlot1<Arg1Type>* pkSlot) const;
    unsigned int NumSubscribed() const;
protected:
    NiTPrimitiveSet <NiUIBaseSlot1<Arg1Type>*> m_pkSlots;
};
//---------------------------------------------------------------------------
template <typename Arg1Type, typename Arg2Type>
class NiUISignal2 : public NiMemObject
{
public:
    NiUISignal2();
    virtual ~NiUISignal2();
    void EmitSignal(Arg1Type Arg1, Arg2Type Arg2) const;
    void Subscribe(NiUIBaseSlot2<Arg1Type, Arg2Type>* pkSlot);
    void Unsubscribe(NiUIBaseSlot2<Arg1Type, Arg2Type>* pkSlot);
    bool IsSubscribed(NiUIBaseSlot2<Arg1Type, Arg2Type>* pkSlot) const;
    unsigned int NumSubscribed() const;
protected:
    NiTPrimitiveSet <NiUIBaseSlot2<Arg1Type, Arg2Type>*> m_pkSlots;
};
//---------------------------------------------------------------------------

#include "NiUISignal.inl"

#endif // NIUISIGNAL_H

