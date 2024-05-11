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

#ifndef NIENTITYSELECTIONSET_H
#define NIENTITYSELECTIONSET_H

#include <NiTPtrSet.h>
#include "NiEntityInterface.h"

class NIENTITY_ENTRY NiEntitySelectionSet : public NiRefObject
{
public:
    NiEntitySelectionSet(const NiFixedString& kName,
        unsigned int uiInitialSize = 0);

    const NiFixedString& GetName() const;
    void SetName(const NiFixedString& kName);

    void AddEntity(NiEntityInterface* pkEntity);
    unsigned int GetEntityCount() const;
    NiEntityInterface* GetEntityAt(unsigned int uiIndex) const;
    void RemoveEntity(NiEntityInterface* pkEntity);
    void RemoveEntityAt(unsigned int uiIndex);
    void RemoveAllEntities();

private:
    NiFixedString m_kName;
    NiTObjectPtrSet<NiEntityInterfaceIPtr> m_kEntities;
};

NiSmartPointer(NiEntitySelectionSet);

#include "NiEntitySelectionSet.inl"

#endif // NIENTITYSELECTIONSET_H
