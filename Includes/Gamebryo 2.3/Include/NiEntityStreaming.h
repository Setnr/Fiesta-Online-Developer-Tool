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

#ifndef NIENTITYSTREAMING_H
#define NIENTITYSTREAMING_H

#include "NiEntityLibType.h"
#include "NiRefObject.h"
#include "NiScene.h"
#include <NiTArray.h>

// Abstract class for dealing with various streaming methods
class NIENTITY_ENTRY NiEntityStreaming : public NiRefObject
{
public:
    NiEntityStreaming();
    virtual ~NiEntityStreaming();

    // Scene Management Functions.
    void InsertScene(NiScene* pkScene);
    void RemoveScene(NiScene* pkScene);
    void RemoveAllScenes();

    unsigned int GetSceneCount() const;
    unsigned int GetEffectiveSceneCount() const;
    NiScene* GetSceneAt(unsigned int ui) const;

    // I/O Functions.
    virtual NiBool Load(const char* pcFileName) = 0;
    virtual NiBool Save(const char* pcFileName) = 0;

    // Set Error Handler
    void SetErrorHandler(NiEntityErrorInterface* pkErrorIntf);

    // File Extension of format that can be load/saved.
    // Also used for registering factory so that different files
    // can be read and saved appropriately.
    virtual NiFixedString GetFileExtension() const = 0;
    virtual NiFixedString GetFileDescription() const = 0;

protected:
    class NIENTITY_ENTRY IDLinkMaps
    {
    public:
        void RemoveLinks();

        void AssignID(NiEntityPropertyInterface* pkPropIntf);
        void AssignID(NiEntityPropertyInterface* pkPropIntf,
            unsigned int uiID);

        NiEntityPropertyInterface* GetAt(unsigned int uiID);
        NiBool GetID(NiEntityPropertyInterface* pkPropIntf,
            unsigned int& uiID);
        unsigned int GetIDCount();

    protected:
        NiTMap<NiEntityPropertyInterface*, unsigned int> m_kPropToIDMap;
        NiTMap<unsigned int, NiEntityPropertyInterfaceIPtr> m_kIDMap;
    };
   
    void RecurseEntityPointerProperties(IDLinkMaps& kMaps,
        NiEntityPropertyInterface* pkPropIntf);
    void AssignUniqueLinkIDs(IDLinkMaps& kMaps);

    void ReportError(const NiFixedString& kErrorMessage,
        const NiFixedString& kErrorDescription,
        const NiFixedString& kEntityName, const NiFixedString& kPropertyName);

    void MapToID(IDLinkMaps& kMaps, NiEntityPropertyInterface* pkPropIntf);
    void RecursiveMapToID(IDLinkMaps& kMaps, NiEntityInterface* pkEntity);
    void RecursiveMapToID(IDLinkMaps& kMaps, 
        NiEntityComponentInterface* pkComp);

    NiTObjectArray<NiScenePtr> m_kTopScenes;
    NiEntityErrorInterface* m_pkErrorHandler;
};

#endif // NIENTITYSTREAMING_H
