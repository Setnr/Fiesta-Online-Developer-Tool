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
inline void NiEntityStreamingAscii::WriteEntityPointerToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    // In this case, we must recurse...
    NiEntityInterface* pkPropIntfSubComp = NULL;
    pkPropIntf->GetPropertyData(
        kPropertyName, pkPropIntfSubComp, uiIndex);

    if (pkPropIntfSubComp == NULL)
    {
        m_kDOM.AssignAttribute("RefLinkID", "NULL");
        return;
    }

    unsigned int uiID;
    if (m_kLinkMaps.GetID(pkPropIntfSubComp, uiID))
    {
        m_kDOM.AssignAttribute("RefLinkID", uiID);
    }
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteObjectPointerToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    NIASSERT(!"Streaming NiObject Pointer properties is not supported!");
    return;
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteFloatToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    float kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteBoolToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    bool kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteIntToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    int kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteUIntToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    unsigned int kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteShortToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    short kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteUShortToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    unsigned short kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteStringToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    NiFixedString kData;
    NIVERIFY(pkPropIntf->GetPropertyData(kPropertyName, kData, uiIndex));

    if (kData)
    {
        // Is this string a path and is it registered? If so, we need
        // to convert it to be file relative.
        bool bIsExternalAssetPath;
        NIVERIFY(pkPropIntf->IsExternalAssetPath(kPropertyName, uiIndex,
            bIsExternalAssetPath));
        if (bIsExternalAssetPath)
        {
            // This string is a path that has been registered.
            NiUInt32 uiBytes;
            char acRelativePath[NI_MAX_PATH];
            uiBytes = NiPath::ConvertToRelative(acRelativePath ,NI_MAX_PATH, 
                kData, m_acFullPath);
            if (uiBytes == 0)
            {
                // NiPath::ConvertToRelative could have failed if pcKey was
                // a network path or if the path was another drive letter.
                // In either case, we can only store the absolute path.
                char acStdAbsPath[NI_MAX_PATH];
                NiStrcpy(acStdAbsPath, NI_MAX_PATH, kData);
                NiPath::Standardize(acStdAbsPath); // Standardize the path
                NiStrcpy(acRelativePath, NI_MAX_PATH, acStdAbsPath);
            }
            m_kDOM.AssignAttribute("Asset", "TRUE");
            m_kDOM.WritePrimitive(NiFixedString(acRelativePath));
        }
        else
        {
            m_kDOM.WritePrimitive(kData);
        }
    }
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WritePoint2ToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    NiPoint2 kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WritePoint3ToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    NiPoint3 kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteQuaternionToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    NiQuaternion kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteMatrix3ToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    NiMatrix3 kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteColorToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    NiColor kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteColorAToDOM(
    NiEntityPropertyInterface* pkPropIntf, const NiFixedString& kPropertyName,
    unsigned int uiIndex)
{
    NiColorA kData;
    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, kData, 
        uiIndex)));
    m_kDOM.WritePrimitive(kData);
}
//---------------------------------------------------------------------------
inline void NiEntityStreamingAscii::WriteUnknownTypeToDOM(
    NiEntityPropertyInterface* pkPropIntf, 
    const NiFixedString& kPropertyName, unsigned int uiIndex)
{
    void* pkData;
    size_t stDataSizeInBytes;

    NIVERIFY(NIBOOL_IS_TRUE(pkPropIntf->GetPropertyData(kPropertyName, 
        pkData, stDataSizeInBytes, uiIndex)));
    const unsigned char* pucData = (const unsigned char*)pkData;
    m_kDOM.WritePrimitive(pucData, stDataSizeInBytes);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadEntityPointerFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    // Get the RefLinkID
    // Link it...
    unsigned int uiRefLinkID;
    if (!ReadRefLinkID(uiRefLinkID))
    {
        return false;
    }

    NiBool bSuccess;

    if (uiRefLinkID == REF_LINK_NULL)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, 
            (NiEntityInterface*)NULL, uiIndex);
        return bSuccess;
    }

    NiEntityPropertyInterface* pkRefEntity = m_kLinkMaps.GetAt(uiRefLinkID);
    NIASSERT(pkRefEntity);
    NIASSERT(pkRefEntity->GetInterfaceType() ==
        NiEntityInterface::IT_ENTITYINTERFACE);

    bSuccess = pkPropIntf->SetPropertyData(pcName, 
        (NiEntityInterface*)pkRefEntity, uiIndex);
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadObjectPointerFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    return pkPropIntf->SetPropertyData(pcName, (NiObject*) NULL, uiIndex);
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadFloatFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    float kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadBoolFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{       
    bool kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadIntFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    int kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadUIntFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    unsigned int kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadShortFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    short kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadUShortFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    unsigned short kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadStringFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    NiFixedString kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (!bSuccess)
    {
        return false;
    }

    // Is the string a path and does it need to be converted to an 
    // absolute path?
    const char* pkAsset = m_kDOM.GetAttributeFromCurrent("Asset");
    if (pkAsset && NiStricmp(pkAsset, "TRUE") == 0)
    {
        // This string is really a path that references an asset.
        // The path needs to be converted to an absolute path.
        char acAbsolutePath[NI_MAX_PATH];
        NiStrcpy(acAbsolutePath, NI_MAX_PATH, kData);
        NiPath::Standardize(acAbsolutePath);
        if (NiPath::IsRelative(acAbsolutePath))
        {
            NIVERIFY(NiPath::ConvertToAbsolute(acAbsolutePath,
                NI_MAX_PATH, m_acFullPath) > 0);
        }

        const NiFixedString kAbsolutePath = acAbsolutePath;
        bSuccess = pkPropIntf->SetPropertyData(pcName, kAbsolutePath,
            uiIndex);
    }
    else
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }

    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadPoint2FromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    NiPoint2 kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadPoint3FromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    NiPoint3 kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadQuaternionFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    NiQuaternion kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadMatrix3FromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    NiMatrix3 kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadColorFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    NiColor kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
inline NiBool NiEntityStreamingAscii::ReadColorAFromDOM(
    NiEntityPropertyInterface* pkPropIntf, const char* pcName, 
    unsigned int uiIndex)
{
    NiColorA kData;
    NiBool bSuccess = m_kDOM.ReadPrimitive(kData);
    if (bSuccess)
    {
        bSuccess = pkPropIntf->SetPropertyData(pcName, kData, uiIndex);
    }
    return bSuccess;
}
//---------------------------------------------------------------------------
