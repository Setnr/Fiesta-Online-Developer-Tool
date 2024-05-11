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

#ifndef NIENTITYSTREAMINGASCII_H
#define NIENTITYSTREAMINGASCII_H

#include "NiEntityLibType.h"
#include "NiEntityStreaming.h"
#include "NiDOMTool.h"
#include "NiEntityPropertyInterface.h"
#include <NiFixedString.h>
#include <NiTSet.h>

#define REF_LINK_NULL 4294967295U // Max value for unsigned int

class NiEntityInterface;
class NiEntityComponentInterface;

// Abstract class for dealing with various streaming methods
class NIENTITY_ENTRY NiEntityStreamingAscii : public NiEntityStreaming
{
public:
    static NiFixedString STREAMING_EXTENSION;
    static NiFixedString STREAMING_DESCRIPTION;

    NiEntityStreamingAscii();
    virtual ~NiEntityStreamingAscii();

    // I/O Functions.
    virtual NiBool Load(const char* pcFileName);
    virtual NiBool Save(const char* pcFileName);

    // File Extension of format that can be load/saved.
    // Also used for registering factory so that different files
    // can be read and saved appropriately.
    virtual NiFixedString GetFileExtension() const;
    virtual NiFixedString GetFileDescription() const;

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    void Flush();
    NiBool StoreWorkingPath(const char* pcFileName);

    NiBool CreateObjectsAndAssignUniqueLinkIDs(
        NiEntityStreaming::IDLinkMaps& kMaps);

    // DOM writing
    void WriteToDOM();
    void WriteVersionToDOM();
    void WriteSceneToDOM(NiScene* pkScene);
    void WriteTemplateIDToDOM(NiEntityPropertyInterface* pkPropIntf);
    void WriteEntityToDOM(NiEntityInterface* pkEntity);
    void WriteEntityRefToDOM(NiEntityInterface* pkEntity);
    void WriteComponentToDOM(NiEntityComponentInterface* pkComp);
    void WriteSelectionSetsToDOM(NiScene* pkScene);
    void WriteMapObjectsToDOM();
    void WritePropertyToDOM(NiEntityPropertyInterface*
        pkPropIntf, const NiFixedString& kPropertyName);
    void WritePropertyArrayToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, NiFixedString& kPrimitiveType,
        unsigned int uiCount);
    void WritePropertyToDOM(NiEntityPropertyInterface* pkPropIntf,
        const NiFixedString& kPropertyName, NiFixedString& kPrimitiveType, 
        unsigned int uiIndex);

    void WritePropertyInfoToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName);
    void WriteEntityPointerToDOM(NiEntityPropertyInterface* pkPropIntf,
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteObjectPointerToDOM(NiEntityPropertyInterface* pkPropIntf,
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteFloatToDOM(NiEntityPropertyInterface* pkPropIntf,
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteBoolToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteIntToDOM(NiEntityPropertyInterface* pkPropIntf,
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteUIntToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteShortToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteUShortToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteStringToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WritePoint2ToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WritePoint3ToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteQuaternionToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteMatrix3ToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteColorToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteColorAToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);
    void WriteUnknownTypeToDOM(NiEntityPropertyInterface* pkPropIntf, 
        const NiFixedString& kPropertyName, unsigned int uiIndex = 0);

    // DOM Reading
    NiBool ReadClassValueNameLinkID(const char*& pcClass, const char*& pcValue,
        const char*& pcName, unsigned int& uiLinkID);
    NiBool ReadMasterRefLinkID(unsigned int& uiRefLinkID);
    NiBool ReadRefLinkID(unsigned int& uiRefLinkID);

    NiBool ReadFromDOM();
    NiBool ReadAndCheckVersionFromDOM();
    //NiBool ReadExternalAssetsFromDOM();
    NiBool ReadScenesFromDOM();
    NiBool ReadSceneFromDOM(NiScene* pkScene);
    NiBool ReadSelectionSetsFromDOM(NiScene* pkScene);
    NiBool ReadMasterFromDOM(NiEntityPropertyInterface* pkPropIntf);
    NiBool ReadEntityOrComponentFromDOM(
        NiEntityPropertyInterface* pkPropIntf);
    NiBool ReadEntityRefFromDOM(NiEntityInterface*& pkEntity);
    NiBool ReadPropertyFromDOM(NiEntityPropertyInterface* pkPropIntf);
    NiBool ReadPropertyFromDOM(const char* pcPrimitive, const char* pcName, 
        NiEntityPropertyInterface* pkPropIntf);
    NiBool ReadPropertyFromDOM(const char* pcPrimitive,
        const char* pcName, const char* pcSemanticType, 
        const char* pcDisplayName, const char* pcDescription,
        NiEntityPropertyInterface* pkPropIntf);
    NiBool ReadPropertyArrayFromDOM(const char* pcPrimitive,
        const char* pcName, NiEntityPropertyInterface* pkPropIntf);
    NiBool ReadObjectsFromDOM();


    NiBool ReadEntityPointerFromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadObjectPointerFromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadFloatFromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadBoolFromDOM(NiEntityPropertyInterface* pkPropIntf,
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadIntFromDOM(NiEntityPropertyInterface* pkPropIntf,
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadUIntFromDOM(NiEntityPropertyInterface* pkPropIntf,
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadShortFromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadUShortFromDOM(NiEntityPropertyInterface* pkPropIntf,
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadStringFromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadPoint2FromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadPoint3FromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadQuaternionFromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadMatrix3FromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadColorFromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadColorAFromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);
    NiBool ReadUnknownTypeFromDOM(NiEntityPropertyInterface* pkPropIntf, 
        const char* pcName, unsigned int uiIndex = 0);

    NiEntityStreaming::IDLinkMaps m_kLinkMaps;

    NiDOMTool m_kDOM;

    char m_acFullPath[NI_MAX_PATH];
    char m_acFilenameErrorMsg[512];
};

#include "NiEntityStreamingAscii.inl"

#endif // NIENTITYSTREAMINGASCII_H
