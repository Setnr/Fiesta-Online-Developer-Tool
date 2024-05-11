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

#ifndef NIADDREMOVEPROPERTYCOMMAND_H
#define NIADDREMOVEPROPERTYCOMMAND_H

#include <NiRefObject.h>
#include <NiSmartPointer.h>
#include "NiEntityCommandInterface.h"
#include "NiEntityInterface.h"
#include "NiEntityComponentInterface.h"

class NIENTITY_ENTRY NiAddRemovePropertyCommand : public NiRefObject,
    public NiEntityCommandInterface
{
public:
    NiAddRemovePropertyCommand(
        NiEntityPropertyInterface* pkPropertyInterface,
        const NiFixedString& kPropertyName, 
        const NiFixedString& kDisplayName,
        const NiFixedString& kPrimitiveType,
        const NiFixedString& kSemanticType, 
        const NiFixedString& kDescription,
        bool bAddProperty);
    NiAddRemovePropertyCommand(
        NiEntityPropertyInterface* pkPropertyInterface,
        const NiFixedString& kPropertyName, 
        const NiFixedString& kDisplayName,
        const NiFixedString& kPrimitiveType,
        const NiFixedString& kSemanticType, 
        bool bCollection,
        const NiFixedString& kDescription,
        bool bAddProperty);
        

    NiEntityPropertyInterface* GetPropertyInterface() const;
    const NiFixedString& GetPropertyName() const;
    NiBool GetAddProperty() const;

private:
    NiFixedString m_kCommandName;

    void GenerateCommandName();

    NiEntityPropertyInterfaceIPtr m_spPropertyInterface;
    const NiFixedString m_kPropertyName;
    const NiFixedString m_kDisplayName;
    const NiFixedString m_kPrimitiveType;
    const NiFixedString m_kSemanticType;
    const NiFixedString m_kDescription;
    bool m_bCollection;
    bool m_bAddProperty;
    bool m_bSuccessfullyExecuted;
    bool m_bShouldUndo;

public:
    // NiEntityCommandInterface overrides.
    virtual void AddReference();
    virtual void RemoveReference();
    virtual NiFixedString GetName();
    virtual void DoCommand(NiEntityErrorInterface* pkErrors, bool bUndoable);
    virtual void UndoCommand(NiEntityErrorInterface* pkErrors);
};

NiSmartPointer(NiAddRemovePropertyCommand);

#include "NiAddRemovePropertyCommand.inl"

#endif // NIADDREMOVEPROPERTYCOMMAND_H
