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

#ifndef NID3D10SHADERLIBRARYVERSION_H
#define NID3D10SHADERLIBRARYVERSION_H


#include "NiD3D10ShaderFactory.h"

// NiD3D10ShaderLibraryVersionInfo
class NID3D10RENDERER_ENTRY NiD3D10ShaderLibraryVersion : public NiMemObject
{
public:
    NiD3D10ShaderLibraryVersion();

    // Requested version and platform
    void SetSystemVertexShaderVersion(unsigned int uiVersion);
    void SetMinVertexShaderVersion(unsigned int uiVersion);
    void SetVertexShaderVersionRequest(unsigned int uiVersion);
    void SetSystemVertexShaderVersion(unsigned int uiMaj, 
        unsigned int uiMin);
    void SetMinVertexShaderVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetVertexShaderVersionRequest(unsigned int uiMaj, 
        unsigned int uiMin);

    void SetSystemGeometryShaderVersion(unsigned int uiVersion);
    void SetMinGeometryShaderVersion(unsigned int uiVersion);
    void SetGeometryShaderVersionRequest(unsigned int uiVersion);
    void SetSystemGeometryShaderVersion(unsigned int uiMaj, 
        unsigned int uiMin);
    void SetMinGeometryShaderVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetGeometryShaderVersionRequest(unsigned int uiMaj, 
        unsigned int uiMin);

    void SetSystemPixelShaderVersion(unsigned int uiVersion);
    void SetMinPixelShaderVersion(unsigned int uiVersion);
    void SetPixelShaderVersionRequest(unsigned int uiVersion);
    void SetSystemPixelShaderVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetMinPixelShaderVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetPixelShaderVersionRequest(unsigned int uiMaj, 
        unsigned int uiMin);

    void SetSystemUserVersion(unsigned int uiVersion);
    void SetMinUserVersion(unsigned int uiVersion);
    void SetUserVersionRequest(unsigned int uiVersion);
    void SetSystemUserVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetMinUserVersion(unsigned int uiMaj, unsigned int uiMin);
    void SetUserVersionRequest(unsigned int uiMaj, unsigned int uiMin);

    void SetPlatform(unsigned int uiPlatform);

    unsigned int GetSystemVertexShaderVersion() const;
    unsigned int GetMinVertexShaderVersion() const;
    unsigned int GetVertexShaderVersionRequest() const;

    unsigned int GetSystemGeometryShaderVersion() const;
    unsigned int GetMinGeometryShaderVersion() const;
    unsigned int GetGeometryShaderVersionRequest() const;

    unsigned int GetSystemPixelShaderVersion() const;
    unsigned int GetMinPixelShaderVersion() const;
    unsigned int GetPixelShaderVersionRequest() const;

    unsigned int GetSystemUserVersion() const;
    unsigned int GetMinUserVersion() const;
    unsigned int GetUserVersionRequest() const;
    unsigned int GetPlatform() const;

protected:
    unsigned int m_uiSystemVertexShaderVersion;
    unsigned int m_uiMinVertexShaderVersion;
    unsigned int m_uiRequestedVertexShaderVersion;

    unsigned int m_uiSystemGeometryShaderVersion;
    unsigned int m_uiMinGeometryShaderVersion;
    unsigned int m_uiRequestedGeometryShaderVersion;

    unsigned int m_uiSystemPixelShaderVersion;
    unsigned int m_uiMinPixelShaderVersion;
    unsigned int m_uiRequestedPixelShaderVersion;

    unsigned int m_uiSystemUserVersion;
    unsigned int m_uiMinUserVersion;
    unsigned int m_uiRequestedUserVersion;
    unsigned int m_uiPlatform;
};

#include "NiD3D10ShaderLibraryVersion.inl"

#endif  //#ifndef NID3D10SHADERLIBRARYVERSION_H
