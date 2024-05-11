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

#ifndef NID3D10VERTEXDESCRIPTION_H
#define NID3D10VERTEXDESCRIPTION_H

#include <NiShaderDeclaration.h>

#include "NiD3D10RendererLibType.h"
#include "NiD3D10Headers.h"

NiSmartPointer(NiD3D10VertexDescription);

class NID3D10RENDERER_ENTRY NiD3D10VertexDescription : 
    public NiShaderDeclaration
{
public:
    virtual ~NiD3D10VertexDescription();

    static bool Create(unsigned int uiMaxStreamEntryCount, 
        unsigned int uiStreamCount, 
        NiD3D10VertexDescriptionPtr& spVertexDescription);

    unsigned int GetVertexStride(unsigned int uiStream);
    unsigned int GetPackingOffset(unsigned int uiStream, unsigned int uiEntry);

    void UpdateInputElementArray();

    // *** begin Emergent internal use only ***

    D3D10_INPUT_ELEMENT_DESC* GetInputElementArray() const;
    unsigned int GetInputElementCount() const;

    static unsigned int GetSPTypeSize(
        NiShaderDeclaration::ShaderParameterType eType);

    static void _SDMInit();
    static void _SDMShutdown();

    static NiD3D10VertexDescription* Create(unsigned int uiMaxStreamEntryCount,
        unsigned int uiStreamCount);

    // *** end Emergent internal use only ***

protected:
    NiD3D10VertexDescription();

    bool Initialize(unsigned int uiMaxStreamEntryCount, 
        unsigned int uiStreamCount);

    unsigned int AddDeclarationEntry(
        ShaderRegisterEntry* pkEntry, unsigned int uiStream, 
        unsigned int uiCurrentElement);

    D3D10_INPUT_ELEMENT_DESC* m_pkInputElementArray;
    unsigned int m_uiInputElementCount;

    static unsigned int ms_auiTypeSizes[SPTYPE_COUNT];
    static DXGI_FORMAT ms_aeFormats[SPTYPE_COUNT];
};

//#include "NiD3D10VertexDescription.inl"

#endif  //#ifndef NID3D10VERTEXDESCRIPTION_H
