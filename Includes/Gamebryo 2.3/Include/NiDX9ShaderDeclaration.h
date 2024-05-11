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
#ifndef NIDX9SHADERDECLARATION_H
#define NIDX9SHADERDECLARATION_H

#include "NiD3DShaderDeclaration.h"

class NID3D_ENTRY NiDX9ShaderDeclaration : public NiD3DShaderDeclaration
{
public:
    virtual ~NiDX9ShaderDeclaration();

    static NiDX9ShaderDeclaration* Create(NiRenderer* pkRenderer,
        unsigned int uiMaxStreamEntryCount, 
        unsigned int uiStreamCount = 1);

    virtual unsigned int GetVertexStride(unsigned int uiStream);

    virtual void SetSoftwareVertexProcessing(bool bSoftwareVB);
    virtual bool GetSoftwareVertexProcessing() const;

    // *** begin Emergent internal use only ***

    virtual NiVBChip* PackUntransformedVB(NiGeometryData* pkData, 
        NiSkinInstance* pkSkin, NiSkinPartition::Partition* pkPartition, 
        unsigned short usDirtyFlags, NiVBChip* pkOldVBChip, 
        unsigned int uiStream, void* pvLockedBuff = NULL);

    virtual void MarkAsModified();

    virtual bool GenerateVertexParameters(
        NiD3DVertexDeclaration& pkDeclaration, unsigned int& uiNumStreams);

    virtual NiD3DVertexDeclaration GetD3DDeclaration();

    static bool GetShaderParameterValues(D3DDECLUSAGE eD3DUsage,
        unsigned int uiUsageIndex, ShaderParameter& eInput, 
        ShaderParameterType& eType, ShaderParameterUsage& eUsage, 
        ShaderParameterTesselator& eTess);

    // *** end Emergent internal use only ***

protected:
    NiDX9ShaderDeclaration();

    unsigned int AddDeclarationEntry(ShaderRegisterEntry* pkEntry, 
        unsigned int uiStream);

    D3DVERTEXELEMENT9* m_pkElements;
    NiD3DVertexDeclaration m_hVertexDecl;

    bool m_bSoftwareVB;

    static D3DDECLTYPE ms_aeTypes[SPTYPE_COUNT];
    static D3DDECLMETHOD ms_aeMethods[SPTESS_COUNT];
    static D3DDECLUSAGE ms_aeUsage[SPUSAGE_COUNT];
};

NiSmartPointer(NiDX9ShaderDeclaration);

#endif  //#ifndef NIDX9SHADERDECLARATION_H
