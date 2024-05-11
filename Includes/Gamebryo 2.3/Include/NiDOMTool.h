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

#ifndef NIDOMTOOL_H
#define NIDOMTOOL_H

#include "NiEntityLibType.h"
#include "NiDOMTool.h"
#include "NiBase64.h"

#include <NiFixedString.h>
#include <NiPoint3.h>
#include <NiPoint2.h>
#include <NiMatrix3.h>
#include <NiColor.h>
#include <NiQuaternion.h>
#include <NiTSet.h>

class TiXmlDocument;
class TiXmlElement;

// Abstract class for dealing with various streaming methods
class NIENTITY_ENTRY NiDOMTool 
{
public:
    NiDOMTool();
    NiDOMTool(const char* pcFileName);
    ~NiDOMTool();

    void Init(const char* pcFileName);
    void Flush();

    // I/O Functions
    NiBool LoadFile();
    NiBool SaveFile();
   
    // DOM section manipulation functions
    TiXmlElement* SetSectionToNextSibling();
    TiXmlElement* SetSectionToFirstChild();
    TiXmlElement* SetSectionTo(const char* pcSection);
    TiXmlElement* ResetSectionTo(const char* pcSection);
    TiXmlElement* GetCurrentSection();
    NiBool IsCurrentSectionValid();
    NiBool EndSection();
    
    // DOM writing functions
    NiBool BeginSection(const char* pcName);
    NiBool InsertComment(const char* pcComment);
    NiBool AssignAttribute(const char* pcAttributeName, int iValue);
    NiBool AssignAttribute(const char* pcAttributeName, const char* pcValue);
    NiBool WriteHeader();
    
    // DOM read functions
    int GetAttributeCountFromCurrent();
    const char* GetAttributeFromCurrent(const char* pcAttribute);
    const char* GetValueFromCurrent();
    const char* GetTextFromCurrent();
    NiBool IsCurrentLeaf();

    // Data serialization methods
    NiBool WritePrimitive(const float& fData);
    NiBool WritePrimitive(const bool& bData);
    NiBool WritePrimitive(const int& iData);
    NiBool WritePrimitive(const unsigned int& uiData);
    NiBool WritePrimitive(const short& sData);
    NiBool WritePrimitive(const unsigned short& usData);
    NiBool WritePrimitive(const NiFixedString& kData);
    NiBool WritePrimitive(const NiPoint2& kData);
    NiBool WritePrimitive(const NiPoint3& kData);
    NiBool WritePrimitive(const NiQuaternion& kData);
    NiBool WritePrimitive(const NiMatrix3& kData);
    NiBool WritePrimitive(const NiColor& kData);
    NiBool WritePrimitive(const NiColorA& kData);
    NiBool WritePrimitive(const unsigned char* pucData,
        size_t stDataSizeInBytes);

    NiBool ReadPrimitive(float& fData);
    NiBool ReadPrimitive(bool& bData);
    NiBool ReadPrimitive(int& iData);
    NiBool ReadPrimitive(unsigned int& uiData);
    NiBool ReadPrimitive(short& sData);
    NiBool ReadPrimitive(unsigned short& usData);
    NiBool ReadPrimitive(NiFixedString& kData);
    NiBool ReadPrimitive(NiPoint2& kData);
    NiBool ReadPrimitive(NiPoint3& kData);
    NiBool ReadPrimitive(NiQuaternion& kData);
    NiBool ReadPrimitive(NiMatrix3& kData);
    NiBool ReadPrimitive(NiColor& kData);
    NiBool ReadPrimitive(NiColorA& kData);
    NiBool ReadPrimitive(unsigned char*& pucData, size_t& stDataSizeInBytes);

    // *** begin Emergent internal use only ***
    // DOM error retrieves
    int GetErrorID();
    const char* GetErrorDesc();
    int GetErrorRow();
    int GetErrorCol();
    // *** end Emergent internal use only ***

protected:
    TiXmlDocument* m_pkXMLDocument;
    NiTPrimitiveSet<TiXmlElement*>* m_pkElementSet;
};

#endif // NIDOMTOOL_H
