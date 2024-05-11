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

#ifndef NISCENE_H
#define NISCENE_H

#include "NiEntityLibType.h"
#include <NiRefObject.h>
#include <NiSmartPointer.h>
#include "NiEntityInterface.h"
#include "NiEntitySelectionSet.h"
#include "NiTPtrSet.h"
#include <NiAVObject.h>

class NiEntityErrorInterface;
class NiExternalAssetManager;

class NIENTITY_ENTRY NiScene : public NiRefObject
{
public:
    NiScene(const NiFixedString& kName, unsigned int uiEntityArraySize = 0,
        unsigned int uiSelectionSetArraySize = 0);

    // Functions for getting and setting the scene name.
    const NiFixedString& GetName() const;
    void SetName(const NiFixedString& kName);

    // Functions for managing entities.
    unsigned int GetEntityCount() const;
    NiEntityInterface* GetEntityAt(unsigned int uiIndex) const;
    NiEntityInterface* GetEntityByName(const NiFixedString& kName) const;
    NiBool AddEntity(NiEntityInterface* pkEntity);
    void RemoveEntity(NiEntityInterface* pkEntity);
    void RemoveEntityAt(unsigned int uiIndex);
    void RemoveAllEntities();
    NiBool IsEntityInScene(NiEntityInterface* pkEntity) const;

    // Functions for managing selection sets.
    unsigned int GetSelectionSetCount() const;
    NiEntitySelectionSet* GetSelectionSetAt(unsigned int uiIndex) const;
    NiEntitySelectionSet* GetSelectionSetByName(const NiFixedString& kName)
        const;
    NiBool AddSelectionSet(NiEntitySelectionSet* pkSelectionSet);
    void RemoveSelectionSet(NiEntitySelectionSet* pkSelectionSet);
    void RemoveSelectionSetAt(unsigned int uiIndex);
    void RemoveAllSelectionSets();

    // Computes the world-space bound of the scene.
    void GetBound(NiBound& kBound);

    // Updates all entites.
    void Update(float fTime, NiEntityErrorInterface* pkErrors,
        NiExternalAssetManager* pkAssetManager);

    // BuildVisibleSet for all entities.
    void BuildVisibleSet(NiEntityRenderingContext* pkRenderingContext,
        NiEntityErrorInterface* pkErrors);

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

private:
    NiFixedString m_kName;
    NiTObjectPtrSet<NiEntityInterfaceIPtr> m_kEntities;
    NiTPointerMap<NiEntityInterface*, bool> m_kQuickEntityLookUpMap;
    NiTObjectPtrSet<NiEntitySelectionSetPtr> m_kSelectionSets;

    static NiFixedString ms_kSceneRootName;
};

NiSmartPointer(NiScene);

#include "NiScene.inl"

#endif // NISCENE_H
